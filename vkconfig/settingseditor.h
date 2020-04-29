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
#include <QLabel>
#include <layerfile.h>

class CSettingsEditor : public QObject
    {
    Q_OBJECT

public:
    CSettingsEditor();

    // Creates controls and sets up any signals
    void CreateGUI(QScrollArea *pDestination, QVector<TLayerSettings *>& layerSettings, bool bSharedOnly);
    void CleanupGUI(void);                  // Clears all the controls and deletes edit area
    bool CollectSettings(void);             // Transfer controls to layerSettings.
                                            // Returns false if no settings were changed

    // Allow disabling and re-enabling all the edit controls
    void SetEnabled(bool bEnable) {
        bEnabled = bEnable;

        // Wait... if there are no settings, just return, otherwise we hide
        // the prompt that tells the user that there are no settings.
        if(inputControls.size() == 0)
            return;

        for(int i = 0; i < inputControls.size(); i++)
            inputControls[i]->setHidden(!bEnabled);

        // No, these may not be the same size as above
        for(int i = 0; i < prompts.size(); i++)
            prompts[i]->setHidden(!bEnabled);

        if(!bEnabled)  {  // Show the prompt
            if(pEnabledPrompt == nullptr)
                pEnabledPrompt = new QLabel(pEditArea);

            pEnabledPrompt->setText(tr("Layer must be enabled (Force On) to allow editing of settings."));
            pEnabledPrompt->setGeometry(6,6, 300, 30);
            pEnabledPrompt->setWordWrap(true);
            pEnabledPrompt->show();
            }
        else { // Remove prompt
            delete pEnabledPrompt;
            pEnabledPrompt = nullptr;
            }


        }

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

    // When disabled, we hide the controls, but display a label
    // that says the controls are hiddent because the layer
    // is not enabled.
    bool        bEnabled;
    QLabel      *pEnabledPrompt;

public Q_SLOTS:
    void browseButtonPressed(void);

    };

#endif // CSETTINGSEDITOR_H
