/**************************************************************************
 *
 * Copyright 2014-2016 Valve Corporation
 * Copyright (C) 2014-2016 LunarG, Inc.
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

#include <QApplication>
#include <QPainter>
#include <QPaintEvent>
#include <QToolTip>
#ifdef _WIN32
// The following line allows Visual Studio to provide the M_PI_2 constant:
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "vktraceviewer_qtimelineview.h"
#include "vktraceviewer_QTraceFileModel.h"

// helper
float u64ToFloat(uint64_t value)
{
    // taken from: http://stackoverflow.com/questions/4400747/converting-from-unsigned-long-long-to-float-with-round-to-nearest-even
    const int mask_bit_count = 31;

    // How many bits are needed?
    int b = sizeof(uint64_t) * CHAR_BIT - 1;
    for (; b >= 0; --b)
    {
        if (value & (1ull << b))
        {
            break;
        }
    }

    // If there are few enough significant bits, use normal cast and done.
    if (b < mask_bit_count)
    {
        return static_cast<float>(value & ~1ull);
    }

    // Save off the low-order useless bits:
    uint64_t low_bits = value & ((1ull << (b - mask_bit_count)) - 1);

    // Now mask away those useless low bits:
    value &= ~((1ull << (b - mask_bit_count)) - 1);

    // Finally, decide how to round the new LSB:
    if (low_bits > ((1ull << (b - mask_bit_count)) / 2ull))
    {
        // Round up.
        value |= (1ull << (b - mask_bit_count));
    }
    else
    {
        // Round down.
        value &= ~(1ull << (b - mask_bit_count));
    }

    return static_cast<float>(value);
}

//=============================================================================
vktraceviewer_QTimelineItemDelegate::vktraceviewer_QTimelineItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    assert(parent != NULL);
}

vktraceviewer_QTimelineItemDelegate::~vktraceviewer_QTimelineItemDelegate()
{

}

void vktraceviewer_QTimelineItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();

    if (pHeader->entrypoint_end_time <= pHeader->entrypoint_begin_time)
    {
        return;
    }

    painter->save();
    {
        vktraceviewer_QTimelineView* pTimeline = (vktraceviewer_QTimelineView*)parent();
        if (pTimeline != NULL)
        {
            QRectF rect = option.rect;

            if (rect.width() == 0)
            {
                rect.setWidth(1);
            }

            float duration = u64ToFloat(pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time);
            float durationRatio = duration / pTimeline->getMaxItemDuration();
            int intensity = std::min(255, (int)(durationRatio * 255.0f));
            QColor color(intensity, 255-intensity, 0);

            // add gradient to the items better distinguish between the end of one and beginning of the next
            QLinearGradient linearGrad(rect.center(), rect.bottomRight());
            linearGrad.setColorAt(0, color);
            linearGrad.setColorAt(1, color.darker(150));

            painter->setBrush(linearGrad);
            painter->setPen(Qt::NoPen);

            painter->drawRect(rect);

            if (rect.width() >= 2)
            {
                // draw shadow and highlight around the item
                painter->setPen(color.darker(175));
                painter->drawLine(rect.right()-1, rect.top(), rect.right()-1, rect.bottom()-1);
                painter->drawLine(rect.right()-1, rect.bottom()-1, rect.left(), rect.bottom()-1);

                painter->setPen(color.lighter());
                painter->drawLine(rect.left(), rect.bottom()-1, rect.left(), rect.top());
                painter->drawLine(rect.left(), rect.top(), rect.right()-1, rect.top());
            }
        }
    }

    painter->restore();
}

QSize vktraceviewer_QTimelineItemDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size;

    vktraceviewer_QTimelineView* pTimeline = (vktraceviewer_QTimelineView*)parent();
    if (pTimeline != NULL)
    {
        QRectF rect = pTimeline->visualRect(index);

        size = rect.toRect().size();
    }
    return QSize();
}

//=============================================================================
vktraceviewer_QTimelineView::vktraceviewer_QTimelineView(QWidget *parent) :
    QAbstractItemView(parent),
    m_maxItemDuration(0),
    m_maxZoom(0.001f),
    m_threadHeight(0),
    m_hashIsDirty(true),
    m_margin(10),
    m_pPixmap(NULL),
    m_itemDelegate(this)
{
    horizontalScrollBar()->setRange(0,0);
    verticalScrollBar()->setRange(0,0);

    m_background = QBrush(QColor(200,200,200));
    m_trianglePen = QPen(Qt::darkGray);
    m_trianglePen.setWidth(1);
    m_textPen = QPen(Qt::white);
    m_textFont.setPixelSize(50);

    // Allows tracking the mouse position when it is over the timeline
    setMouseTracking(true);

    m_durationToViewportScale = 1;
    m_zoomFactor = 1;
    m_lineLength = 1;
    m_scrollBarWidth = QApplication::style()->pixelMetric(QStyle::PM_ScrollBarExtent);
}

//-----------------------------------------------------------------------------
vktraceviewer_QTimelineView::~vktraceviewer_QTimelineView()
{
    m_threadIdList.clear();
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::setModel(QAbstractItemModel* pModel)
{
    QAbstractItemView::setModel(pModel);
    m_hashIsDirty = true;
    setItemDelegate(&m_itemDelegate);

    m_threadIdList.clear();
    m_threadMask.clear();
    m_maxItemDuration = 0;
    m_rawStartTime = 0;
    m_rawEndTime = 0;

    deletePixmap();

    // Gather some stats from the model
    if (model() == NULL)
    {
        horizontalScrollBar()->setRange(0,0);
        verticalScrollBar()->setRange(0,0);
        return;
    }

    int numRows = model()->rowCount();
    for (int i = 0; i < numRows; i++)
    {
        // Count number of unique thread Ids
        QModelIndex item = model()->index(i, vktraceviewer_QTraceFileModel::Column_ThreadId);
        if (item.isValid())
        {
            uint32_t threadId = item.data().toUInt();
            if (!m_threadIdList.contains(threadId))
            {
                m_threadIdList.append(threadId);
                m_threadMask.insert(threadId, QVector<int>());
                m_threadArea.append(QRect());
            }
        }

        // Find duration of longest item
        item = model()->index(i, vktraceviewer_QTraceFileModel::Column_CpuDuration);
        if (item.isValid())
        {
            float duration = item.data().toFloat();
            if (m_maxItemDuration < duration)
            {
                m_maxItemDuration = duration;
            }
        }
    }

    // Get start time
    QModelIndex start = model()->index(0, vktraceviewer_QTraceFileModel::Column_BeginTime);
    if (start.isValid())
    {
        m_rawStartTime = start.data().toULongLong();
    }

    // Get end time
    QModelIndex end = model()->index(numRows - 1, vktraceviewer_QTraceFileModel::Column_EndTime);
    if (end.isValid())
    {
        m_rawEndTime = end.data().toULongLong();
    }

    // the duration to viewport scale should allow us to map the entire timeline into the current window width.
    m_lineLength = m_rawEndTime - m_rawStartTime;

    int initialTimelineWidth = viewport()->width() - 2*m_margin - m_scrollBarWidth;
    m_durationToViewportScale = (float)initialTimelineWidth / u64ToFloat(m_lineLength);

    m_zoomFactor = m_durationToViewportScale;

    verticalScrollBar()->setMaximum(1000);
    verticalScrollBar()->setValue(0);
    verticalScrollBar()->setPageStep(1);
    verticalScrollBar()->setSingleStep(1);
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::calculateRectsIfNecessary()
{
    if (!m_hashIsDirty)
    {
        return;
    }

    if (model() == NULL)
    {
        return;
    }

    int itemHeight = m_threadHeight * 0.4;

    for (int threadIndex = 0; threadIndex < m_threadIdList.size(); threadIndex++)
    {
        int top = (m_threadHeight * threadIndex) + (m_threadHeight * 0.5) - itemHeight/2;
        this->m_threadArea[threadIndex] = QRect(0, top, viewport()->width(), itemHeight);
    }

    int numRows = model()->rowCount();
    for (int row = 0; row < numRows; row++)
    {
        QRectF rect;
        QModelIndex item = model()->index(row, vktraceviewer_QTraceFileModel::Column_EntrypointName);

        vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)item.internalPointer();

        // make sure item is valid size
        if (pHeader->entrypoint_end_time > pHeader->entrypoint_begin_time)
        {
            int threadIndex = m_threadIdList.indexOf(pHeader->thread_id);
            int topOffset = (m_threadHeight * threadIndex) + (m_threadHeight * 0.5);

            uint64_t duration = pHeader->entrypoint_end_time - pHeader->entrypoint_begin_time;

            float leftOffset = u64ToFloat(pHeader->entrypoint_begin_time - m_rawStartTime);
            float Width = u64ToFloat(duration);

            // create the rect that represents this item
            rect.setLeft(leftOffset);
            rect.setTop(topOffset - (itemHeight/2));
            rect.setWidth(Width);
            rect.setHeight(itemHeight);
        }

        m_rowToRectMap[row] = rect;
    }

    m_hashIsDirty = false;
    viewport()->update();
}

//-----------------------------------------------------------------------------
QRectF vktraceviewer_QTimelineView::itemRect(const QModelIndex &item) const
{
    QRectF rect;
    if (item.isValid())
    {
        rect = m_rowToRectMap.value(item.row());
    }
    return rect;
}

//-----------------------------------------------------------------------------
bool vktraceviewer_QTimelineView::event(QEvent * e)
{
    if (e->type() == QEvent::ToolTip)
    {
        QHelpEvent* pHelp = static_cast<QHelpEvent*>(e);
        QModelIndex index = indexAt(pHelp->pos());
        if (index.isValid())
        {
            vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();
            QToolTip::showText(pHelp->globalPos(), QString("Call %1:\n%2").arg(pHeader->global_packet_index).arg(index.data().toString()));
            return true;
        }
        else
        {
            QToolTip::hideText();
        }
    }

    return QAbstractItemView::event(e);
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::resizeEvent(QResizeEvent *event)
{
    m_hashIsDirty = true;
    deletePixmap();

    // The duration to viewport scale should allow us to map the entire timeline into the current window width.
    if (m_lineLength > 0)
    {
        // Calculate zoom ratio prior to the resize
        float ratio = m_zoomFactor / m_durationToViewportScale;

        // Adjust scale that fits the timeline duration to the viewport area
        int timelineViewportWidth = viewport()->width() - 2*m_margin - m_scrollBarWidth;
        m_durationToViewportScale = (float)timelineViewportWidth / u64ToFloat(m_lineLength);

        // Adjust the zoom factor based on the new duration to viewport scale and the previous ratio
        m_zoomFactor = m_durationToViewportScale * ratio;

        // Adjust horizontal scroll bar to maintain current view as best as possible
        float hRatio = (float)horizontalScrollBar()->value() / qMax(1.0f,(float)horizontalScrollBar()->maximum());
        updateGeometries();
        horizontalScrollBar()->setValue(hRatio * horizontalScrollBar()->maximum());
    }
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::scrollContentsBy(int dx, int dy)
{
    deletePixmap();

    if (dy != 0)
    {
        QPoint pos = m_mousePosition;
        int focusX = pos.x();
        if (pos.isNull())
        {
            // If the mouse position is NULL (ie, the mouse is not in the viewport)
            // zooming will happen around the center of the timeline
            focusX = (viewport()->width() - m_scrollBarWidth) / 2;
        }

        int x = focusX + horizontalScrollBar()->value();
        float wx = (float)x / m_zoomFactor;

        // The zoom factor is a smoothed interpolation (based on the vertical scroll bar value and max)
        // between the m_durationToViewportScale (which fits the entire timeline into the viewport width)
        // and m_maxZoom (which is initialized to 1/1000)
        float zoomRatio = (float)verticalScrollBar()->value() / (float)verticalScrollBar()->maximum();
        zoomRatio = 1-cos(zoomRatio*M_PI_2);
        float diff = m_maxZoom - m_durationToViewportScale;
        m_zoomFactor = m_durationToViewportScale + zoomRatio * diff;
        updateGeometries();

        int newValue = (wx * m_zoomFactor) - focusX;

        horizontalScrollBar()->setValue(newValue);
    }

    viewport()->update();
//    scrollDirtyRegion(dx, 0);
//    viewport()->scroll(dx, 0);
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::mousePressEvent(QMouseEvent * event)
{
    QAbstractItemView::mousePressEvent(event);
    QModelIndex index = indexAt(event->pos());
    if (index.isValid())
    {
        setCurrentIndex(index);
    }
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::mouseMoveEvent(QMouseEvent * event)
{
    // Since we are tracking the mouse position, we really should pass the event
    // to our base class, Unfortunately it really sucks up performance, so don't do it for now.
    //QAbstractItemView::mouseMoveEvent(event);

    if (event->pos().x() > viewport()->width() - m_margin)
    {
        // The mouse position was within m_margin of the vertical scroll bar (used for zooming)
        // Make this a null point so that zooming will happen on the center of the timeline
        // rather than the very edge where the mouse cursor was last seen.
        m_mousePosition = QPoint();
    }
    else
    {
        m_mousePosition = event->pos();
    }
    event->accept();
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::updateGeometries()
{
    uint64_t duration = m_rawEndTime - m_rawStartTime;
    int hMax = scaleDurationHorizontally(duration) + 2*m_margin - viewport()->width();
    horizontalScrollBar()->setRange(0, qMax(0, hMax));
    horizontalScrollBar()->setPageStep(viewport()->width());
    horizontalScrollBar()->setSingleStep(1);
}

//-----------------------------------------------------------------------------
QRect vktraceviewer_QTimelineView::visualRect(const QModelIndex &index) const
{
    QRectF rectf = viewportRect(index);
    return rectf.toRect();
}

//-----------------------------------------------------------------------------
QRectF vktraceviewer_QTimelineView::viewportRect(const QModelIndex &index) const
{
    QRectF rectf = itemRect(index);
    if (rectf.isValid())
    {
        rectf.moveLeft( rectf.left() * m_zoomFactor);
        rectf.setWidth( rectf.width() * m_zoomFactor);

        rectf.adjust(-horizontalScrollBar()->value(), 0,
                     -horizontalScrollBar()->value(), 0);

        // the margin is added last so that it is NOT scaled
        rectf.adjust(m_margin, 0,
                     m_margin, 0);
    }

    return rectf;
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::scrollTo(const QModelIndex &index, ScrollHint hint/* = EnsureVisible*/)
{
    if (!index.isValid())
    {
        return;
    }

    QRect viewRect = viewport()->rect();
    QRect itemRect = visualRect(index);

    if (itemRect.left() < viewRect.left())
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + itemRect.center().x() - viewport()->width()/2);
    }
    else if (itemRect.right() > viewRect.right())
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + itemRect.center().x() - viewport()->width()/2);
    }

    if (!viewRect.contains(itemRect))
    {
        deletePixmap();
    }
    viewport()->update();
}

