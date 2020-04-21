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

#ifndef GLVDEBUG_APICALLTREEITEM_H
#define GLVDEBUG_APICALLTREEITEM_H

#include <QList>
#include <QVariant>

typedef unsigned long long uint64_t;

class glvdebug_frameItem;
class glvdebug_groupItem;
class glvdebug_apiCallItem;

class glvdebug_QApiCallTreeModel;

const QString cTREEITEM_STATECHANGES("State changes");
// TODO: Maybe think about a more unique name so as not to be confused with,
//       e.g., a marker_push entrypoint that has also been named "Render"
const QString cTREEITEM_RENDER("Render");

class glvdebug_apiCallTreeItem {
  public:
    // Constructor for the root node
    glvdebug_apiCallTreeItem(int columnCount, glvdebug_QApiCallTreeModel *pModel);

    //// Constructor for frame nodes
    // glvdebug_apiCallTreeItem(glvdebug_frameItem *frameItem, glvdebug_apiCallTreeItem *parent);

    //// Constructor for group nodes
    // glvdebug_apiCallTreeItem(glvdebug_groupItem *groupItem, glvdebug_apiCallTreeItem *parent);

    // Constructor for apiCall nodes
    glvdebug_apiCallTreeItem(glvdebug_apiCallItem *apiCallItem);

    ~glvdebug_apiCallTreeItem();

    void setParent(glvdebug_apiCallTreeItem *pParent);
    glvdebug_apiCallTreeItem *parent() const;

    void appendChild(glvdebug_apiCallTreeItem *pChild);
    void popChild();

    int childCount() const;

    glvdebug_apiCallTreeItem *child(int index) const;

    glvdebug_apiCallItem *apiCallItem() const;
    glvdebug_groupItem *groupItem() const;
    glvdebug_frameItem *frameItem() const;

    int columnCount() const;

    QVariant columnData(int column, int role) const;

    int row() const;

    // bool isApiCall() const;
    // bool isGroup() const;
    // bool isFrame() const;
    // bool isRoot() const;

  private:
    //    void setColumnData(QVariant data, int column);

  private:
    QList<glvdebug_apiCallTreeItem *> m_childItems;
    QVariant *m_columnData;
    int m_columnCount;
    glvdebug_apiCallTreeItem *m_parentItem;
    // glvdebug_apiCallItem *m_pApiCallItem;
    // glvdebug_groupItem *m_pGroupItem;
    // glvdebug_frameItem *m_pFrameItem;
    glvdebug_QApiCallTreeModel *m_pModel;
    int m_localRowIndex;
};

#endif  // GLVDEBUG_APICALLTREEITEM_H
