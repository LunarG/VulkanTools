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

#ifndef _VKTRACEVIEWER_QIMAGEVIEWER_H_
#define _VKTRACEVIEWER_QIMAGEVIEWER_H_

#include <cassert>

#include <QFileInfo>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QWheelEvent>

// Pretend an image is a QScrollArea with some some special event handling.
class vktraceviewer_qimageviewer : public QScrollArea
{
    Q_OBJECT
public:
    explicit vktraceviewer_qimageviewer(QWidget* parent = 0)
        : QScrollArea(parent),
          m_pImageLabel(NULL),
          m_pPanStart(0, 0),
          m_pPan(false),
          m_pAutoFit(false)
    {
        // Create a basic image viewer using a QLabel to display the image.
        m_pImageLabel = new QLabel;
        assert(m_pImageLabel != NULL);

        m_pImageLabel->setBackgroundRole(QPalette::Base);
        m_pImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        m_pImageLabel->setScaledContents(true);

        // The QLabel is embedded in a QScrollArea so the image can be panned.
        this->setBackgroundRole(QPalette::Dark);
        this->setWidget(m_pImageLabel);
    }

    virtual ~vktraceviewer_qimageviewer()
    {
        if (m_pImageLabel != NULL)
        {
            delete m_pImageLabel;
            m_pImageLabel = NULL;
        }
    }

    void mouseMoveEvent(QMouseEvent* event)
    {
        if(m_pPan)
        {
            this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() -
                (event->x() - m_pPanStart.x()));
            this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() -
                (event->y() - m_pPanStart.y()));
            m_pPanStart =  event->pos();

            event->accept();
            return;
        }

        event->ignore();
    }

    void mousePressEvent(QMouseEvent* event)
    {
        if(event->button() == Qt::MiddleButton)
        {
            m_pPan = true;
            setCursor(Qt::ClosedHandCursor);
            m_pPanStart = event->pos();

            event->accept();
            return;
        }

        event->ignore();
    }

    void mouseReleaseEvent(QMouseEvent* event)
    {
        if(event->button() == Qt::MiddleButton)
        {
            m_pPan = false;
            setCursor(Qt::ArrowCursor);

            event->accept();
            return;
        }

        event->ignore();
    }

    void resizeEvent(QResizeEvent* event)
    {
        QSize const size = computeMinimumSize();
        m_pImageLabel->setMinimumSize(size);

        if(m_pAutoFit)
        {
            m_pImageLabel->resize(size);
        }

        event->accept();
    }

    void wheelEvent(QWheelEvent* event)
    {
        if(event->orientation() == Qt::Vertical)
        {
            // Stop automatically resizing the image when zoom is requested.
            m_pAutoFit = false;

            // Compute the scaling factor.
            int const numDegrees = event->delta() / 8;
            int const numSteps = numDegrees / 15;
            double const factor = 1.0 + 0.1 * numSteps;

            m_pImageLabel->resize(m_pImageLabel->size() * factor);

            zoomScrollBar(this->horizontalScrollBar(), factor);
            zoomScrollBar(this->verticalScrollBar(), factor);

            event->accept();
            return;
        }

        event->ignore();
    }

    bool loadImage(QString const& fileName)
    {
        QFileInfo fileInfo(fileName);
        if(!fileInfo.exists() || !fileInfo.isFile())
        {
            return false;
        }

        QImage image(fileName);
        if(image.isNull())
        {
            return false;
        }

        m_pImageLabel->setPixmap(QPixmap::fromImage(image));
        m_pImageLabel->adjustSize();
        m_pImageLabel->setMaximumSize(image.size());

        // Resize the image to the scroll area.
        m_pAutoFit = true;

        return true;
    }

private:
    QSize computeMinimumSize() const
    {
        if(m_pImageLabel->pixmap() == NULL)
        {
            return QSize(0, 0);
        }

        double const aspect = m_pImageLabel->pixmap()->width() /
            m_pImageLabel->pixmap()->height();
        if(aspect > 1.0)
        {
            int const minWidth = this->width() - 2 * this->frameWidth();
            int const minHeight = minWidth * 1.0 / aspect;
            return QSize(minWidth, minHeight);
        }
        else
        {
            int const minHeight = this->height() - 2 * this->frameWidth();
            int const minWidth = minHeight * aspect;
            return QSize(minWidth, minHeight);
        }
    }

    void zoomScrollBar(QScrollBar* scrollBar, double const& factor)
    {
        int const value = static_cast<int>(factor * scrollBar->value() +
            ((factor - 1.0) * scrollBar->pageStep() / 2));
        scrollBar->setValue(value);
    }

    QLabel* m_pImageLabel;
    QPoint m_pPanStart;
    bool m_pPan;
    bool m_pAutoFit;
};

#endif //_VKTRACEVIEWER_QIMAGEVIEWER_H_
