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

#include "settings_validation_areas.h"
#include "configurator.h"

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>

// Keep track of tree/setting correlations
struct TreeSettings {
    const char *prompt;
    const char *token;
    const char *description;
    QTreeWidgetItem *item;
};

// Lookup table for settings descriptions and URL's
struct TableEntry {
    const char *setting;
    const char *description;
    const char *url;
};

static const TableEntry lookup_table[] = {
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

    {"VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT",
     "Enables Vulkan synchronization validation. It is intended to enable flagging error cases where synchronization primitives "
     "are "
     "missing, but also allow for visualization of such primitives and provide for recommendations on optimal use of "
     "synchronization "
     "for any given combination of Vulkan commands and resources.",
     "https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/docs/synchronization.md"},
};

static TreeSettings core_checks[] = {{"Image Layout Validation", "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION", nullptr},
                                     {"Command Buffer State", "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE", nullptr},
                                     {"Object in Use", "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE", nullptr},
                                     {"Query Validation", "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION", nullptr},
                                     {"Idle Descriptor Set", "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET", nullptr},
                                     {"Shader Validation", "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT", nullptr},
                                     {"Push Constant Range", "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE", nullptr}};

static TreeSettings misc_disables[] = {{"Thread Safety", "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT",
                                        "Help with performance to run with thread-checking disabled most of the time, enabling it "
                                        "occasionally for a quick sanity check, or when debugging difficult application behaviors.",
                                        nullptr},
                                       {"Handle Wrapping", "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT",
                                        "Disable this feature if you are running into crashes when authoring new extensions or "
                                        "developing new Vulkan objects/structures",
                                        nullptr},
                                       {"Object Lifetimes", "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT", "", nullptr},
                                       {"Stateless Parameter", "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT", "", nullptr}};

static TreeSettings best_practices[] = {{"Best Practices", "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT", nullptr},
                                        {"ARM-Specific Recommandations", "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM", nullptr}};

static TreeSettings sync_checks[] = {{"Synchronization", "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT", nullptr}};

/// There aren't many... but consider moving this to a QHash type lookup
static QString GetSettingDetails(const QString &setting, QString &url) {
    QString ret_string = "";

    for (std::size_t i = 0, n = countof(lookup_table); i < n; i++) {
        if (setting == lookup_table[i].setting) {
            url = lookup_table[i].url;
            ret_string = lookup_table[i].description;
            if (std::strcmp(lookup_table[i].url, "") == 0) {
                ret_string += "\n<a href =\"";
                ret_string += lookup_table[i].url;
                ret_string += "\">Click for online documentation</a>";
            }
            return ret_string;
        }
    }

    return ret_string;
}

