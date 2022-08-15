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
#include "../vkconfig_core/setting_bool.h"

#include <QSettings>
#include <QMessageBox>

static const char *TOKEN_CORE = "VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT";
static const char *TOKEN_CORE_LAYOUT = "VALIDATION_CHECK_DISABLE_IMAGE_LAYOUT_VALIDATION";
static const char *TOKEN_CORE_CMD = "VALIDATION_CHECK_DISABLE_COMMAND_BUFFER_STATE";
static const char *TOKEN_CORE_OBJECT = "VALIDATION_CHECK_DISABLE_OBJECT_IN_USE";
static const char *TOKEN_CORE_QUERY = "VALIDATION_CHECK_DISABLE_QUERY_VALIDATION";
static const char *TOKEN_CORE_DESC = "VALIDATION_CHECK_DISABLE_IDLE_DESCRIPTOR_SET";
static const char *TOKEN_CORE_SHADER = "VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT";
static const char *TOKEN_CORE_PUSH = "VALIDATION_CHECK_DISABLE_PUSH_CONSTANT_RANGE";
static const char *TOKEN_CORE_CACHING = "VK_VALIDATION_FEATURE_DISABLE_SHADER_VALIDATION_CACHE_EXT";

static const char *TOKEN_MISC_THREAD = "VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT";
static const char *TOKEN_MISC_UNIQUE = "VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT";
static const char *TOKEN_MISC_LIFETIMES = "VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT";
static const char *TOKEN_MISC_PARAM = "VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT";

static const char *TOKEN_SHADER_GPU = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT";
static const char *TOKEN_SHADER_GPU_RESERVE = "VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT";
static const char *TOKEN_SHADER_PRINTF = "VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT";

static const char *TOKEN_SYNC = "VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT";
static const char *TOKEN_SYNC_QUEUE_SUBMIT = "VALIDATION_CHECK_ENABLE_SYNCHRONIZATION_VALIDATION_QUEUE_SUBMIT";

static const char *TOKEN_BEST = "VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT";
static const char *TOKEN_BEST_ARM = "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_ARM";
static const char *TOKEN_BEST_AMD = "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_AMD";
static const char *TOKEN_BEST_NVIDIA = "VALIDATION_CHECK_ENABLE_VENDOR_SPECIFIC_NVIDIA";

