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

#include "widget_setting.h"
#include "settings_validation_areas.h"
#include "configurator.h"

#include "../vkconfig_core/version.h"
#include "../vkconfig_core/platform.h"
#include "../vkconfig_core/util.h"

#include <QSettings>
#include <QMessageBox>
#include <QCheckBox>

static const char *TOKEN_CORE = "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT";
static const char *TOKEN_CORE_LAYOUT = "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION";
static const char *TOKEN_CORE_COMMAND = "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE";
static const char *TOKEN_CORE_OBJECT = "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE";
static const char *TOKEN_CORE_QUERY = "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION";
static const char *TOKEN_CORE_DESCRIPTOR = "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET";
static const char *TOKEN_CORE_SHADERS = "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT";
static const char *TOKEN_CORE_PUSH = "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE";

static const char *TOKEN_MISC_THREAD = "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT";
static const char *TOKEN_MISC_UNIQUE = "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT";
static const char *TOKEN_MISC_LIFETIMES = "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT";
static const char *TOKEN_MISC_PARAM = "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT";

static const char *TOKEN_SHADER_GPU = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";
static const char *TOKEN_SHADER_GPU_RESERVE = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT";
static const char *TOKEN_SHADER_PRINTF = "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT";

static const char *TOKEN_SYNC = "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT";

static const char *TOKEN_BEST = "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT";
static const char *TOKEN_BEST_ARM = "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM";

static bool IsSupported(const SettingEnumValue *value) {
    if (value == nullptr) return false;

    if (value->view == SETTING_VIEW_HIDDEN) return false;

    if (!IsPlatformSupported(value->platform_flags)) return false;

    return true;
}

QCheckBox *WidgetSettingValidation::AddWidgetSetting(QTreeWidgetItem *parent, QTreeWidgetItem **item,
                                                     const SettingEnumValue *value) {
    if (!IsSupported(value)) {
        return nullptr;
    }

    *item = new QTreeWidgetItem();
    (*item)->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    parent->addChild(*item);

    QCheckBox *widget = new QCheckBox(this);
    widget->setText(value->label.c_str());
    widget->setToolTip(value->description.c_str());
    this->tree->setItemWidget(*item, 0, widget);
    return widget;
}

