/**************************************************************************
 *
 * Copyright 2013-2014 RAD Game Tools and Valve Software
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
 **************************************************************************/

#ifndef GLVDEBUG_APICALLITEM_H
#define GLVDEBUG_APICALLITEM_H

//#include "glvdebug_snapshotitem.h"
#include "glv_trace_packet_identifiers.h"

// predeclared classes
// class glvdebug_frameItem;
// class glvdebug_groupItem;
// class vogl_trace_packet;

class glvdebug_apiCallItem  //: public vogleditor_snapshotItem
{
  public:
    glvdebug_apiCallItem(glvdebug_apiCallTreeItem* pParent, glv_trace_packet_header* pTracePacket)
    //: m_pParentFrame(pFrame),
    //  m_glPacket(glPacket),
    // m_pTracePacket(pTracePacket),
    // m_globalCallIndex(glPacket.m_call_counter),
    // m_begin_rdtsc(glPacket.m_packet_begin_rdtsc),
    // m_end_rdtsc(glPacket.m_packet_end_rdtsc),
    // m_backtrace_hash_index(glPacket.m_backtrace_hash_index)
    {
        // if (m_end_rdtsc < m_begin_rdtsc)
        //{
        //    m_end_rdtsc = m_begin_rdtsc + 1;
        //}
    }

    ~glvdebug_apiCallItem() {
        // if (m_pTracePacket != NULL)
        //{
        //    vogl_delete(m_pTracePacket);
        //    m_pTracePacket = NULL;
        //}
    }

    //    inline vogleditor_frameItem *frame() const
    //    {
    //        return m_pParentFrame;
    //    }
    //
    //    inline vogleditor_groupItem *group() const
    //    {
    //        return m_pParentGroup;
    //    }
    //
    //    inline uint64_t globalCallIndex() const
    //    {
    //        return m_globalCallIndex;
    //    }
    //
    //    inline uint64_t startTime() const
    //    {
    //        return m_begin_rdtsc;
    //    }
    //
    //    inline uint64_t endTime() const
    //    {
    //        return m_end_rdtsc;
    //    }
    //
    //    inline uint64_t duration() const
    //    {
    //        return endTime() - startTime();
    //    }
    //
    //    const vogl_trace_gl_entrypoint_packet *getGLPacket() const
    //    {
    //        return &m_glPacket;
    //    }
    //
    //    vogl_trace_packet *getTracePacket()
    //    {
    //        return m_pTracePacket;
    //    }
    //
    //    inline uint64_t backtraceHashIndex() const
    //    {
    //        return m_backtrace_hash_index;
    //    }
    //
    //    // Returns the api function call and its args as a string
    //    QString apiFunctionCall()
    //    {
    //        const gl_entrypoint_desc_t &entrypoint_desc = g_vogl_entrypoint_descs[m_pTracePacket->get_entrypoint_id()];
    //
    //        QString funcCall = entrypoint_desc.m_pName;
    //
    //        // format parameters
    //        funcCall.append("( ");
    //        dynamic_string paramStr;
    //        for (uint param_index = 0; param_index < m_pTracePacket->total_params(); param_index++)
    //        {
    //            if (param_index != 0)
    //                funcCall.append(", ");
    //
    //            paramStr.clear();
    //            m_pTracePacket->pretty_print_param(paramStr, param_index, false);
    //
    //            funcCall.append(paramStr.c_str());
    //        }
    //        funcCall.append(" )");
    //
    //        if (m_pTracePacket->has_return_value())
    //        {
    //            funcCall.append(" = ");
    //            paramStr.clear();
    //            m_pTracePacket->pretty_print_return_value(paramStr, false);
    //            funcCall.append(paramStr.c_str());
    //        }
    //        return funcCall;
    //    }
    //
    //    // Returns the string argument of an apicall in apiFunctionCall() output format
    //    //
    //    // TODO: (as needed) Add logic to return which string (argument count) from
    //    //                   a multi-string argument list (or all as a QStringList)
    //    QString stringArg()
    //    {
    //        QString apiCall = apiFunctionCall();
    //
    //        QString sec, name;
    //        int start = 1;
    //        while (!(sec = apiCall.section('\'', start, start)).isEmpty())
    //        {
    //            name.append(sec);
    //            start += 2;
    //        }
    //        return name;
    //    }
    //
    // private:
    //    glvdebug_apiCallTreeItem *m_pParentFrame;
    //    glvdebug_groupItem *m_pParentGroup;
    ////    const vogl_trace_gl_entrypoint_packet m_glPacket;
    ////    vogl_trace_packet *m_pTracePacket;
    //
    //    uint64_t m_globalCallIndex;
    //    uint64_t m_begin_rdtsc;
    //    uint64_t m_end_rdtsc;
    //    uint64_t m_backtrace_hash_index;
};

#endif  // GLVDEBUG_APICALLITEM_H
