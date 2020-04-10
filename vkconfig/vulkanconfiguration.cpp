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

#include <profiledef.h>
#include "vulkanconfiguration.h"

// I am purposly not flagging these as explicit or implicit as this can be parsed from the location
// and future updates to layer locations will only require a smaller change.
#ifdef _WIN32
const int nSearchPaths = 6;
const char *szSearchPaths[nSearchPaths] = {
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers",
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers",
        "HKEY_CURRENT_USER\\SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers",
        "HKEY_CURRENT_USER\\SOFTWARE\\Khronos\\Vulkan\\ImplicitLayers",
        "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\...\\VulkanExplicitLayers",
        "HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\...\\VulkanImplicitLayers" };
#else
const int nSearchPaths = 10;
const QString szSearchPaths[nSearchPaths] = {
        "/usr/local/etc/vulkan/explicit_layer.d",            // Not used on macOS, okay to just ignore
        "/usr/local/etc/vulkan/implicit_layer.d",            // Not used on macOS, okay to just ignore
        "/usr/local/share/vulkan/explicit_layer.d",
        "/usr/local/share/vulkan/implicit_layer.d",
        "/etc/vulkan/explicit_layer.d",
        "/etc/vulkan/implicit_layer.d",
        "/usr/share/vulkan/explicit_layer.d",
        "/usr/share/vulkan/implicit_layer.d",
        "$HOME/.local/share/vulkan/explicit_layer.d",
        "$HOME/.local/share/vulkan/implicit_layer.d"
};
#endif


// Single pointer to singleton configuration object
CVulkanConfiguration* CVulkanConfiguration::pMe = nullptr;



CVulkanConfiguration::CVulkanConfiguration()
    {
    implicitLayers.reserve(10);
    explicitLayers.reserve(10);
    bLogStdout = false;

    // Load simple app settings, the additional search paths, and the
    // override app list.
    loadAppSettings();
    loadAdditionalSearchPaths();
    loadAppList();
    loadDefaultLayerSettings();    // findAllInstalledLayers uses the results of this.
    findAllInstalledLayers();


    loadProfiles();

    // Default, no active profile
    pActiveProfile = nullptr;
    }


CVulkanConfiguration::~CVulkanConfiguration()
    {
    clearLayerLists();
    qDeleteAll(profileList.begin(), profileList.end());
    profileList.clear();
    }

void CVulkanConfiguration::clearLayerLists(void)
    {
    qDeleteAll(implicitLayers.begin(), implicitLayers.end());
    implicitLayers.clear();
    qDeleteAll(explicitLayers.begin(), explicitLayers.end());
    explicitLayers.clear();
    qDeleteAll(customLayers.begin(), customLayers.end());
    customLayers.clear();
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
    QDir home = QDir::home();
    QString fileName = home.absolutePath() + VKCONFIG_CUSTOM_LAYER_PATHS;

    // If the file doesn't exist, then the count is zero...
    nAdditionalSearchPathCount = 0;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);

    // Okay, there must be data...
    nAdditionalSearchPathCount = 0;

    while(!stream.atEnd()) {
        additionalSearchPaths << stream.readLine();
        nAdditionalSearchPathCount++;
        }

    file.close();   // Just to be explicit, should close anyway when it goes out of scope
    }


/////////////////////////////////////////////////////////////////////////////
/// \brief saveAdditionalSearchPaths
/// We may have additional paths where we want to search for layers.
/// Save the list of paths here.
void CVulkanConfiguration::saveAdditionalSearchPaths(void)
    {
    QDir home = QDir::home();
    QString fileName = home.absolutePath() + VKCONFIG_CUSTOM_LAYER_PATHS;

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // Okay, there must be data...
    QTextStream output(&file);
    for(uint32_t i = 0; i < nAdditionalSearchPathCount; i++)
        output << additionalSearchPaths[i] << "\n";

    file.close();   // Just to be explicit, should close anyway when it goes out of scope
    }