WidgetSettingValidation::WidgetSettingValidation(QTreeWidget *tree, QTreeWidgetItem *item, const SettingMetaSet &meta_set,
                                                 SettingDataSet &data_set)
    : WidgetSettingBase(tree, item),
      item_core(nullptr),
      widget_core(nullptr),
      item_core_layout(nullptr),
      widget_core_layout(nullptr),
      item_core_command(nullptr),
      widget_core_command(nullptr),
      item_core_object(nullptr),
      widget_core_object(nullptr),
      item_core_query(nullptr),
      widget_core_query(nullptr),
      item_core_desc(nullptr),
      widget_core_desc(nullptr),
      item_core_shaders(nullptr),
      widget_core_shaders(nullptr),
      item_core_push(nullptr),
      widget_core_push(nullptr),
      item_misc_thread(nullptr),
      widget_misc_thread(nullptr),
      item_misc_unique(nullptr),
      widget_misc_unique(nullptr),
      item_misc_lifetimes(nullptr),
      widget_misc_lifetimes(nullptr),
      item_misc_param(nullptr),
      widget_misc_param(nullptr),

      item_shader(nullptr),
      widget_shader(nullptr),
      item_shader_gpu(nullptr),
      widget_shader_gpu(nullptr),
      item_shader_gpu_reserve(nullptr),
      widget_shader_gpu_reserve(nullptr),
      item_shader_gpu_oob(nullptr),
      widget_shader_gpu_oob(nullptr),
      item_shader_printf(nullptr),
      widget_shader_printf(nullptr),

      item_shader_printf_to_stdout(nullptr),
      widget_shader_printf_to_stdout(nullptr),
      item_shader_printf_verbose(nullptr),
      widget_shader_printf_verbose(nullptr),
      item_shader_printf_size(nullptr),
      widget_debug_printf_size(nullptr),

      item_sync(nullptr),
      widget_sync(nullptr),
      item_best(nullptr),
      widget_best(nullptr),
      item_best_arm(nullptr),
      widget_best_arm(nullptr),

      meta_set(meta_set),
      meta_enables(*meta_set.Get<SettingMetaFlags>("enables")),
      meta_disables(*meta_set.Get<SettingMetaFlags>("disables")),
      data_set(data_set),
      data_enables(*data_set.Get<SettingDataFlags>("enables")),
      data_disables(*data_set.Get<SettingDataFlags>("disables")) {
    assert(&meta_set);
    assert(&data_set);

    this->item->setText(0, "Validation Areas");
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    // Core
    {
        const SettingEnumValue *core_value = GetFlag("disables", TOKEN_CORE);
        if (core_value != nullptr) {
            this->widget_core = this->AddWidgetSetting(this->item, &this->item_core, core_value);
            this->connect(this->widget_core, SIGNAL(clicked(bool)), this, SLOT(OnCoreChecked(bool)));

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_LAYOUT);
                if (IsSupported(value)) {
                    const bool enabled = !HasFlag("disables", value->key.c_str());
                    this->widget_core_layout = this->AddWidgetSetting(this->item_core, &this->item_core_layout, value);
                    this->connect(this->widget_core_layout, SIGNAL(clicked(bool)), this, SLOT(OnCoreLayoutChecked(bool)));
                }
            }

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_COMMAND);
                if (IsSupported(value)) {
                    const bool enabled = !HasFlag("disables", value->key.c_str());
                    this->widget_core_command = this->AddWidgetSetting(this->item_core, &this->item_core_command, value);
                    this->connect(this->widget_core_command, SIGNAL(clicked(bool)), this, SLOT(OnCoreCommandChecked(bool)));
                }
            }

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_OBJECT);
                if (IsSupported(value)) {
                    const bool enabled = !HasFlag("disables", value->key.c_str());
                    this->widget_core_object = this->AddWidgetSetting(this->item_core, &this->item_core_object, value);
                    this->connect(this->widget_core_object, SIGNAL(clicked(bool)), this, SLOT(OnCoreObjectChecked(bool)));
                }
            }

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_QUERY);
                if (IsSupported(value)) {
                    this->widget_core_query = this->AddWidgetSetting(this->item_core, &this->item_core_query, value);
                    this->connect(this->widget_core_query, SIGNAL(clicked(bool)), this, SLOT(OnCoreQueryChecked(bool)));
                }
            }

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_DESCRIPTOR);
                if (IsSupported(value)) {
                    this->widget_core_desc = this->AddWidgetSetting(this->item_core, &this->item_core_desc, value);
                    this->connect(this->widget_core_desc, SIGNAL(clicked(bool)), this, SLOT(OnCoreDescChecked(bool)));
                }
            }

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_SHADERS);
                if (IsSupported(value)) {
                    this->widget_core_shaders = this->AddWidgetSetting(this->item_core, &this->item_core_shaders, value);
                    this->connect(this->widget_core_shaders, SIGNAL(clicked(bool)), this, SLOT(OnCoreShadersChecked(bool)));
                }
            }

            {
                const SettingEnumValue *value = GetFlag("disables", TOKEN_CORE_PUSH);
                if (IsSupported(value)) {
                    this->widget_core_push = this->AddWidgetSetting(this->item_core, &this->item_core_push, value);
                    this->connect(this->widget_core_push, SIGNAL(clicked(bool)), this, SLOT(OnCorePushChecked(bool)));
                }
            }
        }
    }

    // Misc: VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT
    {
        const SettingEnumValue *value = GetFlag("disables", TOKEN_MISC_THREAD);
        if (IsSupported(value)) {
            this->widget_misc_thread = this->AddWidgetSetting(this->item, &this->item_misc_thread, value);
            this->connect(this->widget_misc_thread, SIGNAL(clicked(bool)), this, SLOT(OnMiscThreadChecked(bool)));
        }
    }

    // Misc: VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT
    {
        const SettingEnumValue *value = GetFlag("disables", TOKEN_MISC_UNIQUE);
        if (IsSupported(value)) {
            this->widget_misc_unique = this->AddWidgetSetting(this->item, &this->item_misc_unique, value);
            this->connect(this->widget_misc_unique, SIGNAL(clicked(bool)), this, SLOT(OnMiscUniqueChecked(bool)));
        }
    }

    // Misc: VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT
    {
        const SettingEnumValue *value = GetFlag("disables", TOKEN_MISC_LIFETIMES);
        if (IsSupported(value)) {
            this->widget_misc_lifetimes = this->AddWidgetSetting(this->item, &this->item_misc_lifetimes, value);
            this->connect(this->widget_misc_lifetimes, SIGNAL(clicked(bool)), this, SLOT(OnMiscLifetimesChecked(bool)));
        }
    }

    // Misc: VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT
    {
        const SettingEnumValue *value = GetFlag("disables", TOKEN_MISC_PARAM);
        if (IsSupported(value)) {
            this->widget_misc_param = this->AddWidgetSetting(this->item, &this->item_misc_param, value);
            this->connect(this->widget_misc_param, SIGNAL(clicked(bool)), this, SLOT(OnMiscParamChecked(bool)));
        }
    }

    // Shader-based
    {
        const SettingEnumValue *value_gpu = GetFlag("enables", TOKEN_SHADER_GPU);
        const SettingEnumValue *value_printf = GetFlag("enables", TOKEN_SHADER_PRINTF);
        if (IsSupported(value_gpu) && IsSupported(value_printf)) {
            this->item_shader = new QTreeWidgetItem();
            this->item_shader->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            this->item->addChild(this->item_shader);

            this->widget_shader = new QCheckBox(this);
            this->widget_shader->setText("Shader-Based");
            this->tree->setItemWidget(this->item_shader, 0, this->widget_shader);
            this->connect(this->widget_shader, SIGNAL(clicked(bool)), this, SLOT(OnShaderBasedChecked(bool)));

            this->item_shader_gpu = new QTreeWidgetItem();
            this->item_shader_gpu->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            this->item_shader->addChild(this->item_shader_gpu);

            this->widget_shader_gpu = new QRadioButton(this);
            this->widget_shader_gpu->setText(value_gpu->label.c_str());
            this->widget_shader_gpu->setToolTip(value_gpu->description.c_str());
            this->tree->setItemWidget(this->item_shader_gpu, 0, this->widget_shader_gpu);
            this->connect(this->widget_shader_gpu, SIGNAL(toggled(bool)), this, SLOT(OnShaderGPUChecked(bool)));

            {
                const SettingEnumValue *value = GetFlag("enables", TOKEN_SHADER_GPU_RESERVE);
                if (IsSupported(value)) {
                    this->widget_shader_gpu_reserve =
                        this->AddWidgetSetting(this->item_shader_gpu, &this->item_shader_gpu_reserve, value);
                    this->connect(this->widget_shader_gpu_reserve, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderGPUReserveChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSettingMeta<SettingMetaBool>(meta_set, "gpuav_buffer_oob");
                if (value != nullptr) {
                    this->item_shader_gpu_oob = new QTreeWidgetItem();
                    this->item_shader_gpu_oob->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_gpu->addChild(this->item_shader_gpu_oob);

                    this->widget_shader_gpu_oob = new QCheckBox(this);
                    this->widget_shader_gpu_oob->setText(value->label.c_str());
                    this->widget_shader_gpu_oob->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_gpu_oob, 0, this->widget_shader_gpu_oob);
                    this->connect(this->widget_shader_gpu_oob, SIGNAL(clicked(bool)), this, SLOT(OnShaderGPUOOBChecked(bool)));
                }
            }

            this->item_shader_printf = new QTreeWidgetItem();
            this->item_shader_printf->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            this->item_shader->addChild(this->item_shader_printf);

            this->widget_shader_printf = new QRadioButton(this);
            this->widget_shader_printf->setText(value_printf->label.c_str());
            this->widget_shader_printf->setToolTip(value_printf->description.c_str());
            this->tree->setItemWidget(this->item_shader_printf, 0, this->widget_shader_printf);
            this->connect(this->widget_shader_printf, SIGNAL(toggled(bool)), this, SLOT(OnShaderPrintfChecked(bool)));

            {
                const SettingMetaBool *value = FindSettingMeta<SettingMetaBool>(meta_set, "printf_to_stdout");
                if (value != nullptr) {
                    this->item_shader_printf_to_stdout = new QTreeWidgetItem();
                    this->item_shader_printf_to_stdout->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_printf->addChild(this->item_shader_printf_to_stdout);

                    this->widget_shader_printf_to_stdout = new QCheckBox(this);
                    this->widget_shader_printf_to_stdout->setText(value->label.c_str());
                    this->widget_shader_printf_to_stdout->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_printf_to_stdout, 0, this->widget_shader_printf_to_stdout);
                    this->connect(this->widget_shader_printf_to_stdout, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderPrintfStdoutChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSettingMeta<SettingMetaBool>(meta_set, "printf_verbose");
                if (value != nullptr) {
                    this->item_shader_printf_verbose = new QTreeWidgetItem();
                    this->item_shader_printf_verbose->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_printf->addChild(this->item_shader_printf_verbose);

                    this->widget_shader_printf_verbose = new QCheckBox(this);
                    this->widget_shader_printf_verbose->setText(value->label.c_str());
                    this->widget_shader_printf_verbose->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_printf_verbose, 0, this->widget_shader_printf_verbose);
                    this->connect(this->widget_shader_printf_verbose, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderPrintfVerboseChecked(bool)));
                }
            }

            {
                const SettingMetaInt *value = FindSettingMeta<SettingMetaInt>(meta_set, "printf_buffer_size");
                if (value != nullptr) {
                    this->item_shader_printf_size = new QTreeWidgetItem();
                    this->item_shader_printf_size->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_printf->addChild(this->item_shader_printf_size);

                    this->widget_debug_printf_size = new WidgetSettingInt(tree, this->item_shader_printf_size, *value, data_set);
                    this->connect(this->widget_debug_printf_size, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
                }
            }
        } else if (IsSupported(value_gpu)) {
            this->widget_shader = this->AddWidgetSetting(this->item, &this->item_shader, value_gpu);
            this->connect(this->widget_shader, SIGNAL(clicked(bool)), this, SLOT(OnShaderGPUChecked(bool)));

            {
                const SettingEnumValue *value = GetFlag("enables", TOKEN_SHADER_GPU_RESERVE);
                if (IsSupported(value)) {
                    this->widget_shader_gpu_reserve =
                        this->AddWidgetSetting(this->item_shader, &this->item_shader_gpu_reserve, value);
                    this->connect(this->widget_shader_gpu_reserve, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderGPUReserveChecked(bool)));
                }
            }
        }
    }

    // Synchronization
    {
        const SettingEnumValue *value = GetFlag("enables", TOKEN_SYNC);
        if (IsSupported(value)) {
            this->widget_sync = this->AddWidgetSetting(this->item, &this->item_sync, value);
            this->connect(this->widget_sync, SIGNAL(clicked(bool)), this, SLOT(OnSyncChecked(bool)));
        }
    }

    // Best Practices
    {
        const SettingEnumValue *enum_best = GetFlag("enables", TOKEN_BEST);
        if (IsSupported(enum_best)) {
            this->widget_best = this->AddWidgetSetting(this->item, &this->item_best, enum_best);
            this->connect(this->widget_best, SIGNAL(clicked(bool)), this, SLOT(OnBestChecked(bool)));

            const SettingEnumValue *enum_best_arm = GetFlag("enables", TOKEN_BEST_ARM);
            if (IsSupported(enum_best_arm)) {
                this->widget_best_arm = this->AddWidgetSetting(this->item_best, &this->item_best_arm, enum_best_arm);
                this->connect(this->widget_best_arm, SIGNAL(clicked(bool)), this, SLOT(OnBestArmChecked(bool)));
            }
        }
    }

    this->tree->setItemWidget(this->item, 0, this);
    this->Refresh();
}

