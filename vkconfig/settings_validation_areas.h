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

#pragma once

#include "../vkconfig_core/layer.h"

#include "widget_setting_int.h"
#include "widget_setting_flags.h"

#include "widget_setting.h"
#include <QRadioButton>
#include <QCheckBox>

#include <vector>

class WidgetSettingValidation : public WidgetSettingBase {
    Q_OBJECT

   public:
    explicit WidgetSettingValidation(QTreeWidget *tree, QTreeWidgetItem *item, const SettingMetaSet &meta_set,
                                     SettingDataSet &data_set);

    void Refresh(RefreshAreas refresh_areas) override;

   private:
    QTreeWidgetItem *item_core;
    QCheckBox *widget_core;

    QTreeWidgetItem *item_core_layout;
    QCheckBox *widget_core_layout;

    QTreeWidgetItem *item_core_command;
    QCheckBox *widget_core_cmd;

    QTreeWidgetItem *item_core_object;
    QCheckBox *widget_core_object;

    QTreeWidgetItem *item_core_query;
    QCheckBox *widget_core_query;

    QTreeWidgetItem *item_core_desc;
    QCheckBox *widget_core_desc;

    QTreeWidgetItem *item_core_shader;
    QCheckBox *widget_core_shader;

    QTreeWidgetItem *item_core_push;
    QCheckBox *widget_core_push;

    QTreeWidgetItem *item_core_caching;
    QCheckBox *widget_core_caching;

    QTreeWidgetItem *item_misc_thread;
    QCheckBox *widget_misc_thread;

    QTreeWidgetItem *item_misc_unique;
    QCheckBox *widget_misc_unique;

    QTreeWidgetItem *item_misc_lifetimes;
    QCheckBox *widget_misc_lifetimes;

    QTreeWidgetItem *item_misc_param;
    QCheckBox *widget_misc_param;

    QTreeWidgetItem *item_shader;
    QCheckBox *widget_shader;

    QTreeWidgetItem *item_shader_gpu;
    QRadioButton *widget_shader_gpu;

    QTreeWidgetItem *item_shader_gpu_reserve;
    QCheckBox *widget_shader_gpu_reserve;

    QTreeWidgetItem *item_shader_gpu_oob;
    QCheckBox *widget_shader_gpu_oob;

    QTreeWidgetItem *item_shader_gpu_desc_indexing;
    QCheckBox *widget_shader_gpu_desc_indexing;

    QTreeWidgetItem *item_shader_gpu_indirect;
    QCheckBox *widget_shader_gpu_indirect;

    QTreeWidgetItem *item_shader_dispatch_indirect;
    QCheckBox *widget_shader_dispatch_indirect;

    QTreeWidgetItem *item_shader_gpu_vma_linear_output;
    QCheckBox *widget_shader_gpu_vma_linear_output;

    QTreeWidgetItem *item_shader_printf;
    QRadioButton *widget_shader_printf;

    QTreeWidgetItem *item_shader_printf_to_stdout;
    QCheckBox *widget_shader_printf_to_stdout;

    QTreeWidgetItem *item_shader_printf_verbose;
    QCheckBox *widget_shader_printf_verbose;

    QTreeWidgetItem *item_shader_printf_size;
    WidgetSettingInt *widget_debug_printf_size;

    QTreeWidgetItem *item_sync;
    QCheckBox *widget_sync;

    QTreeWidgetItem *item_sync_queue_submit;
    QCheckBox *widget_sync_queue_submit;

    QTreeWidgetItem *item_best;
    QCheckBox *widget_best;

    QTreeWidgetItem *item_best_arm;
    QCheckBox *widget_best_arm;

    QTreeWidgetItem *item_best_amd;
    QCheckBox *widget_best_amd;

    QTreeWidgetItem *item_best_nvidia;
    QCheckBox *widget_best_nvidia;

    QCheckBox *CreateWidget(QTreeWidgetItem *parent, QTreeWidgetItem **item, const char *key, const char *flag);

   public Q_SLOTS:
    void OnCoreChecked(bool checked);

    void OnCoreLayoutChecked(bool checked);
    void OnCoreCommandChecked(bool checked);
    void OnCoreObjectChecked(bool checked);
    void OnCoreQueryChecked(bool checked);
    void OnCoreDescChecked(bool checked);
    void OnCoreShaderChecked(bool checked);
    void OnCorePushChecked(bool checked);
    void OnCoreCachingChecked(bool checked);

    void OnMiscThreadChecked(bool checked);
    void OnMiscUniqueChecked(bool checked);
    void OnMiscLifetimesChecked(bool checked);
    void OnMiscParamChecked(bool checked);

    void OnShaderBasedChecked(bool checked);

    void OnShaderGPUChecked(bool checked);
    void OnShaderGPUReserveChecked(bool checked);
    void OnShaderGPUDescIndexingChecked(bool checked);
    void OnShaderGPUOOBChecked(bool checked);
    void OnShaderGPUIndirectChecked(bool checked);
    void OnShaderDispatchIndirectChecked(bool checked);
    void OnShaderGPUVMALinearOutput(bool checked);

    void OnShaderPrintfChecked(bool checked);
    void OnShaderPrintfStdoutChecked(bool checked);
    void OnShaderPrintfVerboseChecked(bool checked);

    void OnSyncChecked(bool checked);
    void OnSyncQueueSubmitChecked(bool checked);

    void OnBestChecked(bool checked);
    void OnBestArmChecked(bool checked);
    void OnBestAmdChecked(bool checked);
    void OnBestNvidiaChecked(bool checked);

    void OnSettingChanged();

   Q_SIGNALS:
    void itemChanged();

   private:
    WidgetSettingValidation(const WidgetSettingValidation &) = delete;
    WidgetSettingValidation &operator=(const WidgetSettingValidation &) = delete;

    enum Overhead {
        OVERHEAD_CORE = 0,
        OVERHEAD_SHADER,
        OVERHEAD_SYNC,
        OVERHEAD_BEST,

        OVERHEAD_FIRST = OVERHEAD_CORE,
        OVERHEAD_LAST = OVERHEAD_BEST,
    };

    enum { OVERHEAD_COUNT = OVERHEAD_LAST - OVERHEAD_FIRST + 1 };

    bool CheckOverhead(Overhead candidate) const;

    void UpdateFlag(const char *key, const char *flag, bool append);
    bool HasDataBool(const char *key) const;
    bool HasDataFlag(const char *key, const char *flag) const;
    const SettingEnumValue *GetMetaFlag(const char *key, const char *flag) const;

    const SettingMetaSet &meta_set;
    SettingDataSet &data_set;
};
