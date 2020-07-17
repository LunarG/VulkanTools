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
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "vku.h"
#include "khronossettingsadvanced.h"
#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>

// Keep track of tree/setting correlations
struct TreeSettings {
    QString prompt;
    QString token;
    QTreeWidgetItem *item;
};

// Lookup table for settings descriptions and URL's
struct TableEntry {
    QString setting;
    QString description;
    QString url;
};

const TableEntry lookup_table[] = {
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

    {"VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION",
     "Enables Vulkan synchronization validation. It is intended to enable flagging error cases where synchronization primitives "
     "are "
     "missing, but also allow for visualization of such primitives and provide for recommendations on optimal use of "
     "synchronization "
     "for any given combination of Vulkan commands and resources.",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/synchronization.md"},
};

static TreeSettings coreChecks[] = {{"Image Layout Validation", "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION", nullptr},
                                    {"Command Buffer State", "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE", nullptr},
                                    {"Object in Use", "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE", nullptr},
                                    {"Query Validation", "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION", nullptr},
                                    {"Idle Descriptor Set", "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET", nullptr},
                                    {"Shader Validation Checks", "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT", nullptr},
                                    {"Push Constant Range", "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE", nullptr}};

static TreeSettings miscDisables[] = {{"Thread Safety Checks", "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT", nullptr},
                                      {"Handle Wrapping", "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT", nullptr},
                                      {"Object Lifetime Validation", "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT", nullptr},
                                      {"Stateless Parameter Checks", "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT", nullptr}};

static TreeSettings bestPractices[] = {
    {"Best Practices Warning Checks", "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT", nullptr},
    {"ARM-Specific Validation", "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM", nullptr}};

static TreeSettings syncChecks[] = {{"Synchronization Checks", "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION", nullptr}};

/////////////////////////////////////////////////////////////////////////////////
/// There aren't many... but consider moving this to a QHash type lookup
QString GetSettingDetails(QString qsSetting, QString &url) {
    QString retString = "";

    for (std::size_t i = 0, n = vku::countof(lookup_table); i < n; i++) {
        if (qsSetting == lookup_table[i].setting) {
            url = lookup_table[i].url;
            retString = lookup_table[i].description;
            if (!lookup_table[i].url.isEmpty()) {
                retString += "\n<a href =\"";
                retString += lookup_table[i].url;
                retString += "\">Click for online documentation</a>";
            }
            return retString;
        }
    }

    return retString;
}

///////////////////////////////////////////////////////////////////////////////
KhronosSettingsAdvanced::KhronosSettingsAdvanced(QTreeWidget *main_tree, QTreeWidgetItem *parent,
                                                 QVector<LayerSettings *> &layer_settings) {
    main_tree_widget_ = main_tree;
    main_parent_ = parent;

    // Find the enables
    enables_ = nullptr;
    for (int i = 0; i < layer_settings.size(); i++)
        if (layer_settings[i]->settings_name == QString("enables")) {
            enables_ = layer_settings[i];
            break;
        }
    Q_ASSERT(enables_ != nullptr);

    // Find the disables
    disables_ = nullptr;
    for (int i = 0; i < layer_settings.size(); i++)
        if (layer_settings[i]->settings_name == QString("disables")) {
            disables_ = layer_settings[i];
            break;
        }
    Q_ASSERT(disables_ != nullptr);

    ///////////////////////////////////////////////////////////////
    /// If this is off, everyone below is disabled
    bool core_validation_disabled = disables_->settings_value.contains("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");
    core_checks_parent_ = new QTreeWidgetItem();
    core_checks_parent_->setText(0, "Core Validation Checks");
    core_checks_parent_->setCheckState(0, (core_validation_disabled) ? Qt::Unchecked : Qt::Checked);
    parent->addChild(core_checks_parent_);

    QTreeWidgetItem *core_child_item;
    for (std::size_t i = 0, n = vku::countof(coreChecks); i < n; i++) {
        core_child_item = new QTreeWidgetItem();
        core_child_item->setText(0, coreChecks[i].prompt);
        if (disables_->settings_value.contains(coreChecks[i].token) || core_validation_disabled)
            core_child_item->setCheckState(0, Qt::Unchecked);
        else
            core_child_item->setCheckState(0, Qt::Checked);

        if (core_validation_disabled) core_child_item->setFlags(core_child_item->flags() & ~Qt::ItemIsEnabled);

        core_checks_parent_->addChild(core_child_item);
        coreChecks[i].item = core_child_item;
    }

    ///////////////////////////////////////////////////////////////
    // Miscellaneous disables
    QTreeWidgetItem *item;
    for (std::size_t i = 0, n = vku::countof(miscDisables); i < n; i++) {
        item = new QTreeWidgetItem();
        item->setText(0, miscDisables[i].prompt);
        if (disables_->settings_value.contains(miscDisables[i].token))
            item->setCheckState(0, Qt::Unchecked);
        else
            item->setCheckState(0, Qt::Checked);

        parent->addChild(item);
        miscDisables[i].item = item;
    }

    ///////////////////////////////////////////////////////////////
    // Now for the GPU specific stuff
    bool shader_based = enables_->settings_value.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT") ||
                        enables_->settings_value.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");

    shader_based_box_ = new QTreeWidgetItem();
    shader_based_box_->setText(0, "Shader-Based Validation");
    if (shader_based)
        shader_based_box_->setCheckState(0, Qt::Checked);
    else
        shader_based_box_->setCheckState(0, Qt::Unchecked);

    parent->addChild(shader_based_box_);

    gpu_assisted_box_ = new QTreeWidgetItem();
    gpu_assisted_box_->setText(0, "     GPU-Assisted");
    shader_based_box_->addChild(gpu_assisted_box_);

    gpu_assisted_ratio_ = new QRadioButton();
    main_tree_widget_->setItemWidget(gpu_assisted_box_, 0, gpu_assisted_ratio_);

    reserve_box_ = new QTreeWidgetItem();
    reserve_box_->setText(0, "Reserve Descriptor Set Binding");
    if (enables_->settings_value.contains("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT"))
        reserve_box_->setCheckState(0, Qt::Checked);
    else
        reserve_box_->setCheckState(0, Qt::Unchecked);

    gpu_assisted_box_->addChild(reserve_box_);

    debug_printf_box_ = new QTreeWidgetItem();
    debug_printf_box_->setText(0, "     Debug printf");
    shader_based_box_->addChild(debug_printf_box_);

    debug_printf_radio_ = new QRadioButton();
    main_tree_widget_->setItemWidget(debug_printf_box_, 0, debug_printf_radio_);
    if (enables_->settings_value.contains("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT")) {
        debug_printf_radio_->setChecked(true);
        reserve_box_->setFlags(reserve_box_->flags() & ~Qt::ItemIsEnabled);
    } else
        gpu_assisted_ratio_->setChecked(true);

    if (!shader_based) {
        debug_printf_radio_->setEnabled(false);
        gpu_assisted_ratio_->setEnabled(false);
        debug_printf_box_->setFlags(debug_printf_box_->flags() & ~Qt::ItemIsEnabled);
        reserve_box_->setFlags(reserve_box_->flags() & ~Qt::ItemIsEnabled);
        gpu_assisted_box_->setFlags(gpu_assisted_box_->flags() & ~Qt::ItemIsEnabled);
    }

    ///////////////////////////////////////////////////////////////
    // Synchronization

    bool synchronization = enables_->settings_value.contains("VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION");
    synchronization_box_ = new QTreeWidgetItem();
    synchronization_box_->setText(0, syncChecks[0].prompt);
    if (synchronization)
        synchronization_box_->setCheckState(0, Qt::Checked);
    else
        synchronization_box_->setCheckState(0, Qt::Unchecked);
    parent->addChild(synchronization_box_);

    syncChecks[0].item = synchronization_box_;

    //////////////////////////////////////////////////////////////////////
    // Best Practices - one parent/child, but we want to be able
    // to go back to these
    core_child_item = new QTreeWidgetItem();
    core_child_item->setText(0, bestPractices[1].prompt);
    if (enables_->settings_value.contains(bestPractices[1].token))
        core_child_item->setCheckState(0, Qt::Checked);
    else
        core_child_item->setCheckState(0, Qt::Unchecked);

    item = new QTreeWidgetItem();
    item->setText(0, bestPractices[0].prompt);
    if (enables_->settings_value.contains(bestPractices[0].token))
        item->setCheckState(0, Qt::Checked);
    else {
        item->setCheckState(0, Qt::Unchecked);
        core_child_item->setFlags(core_child_item->flags() & ~Qt::ItemIsEnabled);
    }

    bestPractices[0].item = item;
    parent->addChild(item);

    item->addChild(core_child_item);
    bestPractices[1].item = core_child_item;

    connect(main_tree_widget_, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(main_tree_widget_, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));
    connect(gpu_assisted_ratio_, SIGNAL(toggled(bool)), this, SLOT(gpuToggled(bool)));
    connect(debug_printf_radio_, SIGNAL(toggled(bool)), this, SLOT(printfToggled(bool)));
}

KhronosSettingsAdvanced::~KhronosSettingsAdvanced() {}

///////////////////////////////////////////////////////////////////////////////
/// A tree item was selected, display the help information to the side
/// This is embarrasingly brute force... temporary sketch in...
void KhronosSettingsAdvanced::itemClicked(QTreeWidgetItem *item, int column) {
    (void)column;
    QString description;
    QString url;

    emit settingChanged();

    // Check for core validation checks
    if (item == core_checks_parent_) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    for (std::size_t i = 0, n = vku::countof(coreChecks); i < n; i++) {
        if (item == coreChecks[i].item) {
            description = GetSettingDetails(coreChecks[i].token, url);
            if (!description.isEmpty()) {
                // ui->labelSettingInfo->setText(description);
                return;
            }
        }
    }

    // Check for miscellaneous disables
    for (std::size_t i = 0, n = vku::countof(miscDisables); i < n; i++) {
        if (item == miscDisables[i].item) {
            description = GetSettingDetails(miscDisables[i].token, url);
            if (!description.isEmpty()) {
                // ui->labelSettingInfo->setText(description);
                return;
            }
        }
    }

    // Best practices
    for (std::size_t i = 0, n = vku::countof(bestPractices); i < n; i++) {
        if (item == bestPractices[i].item) {
            description = GetSettingDetails(bestPractices[i].token, url);
            if (!description.isEmpty()) {
                // ui->labelSettingInfo->setText(description);
                return;
            }
        }
    }

    // GPU Stuff
    if (item == gpu_assisted_box_) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    if (item == reserve_box_) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    if (item == debug_printf_box_) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    if (item == synchronization_box_) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION", url);
        // ui->labelSettingInfo->setText(description);
        return;
    }

    // Not found?
    // ui->labelSettingInfo->setText(tr("Click on a setting for additional information."));
}

