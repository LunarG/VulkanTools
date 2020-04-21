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

#ifndef GLVDEBUG_QAPICALLTREEMODEL_H
#define GLVDEBUG_QAPICALLTREEMODEL_H

#include <QAbstractItemModel>
#include <QLinkedList>

#include "glv_common.h"

class QVariant;
class glvdebug_apiCallTreeItem;
// class glvdebug_groupItem;
// class glvdebug_frameItem;
class glvdebug_apiCallItem;

class glvdebug_QApiCallTreeModel : public QAbstractItemModel {
    Q_OBJECT

  public:
    glvdebug_QApiCallTreeModel(int columnCount, QObject *parent = 0);
    ~glvdebug_QApiCallTreeModel();

    // required to implement
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // void appendChild(glvdebug_apiCallTreeItem* pItem)
    //{
    //    m_rootItem->appendChild(pItem);
    //}

    // virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    // virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    // QModelIndex indexOf(const glvdebug_apiCallTreeItem *pItem) const;

    // glvdebug_apiCallTreeItem *root() const
    //{
    //    return m_rootItem;
    //}

    // glvdebug_apiCallTreeItem *create_group(glvdebug_frameItem *pFrameObj,
    //                                         glvdebug_groupItem *&pGroupObj,
    //                                         glvdebug_apiCallTreeItem *pParentNode);
    // void set_highlight_search_string(const QString searchString);
    // QModelIndex find_prev_search_result(glvdebug_apiCallTreeItem *start, const QString searchText);
    // QModelIndex find_next_search_result(glvdebug_apiCallTreeItem *start, const QString searchText);

    // glvdebug_apiCallTreeItem *find_prev_snapshot(glvdebug_apiCallTreeItem *start);
    // glvdebug_apiCallTreeItem *find_next_snapshot(glvdebug_apiCallTreeItem *start);

    // glvdebug_apiCallTreeItem *find_prev_drawcall(glvdebug_apiCallTreeItem *start);
    // glvdebug_apiCallTreeItem *find_next_drawcall(glvdebug_apiCallTreeItem *start);

    // glvdebug_apiCallTreeItem *find_call_number(unsigned int callNumber);
    // glvdebug_apiCallTreeItem *find_frame_number(unsigned int frameNumber);

  signals:

  public slots:

  private:
    // gl_entrypoint_id_t itemApiCallId(glvdebug_apiCallTreeItem *apiCall) const;
    // gl_entrypoint_id_t lastItemApiCallId() const;

    // bool processMarkerPushEntrypoint(gl_entrypoint_id_t id);
    // bool processMarkerPopEntrypoint(gl_entrypoint_id_t id);
    // bool processStartNestedEntrypoint(gl_entrypoint_id_t id);
    // bool processEndNestedEntrypoint(gl_entrypoint_id_t id);
    // bool processFrameBufferWriteEntrypoint(gl_entrypoint_id_t id);

  private:
    int m_columnCount;
    glvdebug_apiCallTreeItem *m_rootItem;
    QLinkedList<glvdebug_apiCallTreeItem *> m_itemList;
    //    QString m_searchString;
};

#endif  // GLVDEBUG_QAPICALLTREEMODEL_H
