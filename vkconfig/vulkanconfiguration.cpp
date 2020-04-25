/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * The vkConfig2 program monitors and adjusts the Vulkan configuration
 * environment. These settings are wrapped in this class, which serves
 * as the "model" of the system.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <QDir>
#include <QSettings>
#include <QTextStream>

#include <vulkan/vulkan.h>

#include <profiledef.h>
#include "vulkanconfiguration.h"


//////////////////////////////////////////////////////////////////////////////
// Constructor does all the work.
CPathFinder::CPathFinder(const QString& qsPath)
    {
#ifdef _WIN32
    QSettings files(qsPath, QSettings::NativeFormat);
    fileList = files.allKeys();
#else
    QDir dir(qsPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QStringList() << "*.json", QDir::Files);

    for(int iFile = 0; iFile < fileInfoList.size(); iFile++)
        fileList << fileInfoList[iFile].filePath();
#endif
    }


// I am purposly not flagging these as explicit or implicit as this can be parsed from the location
// and future updates to layer locations will only require a smaller change.
#ifdef _WIN32
const int nSearchPaths = 6;
const QString szSearchPaths[nSearchPaths] = {
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers",
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers",
        "HKEY_CURRENT_USER\\SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers",
        "HKEY_CURRENT_USER\\SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers",
        "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers",
        "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers" };
#else
const int nSearchPaths = 8;
const QString szSearchPaths[nSearchPaths] = {
        "/usr/local/etc/vulkan/explicit_layer.d",            // Not used on macOS, okay to just ignore
        "/usr/local/etc/vulkan/implicit_layer.d",            // Not used on macOS, okay to just ignore
        "/usr/local/share/vulkan/explicit_layer.d",
        "/usr/local/share/vulkan/implicit_layer.d",
        "/etc/vulkan/explicit_layer.d",
        "/etc/vulkan/implicit_layer.d",
        "/usr/share/vulkan/explicit_layer.d",
        "/usr/share/vulkan/implicit_layer.d",
//        "$HOME/.local/share/vulkan/explicit_layer.d",
//        "$HOME/.local/share/vulkan/implicit_layer.d"
};
#endif

// Single pointer to singleton configuration object
CVulkanConfiguration* CVulkanConfiguration::pMe = nullptr;



CVulkanConfiguration::CVulkanConfiguration()
    {
    allLayers.reserve(10);
    bLogStdout = false;
    pActiveProfile = nullptr;

    // Where is stuff
#ifdef _WIN32
    // Assemble the path name where the overide .json file goes
    QDir tempPath = QDir::temp();
    if (!tempPath.cd("VulkanLayerManager")) {
        tempPath.mkpath("VulkanLayerManager");
        tempPath.cd("VulkanLayerManager");
        }
    qsOverrideJsonPath = QDir::toNativeSeparators(tempPath.absoluteFilePath("VkLayer_override.json"));
    qsOverrideSettingsPath = QDir::toNativeSeparators(tempPath.absoluteFilePath("vk_layer_settings.txt"));

    QDir home = QDir::home();
    qsProfileFilesPath = home.path() + QString("/AppData/Local/");
    if(!home.cd("LunarG")) {
        home.mkpath("LunarG");
        home.cd("LunarG");
        }

    if(!home.cd("vkconfig"))
        home.mkpath("vkconfig");

    qsProfileFilesPath += "LunarG/vkconfig";

#else
    QDir home = QDir::home();
    qsProfileFilesPath = home.path() + QString("/.local/share/vulkan/");     // TBD, where do profiles go if not here...
    qsOverrideSettingsPath = qsProfileFilesPath + "settings.d/vk_layer_settings.txt";
    qsOverrideJsonPath = qsProfileFilesPath + "implicit_layer.d/VkLayer_override.json";
#endif
    // Load simple app settings, the additional search paths, and the
    // override app list.
    loadAppSettings();
    loadAdditionalSearchPaths();
    loadAppList();
    loadDefaultLayerSettings();    // findAllInstalledLayers uses the results of this.
    findAllInstalledLayers();
    loadAllProfiles();

    // This will reset or clear the current profile if the files have been
    // manually manipulated
    SetCurrentActiveProfile(pActiveProfile);
    }