///////////////////////////////////////////////////////////////////////////////
/// Something was checked or unchecked
void KhronosSettingsAdvanced::itemChanged(QTreeWidgetItem *item, int nColumn) {
    if (nColumn != 0) return;

    emit settingChanged();

    // Anything toggled needs to be selected in order to work well with the
    // information display.
    main_tree_widget_->setCurrentItem(item);

    // Best Practices
    main_tree_widget_->blockSignals(true);
    if (item == bestPractices[0].item) {
        // If we turned it on, we need to enable AMD
        if (item->checkState(0) == Qt::Checked) {
            bestPractices[1].item->setFlags(bestPractices[1].item->flags() | Qt::ItemIsEnabled);
        } else {
            bestPractices[1].item->setFlags(bestPractices[1].item->flags() & ~Qt::ItemIsEnabled);
            bestPractices[1].item->setCheckState(0, Qt::Unchecked);
        }
    }

    // Core Validation.
    if (item == core_checks_parent_) {
        // If checked, enable all below it.
        if (item->checkState(0) == Qt::Checked) {
            for (int i = 0; i < 7; i++) {
                coreChecks[i].item->setFlags(coreChecks[i].item->flags() | Qt::ItemIsEnabled);
                coreChecks[i].item->setCheckState(0, Qt::Checked);
            }
        } else {  // If unchecked both clear, and disable all below it
            for (int i = 0; i < 7; i++) {
                coreChecks[i].item->setFlags(coreChecks[i].item->flags() & ~Qt::ItemIsEnabled);
                coreChecks[i].item->setCheckState(0, Qt::Unchecked);
            }
        }
    }

    // Shader based stuff
    if (item == shader_based_box_) {  // Just enable/disable the items below it
        if (shader_based_box_->checkState(0) == Qt::Checked) {
            debug_printf_radio_->setEnabled(true);
            gpu_assisted_ratio_->setEnabled(true);
            debug_printf_box_->setFlags(debug_printf_box_->flags() | Qt::ItemIsEnabled);
            reserve_box_->setFlags(reserve_box_->flags() | Qt::ItemIsEnabled);
            gpu_assisted_box_->setFlags(gpu_assisted_box_->flags() | Qt::ItemIsEnabled);
        } else {
            debug_printf_radio_->setEnabled(false);
            gpu_assisted_ratio_->setEnabled(false);
            debug_printf_box_->setFlags(debug_printf_box_->flags() & ~Qt::ItemIsEnabled);
            reserve_box_->setFlags(reserve_box_->flags() & ~Qt::ItemIsEnabled);
            gpu_assisted_box_->setFlags(gpu_assisted_box_->flags() & ~Qt::ItemIsEnabled);
        }
    }

    // Debug printf or GPU based also enables/disables the checkbox for reserving a slot
    if (item == debug_printf_box_ && debug_printf_radio_->isChecked())
        reserve_box_->setFlags(reserve_box_->flags() & ~Qt::ItemIsEnabled);

    main_tree_widget_->blockSignals(false);

    // Check for performance issues. There are three different variations, and I think
    // we should alert the user to all three exactly/explicitly to what they are
    QSettings settings;

    if (core_checks_parent_->checkState(0) == Qt::Checked && shader_based_box_->checkState(0) == Qt::Checked ||
        core_checks_parent_->checkState(0) == Qt::Checked && bestPractices[0].item->checkState(0) == Qt::Checked ||
        shader_based_box_->checkState(0) == Qt::Checked && bestPractices[0].item->checkState(0) == Qt::Checked) {
        if (settings.value("VKCONFIG_WARN_CORE_SHADER_IGNORE").toBool() == false) {
            QMessageBox alert(main_tree_widget_);
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
    if (toggle) reserve_box_->setFlags(reserve_box_->flags() | Qt::ItemIsEnabled);

    CollectSettings();
    emit settingChanged();
}

void KhronosSettingsAdvanced::printfToggled(bool toggle) {
    if (toggle) {
        reserve_box_->setFlags(reserve_box_->flags() & ~Qt::ItemIsEnabled);
        reserve_box_->setCheckState(0, Qt::Unchecked);
    }

    CollectSettings();
    emit settingChanged();
}

///////////////////////////////////////////////////////////////////////////
/// Collect all the settings
bool KhronosSettingsAdvanced::CollectSettings() {
    QString enables;
    QString disables;

    // GPU stuff
    if (shader_based_box_->checkState(0) == Qt::Checked) {
        if (gpu_assisted_ratio_->isChecked()) {
            enables = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";

            if (reserve_box_->checkState(0) == Qt::Checked)
                enables += ",VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT";
        } else  // Debug printf must be checked
            enables = "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT";
    }

    // The rest is cake... just reap the controls

    // Best practice enables
    for (std::size_t i = 0, n = vku::countof(bestPractices); i < n; i++)
        if (bestPractices[i].item->checkState(0) == Qt::Checked) AddString(enables, bestPractices[i].token);

    // Sync Validation
    for (std::size_t i = 0, n = vku::countof(syncChecks); i < n; i++)
        if (syncChecks[0].item->checkState(0) == Qt::Checked) AddString(enables, syncChecks[i].token);

    ///////////////////////////////////////////////////////
    // Everything else is a disable. Remember, these are backwards
    // because they are exposed to the end user as an enable.
    // If they are NOT checked, we add them to disables
    for (std::size_t i = 0, n = vku::countof(miscDisables); i < n; i++) {
        if (miscDisables[i].item->checkState(0) != Qt::Checked) AddString(disables, miscDisables[i].token);
    }

    // Core checks. If unchecked, then individual ones might still be checked
    if (core_checks_parent_->checkState(0) == Qt::Checked) {
        for (std::size_t i = 0, n = vku::countof(coreChecks); i < n; i++)
            if (coreChecks[i].item->checkState(0) == Qt::Unchecked) AddString(disables, coreChecks[i].token);
    } else  // Not checked, turn them all off
        AddString(disables, "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");

    disables_->settings_value = disables;
    enables_->settings_value = enables;

    return true;
}