SettingsValidationAreas::SettingsValidationAreas(QTreeWidget *main_tree, QTreeWidgetItem *parent,
                                                 const std::vector<LayerSettingMeta> &settings_meta,
                                                 std::vector<LayerSettingData> &settings_data)
    : _main_tree_widget(main_tree),
      _main_parent(parent),
      _core_checks_parent(nullptr),
      _synchronization_box(nullptr),
      _shader_based_box(nullptr),
      _gpu_assisted_box(nullptr),
      _gpu_assisted_radio(nullptr),
      _gpu_assisted_reserve_box(nullptr),
      _gpu_assisted_oob_box(nullptr),
      _debug_printf_box(nullptr),
      _debug_printf_radio(nullptr),
      _debug_printf_to_stdout(nullptr),
      _debug_printf_verbose(nullptr),
      _debug_printf_buffer_size(nullptr),
      settings_meta(settings_meta),
      settings_data(settings_data) {
    assert(main_tree && parent);

    const bool core_validation_disabled = HasDisable("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");

    _core_checks_parent = new QTreeWidgetItem();
    _core_checks_parent->setText(0, "Core");
    _core_checks_parent->setCheckState(0, core_validation_disabled ? Qt::Unchecked : Qt::Checked);
    parent->addChild(_core_checks_parent);

    for (std::size_t i = 0, n = countof(core_checks); i < n; i++) {
        QTreeWidgetItem *core_child_item = new QTreeWidgetItem();
        core_child_item->setText(0, core_checks[i].prompt);

        if (HasDisable(core_checks[i].token) || core_validation_disabled)
            core_child_item->setCheckState(0, Qt::Unchecked);
        else
            core_child_item->setCheckState(0, Qt::Checked);

        if (core_validation_disabled) core_child_item->setFlags(core_child_item->flags() & ~Qt::ItemIsEnabled);

        _core_checks_parent->addChild(core_child_item);
        core_checks[i].item = core_child_item;
    }

    // Miscellaneous disables
    for (std::size_t i = 0, n = countof(misc_disables); i < n; ++i) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, misc_disables[i].prompt);
        item->setToolTip(0, misc_disables[i].description);
        item->setCheckState(0, HasDisable(misc_disables[i].token) ? Qt::Unchecked : Qt::Checked);
        parent->addChild(item);
        misc_disables[i].item = item;
    }

    const LayerSettingMeta *setting_meta_enables = FindByKey(settings_meta, "enables");

    // Now for the GPU specific stuff
    const bool has_debug_printf = HasEnable("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT");
    const bool has_gpu_assisted = HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");
    const bool shader_based = has_debug_printf || has_gpu_assisted;

    if (VKC_PLATFORM != VKC_PLATFORM_MACOS) {
        if (HasValue(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT")) {
            _shader_based_box = new QTreeWidgetItem();
            _shader_based_box->setText(0, "Shader-Based");
            _shader_based_box->setCheckState(0, shader_based ? Qt::Checked : Qt::Unchecked);
            parent->addChild(_shader_based_box);

            _gpu_assisted_box = new QTreeWidgetItem();
            _gpu_assisted_box->setText(0, "     GPU-Assisted");
            _gpu_assisted_box->setToolTip(0, "Check for API usage errors at shader execution time");
            _shader_based_box->addChild(_gpu_assisted_box);

            _gpu_assisted_radio = new QRadioButton();
            _main_tree_widget->setItemWidget(_gpu_assisted_box, 0, _gpu_assisted_radio);

            const bool reserve_binding_slot = HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
            _gpu_assisted_reserve_box = new QTreeWidgetItem();
            _gpu_assisted_reserve_box->setText(0, "Reserve Descriptor Set Binding Slot");
            _gpu_assisted_reserve_box->setCheckState(0, reserve_binding_slot ? Qt::Checked : Qt::Unchecked);
            _gpu_assisted_box->addChild(_gpu_assisted_reserve_box);

            _gpu_assisted_oob_box = CreateSettingWidget(_gpu_assisted_box, "gpuav_buffer_oob");

            _debug_printf_box = new QTreeWidgetItem();
            _debug_printf_box->setText(0, "     Debug printf");
            _debug_printf_box->setToolTip(
                0, "Debug shader code by \"printing\" any values of interest to the debug callback or stdout");
            _shader_based_box->addChild(_debug_printf_box);

            _debug_printf_to_stdout = CreateSettingWidget(_debug_printf_box, "printf_to_stdout");
            _debug_printf_verbose = CreateSettingWidget(_debug_printf_box, "printf_verbose");
            _debug_printf_buffer_size = CreateSettingWidget(_debug_printf_box, "printf_buffer_size");

            _debug_printf_radio = new QRadioButton();
            _main_tree_widget->setItemWidget(_debug_printf_box, 0, _debug_printf_radio);

            if (!shader_based) {
                _debug_printf_radio->setEnabled(false);
                EnableSettingWidget(_debug_printf_box, false);
                EnableSettingWidget(_debug_printf_to_stdout, false);
                EnableSettingWidget(_debug_printf_verbose, false);
                EnableSettingWidget(_debug_printf_buffer_size, false);

                _gpu_assisted_radio->setEnabled(false);
                EnableSettingWidget(_gpu_assisted_box, false);
                EnableSettingWidget(_gpu_assisted_reserve_box, false);
                EnableSettingWidget(_gpu_assisted_oob_box, false);
            } else if (HasEnable("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT")) {
                _debug_printf_radio->setChecked(true);
                EnableSettingWidget(_gpu_assisted_reserve_box, false);
                EnableSettingWidget(_gpu_assisted_oob_box, false);
            } else {
                _gpu_assisted_radio->setChecked(true);
                EnableSettingWidget(_debug_printf_to_stdout, false);
                EnableSettingWidget(_debug_printf_verbose, false);
                EnableSettingWidget(_debug_printf_buffer_size, false);
            }
        } else {
            _gpu_assisted_box = new QTreeWidgetItem();
            _gpu_assisted_box->setText(0, "GPU-Assisted");
            _gpu_assisted_box->setToolTip(0, "Check for API usage errors at shader execution time");
            _gpu_assisted_box->setCheckState(
                0, HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT") ? Qt::Checked : Qt::Unchecked);
            parent->addChild(_gpu_assisted_box);

            _gpu_assisted_reserve_box = new QTreeWidgetItem();
            _gpu_assisted_reserve_box->setText(0, "Reserve Descriptor Set Binding Slot");
            _gpu_assisted_reserve_box->setCheckState(
                0, HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT") ? Qt::Checked : Qt::Unchecked);
            _gpu_assisted_box->addChild(_gpu_assisted_reserve_box);

            EnableSettingWidget(_gpu_assisted_reserve_box, _gpu_assisted_box->checkState(0) == Qt::Checked);
        }
    }

    // Synchronization
    if (HasValue(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT")) {
        const bool synchronization = HasEnable(sync_checks[0].token);
        _synchronization_box = new QTreeWidgetItem();
        _synchronization_box->setText(0, sync_checks[0].prompt);
        _synchronization_box->setToolTip(
            0,
            "Identify resource access conflicts due to missing or incorrect synchronization operations between actions (Draw, "
            "Copy, Dispatch, Blit) reading or writing the same regions of memory");
        _synchronization_box->setCheckState(0, synchronization ? Qt::Checked : Qt::Unchecked);
        parent->addChild(_synchronization_box);
        sync_checks[0].item = _synchronization_box;
    }

    // Best Practices - one parent/child, but we want to be able to go back to these
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, best_practices[0].prompt);
    item->setToolTip(0,
                     "Highlight potential performance issues, questionable usage patterns, items not specifically prohibited by "
                     "the specification but that may lead to application problems.");
    item->setCheckState(0, HasEnable(best_practices[0].token) ? Qt::Checked : Qt::Unchecked);
    parent->addChild(item);
    best_practices[0].item = item;

    // ARM best practices
    if (HasValue(setting_meta_enables->enum_values, "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM")) {
        QTreeWidgetItem *core_child_item = new QTreeWidgetItem();
        core_child_item->setText(0, best_practices[1].prompt);
        core_child_item->setCheckState(0, HasEnable(best_practices[1].token) ? Qt::Checked : Qt::Unchecked);
        EnableSettingWidget(core_child_item, HasEnable(best_practices[0].token));
        item->addChild(core_child_item);
        best_practices[1].item = core_child_item;
    }

    connect(_main_tree_widget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(_main_tree_widget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));

    if (VKC_PLATFORM != VKC_PLATFORM_MACOS) {
        connect(_gpu_assisted_radio, SIGNAL(toggled(bool)), this, SLOT(gpuToggled(bool)));
        connect(_debug_printf_radio, SIGNAL(toggled(bool)), this, SLOT(printfToggled(bool)));
    }
}

/// A tree item was selected, display the help information to the side
/// This is embarrasingly brute force... temporary sketch in...
void SettingsValidationAreas::itemClicked(QTreeWidgetItem *item, int column) {
    (void)column;
    QString description;
    QString url;

    emit settingChanged();

    // Check for core validation checks
    if (item == _core_checks_parent) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT", url);
        return;
    }

    for (std::size_t i = 0, n = countof(core_checks); i < n; ++i) {
        if (item == core_checks[i].item) {
            description = GetSettingDetails(core_checks[i].token, url);
            if (!description.isEmpty()) {
                return;
            }
        }
    }

    // Check for miscellaneous disables
    for (std::size_t i = 0, n = countof(misc_disables); i < n; ++i) {
        if (item == misc_disables[i].item) {
            description = GetSettingDetails(misc_disables[i].token, url);
            if (!description.isEmpty()) {
                return;
            }
        }
    }

    // Best practices
    for (std::size_t i = 0, n = countof(best_practices); i < n; ++i) {
        if (item == best_practices[i].item) {
            description = GetSettingDetails(best_practices[i].token, url);
            if (!description.isEmpty()) {
                return;
            }
        }
    }

    // GPU Stuff
    if (item == _gpu_assisted_box) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT", url);
    } else if (item == _gpu_assisted_reserve_box) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT", url);
    } else if (item == _debug_printf_box) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT", url);
    } else if (item == _synchronization_box) {
        description = GetSettingDetails("VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT", url);
    }
}

