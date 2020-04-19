#ifndef CSETTINGSEDITOR_H
#define CSETTINGSEDITOR_H
/*
 * Copyright (c) 2020 Valve Corporation
 * Copyright (c) 2020 LunarG, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This class takes a pointer to a widget and a list of layer settings
 * and creates a gui for displaying and editing those settings.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */


#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <layerfile.h>

class CSettingsEditor : public QObject
    {
    Q_OBJECT

public:
    CSettingsEditor();

    // Creates controls and sets up any signals
    void CreateGUI(QScrollArea *pDestination, QVector<TLayerSettings *>& layerSettings, bool bApplyButton = false);
    void CleanupGUI(void);                  // Clears all the controls and deletes edit area
    bool CollectSettings(void);             // Transfer controls to layerSettings.
                                            // Returns false if no settings were changed
    QPushButton   *pApplyButton;            // Button pressed to apply settings now

protected:
    // Every edit control has one of these
    QVector<QWidget *> prompts;             // Just QLabels...

    // Actual edit controls, there may be more than one control per settings
    // entry, and so settings fields may have duplicates as these two mirror
    // each other exactly.
    QVector<QWidget *> inputControls;
    QVector<TLayerSettings *> linkedSetting;

    QWidget *pEditArea;

    ///////////////////////////////////////////////////////////////////
    // This might not be a good idea... but I think it's okay. There
    // is only ever one button and one edit field that goes with it.
    // This could be more general and I have an idea how to make it
    // work, but for now this is cheap and cheerful and will work.
    QLineEdit     *pButtonBuddy;
    QPushButton   *pBrowseButton;

public Q_SLOTS:
    void browseButtonPressed(void);

    };

#endif // CSETTINGSEDITOR_H