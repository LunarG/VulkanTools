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
#ifndef VKTRACEVIEWER_CONTROLLER_FACTORY_H
#define VKTRACEVIEWER_CONTROLLER_FACTORY_H

#include <QMap>

extern "C" {
#include "vktrace_common.h"
#include "vktrace_trace_packet_identifiers.h"
}

#include "vktraceviewer_controller.h"

class vktraceviewer_controller_factory {
  public:
    vktraceviewer_controller_factory();
    ~vktraceviewer_controller_factory();

    vktraceviewer_QController* Load(const char* filename);
    void Unload(vktraceviewer_QController** ppController);

  private:
    QMap<vktraceviewer_QController*, void*> m_controllerToLibraryMap;
};

#endif  // VKTRACEVIEWER_CONTROLLER_FACTORY_H