/// Something was checked or unchecked
void SettingsValidationAreas::itemChanged(QTreeWidgetItem *item, int column) {
    if (column != 0) return;

    emit settingChanged();

    // Anything toggled needs to be selected in order to work well with the
    // information display.
    _main_tree_widget->setCurrentItem(item);

    // Best Practices
    _main_tree_widget->blockSignals(true);
    if (item == best_practices[0].item && best_practices[1].item != nullptr) {
        if (best_practices[1].item && item->checkState(0) == Qt::Checked) {
            best_practices[1].item->setFlags(best_practices[1].item->flags() | Qt::ItemIsEnabled);
        } else {
            best_practices[1].item->setFlags(best_practices[1].item->flags() & ~Qt::ItemIsEnabled);
            best_practices[1].item->setCheckState(0, Qt::Unchecked);
        }
    }

    // Core Validation.
    if (item == _core_checks_parent) {
        // If checked, enable all below it.
        if (item->checkState(0) == Qt::Checked) {
            for (int i = 0; i < 7; i++) {
                core_checks[i].item->setFlags(core_checks[i].item->flags() | Qt::ItemIsEnabled);
                core_checks[i].item->setCheckState(0, Qt::Checked);
            }
        } else {  // If unchecked both clear, and disable all below it
            for (int i = 0; i < 7; i++) {
                core_checks[i].item->setFlags(core_checks[i].item->flags() & ~Qt::ItemIsEnabled);
                core_checks[i].item->setCheckState(0, Qt::Unchecked);
            }
        }
    }

    // Shader based stuff
    if (item == _shader_based_box) {  // Just enable/disable the items below it
        if (_shader_based_box->checkState(0) == Qt::Checked) {
            _debug_printf_radio->setEnabled(true);
            EnableSettingWidget(_debug_printf_box, true);
            EnableSettingWidget(_debug_printf_to_stdout, true);
            EnableSettingWidget(_debug_printf_verbose, true);
            EnableSettingWidget(_debug_printf_buffer_size, true);

            _gpu_assisted_radio->setEnabled(true);
            EnableSettingWidget(_gpu_assisted_box, true);
            EnableSettingWidget(_gpu_assisted_reserve_box, true);
            EnableSettingWidget(_gpu_assisted_oob_box, true);
        } else {
            _debug_printf_radio->setEnabled(false);
            EnableSettingWidget(_debug_printf_box, false);
            EnableSettingWidget(_debug_printf_to_stdout, false);
            EnableSettingWidget(_debug_printf_verbose, false);
            EnableSettingWidget(_debug_printf_buffer_size, false);

            _gpu_assisted_radio->setEnabled(false);
            EnableSettingWidget(_gpu_assisted_box, false);
            EnableSettingWidget(_gpu_assisted_reserve_box, false);
            EnableSettingWidget(_gpu_assisted_oob_box, false);
        }
    } else {
        EnableSettingWidget(_gpu_assisted_reserve_box, _gpu_assisted_box->checkState(0) == Qt::Checked);
    }

    // Debug printf or GPU based also enables/disables the checkbox for reserving a slot
    if (_debug_printf_radio) {
        if (item == _debug_printf_box && _debug_printf_radio->isChecked()) {
            EnableSettingWidget(_gpu_assisted_reserve_box, false);
            EnableSettingWidget(_gpu_assisted_oob_box, false);
        }
    }

    if (_gpu_assisted_radio) {
        if (item == _gpu_assisted_box && _gpu_assisted_radio->isChecked()) {
            EnableSettingWidget(_debug_printf_to_stdout, false);
            EnableSettingWidget(_debug_printf_verbose, false);
            EnableSettingWidget(_debug_printf_buffer_size, false);
        }
    }

    _main_tree_widget->blockSignals(false);

    // Check for performance issues. There are three different variations, and I think
    // we should alert the user to all three exactly/explicitly to what they are

    const bool features_to_run_alone[] = {_core_checks_parent->checkState(0) == Qt::Checked,
                                          _synchronization_box ? _synchronization_box->checkState(0) == Qt::Checked : false,
                                          best_practices[0].item->checkState(0) == Qt::Checked,
                                          _shader_based_box ? _shader_based_box->checkState(0) == Qt::Checked : false};

    int count_enabled_features = 0;
    for (std::size_t i = 0, n = countof(features_to_run_alone); i < n; ++i)
        count_enabled_features += features_to_run_alone[i] ? 1 : 0;

    if (count_enabled_features > 1) {
        QSettings settings;
        if (settings.value("VKCONFIG_WARN_CORE_SHADER_IGNORE").toBool() == false) {
            QMessageBox alert(_main_tree_widget);
            alert.setWindowTitle("High Validation Layer Overhead");
            alert.setText(
                "<i>Core Validation</i>, <i>Shader Based Validation</i>, <i>Synchronization Validation</i> and <i>Best Practices "
                "Warnings</i> require a state tracking object each.");
            alert.setInformativeText("Combining two of these options will result in high performance degradation.");
            alert.setIcon(QMessageBox::Warning);
            alert.setCheckBox(new QCheckBox("Do not show again."));
            alert.exec();
            if (alert.checkBox()->isChecked()) {
                settings.setValue("VKCONFIG_WARN_CORE_SHADER_IGNORE", true);
            }
        }
    }

    CollectSettings();
}