//-----------------------------------------------------------------------------
QModelIndex vktraceviewer_QTimelineView::indexAt(const QPoint &point) const
{
    if (model() == NULL)
        return QModelIndex();

    float wy = (float)point.y();

    // Early out if the point is not in the areas covered by timeline items
    bool inThreadArea = false;
    for (int i = 0; i < m_threadArea.size(); i++)
    {
        if (wy >= m_threadArea[i].top() && wy <= m_threadArea[i].bottom())
        {
            inThreadArea = true;
            break;
        }
    }

    if (inThreadArea == false)
    {
        // point is outside the areas that timeline items are drawn to.
        return QModelIndex();
    }

    // Transform the view coordinates into content widget coordinates.
    int x = point.x() - m_margin + horizontalScrollBar()->value();
    float wx = (float)x / m_zoomFactor;

    QHashIterator<int, QRectF> iter(m_rowToRectMap);
    while (iter.hasNext())
    {
        iter.next();
        QRectF value = iter.value();
        if (value.contains(wx, wy))
        {
            return model()->index(iter.key(), vktraceviewer_QTraceFileModel::Column_EntrypointName);
        }
    }

    return QModelIndex();
}

//-----------------------------------------------------------------------------
QRegion vktraceviewer_QTimelineView::itemRegion(const QModelIndex &index) const
{
    if (!index.isValid())
        return QRegion();

    return QRegion(viewportRect(index).toRect());
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::paintEvent(QPaintEvent *event)
{
    QPainter painter(viewport());
    paint(&painter, event);
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::drawBaseTimelines(QPainter* painter, const QRect& rect, const QList<uint32_t> &threadList)
{
    int numThreads = threadList.count();

    painter->save();
    QFont font = painter->font();
    int fontHeight = qMin((int)(m_threadHeight * 0.3), font.pointSize());
    font.setPointSize(fontHeight);
    painter->setFont(font);

    for (int i = 0; i < numThreads; i++)
    {
        int threadTop = (i*m_threadHeight);

        painter->drawText(0, threadTop + fontHeight, QString("Thread %1").arg(threadList[i]));

        // draw the timeline in the middle of this thread's area
        int lineStart = m_margin - horizontalOffset();
        int lineEnd = lineStart + scaleDurationHorizontally(m_lineLength);
        int lineY = threadTop + m_threadHeight/2;
        painter->drawLine(lineStart, lineY, lineEnd, lineY);
    }
    painter->restore();
}

//-----------------------------------------------------------------------------
QList<uint32_t> vktraceviewer_QTimelineView::getModelThreadList() const
{
    return m_threadIdList;
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::paint(QPainter *painter, QPaintEvent *event)
{
    m_threadHeight = event->rect().height();
    if (m_threadIdList.count() > 0)
    {
        m_threadHeight /= m_threadIdList.count();
    }

    int arrowHeight = 12;
    int arrowTop = 2;
    int arrowHalfWidth = 4;

    QPolygon triangle(3);
    triangle.setPoint(0, 0, arrowTop);
    triangle.setPoint(1, -arrowHalfWidth, arrowTop+arrowHeight);
    triangle.setPoint(2, arrowHalfWidth, arrowTop+arrowHeight);

    QList<uint32_t> threadList = getModelThreadList();

    calculateRectsIfNecessary();

    if (m_pPixmap == NULL)
    {
        int pixmapHeight = event->rect().height();
        int pixmapWidth = event->rect().width();

        m_pPixmap = new QPixmap(pixmapWidth, pixmapHeight);

        for (int t = 0; t < m_threadIdList.size(); t++)
        {
            m_threadMask[m_threadIdList[t]] = QVector<int>(pixmapWidth, 0);
        }

        QPainter pixmapPainter(m_pPixmap);

        // fill entire background with background color
        pixmapPainter.fillRect(event->rect(), m_background);
        drawBaseTimelines(&pixmapPainter, event->rect(), threadList);

        if (model() != NULL)
        {
            int numRows = model()->rowCount();

            for (int r = 0; r < numRows; r++)
            {
                QModelIndex index = model()->index(r, vktraceviewer_QTraceFileModel::Column_EntrypointName);

                drawTimelineItem(&pixmapPainter, index);
            }
        }
    }
    painter->drawPixmap(event->rect(), *m_pPixmap, m_pPixmap->rect());

    if (model() == NULL)
    {
        return;
    }

    // draw current api call marker
    int currentIndexRow = currentIndex().row();
    if (currentIndexRow >= 0)
    {
        // Overlay a black rectangle around the current item.
        // For more information on how rects are drawn as outlines,
        // see here: http://qt-project.org/doc/qt-4.8/qrectf.html#rendering
        int penWidth = 2;
        int penWidthHalf = 1;
        QPen blackPen(Qt::black);
        blackPen.setWidth(penWidth);
        blackPen.setJoinStyle(Qt::MiterJoin);
        painter->setPen(blackPen);

        // Don't fill the rectangle
        painter->setBrush(Qt::NoBrush);

        QModelIndex index = model()->index(currentIndexRow, vktraceviewer_QTraceFileModel::Column_EntrypointName);
        QRectF rect = visualRect(index);
        rect.adjust(-penWidthHalf, -penWidthHalf, penWidthHalf, penWidthHalf);
        painter->drawRect(rect);

        // Draw marker underneath the current rect
        painter->save();
        QPainter::RenderHints hints = painter->renderHints();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(m_trianglePen);
        painter->setBrush(QColor(Qt::yellow));
        painter->translate(rect.center().x(), rect.bottom());
        painter->drawPolygon(triangle);
        painter->setRenderHints(hints, false);
        painter->restore();
    }
}

//-----------------------------------------------------------------------------
float vktraceviewer_QTimelineView::scaleDurationHorizontally(uint64_t value) const
{
    float scaled = u64ToFloat(value) * m_zoomFactor;

    return scaled;
}

//-----------------------------------------------------------------------------
float vktraceviewer_QTimelineView::scalePositionHorizontally(uint64_t value) const
{
    uint64_t shiftedValue = value - m_rawStartTime;
    float offset = scaleDurationHorizontally(shiftedValue);

    return offset;
}

//-----------------------------------------------------------------------------
void vktraceviewer_QTimelineView::drawTimelineItem(QPainter* painter, const QModelIndex &index)
{
    QRectF rect = viewportRect(index);

    // don't draw if the rect is outside the viewport
    if (!rect.isValid() ||
        rect.bottom() < 0 ||
        rect.y() > viewport()->height() ||
        rect.x() > viewport()->width() ||
        rect.right() < 0)
    {
        return;
    }

    QStyleOptionViewItem option = viewOptions();
    option.rect = rect.toRect();
    if (selectionModel()->isSelected(index))
        option.state |= QStyle::State_Selected;
    if (currentIndex() == index)
        option.state |= QStyle::State_HasFocus;

    // check mask to determine if this item should be drawn, or if something has already covered it's pixels
    vktrace_trace_packet_header* pHeader = (vktrace_trace_packet_header*)index.internalPointer();
    QVector<int>& mask = m_threadMask[pHeader->thread_id];
    bool drawItem = false;
    int x = option.rect.x();
    int right = qMin( qMax(x, option.rect.right()), viewport()->width()-1);
    for (int pixel = qMax(0, x); pixel <= right; pixel++)
    {
        if (mask[pixel] == 0)
        {
            drawItem = true;
            mask[pixel] = 1;
        }
    }

    // draw item if it should be visible
    if (drawItem)
    {
        itemDelegate()->paint(painter, option, index);
    }
}
