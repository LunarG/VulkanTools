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

#ifndef VKTRACEVIEWER_QTIMELINEVIEW_H
#define VKTRACEVIEWER_QTIMELINEVIEW_H

#include <stdint.h>
#include "vktrace_trace_packet_identifiers.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPainter;
class QPaintEvent;
QT_END_NAMESPACE

#include <QAbstractItemView>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QScrollBar>

class vktraceviewer_QTimelineItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    vktraceviewer_QTimelineItemDelegate(QObject *parent = 0);
    virtual ~vktraceviewer_QTimelineItemDelegate();

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

// Implementation of the QTimelineView has benefited greatly from the following site:
// http://www.informit.com/articles/article.aspx?p=1613548
class vktraceviewer_QTimelineView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit vktraceviewer_QTimelineView(QWidget *parent = 0);
    virtual ~vktraceviewer_QTimelineView();

    virtual void setModel(QAbstractItemModel* pModel);

    // Begin public virtual functions of QAbstractItemView
    virtual QRect visualRect(const QModelIndex &index) const;
    virtual void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible);
    virtual QModelIndex indexAt(const QPoint &point) const;
    // End public virtual functions of QAbstractItemView

    QList<uint32_t> getModelThreadList() const;
    QRectF itemRect(const QModelIndex &item) const;
    float getMaxItemDuration() const
    {
        return m_maxItemDuration;
    }

    void deletePixmap()
    {
        if (m_pPixmap != NULL)
        {
            delete m_pPixmap;
            m_pPixmap = NULL;
        }
    }

private:
    QBrush m_background;
    QPen m_trianglePen;
    QPen m_textPen;
    QFont m_textFont;

    // new members
    QList<uint32_t> m_threadIdList;
    QHash< uint32_t, QVector<int> > m_threadMask;
    QList<QRect> m_threadArea;
    float m_maxItemDuration;
    uint64_t m_rawStartTime;
    uint64_t m_rawEndTime;
    uint64_t m_lineLength;
    float m_durationToViewportScale;
    float m_zoomFactor;
    float m_maxZoom;
    int m_threadHeight;
    QHash<int, QRectF> m_rowToRectMap;
    bool m_hashIsDirty;
    int m_margin;
    int m_scrollBarWidth;
    QPoint m_mousePosition;

    QPixmap *m_pPixmap;
    vktraceviewer_QTimelineItemDelegate m_itemDelegate;

    void calculateRectsIfNecessary();
    void drawBaseTimelines(QPainter *painter, const QRect &rect, const QList<uint32_t> &threadList);
    void drawTimelineItem(QPainter* painter, const QModelIndex &index);

    QRectF viewportRect(const QModelIndex &index) const;
    float scaleDurationHorizontally(uint64_t value) const;
    float scalePositionHorizontally(uint64_t value) const;

    // Begin Private...
    virtual QRegion itemRegion(const QModelIndex &index) const;
    // End private...

protected:
    void paintEvent(QPaintEvent *event);
    void paint(QPainter *painter, QPaintEvent *event);

    virtual bool event(QEvent * e);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void scrollContentsBy(int dx, int dy);

    // Begin protected virtual functions of QAbstractItemView
    virtual QModelIndex moveCursor(CursorAction cursorAction,
                                   Qt::KeyboardModifiers modifiers)
    {
        return QModelIndex();
    }

    virtual int horizontalOffset() const
    {
        return horizontalScrollBar()->value();
    }
    virtual int verticalOffset() const
    {
        return verticalScrollBar()->value();
    }

    virtual bool isIndexHidden(const QModelIndex &index) const
    {
        return false;
    }

    virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) {}
    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const
    {
        return QRegion();
    }
    // End protected virtual functions of QAbstractItemView

protected slots:
    virtual void updateGeometries();

signals:

public
slots:
};

#endif // VKTRACEVIEWER_QTIMELINEVIEW_H