void SettingsValidationAreas::gpuToggled(bool toggle) {
    if (toggle) {
        EnableSettingWidget(_gpu_assisted_reserve_box, true);
        EnableSettingWidget(_gpu_assisted_oob_box, true);

        EnableSettingWidget(_debug_printf_to_stdout, false);
        EnableSettingWidget(_debug_printf_verbose, false);
        EnableSettingWidget(_debug_printf_buffer_size, false);
    }

    CollectSettings();
    emit settingChanged();
}

void SettingsValidationAreas::printfToggled(bool toggle) {
    if (toggle) {
        EnableSettingWidget(_gpu_assisted_reserve_box, false);
        EnableSettingWidget(_gpu_assisted_oob_box, false);

        EnableSettingWidget(_debug_printf_to_stdout, true);
        EnableSettingWidget(_debug_printf_verbose, true);
        EnableSettingWidget(_debug_printf_buffer_size, true);
    }

    CollectSettings();
    emit settingChanged();
}

/// Collect all the settings
bool SettingsValidationAreas::CollectSettings() {
    std::string enables;
    std::string disables;

    // GPU stuff
    if (_shader_based_box != nullptr) {
        if (_shader_based_box->checkState(0) == Qt::Checked) {
            if (_gpu_assisted_radio->isChecked()) {
                enables = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";

                if (_gpu_assisted_reserve_box->checkState(0) == Qt::Checked) {
                    AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
                }
            } else {  // Debug printf must be checked
                enables = "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT";
            }
        }
    } else {
        if (_gpu_assisted_box->checkState(0) == Qt::Checked) {
            enables = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";

            if (_gpu_assisted_reserve_box->checkState(0) == Qt::Checked) {
                AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
            }
        }
    }

    StoreBoolSetting(_gpu_assisted_oob_box, "gpuav_buffer_oob");
    StoreBoolSetting(_debug_printf_to_stdout, "printf_to_stdout");
    StoreBoolSetting(_debug_printf_verbose, "printf_verbose");
    StoreBoolSetting(_debug_printf_buffer_size, "printf_buffer_size");

    // Best practice enables
    for (std::size_t i = 0, n = countof(best_practices); i < n; ++i) {
        if (best_practices[i].item == nullptr) continue;

        if (best_practices[i].item->checkState(0) == Qt::Checked) AppendString(enables, best_practices[i].token);
    }

    // Sync Validation
    for (std::size_t i = 0, n = countof(sync_checks); i < n; ++i) {
        if (sync_checks[i].item == nullptr) continue;

        if (sync_checks[i].item->checkState(0) == Qt::Checked) AppendString(enables, sync_checks[i].token);
    }

    // Everything else is a disable. Remember, these are backwards
    // because they are exposed to the end user as an enable.
    // If they are NOT checked, we add them to disables
    for (std::size_t i = 0, n = countof(misc_disables); i < n; ++i) {
        if (misc_disables[i].item->checkState(0) != Qt::Checked) AppendString(disables, misc_disables[i].token);
    }

    // Core checks. If unchecked, then individual ones might still be checked
    if (_core_checks_parent->checkState(0) == Qt::Checked) {
        for (std::size_t i = 0, n = countof(core_checks); i < n; ++i)
            if (core_checks[i].item->checkState(0) == Qt::Unchecked) AppendString(disables, core_checks[i].token);
    } else  // Not checked, turn them all off
        AppendString(disables, "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");

    FindByKey(settings_data, "disables")->value = disables;
    FindByKey(settings_data, "enables")->value = enables;

    return true;
}