QCheckBox *WidgetSettingValidation::CreateWidget(QTreeWidgetItem *parent, QTreeWidgetItem **item, const char *key,
                                                 const char *flag) {
    const SettingEnumValue *value = this->GetMetaFlag(key, flag);

    if (!IsSupported(value)) {
        return nullptr;
    }

    const std::string status = value->status == STATUS_STABLE ? "" : std::string(" (") + GetToken(value->status) + ")";

    *item = new QTreeWidgetItem();
    (*item)->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    parent->addChild(*item);
    (*item)->setExpanded(true);

    QCheckBox *widget = new QCheckBox(this);
    widget->setText((value->label + status).c_str());
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
      item_shader_gpu_desc_indexing(nullptr),
      widget_shader_gpu_desc_indexing(nullptr),
      item_shader_gpu_indirect(nullptr),
      widget_shader_gpu_indirect(nullptr),
      item_shader_dispatch_indirect(nullptr),
      widget_shader_dispatch_indirect(nullptr),
      item_shader_gpu_vma_linear_output(nullptr),
      widget_shader_gpu_vma_linear_output(nullptr),
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
      item_sync_queue_submit(nullptr),
      widget_sync_queue_submit(nullptr),
      item_best(nullptr),
      widget_best(nullptr),
      item_best_arm(nullptr),
      widget_best_arm(nullptr),
      item_best_amd(nullptr),
      widget_best_amd(nullptr),
      item_best_nvidia(nullptr),
      widget_best_nvidia(nullptr),

      meta_set(meta_set),
      data_set(data_set) {
    assert(&meta_set);
    assert(&data_set);
    assert(FindSetting(meta_set, "enables") != nullptr);
    assert(FindSetting(meta_set, "disables") != nullptr);
    assert(FindSetting(data_set, "enables") != nullptr);
    assert(FindSetting(data_set, "disables") != nullptr);

    this->item->setText(0, "Validation Areas");
    this->item->setSizeHint(0, QSize(0, ITEM_HEIGHT));
    this->item->setExpanded(false);

    // Core
    this->widget_core = this->CreateWidget(this->item, &this->item_core, "disables", TOKEN_CORE);
    if (this->widget_core != nullptr) {
        this->connect(this->widget_core, SIGNAL(clicked(bool)), this, SLOT(OnCoreChecked(bool)));

        Configurator &configurator = Configurator::Get();
        Configuration *configuration = configurator.configurations.GetActiveConfiguration();

        if (configuration->view_advanced_settings) {
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
            if (this->widget_core_shader != nullptr) {
                this->connect(this->widget_core_shader, SIGNAL(clicked(bool)), this, SLOT(OnCoreShaderChecked(bool)));

                this->widget_core_caching =
                    this->CreateWidget(this->item_core_shader, &this->item_core_caching, "disables", TOKEN_CORE_CACHING);
                if (this->widget_core_caching != nullptr)
                    this->connect(this->widget_core_caching, SIGNAL(clicked(bool)), this, SLOT(OnCoreCachingChecked(bool)));
            }

            this->widget_core_push = this->CreateWidget(this->item_core, &this->item_core_push, "disables", TOKEN_CORE_PUSH);
            if (this->widget_core_push != nullptr)
                this->connect(this->widget_core_push, SIGNAL(clicked(bool)), this, SLOT(OnCorePushChecked(bool)));
        }
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
            this->item_shader->setExpanded(true);
            this->item_shader_gpu->setExpanded(true);

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
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "gpuav_descriptor_indexing");
                if (IsSupported(value)) {
                    this->item_shader_gpu_desc_indexing = new QTreeWidgetItem();
                    this->item_shader_gpu_desc_indexing->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_gpu->addChild(this->item_shader_gpu_desc_indexing);
                    this->item_shader_gpu->setExpanded(true);

                    this->widget_shader_gpu_desc_indexing = new QCheckBox(this);
                    this->widget_shader_gpu_desc_indexing->setText(value->label.c_str());
                    this->widget_shader_gpu_desc_indexing->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_gpu_desc_indexing, 0, this->widget_shader_gpu_desc_indexing);
                    this->connect(this->widget_shader_gpu_desc_indexing, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderGPUDescIndexingChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "gpuav_buffer_oob");
                if (IsSupported(value)) {
                    this->item_shader_gpu_oob = new QTreeWidgetItem();
                    this->item_shader_gpu_oob->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_gpu->addChild(this->item_shader_gpu_oob);
                    this->item_shader_gpu->setExpanded(true);

                    this->widget_shader_gpu_oob = new QCheckBox(this);
                    this->widget_shader_gpu_oob->setText(value->label.c_str());
                    this->widget_shader_gpu_oob->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_gpu_oob, 0, this->widget_shader_gpu_oob);
                    this->connect(this->widget_shader_gpu_oob, SIGNAL(clicked(bool)), this, SLOT(OnShaderGPUOOBChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "validate_draw_indirect");
                if (IsSupported(value)) {
                    this->item_shader_gpu_indirect = new QTreeWidgetItem();
                    this->item_shader_gpu_indirect->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_gpu->addChild(this->item_shader_gpu_indirect);
                    this->item_shader_gpu->setExpanded(true);

                    this->widget_shader_gpu_indirect = new QCheckBox(this);
                    this->widget_shader_gpu_indirect->setText(value->label.c_str());
                    this->widget_shader_gpu_indirect->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_gpu_indirect, 0, this->widget_shader_gpu_indirect);
                    this->connect(this->widget_shader_gpu_indirect, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderGPUIndirectChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "validate_dispatch_indirect");
                if (IsSupported(value)) {
                    this->item_shader_dispatch_indirect = new QTreeWidgetItem();
                    this->item_shader_dispatch_indirect->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_gpu->addChild(this->item_shader_dispatch_indirect);
                    this->item_shader_gpu->setExpanded(true);

                    this->widget_shader_dispatch_indirect = new QCheckBox(this);
                    this->widget_shader_dispatch_indirect->setText(value->label.c_str());
                    this->widget_shader_dispatch_indirect->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_dispatch_indirect, 0, this->widget_shader_dispatch_indirect);
                    this->connect(this->widget_shader_dispatch_indirect, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderDispatchIndirectChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "vma_linear_output");
                if (IsSupported(value)) {
                    this->item_shader_gpu_vma_linear_output = new QTreeWidgetItem();
                    this->item_shader_gpu_vma_linear_output->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_gpu->addChild(this->item_shader_gpu_vma_linear_output);
                    this->item_shader_gpu->setExpanded(true);

                    this->widget_shader_gpu_vma_linear_output = new QCheckBox(this);
                    this->widget_shader_gpu_vma_linear_output->setText(value->label.c_str());
                    this->widget_shader_gpu_vma_linear_output->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_gpu_vma_linear_output, 0,
                                              this->widget_shader_gpu_vma_linear_output);
                    this->connect(this->widget_shader_gpu_vma_linear_output, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderGPUVMALinearOutput(bool)));
                }
            }

            this->item_shader_printf = new QTreeWidgetItem();
            this->item_shader_printf->setSizeHint(0, QSize(0, ITEM_HEIGHT));
            this->item_shader->addChild(this->item_shader_printf);
            this->item_shader->setExpanded(true);
            this->item_shader_printf->setExpanded(true);

            this->widget_shader_printf = new QRadioButton(this);
            this->widget_shader_printf->setText(value_printf->label.c_str());
            this->widget_shader_printf->setToolTip(value_printf->description.c_str());
            this->tree->setItemWidget(this->item_shader_printf, 0, this->widget_shader_printf);
            this->connect(this->widget_shader_printf, SIGNAL(toggled(bool)), this, SLOT(OnShaderPrintfChecked(bool)));

            {
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "printf_to_stdout");
                if (IsSupported(value)) {
                    this->item_shader_printf_to_stdout = new QTreeWidgetItem();
                    this->item_shader_printf_to_stdout->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_printf->addChild(this->item_shader_printf_to_stdout);
                    this->item_shader_printf->setExpanded(true);

                    this->widget_shader_printf_to_stdout = new QCheckBox(this);
                    this->widget_shader_printf_to_stdout->setText(value->label.c_str());
                    this->widget_shader_printf_to_stdout->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_printf_to_stdout, 0, this->widget_shader_printf_to_stdout);
                    this->connect(this->widget_shader_printf_to_stdout, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderPrintfStdoutChecked(bool)));
                }
            }

            {
                const SettingMetaBool *value = FindSetting<SettingMetaBool>(meta_set, "printf_verbose");
                if (IsSupported(value)) {
                    this->item_shader_printf_verbose = new QTreeWidgetItem();
                    this->item_shader_printf_verbose->setSizeHint(0, QSize(0, ITEM_HEIGHT));
                    this->item_shader_printf->addChild(this->item_shader_printf_verbose);
                    this->item_shader_printf->setExpanded(true);

                    this->widget_shader_printf_verbose = new QCheckBox(this);
                    this->widget_shader_printf_verbose->setText(value->label.c_str());
                    this->widget_shader_printf_verbose->setToolTip(value->description.c_str());
                    this->tree->setItemWidget(this->item_shader_printf_verbose, 0, this->widget_shader_printf_verbose);
                    this->connect(this->widget_shader_printf_verbose, SIGNAL(clicked(bool)), this,
                                  SLOT(OnShaderPrintfVerboseChecked(bool)));
                }
            }

            {
                const SettingMetaInt *value = FindSetting<SettingMetaInt>(meta_set, "printf_buffer_size");
                if (IsSupported(value)) {
                    this->item_shader_printf_size = new QTreeWidgetItem();
                    this->item_shader_printf->addChild(this->item_shader_printf_size);
                    this->item_shader_printf->setExpanded(true);

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

        this->widget_sync_queue_submit =
            this->CreateWidget(this->item_sync, &this->item_sync_queue_submit, "enables", TOKEN_SYNC_QUEUE_SUBMIT);
        if (this->widget_sync_queue_submit != nullptr)
            this->connect(this->widget_sync_queue_submit, SIGNAL(clicked(bool)), this, SLOT(OnSyncQueueSubmitChecked(bool)));
    }

    // Best Practices
    this->widget_best = this->CreateWidget(this->item, &this->item_best, "enables", TOKEN_BEST);
    if (this->widget_best != nullptr) {
        this->connect(this->widget_best, SIGNAL(clicked(bool)), this, SLOT(OnBestChecked(bool)));

        this->widget_best_amd = this->CreateWidget(this->item_best, &this->item_best_amd, "enables", TOKEN_BEST_AMD);
        if (this->widget_best_amd != nullptr)
            this->connect(this->widget_best_amd, SIGNAL(clicked(bool)), this, SLOT(OnBestAmdChecked(bool)));

        this->widget_best_arm = this->CreateWidget(this->item_best, &this->item_best_arm, "enables", TOKEN_BEST_ARM);
        if (this->widget_best_arm != nullptr)
            this->connect(this->widget_best_arm, SIGNAL(clicked(bool)), this, SLOT(OnBestArmChecked(bool)));

        this->widget_best_nvidia = this->CreateWidget(this->item_best, &this->item_best_nvidia, "enables", TOKEN_BEST_NVIDIA);
        if (this->widget_best_nvidia != nullptr)
            this->connect(this->widget_best_nvidia, SIGNAL(clicked(bool)), this, SLOT(OnBestNvidiaChecked(bool)));
    }

    this->tree->setItemWidget(this->item, 0, this);
    this->Refresh(REFRESH_ENABLE_AND_STATE);
}

