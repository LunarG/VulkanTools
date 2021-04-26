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

static const char *TOKEN_CORE = "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT";
static const char *TOKEN_CORE_LAYOUT = "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION";
static const char *TOKEN_CORE_CMD = "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE";
static const char *TOKEN_CORE_OBJECT = "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE";
static const char *TOKEN_CORE_QUERY = "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION";
static const char *TOKEN_CORE_DESC = "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET";
static const char *TOKEN_CORE_SHADER = "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT";
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

QCheckBox *WidgetSettingValidation::CreateWidget(QTreeWidgetItem *parent, QTreeWidgetItem **item, const char *key,
                                                 const char *flag) {
    const SettingEnumValue *value = this->GetMetaFlag(key, flag);

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
      widget_core_cmd(nullptr),
      item_core_object(nullptr),
      widget_core_object(nullptr),
      item_core_query(nullptr),
      widget_core_query(nullptr),
      item_core_desc(nullptr),
      widget_core_desc(nullptr),
      item_core_shader(nullptr),
      widget_core_shader(nullptr),
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
      data_set(data_set) {
    assert(&meta_set);
    assert(&data_set);
    assert(meta_set.Get<SettingMetaFlags>("enables") != nullptr);
    assert(meta_set.Get<SettingMetaFlags>("disables") != nullptr);
    assert(data_set.Get<SettingDataFlags>("enables") != nullptr);
    assert(data_set.Get<SettingDataFlags>("disables") != nullptr);

    this->item->setText(0, "Validation Areas");
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));

    // Core
    this->widget_core = this->CreateWidget(this->item, &this->item_core, "disables", TOKEN_CORE);
    if (this->widget_core != nullptr) {
        this->connect(this->widget_core, SIGNAL(clicked(bool)), this, SLOT(OnCoreChecked(bool)));

        this->widget_core_layout = this->CreateWidget(this->item_core, &this->item_core_layout, "disables", TOKEN_CORE_LAYOUT);
        if (this->widget_core_layout != nullptr)
            this->connect(this->widget_core_layout, SIGNAL(clicked(bool)), this, SLOT(OnCoreLayoutChecked(bool)));

        this->widget_core_cmd = this->CreateWidget(this->item_core, &this->item_core_command, "disables", TOKEN_CORE_CMD);
        if (this->widget_core_cmd != nullptr)
            this->connect(this->widget_core_cmd, SIGNAL(clicked(bool)), this, SLOT(OnCoreCommandChecked(bool)));

        this->widget_core_object = this->CreateWidget(this->item_core, &this->item_core_object, "disables", TOKEN_CORE_OBJECT);
        if (this->widget_core_object != nullptr)
            this->connect(this->widget_core_object, SIGNAL(clicked(bool)), this, SLOT(OnCoreObjectChecked(bool)));

        this->widget_core_query = this->CreateWidget(this->item_core, &this->item_core_query, "disables", TOKEN_CORE_QUERY);
        if (this->widget_core_query != nullptr)
            this->connect(this->widget_core_query, SIGNAL(clicked(bool)), this, SLOT(OnCoreQueryChecked(bool)));

        this->widget_core_desc = this->CreateWidget(this->item_core, &this->item_core_desc, "disables", TOKEN_CORE_DESC);
        if (this->widget_core_desc != nullptr)
            this->connect(this->widget_core_desc, SIGNAL(clicked(bool)), this, SLOT(OnCoreDescChecked(bool)));

        this->widget_core_shader = this->CreateWidget(this->item_core, &this->item_core_shader, "disables", TOKEN_CORE_SHADER);
        if (this->widget_core_shader != nullptr)
            this->connect(this->widget_core_shader, SIGNAL(clicked(bool)), this, SLOT(OnCoreShaderChecked(bool)));

        this->widget_core_push = this->CreateWidget(this->item_core, &this->item_core_push, "disables", TOKEN_CORE_PUSH);
        if (this->widget_core_push != nullptr)
            this->connect(this->widget_core_push, SIGNAL(clicked(bool)), this, SLOT(OnCorePushChecked(bool)));
    }

    // Misc: VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT
    this->widget_misc_thread = this->CreateWidget(this->item, &this->item_misc_thread, "disables", TOKEN_MISC_THREAD);
    if (this->widget_misc_thread != nullptr)
        this->connect(this->widget_misc_thread, SIGNAL(clicked(bool)), this, SLOT(OnMiscThreadChecked(bool)));

    // Misc: VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT
    this->widget_misc_unique = this->CreateWidget(this->item, &this->item_misc_unique, "disables", TOKEN_MISC_UNIQUE);
    if (this->widget_misc_unique != nullptr)
        this->connect(this->widget_misc_unique, SIGNAL(clicked(bool)), this, SLOT(OnMiscUniqueChecked(bool)));

    // Misc: VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT
    this->widget_misc_lifetimes = this->CreateWidget(this->item, &this->item_misc_lifetimes, "disables", TOKEN_MISC_LIFETIMES);
    if (this->widget_misc_lifetimes != nullptr)
        this->connect(this->widget_misc_lifetimes, SIGNAL(clicked(bool)), this, SLOT(OnMiscLifetimesChecked(bool)));

    // Misc: VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT
    this->widget_misc_param = this->CreateWidget(this->item, &this->item_misc_param, "disables", TOKEN_MISC_PARAM);
    if (this->widget_misc_param != nullptr)
        this->connect(this->widget_misc_param, SIGNAL(clicked(bool)), this, SLOT(OnMiscParamChecked(bool)));

    // Shader-based
    {
        const SettingEnumValue *value_gpu = GetMetaFlag("enables", TOKEN_SHADER_GPU);
        const SettingEnumValue *value_printf = GetMetaFlag("enables", TOKEN_SHADER_PRINTF);
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

            this->widget_shader_gpu_reserve =
                this->CreateWidget(this->item_shader_gpu, &this->item_shader_gpu_reserve, "enables", TOKEN_SHADER_GPU_RESERVE);

            if (this->widget_shader_gpu_reserve != nullptr)
                this->connect(this->widget_shader_gpu_reserve, SIGNAL(clicked(bool)), this, SLOT(OnShaderGPUReserveChecked(bool)));

            {
                const SettingMetaBool *value = FindSettingMeta<SettingMetaBool>(meta_set, "gpuav_buffer_oob");
                if (IsSupported(value)) {
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
                if (IsSupported(value)) {
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
                if (IsSupported(value)) {
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
                if (IsSupported(value)) {
                    this->item_shader_printf_size = new QTreeWidgetItem();
                    this->item_shader_printf->addChild(this->item_shader_printf_size);

                    this->widget_debug_printf_size = new WidgetSettingInt(tree, this->item_shader_printf_size, *value, data_set);
                    this->connect(this->widget_debug_printf_size, SIGNAL(itemChanged()), this, SLOT(OnSettingChanged()));
                }
            }
        } else if (IsSupported(value_gpu)) {
            this->widget_shader = this->CreateWidget(this->item, &this->item_shader, "enables", TOKEN_SHADER_GPU);
            this->connect(this->widget_shader, SIGNAL(clicked(bool)), this, SLOT(OnShaderGPUChecked(bool)));

            this->widget_shader_gpu_reserve =
                this->CreateWidget(this->item_shader, &this->item_shader_gpu_reserve, "enables", TOKEN_SHADER_GPU_RESERVE);
            if (this->widget_shader_gpu_reserve != nullptr)
                this->connect(this->widget_shader_gpu_reserve, SIGNAL(clicked(bool)), this, SLOT(OnShaderGPUReserveChecked(bool)));
        }
    }

    // Synchronization
    this->widget_sync = this->CreateWidget(this->item, &this->item_sync, "enables", TOKEN_SYNC);
    if (this->widget_sync != nullptr) {
        this->connect(this->widget_sync, SIGNAL(clicked(bool)), this, SLOT(OnSyncChecked(bool)));
    }

    // Best Practices
    this->widget_best = this->CreateWidget(this->item, &this->item_best, "enables", TOKEN_BEST);
    if (this->widget_best != nullptr) {
        this->connect(this->widget_best, SIGNAL(clicked(bool)), this, SLOT(OnBestChecked(bool)));

        this->widget_best_arm = this->CreateWidget(this->item_best, &this->item_best_arm, "enables", TOKEN_BEST_ARM);
        if (this->widget_best_arm != nullptr)
            this->connect(this->widget_best_arm, SIGNAL(clicked(bool)), this, SLOT(OnBestArmChecked(bool)));
    }

    this->tree->setItemWidget(this->item, 0, this);
    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingValidation::OnCoreChecked(bool checked) {
    if (!checked)
        this->UpdateFlag("disables", TOKEN_CORE, true);
    else {
        this->UpdateFlag("disables", TOKEN_CORE, false);
        this->UpdateFlag("disables", TOKEN_CORE_LAYOUT, false);
        this->UpdateFlag("disables", TOKEN_CORE_CMD, false);
        this->UpdateFlag("disables", TOKEN_CORE_OBJECT, false);
        this->UpdateFlag("disables", TOKEN_CORE_QUERY, false);
        this->UpdateFlag("disables", TOKEN_CORE_DESC, false);
        this->UpdateFlag("disables", TOKEN_CORE_SHADER, false);
        this->UpdateFlag("disables", TOKEN_CORE_PUSH, false);
    }

    if (widget_core_layout != nullptr) {
        widget_core_layout->setChecked(!HasDataFlag("disables", TOKEN_CORE_LAYOUT));
    }
    if (widget_core_cmd != nullptr) {
        widget_core_cmd->setChecked(!HasDataFlag("disables", TOKEN_CORE_CMD));
    }
    if (widget_core_object != nullptr) {
        widget_core_object->setChecked(!HasDataFlag("disables", TOKEN_CORE_OBJECT));
    }
    if (widget_core_query != nullptr) {
        widget_core_query->setChecked(!HasDataFlag("disables", TOKEN_CORE_QUERY));
    }
    if (widget_core_desc != nullptr) {
        widget_core_desc->setChecked(!HasDataFlag("disables", TOKEN_CORE_DESC));
    }
    if (widget_core_shader != nullptr) {
        widget_core_shader->setChecked(!HasDataFlag("disables", TOKEN_CORE_SHADER));
    }
    if (widget_core_push != nullptr) {
        widget_core_push->setChecked(!HasDataFlag("disables", TOKEN_CORE_PUSH));
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnCoreLayoutChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_LAYOUT, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnCoreCommandChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_CMD, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnCoreObjectChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_OBJECT, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnCoreQueryChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_QUERY, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnCoreDescChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_DESC, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnCoreShaderChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_SHADER, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnCorePushChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_PUSH, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnMiscThreadChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_THREAD, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnMiscUniqueChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_UNIQUE, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnMiscLifetimesChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_LIFETIMES, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnMiscParamChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_PARAM, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnShaderBasedChecked(bool checked) {
    if (!checked) {
        this->UpdateFlag("enables", TOKEN_SHADER_GPU, false);
        this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, false);
    } else if (this->widget_shader_gpu->isChecked()) {
        this->UpdateFlag("enables", TOKEN_SHADER_GPU, true);
        this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, false);
    } else if (this->widget_shader_printf->isChecked()) {
        this->UpdateFlag("enables", TOKEN_SHADER_GPU, false);
        this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, true);
    }

    emit itemChanged();
}

void WidgetSettingValidation::OnShaderGPUChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SHADER_GPU, checked);
    this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, !checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnShaderGPUReserveChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SHADER_GPU_RESERVE, checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnShaderGPUOOBChecked(bool checked) {
    this->data_set.Get<SettingDataBool>("gpuav_buffer_oob")->value = checked;
    emit itemChanged();
}

