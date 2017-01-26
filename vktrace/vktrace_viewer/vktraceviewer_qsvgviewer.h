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

#ifndef _VKTRACEVIEWER_QSVGVIEWER_H_
#define _VKTRACEVIEWER_QSVGVIEWER_H_

#include <QFileInfo>
#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QWheelEvent>

class vktraceviewer_qsvgviewer : public QGraphicsView
{
    Q_OBJECT
public:
    vktraceviewer_qsvgviewer(QWidget* parent = 0) :
        QGraphicsView(parent),
        disabledScene(NULL),
        enabledScene(NULL),
        autoFit(false)
    {
        // The destructor for QGraphicsScene will be called when this QGraphicsView is
        // destroyed.
        enabledScene = new QGraphicsScene(this);
        disabledScene = new QGraphicsScene(this);
        this->setScene(disabledScene);

        // Anchor the point under the mouse during view transformations.
        this->setTransformationAnchor(AnchorUnderMouse);

        // Enable drag scrolling with the left mouse button.
        this->setDragMode(ScrollHandDrag);

        // Always update the entire viewport. Don't waste time trying to figure out
        // which items need to be updated since there is only one.
        this->setViewportUpdateMode(FullViewportUpdate);
    }

    void changeEvent(QEvent* event)
    {
        switch(event->type())
        {
            case QEvent::EnabledChange:
                if(this->isEnabled())
                {
                    this->setScene(enabledScene);
                }
                else
                {
                    this->setScene(disabledScene);
                }
                break;
            default:
                break;
        }
    }

    void paintEvent(QPaintEvent* event)
    {
        // Resize the scene to fit the widget. This is deferred until the first paint
        // event (when the widget size is known).
        if(autoFit)
        {
            this->fitInView(enabledScene->itemsBoundingRect(), Qt::KeepAspectRatio);
            autoFit = false;
        }

        QGraphicsView::paintEvent(event);
    }

    void wheelEvent(QWheelEvent* event)
    {
        if(event->orientation() == Qt::Vertical)
        {
            // The delta value is in units of eighths of a degree.
            qreal const degrees = event->delta() / 8.0;

            // According to Qt documentation, mice have steps of 15-degrees.
            qreal const steps = degrees / 15.0;

            qreal factor = 1.0 + 0.1 * steps;

            this->scale(factor, factor);

            event->accept();
        }
    }

    bool load(QString const& fileName)
    {
        QFileInfo fileInfo(fileName);
        if(!fileInfo.exists() || !fileInfo.isFile())
        {
            return false;
        }

        this->resetTransform();

        enabledScene->clear();

        // The destructor for QGraphicsSvgItem will be called when the scene is cleared.
        // This occurs when a SVG is loaded or when the QGraphicsScene is destroyed.
        enabledScene->addItem(new QGraphicsSvgItem(fileName));

        autoFit = true;

        return true;
    }

private:
    QGraphicsScene* disabledScene;
    QGraphicsScene* enabledScene;

    bool autoFit;
};

#endif // _VKTRACEVIEWER_QSVGVIEWER_H_