void WidgetSettingValidation::OnCoreChecked(bool checked) {
    if (checked && !CheckOverhead(OVERHEAD_CORE)) {
        this->widget_core->setChecked(false);
        return;
    }

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
        this->UpdateFlag("disables", TOKEN_CORE_CACHING, false);
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
        if (widget_core_caching != nullptr) {
            widget_core_caching->setChecked(!HasDataFlag("disables", TOKEN_CORE_CACHING));
        }
    }
    if (widget_core_push != nullptr) {
        widget_core_push->setChecked(!HasDataFlag("disables", TOKEN_CORE_PUSH));
    }

    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreLayoutChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_LAYOUT, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreCommandChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_CMD, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreObjectChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_OBJECT, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreQueryChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_QUERY, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreDescChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_DESC, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreShaderChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_SHADER, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCorePushChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_PUSH, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnCoreCachingChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_CORE_CACHING, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnMiscThreadChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_THREAD, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnMiscUniqueChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_UNIQUE, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnMiscLifetimesChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_LIFETIMES, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnMiscParamChecked(bool checked) {
    this->UpdateFlag("disables", TOKEN_MISC_PARAM, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderBasedChecked(bool checked) {
    if (checked && !CheckOverhead(OVERHEAD_SHADER)) {
        this->widget_shader->setChecked(false);
        return;
    }

    if (!checked) {
        this->UpdateFlag("enables", TOKEN_SHADER_GPU, false);
        this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, false);
    } else if (this->widget_shader_printf->isChecked()) {
        this->UpdateFlag("enables", TOKEN_SHADER_GPU, false);
        this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, true);
    } else {
        if (!this->widget_shader_gpu->isChecked()) this->widget_shader_gpu->setChecked(true);
        this->UpdateFlag("enables", TOKEN_SHADER_GPU, true);
        this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, false);
    }

    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderGPUChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SHADER_GPU, checked);
    this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderGPUReserveChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SHADER_GPU_RESERVE, checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderGPUDescIndexingChecked(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "gpuav_descriptor_indexing"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderGPUOOBChecked(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "gpuav_buffer_oob"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderGPUIndirectChecked(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "validate_draw_indirect"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderDispatchIndirectChecked(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "validate_dispatch_indirect"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderGPUVMALinearOutput(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "vma_linear_output"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderPrintfChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SHADER_PRINTF, checked);
    this->UpdateFlag("enables", TOKEN_SHADER_GPU, !checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderPrintfStdoutChecked(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "printf_to_stdout"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnShaderPrintfVerboseChecked(bool checked) {
    static_cast<SettingDataBool *>(FindSetting(this->data_set, "printf_verbose"))->value = checked;
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnSyncChecked(bool checked) {
    if (checked && !CheckOverhead(OVERHEAD_SYNC)) {
        this->widget_sync->setChecked(false);
        return;
    }

    this->UpdateFlag("enables", TOKEN_SYNC, checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnSyncQueueSubmitChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_SYNC_QUEUE_SUBMIT, checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnBestChecked(bool checked) {
    if (checked && !CheckOverhead(OVERHEAD_BEST)) {
        this->widget_best->setChecked(false);
        return;
    }

    this->UpdateFlag("enables", TOKEN_BEST, checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnBestArmChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_BEST_ARM, checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnBestAmdChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_BEST_AMD, checked);
    this->OnSettingChanged();
}

void WidgetSettingValidation::OnBestNvidiaChecked(bool checked) {
    this->UpdateFlag("enables", TOKEN_BEST_NVIDIA, checked);
    this->OnSettingChanged();
}

bool WidgetSettingValidation::CheckOverhead(Overhead candidate) const {
    QSettings settings;
    if (settings.value("VKCONFIG_WARN_CORE_SHADER_IGNORE").toBool() == false) {
        const bool features_to_run_alone[] = {
            this->widget_core != nullptr ? this->widget_core->isChecked() || candidate == OVERHEAD_CORE : false,
            this->widget_shader != nullptr ? this->widget_shader->isChecked() || candidate == OVERHEAD_SHADER : false,
            this->widget_sync != nullptr ? this->widget_sync->isChecked() || candidate == OVERHEAD_SYNC : false,
            this->widget_best != nullptr ? this->widget_best->isChecked() || candidate == OVERHEAD_BEST : false};

        static const char *LABELS[]{
            "<i>Core Validation</i>",             // OVERHEAD_CORE
            "<i>Shader Based Validation</i>",     // OVERHEAD_SHADER
            "<i>Synchronization Validation</i>",  // OVERHEAD_SYNC
            "<i>Best Practices Warnings</i>"      // OVERHEAD_BEST
        };
        static_assert(countof(LABELS) == OVERHEAD_COUNT, "The tranlation table size doesn't match the enum number of elements");

        int count_enabled_features = 0;
        for (std::size_t i = 0, n = countof(features_to_run_alone); i < n; ++i)
            count_enabled_features += features_to_run_alone[i] ? 1 : 0;

        if (count_enabled_features > 1) {
            QMessageBox alert(this->tree);
            alert.setWindowTitle("High Validation Layer Overhead");
            alert.setText(format("Using %s, %s, %s and %s simultanously results in high performance degradation.",
                                 LABELS[OVERHEAD_CORE], LABELS[OVERHEAD_SHADER], LABELS[OVERHEAD_SYNC], LABELS[OVERHEAD_BEST])
                              .c_str());
            alert.setInformativeText(format("Do you want to add %s anyway?", LABELS[candidate]).c_str());
            alert.setIcon(QMessageBox::Warning);
            alert.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            alert.setDefaultButton(QMessageBox::Yes);
            alert.setCheckBox(new QCheckBox("Do not show again."));
            const bool result = alert.exec() == QMessageBox::Yes;
            if (alert.checkBox()->isChecked()) {
                settings.setValue("VKCONFIG_WARN_CORE_SHADER_IGNORE", true);
            }
            return result;
        }
    }

    return true;
}

void WidgetSettingValidation::OnSettingChanged() { emit itemChanged(); }

void WidgetSettingValidation::UpdateFlag(const char *key, const char *flag, bool append) {
    SettingDataFlags *data = static_cast<SettingDataFlags *>(FindSetting(this->data_set, key));
    assert(data != nullptr);

    if (append) {
        AppendString(data->value, flag);
    } else {
        RemoveString(data->value, flag);
    }
}

bool WidgetSettingValidation::HasDataBool(const char *key) const {
    return static_cast<SettingDataBool *>(FindSetting(this->data_set, key))->value;
}

bool WidgetSettingValidation::HasDataFlag(const char *key, const char *flag) const {
    const SettingDataFlags *data = static_cast<const SettingDataFlags *>(FindSetting(this->data_set, key));

    return IsStringFound(data->value, flag);
}

const SettingEnumValue *WidgetSettingValidation::GetMetaFlag(const char *key, const char *flag) const {
    return FindByKey(static_cast<const SettingMetaFlags *>(FindSetting(this->meta_set, key))->enum_values, flag);
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

        const bool shader_enabled = !HasDataFlag("disables", TOKEN_CORE_SHADER);

        if (refresh_areas == REFRESH_ENABLE_AND_STATE) this->widget_core_shader->setChecked(shader_enabled);

        if (this->widget_core_caching != nullptr) {
            this->widget_core_caching->setEnabled(core_enabled && shader_enabled);
            if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                this->widget_core_caching->setChecked(!HasDataFlag("disables", TOKEN_CORE_CACHING));
        }
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
                this->widget_shader_gpu->setEnabled(this->widget_shader->isChecked());
                if (refresh_areas == REFRESH_ENABLE_AND_STATE) this->widget_shader_gpu->setChecked(shader_gpu);
            }

            if (this->widget_shader_gpu_reserve != nullptr) {
                this->widget_shader_gpu_reserve->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_reserve->setChecked(this->HasDataFlag("enables", TOKEN_SHADER_GPU_RESERVE));
            }

            if (this->widget_shader_gpu_desc_indexing != nullptr) {
                this->widget_shader_gpu_desc_indexing->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_desc_indexing->setChecked(this->HasDataBool("gpuav_descriptor_indexing"));
            }

            if (this->widget_shader_gpu_oob != nullptr) {
                this->widget_shader_gpu_oob->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_oob->setChecked(this->HasDataBool("gpuav_buffer_oob"));
            }

            if (this->widget_shader_gpu_indirect != nullptr) {
                this->widget_shader_gpu_indirect->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_indirect->setChecked(this->HasDataBool("validate_draw_indirect"));
            }

            if (this->widget_shader_dispatch_indirect != nullptr) {
                this->widget_shader_dispatch_indirect->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_dispatch_indirect->setChecked(this->HasDataBool("validate_dispatch_indirect"));
            }

            if (this->widget_shader_gpu_vma_linear_output != nullptr) {
                this->widget_shader_gpu_vma_linear_output->setEnabled(shader_gpu);
                if (refresh_areas == REFRESH_ENABLE_AND_STATE)
                    this->widget_shader_gpu_vma_linear_output->setChecked(this->HasDataBool("vma_linear_output"));
            }

            if (this->widget_shader_printf != nullptr) {
                this->widget_shader_printf->setEnabled(this->widget_shader->isChecked());
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

    if (this->widget_sync_queue_submit != nullptr) {
        this->widget_sync_queue_submit->setEnabled(HasDataFlag("enables", TOKEN_SYNC));
        if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
            this->widget_sync_queue_submit->setChecked(HasDataFlag("enables", TOKEN_SYNC_QUEUE_SUBMIT));
        }
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

    if (this->widget_best_amd != nullptr) {
        this->widget_best_amd->setEnabled(HasDataFlag("enables", TOKEN_BEST));
        if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
            this->widget_best_amd->setChecked(HasDataFlag("enables", TOKEN_BEST_AMD));
        }
    }

    if (this->widget_best_nvidia != nullptr) {
        this->widget_best_nvidia->setEnabled(HasDataFlag("enables", TOKEN_BEST));
        if (refresh_areas == REFRESH_ENABLE_AND_STATE) {
            this->widget_best_nvidia->setChecked(HasDataFlag("enables", TOKEN_BEST_NVIDIA));
        }
    }

    this->blockSignals(false);
}