void WidgetSettingValidation::OnShaderPrintfChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, checked);
    this->UpdateFlag("enables", TOKEN_SHADER_GPU, !checked);
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
    this->UpdateFlag("enables", TOKEN_SYNC, checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnBestChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_BEST, checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnBestArmChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_BEST_ARM, checked);
    emit itemChanged();
}

void WidgetSettingValidation::OnSettingChanged() { emit itemChanged(); }

void WidgetSettingValidation::UpdateFlag(const char *key, const char *flag, bool append) {
    SettingDataFlags *data = data_set.Get<SettingDataFlags>(key);
    assert(data != nullptr);

    if (append) {
        AppendString(data->value, flag);
    } else {
        RemoveString(data->value, flag);
    }
}

bool WidgetSettingValidation::HasDataBool(const char *key) const { return data_set.Get<SettingDataBool>(key)->value; }

bool WidgetSettingValidation::HasDataFlag(const char *key, const char *flag) const {
    return IsStringFound(data_set.Get<SettingDataFlags>(key)->value, flag);
}

const SettingEnumValue *WidgetSettingValidation::GetMetaFlag(const char *key, const char *flag) const {
    return FindByKey(meta_set.Get<SettingMetaFlags>(key)->enum_values, flag);
}

void WidgetSettingValidation::Refresh(RefreshAreas refresh_areas) {
    const bool core_enabled = !HasDataFlag("disables", TOKEN_CORE);

    if (this->widget_core != nullptr) {
        this->widget_core->setChecked(core_enabled);
    }

    if (this->widget_core_layout != nullptr) {
        this->widget_core_layout->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE)
            this->widget_core_layout->setChecked(!HasDataFlag("disables", TOKEN_CORE_LAYOUT));
    }

    if (this->widget_core_cmd != nullptr) {
        this->widget_core_cmd->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE) this->widget_core_cmd->setChecked(!HasDataFlag("disables", TOKEN_CORE_CMD));
    }

    if (this->widget_core_object != nullptr) {
        this->widget_core_object->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE)
            this->widget_core_object->setChecked(!HasDataFlag("disables", TOKEN_CORE_OBJECT));
    }

    if (this->widget_core_query != nullptr) {
        this->widget_core_query->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE)
            this->widget_core_query->setChecked(!HasDataFlag("disables", TOKEN_CORE_QUERY));
    }

    if (this->widget_core_desc != nullptr) {
        this->widget_core_desc->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE)
            this->widget_core_desc->setChecked(!HasDataFlag("disables", TOKEN_CORE_DESC));
    }

    if (this->widget_core_shader != nullptr) {
        this->widget_core_shader->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE)
            this->widget_core_shader->setChecked(!HasDataFlag("disables", TOKEN_CORE_SHADER));
    }

    if (this->widget_core_push != nullptr) {
        this->widget_core_push->setEnabled(core_enabled);
        if (refresh_areas == REFRESH_ENABLE_AND_STATE)
            this->widget_core_push->setChecked(!HasDataFlag("disables", TOKEN_CORE_PUSH));
    }

    if (this->widget_misc_thread != nullptr && refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->widget_misc_thread->setChecked(!HasDataFlag("disables", TOKEN_MISC_THREAD));
    }

    if (this->widget_misc_unique != nullptr && refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->widget_misc_unique->setChecked(!HasDataFlag("disables", TOKEN_MISC_UNIQUE));
    }

    if (this->widget_misc_lifetimes != nullptr && refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->widget_misc_lifetimes->setChecked(!HasDataFlag("disables", TOKEN_MISC_LIFETIMES));
    }

    if (this->widget_misc_param != nullptr && refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->widget_misc_param->setChecked(!HasDataFlag("disables", TOKEN_MISC_PARAM));
    }

    if (this->widget_shader != nullptr) {
        const bool shader_gpu = HasDataFlag("enables", TOKEN_SHADER_GPU);
        const bool shader_printf = HasDataFlag("enables", TOKEN_SHADER_PRINTF);

        if (GetMetaFlag("enables", TOKEN_SHADER_GPU) != nullptr && GetMetaFlag("enables", TOKEN_SHADER_PRINTF) != nullptr) {
            const bool shader_enabled = this->widget_shader->isChecked() || shader_gpu || shader_printf;
            if (shader_enabled && refresh_areas == REFRESH_ENABLE_AND_STATE) {
                this->widget_shader->setChecked(shader_gpu || shader_printf);
            }

            if (this->widget_shader_gpu != nullptr) {
                this->widget_shader_gpu->setEnabled(shader_enabled);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE) this->widget_shader_gpu->setChecked(shader_gpu);
            }

            if (this->widget_shader_gpu_reserve != nullptr) {
                this->widget_shader_gpu_reserve->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_reserve->setChecked(this->HasDataFlag("enables", TOKEN_SHADER_GPU_RESERVE));
            }

            if (this->widget_shader_gpu_oob != nullptr) {
                this->widget_shader_gpu_oob->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_oob->setChecked(this->HasDataBool("gpuav_buffer_oob"));
            }

            if (this->widget_shader_printf != nullptr) {
                this->widget_shader_printf->setEnabled(shader_enabled);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE) this->widget_shader_printf->setChecked(shader_printf);
            }

            if (this->widget_shader_printf_to_stdout != nullptr) {
                this->widget_shader_printf_to_stdout->setEnabled(shader_printf);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_printf_to_stdout->setChecked(this->HasDataBool("printf_to_stdout"));
            }

            if (this->widget_shader_printf_verbose != nullptr) {
                this->widget_shader_printf_verbose->setEnabled(shader_printf);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_printf_verbose->setChecked(this->HasDataBool("printf_verbose"));
            }

            if (this->widget_debug_printf_size != nullptr) {
                this->widget_debug_printf_size->setEnabled(shader_printf);
            }
        } else if (GetMetaFlag("enables", TOKEN_SHADER_GPU) != nullptr) {
            if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
                this->widget_shader->setChecked(HasDataFlag("enables", TOKEN_SHADER_GPU));
            }

            if (this->widget_shader_gpu_reserve != nullptr) {
                this->widget_shader_gpu_reserve->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_reserve->setChecked(HasDataFlag("enables", TOKEN_SHADER_GPU));
            }
        }
    }

    if (this->widget_sync != nullptr && refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->widget_sync->setChecked(HasDataFlag("enables", TOKEN_SYNC));
    }

    if (this->widget_best != nullptr && refresh_areas == REFRESH_ENABLE_AND_STATE) {
        this->widget_best->setChecked(HasDataFlag("enables", TOKEN_BEST));
    }

    if (this->widget_best_arm != nullptr) {
        this->widget_best_arm->setEnabled(HasDataFlag("enables", TOKEN_BEST));
        if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
            this->widget_best_arm->setChecked(HasDataFlag("enables", TOKEN_BEST_ARM));
        }
    }

    this->blockSignals(false);
}