///////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadAppList
/// Load the custom application list
void CVulkanConfiguration::loadAppList(void)
    {
    QDir home = QDir::home();
    QString fileName = home.absolutePath() + VKCONIFG_CUSTOM_APP_LIST;
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream stream(&file);
    while(!stream.atEnd())
        appList << stream.readLine();

    file.close();   // Just to be explicit, should close anyway when it goes out of scope
    }


//////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::saveAppList
/// Save the custom applicaiton list
void CVulkanConfiguration::saveAppList(void)
    {
    QDir home = QDir::home();
    QString fileName = home.absolutePath() + VKCONIFG_CUSTOM_APP_LIST;
    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    // Okay, there must be data...
    QTextStream output(&file);
    for(int i = 0; i < appList.size(); i++)
        output << appList[i] << "\n";

    file.close();   // Just to be explicit, should close anyway when it goes out of scope
    }


///////////////////////////////////////////////////////////////////////////////
// Find all installed layers on the system.
void CVulkanConfiguration::findAllInstalledLayers(void)
{
    // This is called initially, but also when custom search paths are set, so
    // we need to clear out the old data and just do a clean refresh
    clearLayerLists();


#ifdef _WIN32
    for(int i = 0; i < nSearchPaths; i++) {
        wchar_t keyName[128];
        memset(keyName, 0, 128*sizeof(wchar_t));
        QString qsFullRegistryPath =   "SOFTWARE\\Khronos\\Vulkan\\ExplicitLayers";//  szSearchPaths[i];
        qsFullRegistryPath.toWCharArray(keyName);
        HKEY hKey;

    LONG lRes = RegOpenKeyExW(HKEY_LOCAL_MACHINE, keyName, 0, KEY_READ, &hKey);
    bool bExistsAndSuccess (lRes == ERROR_SUCCESS);
    bool bDoesNotExistsSpecifically (lRes == ERROR_FILE_NOT_FOUND);

    std::wstring strValueOfBinDir;
    std::wstring strKeyDefaultValue;

    //    strValue = strDefaultValue;
 /*   WCHAR szBuffer[512];
    DWORD dwBufferSize = sizeof(szBuffer);
    ULONG nError;
    nError = RegQueryValueExW(hKey, strValueName.c_str(), 0, NULL, (LPBYTE)szBuffer, &dwBufferSize);
    if (ERROR_SUCCESS == nError)
    {
        strValue = szBuffer;
    }
    */
//    GetStringRegKey(hKey, L"BinDir", strValueOfBinDir, L"bad");
//    GetStringRegKey(hKey, L"", strKeyDefaultValue, L"bad");


 //       CLayerFile* pLayerFile = new CLayerFile();
 //       pLayerFile->readLayerKeys(szSearchPaths[i], LAYER_TYPE_EXPLICIT);

    }


#endif

#ifndef _WIN32  // All the non-Windows layer locations

    // Standard layer paths
    for(uint32_t i = 0; i < nSearchPaths; i++) {
        // Does the path exist
        QDir dir(szSearchPaths[i]);
        if(!dir.exists())
            continue;

        TLayerType type = (szSearchPaths[i].contains("implicit")) ? LAYER_TYPE_IMPLICIT : LAYER_TYPE_EXPLICIT;
        if(type == LAYER_TYPE_IMPLICIT)
            loadLayersFromPath(szSearchPaths[i], implicitLayers, type);
        else
            loadLayersFromPath(szSearchPaths[i], explicitLayers, type);
        }

    // Any custom paths? All layers from all paths are appended together here
    for(uint32_t i = 0; i < nAdditionalSearchPathCount; i++)
        loadLayersFromPath(additionalSearchPaths[i], customLayers, LAYER_TYPE_CUSTOM);

#endif

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadLayersFromPath
/// \param szPath
/// \param layerList
/// Search a folder and load up all the layers found there.
void CVulkanConfiguration::loadLayersFromPath(const QString &qsPath, QVector<CLayerFile *>& layerList, TLayerType type)
    {
    // Does the path exist
    QDir dir(qsPath);
    if(!dir.exists())
        return;

    // Path exists, let's loop through all the .json files
    QFileInfoList fileList = dir.entryInfoList(QStringList() << "*.json", QDir::Files);
    for(int iFile = 0; iFile < fileList.size(); iFile++) {
        CLayerFile *pLayerFile = new CLayerFile();
        if(pLayerFile->readLayerFile(fileList[iFile].filePath(), type)) {
            // Look for duplicates - Path name AND name must be the same TBD
            for(int i = 0; i < layerList.size(); i++)
                if(layerList[i]->library_path == pLayerFile->library_path &&
                        layerList[i]->name == pLayerFile->name) {
                    delete pLayerFile;
                    pLayerFile = nullptr;
                    break;
                    }

            // We have a layer! See if we need to add the settings list to it, and then add it to our list
            if(pLayerFile != nullptr) {
                // Does this layer have settings?
                const LayerSettingsDefaults *pSettings = findSettingsFor(pLayerFile->name);
                if(pSettings != nullptr)
                    for(int i = 0; i < pSettings->defaultSettings.size(); i++) {
                        TLayerSettings* pLayerSettings = new TLayerSettings();
                        *pLayerSettings = *pSettings->defaultSettings[i];
                        }

                layerList.push_back(pLayerFile);
                }
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



///////////////////////////////////////////////////////////////////////////////
/// \brief CVulkanConfiguration::loadProfiles
/// Load all the  profiles. If the canned profiles don't exist,
/// they are created from the embedded json files
void CVulkanConfiguration::loadProfiles(void)
    {
    // Get a list of all files that end in .profile in the folder
    QString whereAreTheProfiles = QDir::homePath();
    whereAreTheProfiles += "/.local/share/vulkan";
    QDir dir(whereAreTheProfiles);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(QStringList() << "*.profile");
    QFileInfoList profileFiles = dir.entryInfoList();


    // Loop through all the profiles found and list them
    for(int iProfile = 0; iProfile < profileFiles.size(); iProfile++)
        {
        QFileInfo info = profileFiles.at(iProfile);
        CProfileDef *pProfile = LoadProfile(info.absoluteFilePath());
        if(pProfile != nullptr) {
            pProfile->qsFileName = info.baseName(); // Easier than parsing it myself ;-)
            profileList.push_back(pProfile);
            }
        }

    // All profile files found are loaded. Now, double check for the canned
    // profiles and if they aren't already found, load them from the
    // resource file.
    const char *szCannedProfiles[10] = {
    "Basic Validation",             ":/BasicValidation.profile",
    "Best Practices Validation",    ":/BestPracticesValidation.profile",
    "GPU-Assisted Validation",      ":/GPU-AssistedValidation",
    "Synchronization Validation",   ":/LightweightValidation",
    "Lightweight Validation",       ":/SynchronizationValidation"
    };

/*    int iFound;
    for(int iLoaded = 0; iLoaded < profileList.length(); iLoaded++) { // For each already loaded profile
        for(iFound = 0; iFound < 10; iFound+=2) {      // Look for this canned profile
            if(profileList[iLoaded]->qsProfileName == QString(szCannedProfiles[iFound+1])) // Found it?
                break;
            }

        // Did we find it?
        if(iFound >= 10) { // Nope
            CProfileDef *pProfile = new CProfileDef;
            pProfile->LoadProfile(szCannedProfiles[iLoaded*2]);
            profileList.push_back(pProfile);
            }
        }
*/

    /*
    // Read in the database
    QFile file(":/resourcefiles/presets_info.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString jsonText = file.readAll();
    file.close();

    // Tease it apart
    QJsonDocument jsonDoc;
    QJsonParseError parseError;
    jsonDoc = QJsonDocument::fromJson(jsonText.toUtf8(), &parseError);
    QJsonObject jsonTopObject = jsonDoc.object();

    // Loop through the list, read in all the data
    QStringList profileNames = jsonTopObject.keys();
    for(int i = 0; i < profileNames.size(); i++) {
        // Get the name of the profile
        CProfileDef* profileDef = new CProfileDef;
        profileDef->profileName = profileNames[i];

        // Get the actual profile object
        QJsonValue profile = jsonTopObject.value(profileNames[i]);
        QJsonObject profileObject = profile.toObject();

        QStringList layerList = profileObject.keys();

        // List of layers and settings
        for(int nLayer = 0; nLayer < layerList.length(); nLayer++) {




            }

        profileList.push_back(profileDef);
        }


    // This is fake profile that is used as a spacer
//    CProfileDef* myProfile = new CProfileDef;
//    myProfile->profileName = "User Profiles";
//    myProfile->readOnly = false;
//    profileList.push_back(myProfile);

    // Now load the user defined profiles
//    myProfile = new CProfileDef;
//    myProfile->profileName = "Starstone Standard Test Suite";
//    myProfile->readOnly = false;
//    profileList.push_back(myProfile);
*/
    }


/////////////////////////////////////////////////////////////////////////////////////////
// This saves all of the profile definitions, along with which one is active
void CVulkanConfiguration::saveProfiles(void)
    {
    /*
    QJsonObject     allProfiles;        // This is the top object

    // Top item is the name of the currently active profile
    if(nActiveProfile == -1)
        allProfiles.insert("Current Profile Name", "None");
    else
        allProfiles.insert("Current Profile Name", profileList[nActiveProfile]->profileName);

    // Followed by an array of profiles
    QJsonArray profileArray;

    // Top loop through the profiles
    for(int iProfile = 0; iProfile < profileList.size(); iProfile++) {
        QJsonObject profile;
        profile.insert("Name", profileList[iProfile]->profileName);
        profile.insert("Read Only", profileList[iProfile]->readOnly);

        // Now loop through the layers
        QJsonArray layerArray;
        for(int iLayer = 0; iLayer < profileList[iProfile]->layers.size(); iLayer++) {
            QJsonObject layerObject;
            // The order of the layers is LOST by the json parser, so we need to store the ranking so
            // we can resort them on load.
            layerObject.insert("Rank", QJsonValue(iLayer));
            layerObject.insert("Name", profileList[iProfile]->layers[iLayer]->name);
            layerObject.insert("type", profileList[iProfile]->layers[iLayer]->type);
            layerObject.insert("layertype", profileList[iProfile]->layers[iLayer]->layerType);


            layerArray.push_back(layerObject);
            }
        profile.insert("Layers", layerArray);
        profileArray.push_back(profile);
        }

    allProfiles.insert("Profiles", profileArray);

    //QJsonObject     activeProfile;
    //activeProfile.insert("Current Profile", "None");


    // Put together the list of profiles that are enabled
//    QVector <CLayerFile*> layers;
//    createProfileList(layers);

//    if(layers.count() == 0) {
//        QMessageBox msg;
//        msg.setText(tr("Can't create profile."));
//        msg.setInformativeText(tr("There are no selected layers"));
//        msg.setStandardButtons(QMessageBox::Ok);
//        msg.exec();
//        return;
//        }

//        QJsonObject     profileList;


//        QJsonObject     profile1;
//        profile1.insert("Readonly", true);

//        QJsonArray apps;
//        apps.push_back("/Users/rwright/Desktop/stuff");
//        apps.push_back("/User/rwright/Applicaitons/Lunartic");
//        profile1.insert("Apps",apps);

//        QJsonArray layers;
//        layers.push_back("VK_LAYER1");
//        layers.push_back("VK_LAYER_N");
//        profile1.insert("Layers", layers);


//        profileList.insert("Performance Suite", profile1);


//        profileArray.push_back("API Usage Validation");



 //       profileList.insert("Profiles", profileArray);








    //    lunarg_api_dump.use_spaces = TRUE
    //    lunarg_api_dump.no_addr = FALSE
    //    lunarg_api_dump.name_size = 32
    //    lunarg_api_dump.show_shader = FALSE
    //    lunarg_api_dump.detailed = TRUE
    //    lunarg_api_dump.flush = TRUE
    //    lunarg_api_dump.file = FALSE
    //    lunarg_api_dump.output_range = 0-0
    //    lunarg_api_dump.log_filename = vk_apidump.txt
    //    lunarg_api_dump.output_format = Text
    //    lunarg_api_dump.show_timestamp = FALSE
    //    lunarg_api_dump.show_types = TRUE
    //    lunarg_api_dump.indent_size = 4
    //    lunarg_api_dump.type_size = 0
    //

    //
    // # VK_LAYER_KHRONOS_validation
    //khronos_validation.debug_action = VK_DBG_LAYER_ACTION_LOG_MSG
    //khronos_validation.log_filename = stdout
    //khronos_validation.report_flags = warn,error,perf
    //khronos_validation.disables =
    //khronos_validation.enables =
    //

    QJsonDocument   jsonDoc(allProfiles);

    QDir home = QDir::home();
    QString fileOut = home.absolutePath() + VKCONFIG_PROFILE_LIST;
    QFile file(fileOut);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(jsonDoc.toJson());
    file.close();

*/

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
    for(int i = 0; i < implicitLayers.size(); i++)
        if(qsLayerName == implicitLayers[i]->name)
            return implicitLayers[i];

    // Search explicit layers
    for(int i = 0; i < explicitLayers.size(); i++)
        if(qsLayerName == explicitLayers[i]->name)
            return explicitLayers[i];

    // Search custom layers
    for(int i = 0; i < customLayers.size(); i++)
        if(qsLayerName == customLayers[i]->name)
            return customLayers[i];

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

    //if(parseError.error == )

    CProfileDef *pProfile = new CProfileDef();

    QJsonObject jsonTopObject = jsonDoc.object();
    QStringList key = jsonTopObject.keys();
    pProfile->qsProfileName = key[0];

    QJsonValue layerValues = jsonTopObject.value(key[0]);
    QJsonObject layerObjects = layerValues.toObject();
    QStringList layerList = layerObjects.keys();

    // Build the list of layers
    for(int iLayer = 0; iLayer < layerList.length(); iLayer++) {
        // Find this in our lookup of layers
        const CLayerFile *pLayer = findLayerNamed(layerList[iLayer]);
        if(pLayer == nullptr)           // If not found, we have a layer missing....
            continue;

        // Make a copy add it to this layer
        CLayerFile *pProfileLayer = new CLayerFile();
        pLayer->CopyLayer(pProfileLayer);
        pProfile->layers.push_back(pProfileLayer);

        // Layer settings have all just been copied with their default values


        // Found the layer name, it has info we need, and a list of settings
        // and their defaults. Copy all of this into the layer

        // Now we need to go through the list of "overrides", both read only and user editable
        // and replace the settings with the ones specified for this layer.

        // Now, we have a profile, the list of layers, and all the settings for those layers

        // Go somewhere else and write that to the override.json stuff to make it active.


        QJsonValue layer = layerObjects.value(layerList[iLayer]);
        QJsonObject layerObject = layer.toObject();
        QStringList sections = layerObject.keys();
        if(sections[0] == QString("Read Only"))
            pProfile->bContainsReadOnlyFields = true;

        // TBD, READ IN ALL THE STUFF...
        }



    return pProfile;
    }


void CVulkanConfiguration::SaveProfile(CProfileDef *pProfile, QString pathToProfile)
{



}