bool SettingsValidationAreas::HasEnable(const char *token) const {
    const LayerSettingData *setting = FindByKey(settings_data, "enables");
    assert(setting);
    return setting->value.find(token) != std::string::npos;
}

bool SettingsValidationAreas::HasDisable(const char *token) const {
    const LayerSettingData *setting = FindByKey(settings_data, "disables");
    assert(setting);
    return setting->value.find(token) != std::string::npos;
}

QTreeWidgetItem *SettingsValidationAreas::CreateSettingWidget(QTreeWidgetItem *parent, const char *key) const {
    LayerSettingData *setting_data = FindByKey(settings_data, key);
    const LayerSettingMeta *setting_meta = FindByKey(settings_meta, key);

    QTreeWidgetItem *child = nullptr;

    if (setting_data && setting_meta) {
        const bool value = setting_data->value == "TRUE";
        child = new QTreeWidgetItem();
        child->setText(0, setting_meta->label);
        child->setToolTip(0, setting_meta->description);
        child->setCheckState(0, value ? Qt::Checked : Qt::Unchecked);
        parent->addChild(child);
    }

    return child;
}

void SettingsValidationAreas::StoreBoolSetting(QTreeWidgetItem *setting, const char *key) {
    assert(key);
    assert(std::strcmp(key, "") != 0);

    if (setting == nullptr) return;

    FindByKey(settings_data, key)->value = setting->checkState(0) == Qt::Checked ? "TRUE" : "FALSE";
}

void SettingsValidationAreas::EnableSettingWidget(QTreeWidgetItem *setting, bool enable) {
    if (setting == nullptr) return;

    if (enable) {
        setting->setFlags(setting->flags() | Qt::ItemIsEnabled);
    } else {
        setting->setFlags(setting->flags() & ~Qt::ItemIsEnabled);
    }
}