void WidgetSettingValidation::OnCoreChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE);
    else {
        RemoveString(this->data_disables.value, TOKEN_CORE);
        RemoveString(this->data_disables.value, TOKEN_CORE_LAYOUT);
        RemoveString(this->data_disables.value, TOKEN_CORE_COMMAND);
        RemoveString(this->data_disables.value, TOKEN_CORE_OBJECT);
        RemoveString(this->data_disables.value, TOKEN_CORE_QUERY);
        RemoveString(this->data_disables.value, TOKEN_CORE_DESCRIPTOR);
        RemoveString(this->data_disables.value, TOKEN_CORE_SHADERS);
        RemoveString(this->data_disables.value, TOKEN_CORE_PUSH);
    }

    if (widget_core_layout != nullptr) {
        widget_core_layout->setChecked(!HasFlag("disables", TOKEN_CORE_LAYOUT));
    }
    if (widget_core_command != nullptr) {
        widget_core_command->setChecked(!HasFlag("disables", TOKEN_CORE_COMMAND));
    }
    if (widget_core_object != nullptr) {
        widget_core_object->setChecked(!HasFlag("disables", TOKEN_CORE_OBJECT));
    }
    if (widget_core_query != nullptr) {
        widget_core_query->setChecked(!HasFlag("disables", TOKEN_CORE_QUERY));
    }
    if (widget_core_desc != nullptr) {
        widget_core_desc->setChecked(!HasFlag("disables", TOKEN_CORE_DESCRIPTOR));
    }
    if (widget_core_shaders != nullptr) {
        widget_core_shaders->setChecked(!HasFlag("disables", TOKEN_CORE_SHADERS));
    }
    if (widget_core_push != nullptr) {
        widget_core_push->setChecked(!HasFlag("disables", TOKEN_CORE_PUSH));
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreLayoutChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_LAYOUT);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_LAYOUT);

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreCommandChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_COMMAND);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_COMMAND);

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreObjectChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_OBJECT);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_OBJECT);

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreQueryChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_QUERY);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_QUERY);

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreDescChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_DESCRIPTOR);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_DESCRIPTOR);

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreShadersChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_SHADERS);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_SHADERS);

    emit itemChanged();
}

