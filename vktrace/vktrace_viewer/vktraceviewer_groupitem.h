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

#pragma once

#include <QList>
//#include "glvdebug_snapshotitem.h"
#include "glvdebug_apicallitem.h"

class vogleditor_frameItem;

class vogleditor_groupItem  //: public vogleditor_snapshotItem
{
  public:
    vogleditor_groupItem(vogleditor_frameItem *pFrameItem) : m_pParentFrame(pFrameItem) {}

    ~vogleditor_groupItem() { m_apiCallList.clear(); }

    void appendCall(glvdebug_apiCallItem *pItem) { m_apiCallList.append(pItem); }

    inline int callCount() const { return m_apiCallList.size(); }

    glvdebug_apiCallItem *call(int index) const {
        if (index < 0 || index > callCount()) {
            return NULL;
        }
        return m_apiCallList[index];
    }

    // inline uint64_t firstApiCallIndex() const
    //{
    //    return apiCallIndex(0);
    //}

    // inline uint64_t apiCallIndex(int index = 0) const
    //{
    //    if (vogleditor_apiCallItem *apiCallItem = call(index))
    //    {
    //        return apiCallItem->globalCallIndex();
    //    }
    //    return uint64_t(-1); // (-1 index won't be found)
    //}

    // inline uint64_t startTime() const
    //{
    //    return m_apiCallList[0]->startTime();
    //}

    // inline uint64_t endTime() const
    //{
    //    return m_apiCallList[callCount() - 1]->endTime();
    //}

    // inline uint64_t duration() const
    //{
    //    return endTime() - startTime();
    //}

  private:
    vogleditor_frameItem *m_pParentFrame;
    QList<glvdebug_apiCallItem *> m_apiCallList;
};