CVulkanConfiguration::~CVulkanConfiguration()
    {
    clearLayerLists();
    qDeleteAll(profileList.begin(), profileList.end());
    profileList.clear();
    }

void CVulkanConfiguration::clearLayerLists(void)
    {
    qDeleteAll(allLayers.begin(), allLayers.end());
    allLayers.clear();
    }


///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void CVulkanConfiguration::loadAppSettings(void)
    {
    // Load the launch app name from the last session
    QSettings settings;
    qsLaunchApplicationWPath = settings.value(VKCONFIG_KEY_LAUNCHAPP).toString();
    qsLaunchApplicatinArgs = settings.value(VKCONFIG_KEY_LAUNCHAPP_ARGS).toString();
    qsLaunchApplicationWorkingDir = settings.value(VKCONFIG_KEY_LAUNCHAPP_CWD).toString();
    qsLogFileWPath = settings.value(VKCONFIG_KEY_LOGFILE).toString();
    bLogStdout = settings.value(VKCONFIG_KEY_LOGSTDOUT).toBool();
    }


///////////////////////////////////////////////////////////////////////////////
/// This is for the local application settings, not the system Vulkan settings
void CVulkanConfiguration::saveAppSettings(void)
    {
    QSettings settings;
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP, qsLaunchApplicationWPath);
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP_ARGS, qsLaunchApplicatinArgs);
    settings.setValue(VKCONFIG_KEY_LAUNCHAPP_CWD, qsLaunchApplicationWorkingDir);
    settings.setValue(VKCONFIG_KEY_LOGFILE, qsLogFileWPath);
    settings.setValue(VKCONFIG_KEY_LOGSTDOUT, bLogStdout);
    }


/////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Load the list of paths here.
void CVulkanConfiguration::loadAdditionalSearchPaths(void)
    {
    QSettings searchPaths;
    additionalSearchPaths = searchPaths.value(VKCONFIG_KEY_CUSTOM_PATHS).toStringList();
    }


/////////////////////////////////////////////////////////////////////////////
/// \brief saveAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Save the list of paths here.
void CVulkanConfiguration::saveAdditionalSearchPaths(void)
    {
    QSettings searchPaths;
    searchPaths.setValue(VKCONFIG_KEY_CUSTOM_PATHS, additionalSearchPaths);
    }


///////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAppList
/// Load the custom application list
void CVulkanConfiguration::loadAppList(void)
    {
    QSettings appPaths;
    appList = appPaths.value(VKCONFIG_KEY_APPLIST).toStringList();
    }


//////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::saveAppList
/// Save the custom applicaiton list
void CVulkanConfiguration::saveAppList(void)
    {
    QSettings appPaths;
    appPaths.setValue(VKCONFIG_KEY_APPLIST, appList);
    }


