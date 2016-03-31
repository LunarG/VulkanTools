/**************************************************************************
*
* Copyright 2015-2016 Valve Corporation
* Copyright (C) 2015-2016 LunarG, Inc.
* All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
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

class vktraceviewer_controller_factory
{
public:
    vktraceviewer_controller_factory();
    ~vktraceviewer_controller_factory();

    vktraceviewer_QController* Load(const char* filename);
    void Unload(vktraceviewer_QController** ppController);

private:
    QMap<vktraceviewer_QController*, void*> m_controllerToLibraryMap;
};

#endif // VKTRACEVIEWER_CONTROLLER_FACTORY_H