void WidgetSettingValidation::OnCorePushChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_CORE_PUSH);
    else
        RemoveString(this->data_disables.value, TOKEN_CORE_PUSH);

    emit itemChanged();
}

void WidgetSettingValidation::OnMiscThreadChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_MISC_THREAD);
    else
        RemoveString(this->data_disables.value, TOKEN_MISC_THREAD);

    emit itemChanged();
}

void WidgetSettingValidation::OnMiscUniqueChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_MISC_UNIQUE);
    else
        RemoveString(this->data_disables.value, TOKEN_MISC_UNIQUE);

    emit itemChanged();
}

void WidgetSettingValidation::OnMiscLifetimesChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_MISC_LIFETIMES);
    else
        RemoveString(this->data_disables.value, TOKEN_MISC_LIFETIMES);

    emit itemChanged();
}

void WidgetSettingValidation::OnMiscParamChecked(bool checked) {
    if (!checked)
        AppendString(this->data_disables.value, TOKEN_MISC_PARAM);
    else
        RemoveString(this->data_disables.value, TOKEN_MISC_PARAM);

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderBasedChecked(bool checked) {
    if (!checked) {
        RemoveString(this->data_enables.value, TOKEN_SHADER_GPU);
        RemoveString(this->data_enables.value, TOKEN_SHADER_PRINTF);
    } else if (this->widget_shader_gpu->isChecked()) {
        AppendString(this->data_enables.value, TOKEN_SHADER_GPU);
        RemoveString(this->data_enables.value, TOKEN_SHADER_PRINTF);
    } else if (this->widget_shader_printf->isChecked()) {
        AppendString(this->data_enables.value, TOKEN_SHADER_PRINTF);
        RemoveString(this->data_enables.value, TOKEN_SHADER_GPU);
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderGPUChecked(bool checked) {
    if (checked) {
        AppendString(this->data_enables.value, TOKEN_SHADER_GPU);
        RemoveString(this->data_enables.value, TOKEN_SHADER_PRINTF);
    } else {
        AppendString(this->data_enables.value, TOKEN_SHADER_PRINTF);
        RemoveString(this->data_enables.value, TOKEN_SHADER_GPU);
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderGPUReserveChecked(bool checked) {
    if (checked) {
        AppendString(this->data_enables.value, TOKEN_SHADER_GPU_RESERVE);
    } else {
        RemoveString(this->data_enables.value, TOKEN_SHADER_GPU_RESERVE);
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderGPUOOBChecked(bool checked) {
    this->data_set.Get<SettingDataBool>("gpuav_buffer_oob")->value = checked;

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderPrintfChecked(bool checked) {
    if (checked) {
        AppendString(this->data_enables.value, TOKEN_SHADER_PRINTF);
        RemoveString(this->data_enables.value, TOKEN_SHADER_GPU);
    } else {
        AppendString(this->data_enables.value, TOKEN_SHADER_GPU);
        RemoveString(this->data_enables.value, TOKEN_SHADER_PRINTF);
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderPrintfStdoutChecked(bool checked) {
    this->data_set.Get<SettingDataBool>("printf_to_stdout")->value = checked;

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderPrintfVerboseChecked(bool checked) {
    this->data_set.Get<SettingDataBool>("printf_verbose")->value = checked;

    emit itemChanged();
}

void WidgetSettingValidation::OnSyncChecked(bool checked) {
    if (checked)
        AppendString(this->data_enables.value, TOKEN_SYNC);
    else
        RemoveString(this->data_enables.value, TOKEN_SYNC);
    emit itemChanged();
}

void WidgetSettingValidation::OnBestChecked(bool checked) {
    if (checked)
        AppendString(this->data_enables.value, TOKEN_BEST);
    else
        RemoveString(this->data_enables.value, TOKEN_BEST);
    emit itemChanged();
}

void WidgetSettingValidation::OnBestArmChecked(bool checked) {
    if (checked)
        AppendString(this->data_enables.value, TOKEN_BEST_ARM);
    else
        RemoveString(this->data_enables.value, TOKEN_BEST_ARM);
    emit itemChanged();
}

void WidgetSettingValidation::OnSettingChanged() { emit itemChanged(); }

bool WidgetSettingValidation::HasFlag(const char *key, const char *flag) const {
    return IsStringFound(data_set.Get<SettingDataFlags>(key)->value, flag);
}

const SettingEnumValue *WidgetSettingValidation::GetFlag(const char *key, const char *flag) const {
    return FindByKey(meta_set.Get<SettingMetaFlags>(key)->enum_values, flag);
}

void WidgetSettingValidation::Refresh() {
    const bool core_enabled = !HasFlag("disables", TOKEN_CORE);

    if (this->widget_core != nullptr) {
        this->widget_core->setChecked(core_enabled);
    }

    if (this->widget_core_layout != nullptr) {
        this->widget_core_layout->setEnabled(core_enabled);
        this->widget_core_layout->setChecked(!HasFlag("disables", TOKEN_CORE_LAYOUT));
    }

    if (this->widget_core_command != nullptr) {
        this->widget_core_command->setEnabled(core_enabled);
        this->widget_core_command->setChecked(!HasFlag("disables", TOKEN_CORE_COMMAND));
    }

    if (this->widget_core_object != nullptr) {
        this->widget_core_object->setEnabled(core_enabled);
        this->widget_core_object->setChecked(!HasFlag("disables", TOKEN_CORE_OBJECT));
    }

    if (this->widget_core_query != nullptr) {
        this->widget_core_query->setEnabled(core_enabled);
        this->widget_core_query->setChecked(!HasFlag("disables", TOKEN_CORE_QUERY));
    }

    if (this->widget_core_desc != nullptr) {
        this->widget_core_desc->setEnabled(core_enabled);
        this->widget_core_desc->setChecked(!HasFlag("disables", TOKEN_CORE_DESCRIPTOR));
    }

    if (this->widget_core_shaders != nullptr) {
        this->widget_core_shaders->setEnabled(core_enabled);
        this->widget_core_shaders->setChecked(!HasFlag("disables", TOKEN_CORE_SHADERS));
    }

    if (this->widget_core_push != nullptr) {
        this->widget_core_push->setEnabled(core_enabled);
        this->widget_core_push->setChecked(!HasFlag("disables", TOKEN_CORE_PUSH));
    }

    if (this->widget_misc_thread != nullptr) {
        this->widget_misc_thread->setChecked(!HasFlag("disables", TOKEN_MISC_THREAD));
    }

    if (this->widget_misc_unique != nullptr) {
        this->widget_misc_unique->setChecked(!HasFlag("disables", TOKEN_MISC_UNIQUE));
    }

    if (this->widget_misc_lifetimes != nullptr) {
        this->widget_misc_lifetimes->setChecked(!HasFlag("disables", TOKEN_MISC_LIFETIMES));
    }

    if (this->widget_misc_param != nullptr) {
        this->widget_misc_param->setChecked(!HasFlag("disables", TOKEN_MISC_PARAM));
    }

    if (this->widget_shader != nullptr) {
        const bool shader_gpu = HasFlag("enables", TOKEN_SHADER_GPU);
        const bool shader_printf = HasFlag("enables", TOKEN_SHADER_PRINTF);

        if (GetFlag("enables", TOKEN_SHADER_GPU) != nullptr && GetFlag("enables", TOKEN_SHADER_PRINTF) != nullptr) {
            const bool shader_enabled = this->widget_shader->isChecked() || shader_gpu || shader_printf;
            if (shader_gpu || shader_printf) {
                this->widget_shader->setChecked(shader_gpu || shader_printf);
            }

            if (this->widget_shader_gpu != nullptr) {
                this->widget_shader_gpu->setEnabled(shader_enabled);
                this->widget_shader_gpu->setChecked(shader_gpu);

                if (this->widget_shader_gpu_reserve != nullptr) {
                    this->widget_shader_gpu_reserve->setEnabled(shader_gpu);
                    this->widget_shader_gpu_reserve->setChecked(HasFlag("enables", TOKEN_SHADER_GPU_RESERVE));
                }

                if (this->widget_shader_gpu_oob != nullptr) {
                    this->widget_shader_gpu_oob->setEnabled(shader_gpu);
                    this->widget_shader_gpu_oob->setChecked(data_set.Get<SettingDataBool>("gpuav_buffer_oob")->value);
                }
            }

            if (this->widget_shader_printf != nullptr) {
                this->widget_shader_printf->setEnabled(shader_enabled);
                this->widget_shader_printf->setChecked(shader_printf);

                if (this->widget_shader_printf_to_stdout != nullptr) {
                    this->widget_shader_printf_to_stdout->setEnabled(shader_printf);
                    this->widget_shader_printf_to_stdout->setChecked(data_set.Get<SettingDataBool>("printf_to_stdout")->value);
                }

                if (this->widget_shader_printf_verbose != nullptr) {
                    this->widget_shader_printf_verbose->setEnabled(shader_printf);
                    this->widget_shader_printf_verbose->setChecked(data_set.Get<SettingDataBool>("printf_verbose")->value);
                }

                if (this->widget_debug_printf_size != nullptr) {
                    this->widget_debug_printf_size->setEnabled(shader_printf);
                    this->widget_shader_printf_verbose->setChecked(data_set.Get<SettingDataBool>("printf_verbose")->value);
                }
            }
        } else if (GetFlag("enables", TOKEN_SHADER_GPU) != nullptr) {
            if (shader_gpu) {
                this->widget_shader->setChecked(HasFlag("enables", TOKEN_SHADER_GPU));
            }

            if (this->widget_shader_gpu_reserve != nullptr) {
                this->widget_shader_gpu_reserve->setEnabled(shader_gpu);
                this->widget_shader_gpu_reserve->setChecked(HasFlag("enables", TOKEN_SHADER_GPU));
            }
        }
    }

    if (this->widget_sync != nullptr) {
        this->widget_sync->setChecked(HasFlag("enables", TOKEN_SYNC));
    }

    if (this->widget_best != nullptr) {
        this->widget_best->setChecked(HasFlag("enables", TOKEN_BEST));
    }

    if (this->widget_best_arm != nullptr) {
        this->widget_best_arm->setEnabled(HasFlag("enables", TOKEN_BEST));
    }

    this->blockSignals(false);
}
