/*
 * Copyright (c) 2020-2021 Valve Corporation
 * Copyright (c) 2020-2021 LunarG, Inc.
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

static TreeSettings core_checks[] = {{"Image Layout Validation", "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION", "", nullptr},
                                     {"Command Buffer State", "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE", "", nullptr},
                                     {"Object in Use", "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE", "", nullptr},
                                     {"Query Validation", "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION", "", nullptr},
                                     {"Idle Descriptor Set", "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET", "", nullptr},
                                     {"Shader Validation", "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT", "", nullptr},
                                     {"Push Constant Range", "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE", "", nullptr}};

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

static bool IsSupported(const std::vector<SettingEnumValue> &values, const char *key) {
    const SettingEnumValue *value = FindByKey(values, key);
    if (value == nullptr) return false;
    return (value->platform_flags & (1 << VKC_PLATFORM)) != 0;
}

SettingsValidationAreas::SettingsValidationAreas(QTreeWidget *main_tree, QTreeWidgetItem *parent, const Version &version,
                                                 const SettingMetaSet &settings_meta, SettingDataSet &settings_data)
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
      _debug_printf_buffer_size_value(nullptr),
      _best_practices_box(nullptr),
      _best_practices_arm_box(nullptr),
      version(version),
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

    const SettingMetaFlags *setting_meta_enables = static_cast<const SettingMetaFlags *>(settings_meta.Get("enables"));
    assert(setting_meta_enables);

    // Shader-based enables
    const SettingEnumValue *enum_debug_printf =
        FindByKey(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT");
    const SettingEnumValue *enum_gpu_assisted =
        FindByKey(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");

    const bool has_debug_printf = HasEnable("VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT");
    const bool has_gpu_assisted = HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");

    if (enum_gpu_assisted != nullptr && enum_debug_printf != nullptr) {
        _shader_based_box = new QTreeWidgetItem();
        _shader_based_box->setText(0, "Shader-Based");
        _shader_based_box->setCheckState(0, has_debug_printf || has_gpu_assisted ? Qt::Checked : Qt::Unchecked);
        parent->addChild(_shader_based_box);

        // GPU assisted
        {
            _gpu_assisted_box = new QTreeWidgetItem();
            _gpu_assisted_box->setText(0, ("     " + enum_gpu_assisted->label).c_str());
            _gpu_assisted_box->setToolTip(0, enum_gpu_assisted->description.c_str());
            _shader_based_box->addChild(_gpu_assisted_box);

            _gpu_assisted_radio = new QRadioButton();
            _main_tree_widget->setItemWidget(_gpu_assisted_box, 0, _gpu_assisted_radio);

            const SettingEnumValue *enum_gpu_assisted_slot =
                FindByKey(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");

            if (enum_gpu_assisted_slot != nullptr) {
                const bool reserve_binding_slot = HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");

                _gpu_assisted_reserve_box = new QTreeWidgetItem();
                _gpu_assisted_reserve_box->setText(0, enum_gpu_assisted_slot->label.c_str());
                _gpu_assisted_reserve_box->setToolTip(0, enum_gpu_assisted_slot->description.c_str());
                _gpu_assisted_reserve_box->setCheckState(0, reserve_binding_slot ? Qt::Checked : Qt::Unchecked);
                _gpu_assisted_box->addChild(_gpu_assisted_reserve_box);
            }

            if (settings_meta.Get("gpuav_buffer_oob") != nullptr) {
                _gpu_assisted_oob_box = CreateSettingWidgetBool(_gpu_assisted_box, "gpuav_buffer_oob");
            }
        }

        // Debug Printf
        {
            _debug_printf_box = new QTreeWidgetItem();
            _debug_printf_box->setText(0, ("     " + enum_debug_printf->label).c_str());
            _debug_printf_box->setToolTip(0, enum_debug_printf->description.c_str());
            _shader_based_box->addChild(_debug_printf_box);

            _debug_printf_radio = new QRadioButton();
            _main_tree_widget->setItemWidget(_debug_printf_box, 0, _debug_printf_radio);

            if (settings_meta.Get("printf_to_stdout") != nullptr) {
                _debug_printf_to_stdout = CreateSettingWidgetBool(_debug_printf_box, "printf_to_stdout");
            }
            if (settings_meta.Get("printf_verbose") != nullptr) {
                _debug_printf_verbose = CreateSettingWidgetBool(_debug_printf_box, "printf_verbose");
            }
            if (settings_meta.Get("printf_buffer_size") != nullptr) {
                _debug_printf_buffer_size = CreateSettingWidgetInt(_debug_printf_box, "printf_buffer_size");
            }
        }

        if (has_debug_printf || has_gpu_assisted) {
            _debug_printf_radio->setChecked(has_debug_printf);
            EnableSettingWidget(_debug_printf_to_stdout, has_debug_printf);
            EnableSettingWidget(_debug_printf_verbose, has_debug_printf);
            EnableSettingWidget(_debug_printf_buffer_size, has_debug_printf);
            if (_debug_printf_buffer_size != nullptr) {
                _debug_printf_buffer_size_value->setEnabled(has_debug_printf);
            }

            _gpu_assisted_radio->setChecked(has_gpu_assisted);
            EnableSettingWidget(_gpu_assisted_reserve_box, has_gpu_assisted);
            EnableSettingWidget(_gpu_assisted_oob_box, has_gpu_assisted);
        } else {
            _debug_printf_radio->setEnabled(false);
            EnableSettingWidget(_debug_printf_box, false);
            EnableSettingWidget(_debug_printf_to_stdout, false);
            EnableSettingWidget(_debug_printf_verbose, false);
            EnableSettingWidget(_debug_printf_buffer_size, false);
            if (_debug_printf_buffer_size != nullptr) {
                _debug_printf_buffer_size_value->setEnabled(false);
            }

            _gpu_assisted_radio->setEnabled(false);
            EnableSettingWidget(_gpu_assisted_box, false);
            EnableSettingWidget(_gpu_assisted_reserve_box, false);
            EnableSettingWidget(_gpu_assisted_oob_box, false);
        }

        connect(_gpu_assisted_radio, SIGNAL(toggled(bool)), this, SLOT(gpuToggled(bool)));
        connect(_debug_printf_radio, SIGNAL(toggled(bool)), this, SLOT(printfToggled(bool)));
        connect(_debug_printf_buffer_size_value, SIGNAL(textEdited(const QString &)), this,
                SLOT(printfBufferSizeEdited(const QString &)));
    } else if (enum_gpu_assisted != nullptr) {
        _gpu_assisted_box = new QTreeWidgetItem();
        _gpu_assisted_box->setText(0, enum_gpu_assisted->label.c_str());
        _gpu_assisted_box->setToolTip(0, enum_gpu_assisted->description.c_str());
        _gpu_assisted_box->setCheckState(0, has_gpu_assisted ? Qt::Checked : Qt::Unchecked);
        parent->addChild(_gpu_assisted_box);

        const SettingEnumValue *enum_gpu_assisted_slot =
            FindByKey(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");

        if (enum_gpu_assisted_slot != nullptr) {
            const bool reserve_binding_slot = HasEnable("VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");

            _gpu_assisted_reserve_box = new QTreeWidgetItem();
            _gpu_assisted_reserve_box->setText(0, enum_gpu_assisted_slot->label.c_str());
            _gpu_assisted_reserve_box->setToolTip(0, enum_gpu_assisted_slot->description.c_str());
            _gpu_assisted_reserve_box->setCheckState(0, reserve_binding_slot ? Qt::Checked : Qt::Unchecked);
            _gpu_assisted_box->addChild(_gpu_assisted_reserve_box);

            EnableSettingWidget(_gpu_assisted_reserve_box, _gpu_assisted_box->checkState(0) == Qt::Checked);
        }
    }

    // Synchronization
    const SettingEnumValue *enum_synchronization =
        FindByKey(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT");

    if (enum_synchronization != nullptr) {
        const bool enable_best_practices = HasEnable(enum_synchronization->key.c_str());

        _synchronization_box = new QTreeWidgetItem();
        _synchronization_box->setText(0, enum_synchronization->label.c_str());
        _synchronization_box->setToolTip(0, enum_synchronization->description.c_str());
        _synchronization_box->setCheckState(0, enable_best_practices ? Qt::Checked : Qt::Unchecked);
        parent->addChild(_synchronization_box);
    }

    // Best Practices - one parent/child, but we want to be able to go back to these
    const SettingEnumValue *enum_best_practices =
        FindByKey(setting_meta_enables->enum_values, "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT");

    if (enum_best_practices != nullptr) {
        const bool enable_best_practices = HasEnable(enum_best_practices->key.c_str());

        _best_practices_box = new QTreeWidgetItem();
        _best_practices_box->setText(0, enum_best_practices->label.c_str());
        _best_practices_box->setToolTip(0, enum_best_practices->description.c_str());
        _best_practices_box->setCheckState(0, enable_best_practices ? Qt::Checked : Qt::Unchecked);
        parent->addChild(_best_practices_box);

        // ARM best practices
        const SettingEnumValue *enum_best_practices_arm =
            FindByKey(setting_meta_enables->enum_values, "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM");

        if (enum_best_practices_arm != nullptr) {
            const bool enable_best_practices_arm = HasEnable(enum_best_practices_arm->key.c_str());

            _best_practices_arm_box = new QTreeWidgetItem();
            _best_practices_arm_box->setText(0, enum_best_practices_arm->label.c_str());
            _best_practices_arm_box->setToolTip(0, enum_best_practices_arm->description.c_str());
            _best_practices_arm_box->setCheckState(0, enable_best_practices_arm ? Qt::Checked : Qt::Unchecked);
            _best_practices_box->addChild(_best_practices_arm_box);
            EnableSettingWidget(_best_practices_arm_box, _best_practices_box->checkState(0) == Qt::Checked);
        }
    }

    // Events

    connect(_main_tree_widget, SIGNAL(itemChanged(QTreeWidgetItem *, int)), this, SLOT(itemChanged(QTreeWidgetItem *, int)));
    connect(_main_tree_widget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(itemClicked(QTreeWidgetItem *, int)));
}

void SettingsValidationAreas::itemClicked(QTreeWidgetItem *item, int column) {
    (void)item;
    (void)column;

    emit settingChanged();
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
    if (item == _best_practices_box && _best_practices_arm_box != nullptr) {
        if (_best_practices_arm_box && item->checkState(0) == Qt::Checked) {
            _best_practices_arm_box->setFlags(_best_practices_arm_box->flags() | Qt::ItemIsEnabled);
        } else {
            _best_practices_arm_box->setFlags(_best_practices_arm_box->flags() & ~Qt::ItemIsEnabled);
            _best_practices_arm_box->setCheckState(0, Qt::Unchecked);
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
            if (_debug_printf_buffer_size != nullptr) _debug_printf_buffer_size_value->setEnabled(true);

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
            if (_debug_printf_buffer_size != nullptr) _debug_printf_buffer_size_value->setEnabled(false);

            _gpu_assisted_radio->setEnabled(false);
            EnableSettingWidget(_gpu_assisted_box, false);
            EnableSettingWidget(_gpu_assisted_reserve_box, false);
            EnableSettingWidget(_gpu_assisted_oob_box, false);
        }
    }

    if (item == _gpu_assisted_box && _gpu_assisted_radio == nullptr) {
        EnableSettingWidget(_gpu_assisted_reserve_box, _gpu_assisted_box->checkState(0) == Qt::Checked);
        EnableSettingWidget(_gpu_assisted_oob_box, _gpu_assisted_box->checkState(0) == Qt::Checked);
    }

    // Debug printf or GPU based also enables/disables the checkbox for reserving a slot
    if (_debug_printf_radio != nullptr) {
        if (item == _debug_printf_box && _debug_printf_radio->isChecked()) {
            EnableSettingWidget(_gpu_assisted_reserve_box, false);
            EnableSettingWidget(_gpu_assisted_oob_box, false);
        }
    }

    if (_gpu_assisted_radio != nullptr) {
        if (item == _gpu_assisted_box && _gpu_assisted_radio->isChecked()) {
            EnableSettingWidget(_debug_printf_to_stdout, false);
            EnableSettingWidget(_debug_printf_verbose, false);
            EnableSettingWidget(_debug_printf_buffer_size, false);
            if (_debug_printf_buffer_size != nullptr) _debug_printf_buffer_size_value->setEnabled(false);
        }
    }

    _main_tree_widget->blockSignals(false);

    // Check for performance issues. There are three different variations, and I think
    // we should alert the user to all three exactly/explicitly to what they are

    const bool features_to_run_alone[] = {_core_checks_parent->checkState(0) == Qt::Checked,
                                          _synchronization_box ? _synchronization_box->checkState(0) == Qt::Checked : false,
                                          _best_practices_box->checkState(0) == Qt::Checked,
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
        if (_debug_printf_buffer_size != nullptr) {
            _debug_printf_buffer_size_value->setEnabled(false);
        }
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
        if (_debug_printf_buffer_size != nullptr) {
            _debug_printf_buffer_size_value->setEnabled(true);
        }
    }

    CollectSettings();
    emit settingChanged();
}

void SettingsValidationAreas::printfBufferSizeEdited(const QString &new_value) {
    CollectSettings();
    emit settingChanged();
}

bool SettingsValidationAreas::CollectSettings() {
    std::vector<std::string> enables;
    std::vector<std::string> disables;

    // GPU stuff
    if (_shader_based_box != nullptr) {
        if (_shader_based_box->checkState(0) == Qt::Checked) {
            if (_gpu_assisted_radio->isChecked()) {
                AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");

                if (_gpu_assisted_reserve_box->checkState(0) == Qt::Checked) {
                    AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
                }
            } else {  // Debug printf must be checked
                AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT");
            }
        }
    } else if (_gpu_assisted_box != nullptr) {
        if (_gpu_assisted_box->checkState(0) == Qt::Checked) {
            AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT");

            if (_gpu_assisted_reserve_box->checkState(0) == Qt::Checked) {
                AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT");
            }
        }
    }

    StoreBoolSetting(_gpu_assisted_oob_box, "gpuav_buffer_oob");
    StoreBoolSetting(_debug_printf_to_stdout, "printf_to_stdout");
    StoreBoolSetting(_debug_printf_verbose, "printf_verbose");

    // Best practice enables
    if (_best_practices_box != nullptr) {
        if (_best_practices_box->checkState(0) == Qt::Checked)
            AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT");
    }

    if (_best_practices_arm_box != nullptr) {
        if (_best_practices_arm_box->checkState(0) == Qt::Checked)
            AppendString(enables, "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM");
    }

    // Sync Validation
    if (_synchronization_box != nullptr) {
        if (_synchronization_box->checkState(0) == Qt::Checked)
            AppendString(enables, "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT");
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
    } else {  // Not checked, turn them all off
        AppendString(disables, "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT");
    }

    static_cast<SettingDataFlags *>(settings_data.Get("disables"))->value = disables;
    static_cast<SettingDataFlags *>(settings_data.Get("enables"))->value = enables;

    return true;
}

bool SettingsValidationAreas::HasEnable(const char *token) const {
    return IsStringFound(static_cast<const SettingDataFlags *>(settings_data.Get("enables"))->value, token);
}

bool SettingsValidationAreas::HasDisable(const char *token) const {
    return IsStringFound(static_cast<const SettingDataFlags *>(settings_data.Get("disables"))->value, token);
}

QTreeWidgetItem *SettingsValidationAreas::CreateSettingWidgetBool(QTreeWidgetItem *parent, const char *key) {
    const SettingMetaBool *setting_meta = static_cast<const SettingMetaBool *>(settings_meta.Get(key));
    SettingDataBool *setting_data = static_cast<SettingDataBool *>(settings_data.Get(key));

    QTreeWidgetItem *child = nullptr;

    if (setting_data && setting_meta) {
        assert(setting_meta->type == SETTING_BOOL);

        child = new QTreeWidgetItem();
        child->setText(0, setting_meta->label.c_str());
        child->setToolTip(0, setting_meta->description.c_str());
        child->setCheckState(0, setting_data->value ? Qt::Checked : Qt::Unchecked);
        parent->addChild(child);
    }

    return child;
}

QTreeWidgetItem *SettingsValidationAreas::CreateSettingWidgetInt(QTreeWidgetItem *parent, const char *key) {
    const SettingMetaInt *setting_meta = static_cast<const SettingMetaInt *>(settings_meta.Get(key));
    SettingDataInt *setting_data = static_cast<SettingDataInt *>(settings_data.Get(key));

    QTreeWidgetItem *child = nullptr;

    if (setting_data && setting_meta) {
        assert(setting_meta->type == SETTING_INT);

        child = new QTreeWidgetItem();
        _debug_printf_buffer_size_value = new WidgetSettingInt(child, *setting_meta, *setting_data);
        parent->addChild(child);
        QTreeWidgetItem *place_holder = new QTreeWidgetItem();
        child->addChild(place_holder);
        _main_tree_widget->setItemWidget(place_holder, 0, _debug_printf_buffer_size_value);
    }

    return child;
}

void SettingsValidationAreas::StoreBoolSetting(QTreeWidgetItem *setting_data, const char *key) {
    assert(key);
    assert(std::strcmp(key, "") != 0);

    if (setting_data == nullptr) return;

    static_cast<SettingDataBool &>(*settings_data.Get(key)).value = setting_data->checkState(0) == Qt::Checked;
}

void SettingsValidationAreas::StoreIntSetting(QTreeWidgetItem *setting_data, const char *key) {
    assert(key);
    assert(std::strcmp(key, "") != 0);

    if (setting_data == nullptr) return;

    if (setting_data->text(0).isEmpty()) return;

    static_cast<SettingDataInt &>(*settings_data.Get(key)).value = std::atoi(setting_data->text(0).toStdString().c_str());
}

void SettingsValidationAreas::EnableSettingWidget(QTreeWidgetItem *setting_data, bool enable) {
    if (setting_data == nullptr) return;

    if (enable) {
        setting_data->setFlags(setting_data->flags() | Qt::ItemIsEnabled);
    } else {
        setting_data->setFlags(setting_data->flags() & ~Qt::ItemIsEnabled);
    }
}