///////////////////////////////////////////////////////////////////////////////
// Find all installed layers on the system.
void CVulkanConfiguration::findAllInstalledLayers(void)
    {
    // This is called initially, but also when custom search paths are set, so
    // we need to clear out the old data and just do a clean refresh
    clearLayerLists();

    // Standard layer paths
    for(uint32_t i = 0; i < nSearchPaths; i++) {
        TLayerType type = (szSearchPaths[i].contains("implicit", Qt::CaseInsensitive)) ? LAYER_TYPE_IMPLICIT : LAYER_TYPE_EXPLICIT;
        if(type == LAYER_TYPE_IMPLICIT)
            loadLayersFromPath(szSearchPaths[i], allLayers, type);
        else
            loadLayersFromPath(szSearchPaths[i], allLayers, type);
        }

    // Any custom paths? All layers from all paths are appended together here
    for(int i = 0; i < additionalSearchPaths.size(); i++)
        loadLayersFromPath(additionalSearchPaths[i], allLayers, LAYER_TYPE_CUSTOM);
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadLayersFromPath
/// \param szPath
/// \param layerList
/// Search a folder and load up all the layers found there. This does NOT
/// load the default settings for each layer. This is just a master list of
/// settings to be copied from, and referenced.
void CVulkanConfiguration::loadLayersFromPath(const QString &qsPath,
                               QVector<CLayerFile *>& layerList, TLayerType type)
    {
    CPathFinder fileList(qsPath);
    if(fileList.FileCount() == 0)
        return;

    for(int iFile = 0; iFile < fileList.FileCount(); iFile++) {
        CLayerFile *pLayerFile = new CLayerFile();
        if(pLayerFile->readLayerFile(fileList.GetFileName(iFile), type)) {
            // Look for duplicates - Path name AND name must be the same TBD
            for(int i = 0; i < layerList.size(); i++)
                if(layerList[i]->library_path == pLayerFile->library_path &&
                        layerList[i]->name == pLayerFile->name) {
                    delete pLayerFile;
                    pLayerFile = nullptr;
                    break;
                    }

            // We have a layer! See if we need to add the settings list to it, and then add it to our list
            if(pLayerFile != nullptr)
                layerList.push_back(pLayerFile);
            }
        }
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::findSettingsFor
/// \param layerName
/// \return
/// Find the settings for this named layer. If none found, return nullptr
const LayerSettingsDefaults* CVulkanConfiguration::findSettingsFor(QString layerName)
    {
    for(int i = 0; i < defaultLayerSettings.size(); i++)
        if(layerName == defaultLayerSettings[i]->layerName)
            return defaultLayerSettings[i];

    return nullptr;
    }


//////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::FindProfile
/// \param profileName
/// \return
/// Search the list of loaded profiles and return a pointer
CProfileDef* CVulkanConfiguration::FindProfile(QString profileName)
    {
    for(int i = 0; i < profileList.size(); i++)
        if(profileList[i]->qsProfileName == profileName)
            return profileList[i];

    return nullptr;
    }


///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadProfiles
/// Load all the  profiles. If the canned profiles don't exist,
/// they are created from the embedded json files
void CVulkanConfiguration::loadAllProfiles(void)
    {
    // This might be called to refresh the list...
    qDeleteAll(profileList.begin(), profileList.end());
    profileList.clear();
    pActiveProfile = nullptr;

    // Get a list of all files that end in .json in the folder where
    // we store them. TBD... don't hard code this here.
    QDir dir(qsProfileFilesPath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.json");
    QFileInfoList profileFiles = dir.entryInfoList();

    // Loop through all the profiles found and load them
    for(int iProfile = 0; iProfile < profileFiles.size(); iProfile++)
        {
        QFileInfo info = profileFiles.at(iProfile);
        CProfileDef *pProfile = LoadProfile(info.absoluteFilePath());
        if(pProfile != nullptr) {
            pProfile->qsFileName = info.fileName(); // Easier than parsing it myself ;-)
            profileList.push_back(pProfile);
            }
        }

    // All profile files found are loaded. Now, double check for the canned
    // profiles and if they aren't already loaded, load them from the
    // resource file. Array below is just the name of the profile and
    // the embedded resource location if they are needed.
    const char *szCannedProfiles[10] = {
    "Standard Validation",          ":/resourcefiles/StandardValidation.json",
    "Best Practices Validation",    ":/resourcefiles/BestPracticesValidation.json",
    "GPU-Assisted Validation",      ":/resourcefiles/GPU-AssistedValidation.json",
    "Lightweight Validation",       ":/resourcefiles/LightweightValidation.json",
    };

    // For each canned profile
    for(int i = 0; i < 10; i+=2) {
        // Search the list of loaded profiles
        CProfileDef *pProfile = FindProfile(szCannedProfiles[i]);
        if(pProfile == nullptr) {
            pProfile = LoadProfile(szCannedProfiles[i+1]);
            if(pProfile != nullptr)
                profileList.push_back(pProfile);
            }
        }

    //////////////////////////////////////////////////////////////////////////
    // Which of these profiles is currently active?
    QSettings settings;
    pActiveProfile = nullptr;
    QString qsActiveProfile = settings.value(VKCONFIG_KEY_ACTIVEPROFILE).toString();
    for(int i = 0; i < profileList.size(); i++)
        if(profileList[i]->qsProfileName == qsActiveProfile) {
            pActiveProfile = profileList[i];
            break;
            }
    }



//////////////////////////////////////////////////////////////////////////////
// This function loads the (currently four) sets of profile settings into
// the defaults. These are all stored in layer_info.json
// 4/8/2020
void CVulkanConfiguration::loadDefaultLayerSettings(void)
    {
    // Load the main object into the json document
    QFile file(":/resourcefiles/layer_info.json");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();

    QJsonDocument       jsonlayerInfoDoc;
    jsonlayerInfoDoc = QJsonDocument::fromJson(data.toLocal8Bit());
    if (!jsonlayerInfoDoc.isObject())
            return;

    // Isolate the Json object for each layer
    QJsonObject docObject = jsonlayerInfoDoc.object();
    QJsonValue layerOptionsValue = docObject.value("layer_options");
    QJsonObject layersOptionsObject = layerOptionsValue.toObject();

    // This is a list of layers for which we have user editable settings.
    // there are nine as of this writing, but this code should accomodate
    // if more are added at a later time.
    // All the layers have been loaded, so we can look for matches
    // and let the layers parse the json data to create their own list
    // of settings.
    QStringList layersWithSettings = layersOptionsObject.keys();
    for(int i = 0; i < layersWithSettings.size(); i++) {    // For each setting
        LayerSettingsDefaults *pDefaults = new LayerSettingsDefaults();
        pDefaults->layerName = layersWithSettings[i];

        // Save the name of the layer, and by default none are read only
        pDefaults->layerName = layersWithSettings[i];

        // Get the object for just this layer
        QJsonValue layerValue = layersOptionsObject.value(layersWithSettings[i]);
        QJsonObject layerObject = layerValue.toObject();

        CLayerFile::loadSettingsFromJson(layerObject, pDefaults->defaultSettings);

        // Add to my list of layer settings
        defaultLayerSettings.push_back(pDefaults);
        }
    }


//////////////////////////////////////////////////////////////////////////////
const CLayerFile* CVulkanConfiguration::findLayerNamed(QString qsLayerName)
    {
    // Search implicit layers
    for(int i = 0; i < allLayers.size(); i++)
        if(qsLayerName == allLayers[i]->name)
            return allLayers[i];

    return nullptr;
    }


///////////////////////////////////////////////////////////////////////////////
// Load from a .profile file (.json really)
CProfileDef* CVulkanConfiguration::LoadProfile(QString pathToProfile)
    {
    // Just load the name for now, and if it's read only
    QFile file(pathToProfile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString jsonText = file.readAll();
    file.close();

    // Tease it apart, get the name of the profile
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);

    if(parseError.error != QJsonParseError::NoError)
        return nullptr;

    // Allocate a new profile container
    CProfileDef *pProfile = new CProfileDef();
    pProfile->qsFileName = QFileInfo(pathToProfile).fileName();

    QJsonObject jsonTopObject = jsonDoc.object();
    QStringList key = jsonTopObject.keys();
    pProfile->qsProfileName = key[0];

    QJsonValue profileEntryValue = jsonTopObject.value(key[0]);
    QJsonObject profileEntryObject = profileEntryValue.toObject();

    QJsonValue blackListValue = profileEntryObject.value("blacklisted_layers");
    QJsonArray blackListArray = blackListValue.toArray();
    for(int i = 0; i < blackListArray.size(); i++)
        pProfile->blacklistedLayers << blackListArray[i].toString();

    QJsonValue description = profileEntryObject.value("description");
    pProfile->qsDescription = description.toString();

    QJsonValue fixed = profileEntryObject.value("fixed_profile");
    if(!fixed.isNull())
        pProfile->bFixedProfile = fixed.toBool();

    QJsonValue optionsValue = profileEntryObject.value("layer_options");

    QJsonObject layerObjects = optionsValue.toObject();
    QStringList layerList = layerObjects.keys();

    // Build the list of layers with their settings
    for(int iLayer = 0; iLayer < layerList.length(); iLayer++) {
        // Find this in our lookup of layers. This get's the layer and it's location/information
        const CLayerFile *pLayer = findLayerNamed(layerList[iLayer]);
        if(pLayer == nullptr)           // If not found, we have a layer missing....
            continue;

        // Make a copy add it to this layer
        CLayerFile *pProfileLayer = new CLayerFile();
        pLayer->CopyLayer(pProfileLayer);
        pProfile->layers.push_back(pProfileLayer);

        QJsonValue layerValue = layerObjects.value(layerList[iLayer]);
        QJsonObject layerObject = layerValue.toObject();

        QJsonValue layerRank = layerObject.value("layer_rank");
        pProfileLayer->nRank = layerRank.toInt();
        pProfileLayer->bActive = true;      // Always because it's present in the file

        // If any layer has the Khronos layer, add it here.
        if(true == CLayerFile::loadSettingsFromJson(layerObject, pProfileLayer->layerSettings))
            pProfile->bContainsKhronosOutput = true;
        }

    // We need to sort the layers by their rank. The json sorts alphebetically and we
    // need to undo it.... A bubble quick sort is fine, it's a small list
    if(pProfile->layers.size() > 1)
        for(int i = 0; i < pProfile->layers.size()-1; i++)
            for(int j = i+1; j < pProfile->layers.size(); j++)
                if(pProfile->layers[i]->nRank > pProfile->layers[j]->nRank) {
                    CLayerFile *pTemp = pProfile->layers[i];
                    pProfile->layers[i] = pProfile->layers[j];
                    pProfile->layers[j] = pTemp;
                    }

    return pProfile;
    }


/////////////////////////////////////////////////////////////////////////////////////
void CVulkanConfiguration::SaveProfile(CProfileDef *pProfile)
    {
    // Build the json document
    QJsonArray blackList;
    for(int i = 0; i < pProfile->blacklistedLayers.size(); i++)
        blackList.append(pProfile->blacklistedLayers[i]);

    QJsonObject layerList; // This list of layers

    for(int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        CLayerFile *pLayer = pProfile->layers[iLayer];

        QJsonObject jsonSettings;

        // Rank goes in here with settings
        jsonSettings.insert("layer_rank", pLayer->nRank);

        // Loop through the actual settings
        for(int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
            QJsonObject setting;
            TLayerSettings *pSettingsDetails = pLayer->layerSettings[iSetting];

            // Only write if it's true, we don't want to clutter up
            // the other layers, or tempt someone to see it in the .json and set
            // it to true.
            if(pSettingsDetails->commonKhronosEdit)
                setting.insert("common_edit", true);

            setting.insert("name", pSettingsDetails->settingsPrompt);
            setting.insert("description", pSettingsDetails->settingsDesc);

            switch(pSettingsDetails->settingsType) {
                case LAYER_SETTINGS_STRING:
                    setting.insert("type", "string");
                    setting.insert("default", pSettingsDetails->settingsValue);
                break;

                case LAYER_SETTINGS_FILE:
                    setting.insert("type", "save_file");
                    setting.insert("default", pSettingsDetails->settingsValue);
                break;

                case LAYER_SETTINGS_BOOL:
                    setting.insert("type", "bool");
                    setting.insert("default", pSettingsDetails->settingsValue);
                break;

                case LAYER_SETTINGS_EXCLUSIVE_LIST: {
                    setting.insert("type", "enum");
                    setting.insert("default", pSettingsDetails->settingsValue);

                    QJsonObject options;
                    for(int i = 0; i < pSettingsDetails->settingsListExclusivePrompt.size(); i++)
                        options.insert( pSettingsDetails->settingsListExclusiveValue[i],
                                        pSettingsDetails->settingsListExclusivePrompt[i]);
                    setting.insert("options", options);
                    }
                break;

            case LAYER_SETTINGS_INCLUSIVE_LIST: {
                    setting.insert("type", "multi_enum");
                    QJsonObject options;
                    for(int i = 0; i < pSettingsDetails->settingsListInclusivePrompt.size(); i++)
                        options.insert( pSettingsDetails->settingsListInclusiveValue[i],
                                        pSettingsDetails->settingsListInclusivePrompt[i]);
                    setting.insert("options", options);

                    QJsonArray defaults;
                    if(!pSettingsDetails->settingsValue.isEmpty()) {
                        QStringList list = pSettingsDetails->settingsValue.split(",");
                        for(int i = 0; i < list.size(); i++)
                            defaults.append(list[i]);
                        }

                    setting.insert("default", defaults);
                    }
                break;

                // There is a string field that is actually a complicted series of number or
                // ranges of numbers. We should at some point add this to allow more error free editing of it.
                case LAYER_SETTINGS_RANGE_INT:


                break;

                // We missed somethhing
                default:
                    setting.insert("type", "unknown type");
                    setting.insert("default", "unknown data");
                }

            jsonSettings.insert(pSettingsDetails->settingsName, setting);
            }

        layerList.insert(pLayer->name, jsonSettings);
        }



    //////////////////////////////////////////////////////////
    // Assemble the json
    QJsonObject root;
    QJsonObject json_profile;
    json_profile.insert("blacklisted_layers", blackList);
    json_profile.insert("description", pProfile->qsDescription);
    json_profile.insert("fixed_profile", pProfile->bFixedProfile);
    json_profile.insert("layer_options", layerList);
    root.insert(pProfile->qsProfileName, json_profile);
    QJsonDocument doc(root);


    ///////////////////////////////////////////////////////////
    // Write it out
    QString pathToProfile = qsProfileFilesPath;
    pathToProfile += "/";
    pathToProfile += pProfile->qsFileName;

    QFile jsonFile(pathToProfile);
    if(!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;     // TBD, should we report an error
    jsonFile.write(doc.toJson());
    jsonFile.close();
    }

///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::CreateEmptyProfile
/// \return
/// Create an empty profile definition that contains all available layers.
/// All settings are the default, and the layer order is just the order at
/// which they have come.
CProfileDef* CVulkanConfiguration::CreateEmptyProfile()
    {
    CProfileDef* pNewProfile = new CProfileDef();

    CLayerFile *pTempLayer;
    int nRank = 0;

    // Add layers
    for(int i = 0; i < allLayers.size(); i++) {
        pTempLayer = new CLayerFile();
        allLayers[i]->CopyLayer(pTempLayer);
        pTempLayer->nRank = nRank++;
        pNewProfile->layers.push_back(pTempLayer);
        }

    // Now grab settings defaults
    for(int i = 0; i < pNewProfile->layers.size(); i++)
        LoadDefaultSettings(pNewProfile->layers[i]);

    return pNewProfile;
    }

//////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::LoadDefaultSettings
/// \param pBlankLayer
/// Load the default settings into an empty layer file container
void CVulkanConfiguration::LoadDefaultSettings(CLayerFile* pBlankLayer)
    {
    const LayerSettingsDefaults *pDefaults = findSettingsFor(pBlankLayer->name);

    if(pDefaults == nullptr)  // Did we find any?
        return;

    // Create and pop them in....
    for(int s = 0; s < pDefaults->defaultSettings.size(); s++) {
        TLayerSettings *pSetting = new TLayerSettings();
        *pSetting = *pDefaults->defaultSettings[s];
        pBlankLayer->layerSettings.push_back(pSetting);
        }
    }

///////////////////////////////////////////////////////////////////////////////
// Set this as the current override profile. The profile definition passed in
// is used to construct the override and settings files.
// Passing in nullptr IS valid, and will clear the current profile
void CVulkanConfiguration::SetCurrentActiveProfile(CProfileDef *pProfile)
    {
    pActiveProfile = pProfile;
    QString profileName;
    QSettings settings;

    // Clear the profile if null
    if(pProfile == nullptr) {
        // Delete a bunch of stuff
        remove(qsOverrideSettingsPath.toUtf8().constData());
        remove(qsOverrideJsonPath.toUtf8().constData());
        settings.setValue(VKCONFIG_KEY_ACTIVEPROFILE, "");


        // On Windows only, we need clear these values from the registry
 #ifdef _WIN32
        QSettings registry("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers", QSettings::NativeFormat);
        registry.remove(qsOverrideJsonPath);
        QSettings overrideSettings("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\Settings", QSettings::NativeFormat);
        overrideSettings.remove(qsOverrideSettingsPath);
 #endif
        return;
        }

    /////////////////////////////////////////////
    // Now the fun starts, we need to write out the json file
    // that describes the layers being employed and the settings file

    /////////////////////////
    // vk_layer_settings.txt
    QFile file(qsOverrideSettingsPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);

    // Loop through all the layers
    for(int iLayer = 0; iLayer < pProfile->layers.size(); iLayer++) {
        CLayerFile *pLayer = pProfile->layers[iLayer];
        stream << endl;
        stream << "# " << pLayer->name << endl;


        QString shortLayerName = pLayer->name;
        shortLayerName.remove("VK_LAYER_");
        QString lcLayerName = shortLayerName.toLower();


        for(int iSetting = 0; iSetting < pLayer->layerSettings.size(); iSetting++) {
            TLayerSettings *pSetting = pLayer->layerSettings[iSetting];
            stream << lcLayerName << "." << pSetting->settingsName << " = " << pSetting->settingsValue << endl;
            }
        }
    file.close();

    ////////////////////////
    // VkLayer_override.json
    QJsonArray json_paths;
    // The paths are only used if a layer is used in the user path.
//    for(int i = 0; i < additionalSearchPaths.size(); i++)
//        json_paths.append(additionalSearchPaths[i]);

    QJsonArray json_layers;
    for(int i = 0; i < pProfile->layers.size(); i++)
        json_layers.append(pProfile->layers[i]->name);

    QJsonArray json_blacklist;
    for(int i = 0; i < pProfile->blacklistedLayers.size(); i++)
        json_blacklist.append(pProfile->blacklistedLayers[i]);

    QJsonArray json_applist;
    for(const QString &appName : appList)
        json_applist.append(appName);

    QJsonObject disable;
    disable.insert("DISABLE_VK_LAYER_LUNARG_override", QString("1"));

    QJsonObject layer;
    layer.insert("name", QString("VK_LAYER_LUNARG_override"));
    layer.insert("type", QString("GLOBAL"));
    layer.insert("api_version", "1.2." + QString::number(VK_HEADER_VERSION));
    layer.insert("implementation_version", QString("1"));
    layer.insert("description", QString("LunarG Override Layer"));
    layer.insert("override_paths", json_paths);
    layer.insert("component_layers", json_layers);
    layer.insert("blacklisted_layers", json_blacklist);
    layer.insert("disable_environment", disable);
    layer.insert("app_keys", json_applist);

     QJsonObject root;
     root.insert("file_format_version", QJsonValue(QString("1.1.2")));
     root.insert("layer", layer);
     QJsonDocument doc(root);

    QFile jsonFile(qsOverrideJsonPath);
    if(!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;     // TBD, should we report an error
    jsonFile.write(doc.toJson());
    jsonFile.close();

    settings.setValue(VKCONFIG_KEY_ACTIVEPROFILE, pActiveProfile->qsProfileName);

    // On Windows only, we need to write these values to the registry
#ifdef _WIN32
    QSettings registry("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\ImplicitLayers", QSettings::NativeFormat);
    registry.setValue(qsOverrideJsonPath, 0);
    QSettings overrideSettings("HKEY_CURRENT_USER\\Software\\Khronos\\Vulkan\\Settings", QSettings::NativeFormat);
    overrideSettings.setValue(qsOverrideSettingsPath, 0);
#endif
    }

