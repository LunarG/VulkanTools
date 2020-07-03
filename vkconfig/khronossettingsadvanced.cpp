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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 *
 * Khronos layer advanced settings dialog.
 * The whole lookup description things really needs to be refactored.
 * so much in flux, I just put this in quickly.
 */

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>
#include "khronossettingsadvanced.h"

// Keep track of tree/setting correlations
struct TREE_SETTING {
    QString prompt;
    QString token;
    QTreeWidgetItem *pItem;
};

// Lookup table for settings descriptions and URL's
struct TABLE_ENTRY {
    QString setting;
    QString description;
    QString url;
};

TABLE_ENTRY lookupTable[17] = {
    {"VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT",
     "API usage validation checks: Validate the status of descriptor sets, command buffers, shader modules, pipeline states, "
     "renderpass usage, synchronization, dynamic states, and many other types of valid usage. ",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/core_checks.md"},

    {"VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE", "Command buffer state core validation checks.", ""},

    {"VALIDATION_CHECK_DISABLE_OBJECT_IN_USE", "Object-in-use state core validation checks.", ""},

    {"VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET", "Core validation checks to verify descriptor sets are not in-use.", ""},

    {"VALIDATION_CHECK_DISABLE_QUERY_VALIDATION", "Query state core validation checks.", ""},

    {"VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE", "Push constant range core validation checks.", ""},

    {"VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION", "Image layout validation core validation checks.", ""},

    {"VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT", "Shader validation-related core validation checks.", ""},

    {"VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT",
     "Thread-safety external synchronization checks. Checks performed include ensuring that only one thread at a time uses an "
     "object in free-threaded API calls.",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/thread_safety.md"},

    {"VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT",
     "Stateless API parameter validation checks. This option checks the validity of structures; the validity of enumerated type "
     "values; null pointer conditions; properties and limits requirements. ",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/stateless_validation.md"},

    {"VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT",
     "Object lifetimes core validation checks. Validate that only known objects are referenced and destroyed, that lookups are "
     "performed only on objects being tracked and that objects are correctly freed or destroyed.",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/object_lifetimes.md"},

    {"VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT", "Handle-wrapping which ensures unique object handles",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/handle_wrapping.md"},

    {"VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT", "Enables shader instrumentation for additional diagnostic validation checks.",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/gpu_validation.md"},

    {"VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT",
     "Modifies the value of the VkPhysicalDeviceLimits::maxBoundDescriptorSets property to return a value one less than the actual "
     "device's value to \"reserve\" a descriptor set binding slot for use by shader-based validation. This option is likely only "
     "of interest to applications that dynamically adjust their descriptor set bindings to adjust for the limits of the device.",
     ""},

    {"VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT",
     "Enables the shader Debug Printf functionality. It allows developers to debug their shader code by \"printing\" any values of "
     "interest to the debug callback or stdout. ",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/debug_printf.md"},

    {"VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT",
     "Vulkan Best Practices checks. It is intended to highlight potential performance issues, questionable usage patterns, common "
     "mistakes, and items not specifically prohibited by the Vulkan specification but that may lead to application problems.",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/best_practices.md"},

    {"VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM",
     "ARM GPU-specific Best Practices checks. It is intended to highlight usage patterns that are performance issues on ARM GPUs "
     "but that are not prohibited by the Vulkan specification or issues on other vendor GPUs.",
     ""},
};

#define NUM_CORE_CHECKS 7
static TREE_SETTING coreChecks[NUM_CORE_CHECKS] = {
    {"Image Layout Validation", "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION", nullptr},
    {"Command Buffer State", "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE", nullptr},
    {"Object in Use", "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE", nullptr},
    {"Query Validation", "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION", nullptr},
    {"Idle Descriptor Set", "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET", nullptr},
    {"Shader Validation Checks", "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT", nullptr},
    {"Push Constant Range", "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE", nullptr}};

#define NUM_MISC_DISABLES 4
static TREE_SETTING miscDisables[NUM_MISC_DISABLES] = {
    {"Thread Safety Checks", "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT", nullptr},
    {"Handle Wrapping", "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT", nullptr},
    {"Object Lifetime Validation", "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT", nullptr},
    {"Stateless Parameter Checks", "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT", nullptr}};

#define NUM_BEST_PRACTICES 2
static TREE_SETTING bestPractices[NUM_BEST_PRACTICES] = {
    {"Best Practices Warning Checks", "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT", nullptr},
    {"ARM-Specific Validation", "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM", nullptr}};

/////////////////////////////////////////////////////////////////////////////////
/// \brief GetSettingDetails
/// \param qsSetting
/// \param url
/// \return
/// There aren't many... but consider moving this to a QHash type lookup
QString GetSettingDetails(QString qsSetting, QString &url) {
    QString retString = "";

    for (int i = 0; i < 17; i++) {
        if (qsSetting == lookupTable[i].setting) {
            url = lookupTable[i].url;
            retString = lookupTable[i].description;
            if (!lookupTable[i].url.isEmpty()) {
                retString += "\n<a href =\"";
                retString += lookupTable[i].url;
                retString += "\">Click for online documentation</a>";
            }
            return retString;
        }
    }

    return retString;
}

///////////////////////////////////////////////////////////////////////////////
KhronosSettingsAdvanced::KhronosSettingsAdvanced(QTreeWidget *pMainTree, QTreeWidgetItem *parent,
                                                 QVector<LayerSettings *> &layerSettings) {
    pMainTreeWidget = pMainTree;
    pMainParent = parent;

    // Find the enables
    pEnables = nullptr;
    for (int i = 0; i < layerSettings.size(); i++)
        if (layerSettings[i]->settingsName == QString("enables")) {
            pEnables = layerSettings[i];
            break;
        }
    Q_ASSERT(pEnables != nullptr);

    // Find the disables
    pDisables = nullptr;
    for (int i = 0; i < layerSettings.size(); i++)
        if (layerSettings[i]->settingsName == QString("disables")) {
            pDisables = layerSettings[i];
            break;
        }
    Q_ASSERT(pDisables != nullptr);

    ///////////////////////////////////////////////////////////////
    /// \brief pCoreChecks
    /// If this is off, everyone below is disabled
    bool bCoreValidationDisabled = pDisables->settingsValue.contains("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");
    pCoreChecksParent = new QTreeWidgetItem();
    pCoreChecksParent->setText(0, "Core Validation Checks");
    pCoreChecksParent->setCheckState(0, (bCoreValidationDisabled) ? Qt::Unchecked : Qt::Checked);
    parent->addChild(pCoreChecksParent);

    QTreeWidgetItem *pChild;
    for (int i = 0; i < NUM_CORE_CHECKS; i++) {
        pChild = new QTreeWidgetItem();
        pChild->setText(0, coreChecks[i].prompt);
        if (pDisables->settingsValue.contains(coreChecks[i].token) || bCoreValidationDisabled)
            pChild->setCheckState(0, Qt::Unchecked);
        else
            pChild->setCheckState(0, Qt::Checked);

        if (bCoreValidationDisabled) pChild->setFlags(pChild->flags() & ~Qt::ItemIsEnabled);

        pCoreChecksParent->addChild(pChild);
        coreChecks[i].pItem = pChild;
    }

    ///////////////////////////////////////////////////////////////
    // Miscellaneous disables
    QTreeWidgetItem *pItem;
    for (int i = 0; i < NUM_MISC_DISABLES; i++) {
        pItem = new QTreeWidgetItem();
        pItem->setText(0, miscDisables[i].prompt);
        if (pDisables->settingsValue.contains(miscDisables[i].token))
            pItem->setCheckState(0, Qt::Unchecked);
        else
            pItem->setCheckState(0, Qt::Checked);

        parent->addChild(pItem);
        miscDisables[i].pItem = pItem;
    }

    ///////////////////////////////////////////////////////////////
    // Now for the GPU specific stuff
    bool bGPUStuff = pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT") ||
                     pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");

    pShaderBasedBox = new QTreeWidgetItem();
    pShaderBasedBox->setText(0, "Shader-Based Validation");
    if (bGPUStuff)
        pShaderBasedBox->setCheckState(0, Qt::Checked);
    else
        pShaderBasedBox->setCheckState(0, Qt::Unchecked);

    parent->addChild(pShaderBasedBox);

    pGPUAssistBox = new QTreeWidgetItem();
    pGPUAssistBox->setText(0, "     GPU-Assisted");
    pShaderBasedBox->addChild(pGPUAssistBox);

    pGPURadio = new QRadioButton();
    pMainTreeWidget->setItemWidget(pGPUAssistBox, 0, pGPURadio);

    pReserveBox = new QTreeWidgetItem();
    pReserveBox->setText(0, "Reserve Descriptor Set Binding");
    if (pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT"))
        pReserveBox->setCheckState(0, Qt::Checked);
    else
        pReserveBox->setCheckState(0, Qt::Unchecked);

    pGPUAssistBox->addChild(pReserveBox);

    pDebugPrintfBox = new QTreeWidgetItem();
    pDebugPrintfBox->setText(0, "     Debug printf");
    pShaderBasedBox->addChild(pDebugPrintfBox);

    pDebugRadio = new QRadioButton();
    pMainTreeWidget->setItemWidget(pDebugPrintfBox, 0, pDebugRadio);
    if (pEnables->settingsValue.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT")) {
        pDebugRadio->setChecked(true);
        pReserveBox->setFlags(pReserveBox->flags() & ~Qt::ItemIsEnabled);
    } else
        pGPURadio->setChecked(true);

    if (!bGPUStuff) {
        pDebugRadio->setEnabled(false);
        pGPURadio->setEnabled(false);
        pDebugPrintfBox->setFlags(pDebugPrintfBox->flags() & ~Qt::ItemIsEnabled);
        pReserveBox->setFlags(pReserveBox->flags() & ~Qt::ItemIsEnabled);
        pGPUAssistBox->setFlags(pGPUAssistBox->flags() & ~Qt::ItemIsEnabled);
    }

    //////////////////////////////////////////////////////////////////////
    // Best Practices - one parent/child, but we want to be able
    // to go back to these
    pChild = new QTreeWidgetItem();
    pChild->setText(0, bestPractices[1].prompt);
    if (pEnables->settingsValue.contains(bestPractices[1].token))
        pChild->setCheckState(0, Qt::Checked);
    else
        pChild->setCheckState(0, Qt::Unchecked);

    pItem = new QTreeWidgetItem();
    pItem->setText(0, bestPractices[0].prompt);
    if (pEnables->settingsValue.contains(bestPractices[0].token))
        pItem->setCheckState(0, Qt::Checked);
    else {
        pItem->setCheckState(0, Qt::Unchecked);
        pChild->setFlags(pChild->flags() & ~Qt::ItemIsEnabled);
    }

    bestPractices[0].pItem = pItem;
    parent->addChild(pItem);

    pItem->addChild(pChild);
    bestPractices[1].pItem = pChild;

    connect(pMainTreeWidget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(pMainTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));
    connect(pGPURadio, SIGNAL(toggled(bool)), this, SLOT(gpuToggled(bool)));
    connect(pDebugRadio, SIGNAL(toggled(bool)), this, SLOT(printfToggled(bool)));
}

KhronosSettingsAdvanced::~KhronosSettingsAdvanced() {}

///////////////////////////////////////////////////////////////////////////////
/// \brief KhronosSettingsAdvanced::itemClicked
/// \param pItem
/// \param nColumn
/// A tree item was selected, display the help information to the side
/// This is embarrasingly brute force... temporary sketch in...
void KhronosSettingsAdvanced::itemClicked(QTreeWidgetItem *pItem, int nColumn) {
    (void)nColumn;
    QString description;
    QString url;

    emit settingChanged();

    // Check for core validation checks
    if (pItem == pCoreChecksParent) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    for (int i = 0; i < NUM_CORE_CHECKS; i++) {
        if (pItem == coreChecks[i].pItem) {
            description = GetSettingDetails(coreChecks[i].token, url);
            if (!description.isEmpty()) {
                // ui->labelSettingInfo->setText(description);
                return;
            }
        }
    }

    // Check for miscellaneous disables
    for (int i = 0; i < NUM_MISC_DISABLES; i++) {
        if (pItem == miscDisables[i].pItem) {
            description = GetSettingDetails(miscDisables[i].token, url);
            if (!description.isEmpty()) {
                // ui->labelSettingInfo->setText(description);
                return;
            }
        }
    }

    // Best practices
    for (int i = 0; i < NUM_BEST_PRACTICES; i++) {
        if (pItem == bestPractices[i].pItem) {
            description = GetSettingDetails(bestPractices[i].token, url);
            if (!description.isEmpty()) {
                // ui->labelSettingInfo->setText(description);
                return;
            }
        }
    }

    // GPU Stuff
    if (pItem == pGPUAssistBox) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    if (pItem == pReserveBox) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    if (pItem == pDebugPrintfBox) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    // Not found?
    // ui->labelSettingInfo->setText(tr("Click on a setting for additional information."));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief KhronosSettingsAdvanced::itemChanged
/// \param pItem
/// \param nColumn
/// Something was checked or unchecked
void KhronosSettingsAdvanced::itemChanged(QTreeWidgetItem *pItem, int nColumn) {
    if (nColumn != 0) return;

    emit settingChanged();

    // Anything toggled needs to be selected in order to work well with the
    // information display.
    pMainTreeWidget->setCurrentItem(pItem);

    // Best Practices?
    pMainTreeWidget->blockSignals(true);
    if (pItem == bestPractices[0].pItem) {
        // If we turned it on, we need to enable AMD
        if (pItem->checkState(0) == Qt::Checked) {
            bestPractices[1].pItem->setFlags(bestPractices[1].pItem->flags() | Qt::ItemIsEnabled);
        } else {
            bestPractices[1].pItem->setFlags(bestPractices[1].pItem->flags() & ~Qt::ItemIsEnabled);
            bestPractices[1].pItem->setCheckState(0, Qt::Unchecked);
        }
    }

    // Core Validation.
    if (pItem == pCoreChecksParent) {
        // If checked, enable all below it.
        if (pItem->checkState(0) == Qt::Checked) {
            for (int i = 0; i < 7; i++) {
                coreChecks[i].pItem->setFlags(coreChecks[i].pItem->flags() | Qt::ItemIsEnabled);
                coreChecks[i].pItem->setCheckState(0, Qt::Checked);
            }
        } else {  // If unchecked both clear, and disable all below it
            for (int i = 0; i < 7; i++) {
                coreChecks[i].pItem->setFlags(coreChecks[i].pItem->flags() & ~Qt::ItemIsEnabled);
                coreChecks[i].pItem->setCheckState(0, Qt::Unchecked);
            }
        }
    }

    // Shader based stuff
    if (pItem == pShaderBasedBox) {  // Just enable/disable the items below it
        if (pShaderBasedBox->checkState(0) == Qt::Checked) {
            pDebugRadio->setEnabled(true);
            pGPURadio->setEnabled(true);
            pDebugPrintfBox->setFlags(pDebugPrintfBox->flags() | Qt::ItemIsEnabled);
            pReserveBox->setFlags(pReserveBox->flags() | Qt::ItemIsEnabled);
            pGPUAssistBox->setFlags(pGPUAssistBox->flags() | Qt::ItemIsEnabled);
        } else {
            pDebugRadio->setEnabled(false);
            pGPURadio->setEnabled(false);
            pDebugPrintfBox->setFlags(pDebugPrintfBox->flags() & ~Qt::ItemIsEnabled);
            pReserveBox->setFlags(pReserveBox->flags() & ~Qt::ItemIsEnabled);
            pGPUAssistBox->setFlags(pGPUAssistBox->flags() & ~Qt::ItemIsEnabled);
        }
    }

    // Debug printf or GPU based also enables/disables the checkbox for reserving a slot
    if (pItem == pDebugPrintfBox && pDebugRadio->isChecked()) pReserveBox->setFlags(pReserveBox->flags() & ~Qt::ItemIsEnabled);

    pMainTreeWidget->blockSignals(false);

    // Check for performance issues. There are three different variations, and I think
    // we should alert the user to all three exactly/explicitly to what they are
    QSettings settings;

    if (pCoreChecksParent->checkState(0) == Qt::Checked && pShaderBasedBox->checkState(0) == Qt::Checked ||
        pCoreChecksParent->checkState(0) == Qt::Checked && bestPractices[0].pItem->checkState(0) == Qt::Checked ||
        pShaderBasedBox->checkState(0) == Qt::Checked && bestPractices[0].pItem->checkState(0) == Qt::Checked) {
        if (settings.value("VKCONFIG_WARN_CORE_SHADER_IGNORE").toBool() == false) {
            QMessageBox alert(pMainTreeWidget);
            alert.setText(
                "<i>Core Validation</i>, <i>Shader Based Validation</i> and <i>Best Pracstices Warnings</i> require a state "
                "tracking object each.\n\n<br><br> Combining two of these options will result in high performance degradation.");
            alert.setWindowTitle(tr("High Validation Layer Overhead"));
            alert.setIcon(QMessageBox::Warning);
            alert.setCheckBox(new QCheckBox(tr("Do not show again.")));
            alert.exec();
            if (alert.checkBox()->isChecked()) settings.setValue("VKCONFIG_WARN_CORE_SHADER_IGNORE", true);
            return;
        }
    }

    CollectSettings();
}

void KhronosSettingsAdvanced::gpuToggled(bool toggle) {
    if (toggle) pReserveBox->setFlags(pReserveBox->flags() | Qt::ItemIsEnabled);

    CollectSettings();
    emit settingChanged();
}

void KhronosSettingsAdvanced::printfToggled(bool toggle) {
    if (toggle) {
        pReserveBox->setFlags(pReserveBox->flags() & ~Qt::ItemIsEnabled);
        pReserveBox->setCheckState(0, Qt::Unchecked);
    }

    CollectSettings();
    emit settingChanged();
}

///////////////////////////////////////////////////////////////////////////
/// \brief KhronosSettingsAdvanced::CollectSettings
/// \return
/// Collect all the settings
bool KhronosSettingsAdvanced::CollectSettings(void) {
    QString enables;
    QString disables;

    // GPU stuff
    if (pShaderBasedBox->checkState(0) == Qt::Checked) {
        if (pGPURadio->isChecked()) {
            enables = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";

            if (pReserveBox->checkState(0) == Qt::Checked)
                enables += ",VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT";
        } else  // Debug printf must be checked
            enables = "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT";
    }

    // The rest is cake... just reap the controls

    // Best practice enables
    for (int i = 0; i < NUM_BEST_PRACTICES; i++)
        if (bestPractices[i].pItem->checkState(0) == Qt::Checked) AddString(enables, bestPractices[i].token);

    ///////////////////////////////////////////////////////
    // Everything else is a disable. Remember, these are backwards
    // because they are exposed to the end user as an enable.
    // If they are NOT checked, we add them to disables
    for (int i = 0; i < NUM_MISC_DISABLES; i++) {
        if (miscDisables[i].pItem->checkState(0) != Qt::Checked) AddString(disables, miscDisables[i].token);
    }

    // Core checks. If unchecked, then individual ones might still be checked
    if (pCoreChecksParent->checkState(0) == Qt::Checked) {
        for (int i = 0; i < NUM_CORE_CHECKS; i++)
            if (coreChecks[i].pItem->checkState(0) == Qt::Unchecked) AddString(disables, coreChecks[i].token);
    } else  // Not checked, turn them all off
        AddString(disables, "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");

    pDisables->settingsValue = disables;
    pEnables->settingsValue = enables;

    return true;
}
