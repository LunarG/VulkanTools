/**************************************************************************
 *
 * Copyright 2015-2016 Valve Corporation
 * Copyright (C) 2015-2016 LunarG, Inc.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Author: Peter Lohrmann <peterl@valvesoftware.com> <plohrmann@gmail.com>
 **************************************************************************/
#pragma once

#include "vktraceviewer_QTraceFileModel.h"

struct vktraceviewer_trace_file_info;
struct vktrace_SettingGroup;
class QWidget;
class QToolButton;
class QAction;
class QAbstractProxyModel;

class vktraceviewer_view {
  public:
    virtual void reset_view() = 0;

    //    virtual void output_message(uint64_t packetIndex, QString message) = 0;
    //    virtual void output_warning(uint64_t packetIndex, QString message) = 0;
    //    virtual void output_error(uint64_t packetIndex, QString message) = 0;

    virtual void add_setting_group(vktrace_SettingGroup* pGroup) = 0;
    virtual unsigned int get_global_settings(vktrace_SettingGroup** ppGroups) = 0;

    virtual void set_calltree_model(vktraceviewer_QTraceFileModel* pTraceFileModel, QAbstractProxyModel* pModel) = 0;
    virtual void add_calltree_contextmenu_item(QAction* pAction) = 0;
    virtual void select_call_at_packet_index(unsigned long long packetIndex) = 0;
    virtual void highlight_timeline_item(unsigned long long packetArrayIndex, bool bScrollTo, bool bSelect) = 0;

    // \return tab index of state viewer
    virtual int add_custom_state_viewer(QWidget* pWidget, const QString& title, bool bBringToFront = false) = 0;
    virtual void remove_custom_state_viewer(int tabIndex) = 0;
    virtual void enable_custom_state_viewer(QWidget* pWidget, bool bEnabled) = 0;

    virtual QToolButton* add_toolbar_button(const QString& title, bool bEnabled) = 0;

    virtual void on_replay_state_changed(bool bReplayInProgress) = 0;

    virtual unsigned long long get_current_packet_index() = 0;
};
