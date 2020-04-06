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
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QComboBox>
#include <QStandardItemModel>

#include "settingseditor.h"

CSettingsEditor::CSettingsEditor()
    {
    pEditArea = nullptr;
    }


///////////////////////////////////////////////////////////////////////////////////////////////
// Creates controls and sets up any signals
void CSettingsEditor::CreateGUI(QScrollArea *pDestination, QVector<TLayerSettings *>& layerSettings)
    {
    int nRowHeight = 24;
    int nVerticalPad = 4;
    int nCurrRow = 15;
    int nLeftColumn = 26;
    int nSecondColumn = 150;
    int nThirdColumn = 225;
    int nEditFieldWidth = 200;

    pEditArea = new QWidget();
    pEditArea->setMinimumSize(QSize(600, (nRowHeight * (layerSettings.size()+2))));
    pDestination->setWidget(pEditArea);
    pEditArea->show();

//    typedef enum { LAYER_SETTINGS_STRING = 0, LAYER_SETTINGS_FILE, LAYER_SETTINGS_INT, LAYER_SETTINGS_FLOAT, LAYER_SETTINGS_BOOL,
//                    LAYER_SETTINGS_EXCLUSIVE_LIST, LAYER_SETTINGS_INCLUSIVE_LIST, LAYER_SETTINGS_RANGE_INT } TLayerSettingsType;

//    hashTable.insert("string", LAYER_SETTINGS_STRING);
//    hashTable.insert("save_file", LAYER_SETTINGS_FILE);
//    hashTable.insert("bool", LAYER_SETTINGS_BOOL);
//    hashTable.insert("enum", LAYER_SETTINGS_EXCLUSIVE_LIST);
//    hashTable.insert("multi_enum", LAYER_SETTINGS_INCLUSIVE_LIST);

    for(int i = 0; i < layerSettings.size(); i++) {
        // Prompt doesn't matter what the data type is
        QLabel *pPromptLabel = new QLabel(pEditArea);
        pPromptLabel->setText(layerSettings[i]->settingsPrompt);
        pPromptLabel->setToolTip(layerSettings[i]->settingsDesc);
        pPromptLabel->setGeometry(nLeftColumn, nCurrRow, nSecondColumn, nRowHeight);
        pPromptLabel->show();
        prompts.push_back(pPromptLabel);

        switch(layerSettings[i]->settingsType)
            {
            case LAYER_SETTINGS_STRING:{
                QLineEdit *pField = new QLineEdit(pEditArea);
                pField->setText(layerSettings[i]->settingsValue);
                pField->setGeometry(nSecondColumn, nCurrRow, nEditFieldWidth, nRowHeight);
                pField->show();
                inputControls.push_back(pField);
                break;
                }

            case LAYER_SETTINGS_BOOL: {
                QRadioButton *pTrue = new QRadioButton(pEditArea);
                pTrue->setText("True");
                pTrue->setGeometry(nSecondColumn, nCurrRow, 100, nRowHeight);
                pTrue->show();
                inputControls.push_back(pTrue);

                QRadioButton *pFalse = new QRadioButton(pEditArea);
                pFalse->setText("False");
                pFalse->setGeometry(nThirdColumn, nCurrRow, 100, nRowHeight);
                pFalse->show();
                inputControls.push_back(pFalse);
                break;
                }

        case LAYER_SETTINGS_FILE: {
                QLineEdit *pField = new QLineEdit(pEditArea);
                pField->setText(layerSettings[i]->settingsValue);
                pField->setGeometry(nSecondColumn, nCurrRow, nEditFieldWidth, nRowHeight);
                pField->show();
                inputControls.push_back(pField);

                QPushButton *pButton = new QPushButton(pEditArea);
                pButton->setText("Browse...");
                pButton->setGeometry(nSecondColumn + nEditFieldWidth + 16, nCurrRow-2, 100, nRowHeight+1);
                pButton->show();
                inputControls.push_back(pButton);
                break;
                }

           case  LAYER_SETTINGS_EXCLUSIVE_LIST: {
                QComboBox *pComboBox = new QComboBox(pEditArea);
                pComboBox->setGeometry(nSecondColumn-5, nCurrRow, nEditFieldWidth, nRowHeight);

                // Populate with the user readable values. The default needs to be found as well,
                // so search for it while we popluate the control. The default stored is the actual
                // value, not what is displayed to the user.
                int nFoundDefault = -1;
                for(int p = 0; p < layerSettings[i]->settingsListExclusivePrompt.size(); p++) {
                    pComboBox->addItem(layerSettings[i]->settingsListExclusivePrompt[p]);
                    if(layerSettings[i]->settingsValue == layerSettings[i]->settingsListExclusiveValue[p])
                        nFoundDefault = p;
                    }

                pComboBox->setCurrentIndex(nFoundDefault);
                pComboBox->show();
                inputControls.push_back(pComboBox);
                break;
                }

            case LAYER_SETTINGS_INCLUSIVE_LIST: {
                QStandardItemModel *pSim = new QStandardItemModel(layerSettings[i]->settingsListInclusivePrompt.length(), 1, pEditArea);
                for(int p = 0; p < layerSettings[i]->settingsListInclusivePrompt.length(); p++) {

                    QStandardItem* item = new QStandardItem(layerSettings[i]->settingsListInclusivePrompt[p]);
                    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsUserTristate);
                    item->setData(Qt::Unchecked, Qt::CheckStateRole);
                    item->setSelectable(true);

                    pSim->setItem(p, 0, item);
                    }

                QComboBox *pComboBox = new QComboBox(pEditArea);
                pComboBox->setModel(pSim);
                pComboBox->setGeometry(nSecondColumn-5, nCurrRow, nEditFieldWidth, nRowHeight);
                pComboBox->show();
                inputControls.push_back(pComboBox);

          //      pComboBox->addItem("<Nonne>");


                break;
                }


            default: {
                QLabel *pLabel = new QLabel(pEditArea);
                pLabel->setText(QString().sprintf("Unhandled setting type: %d", layerSettings[i]->settingsType));
                pLabel->setGeometry(nSecondColumn, nCurrRow, 200, nRowHeight);
                pLabel->show();
                prompts.push_back(pLabel);
                }
            }

        nCurrRow += nRowHeight;
        nCurrRow += nVerticalPad;
        }


    }

///////////////////////////////////////////////////////////////////////////////////////////////
// Okay, remove the control, disconnect any signals and free the memory up.
void CSettingsEditor::CleanupGUI(void)
    {
    for(int i = 0; i < inputControls.size(); i++)
        delete inputControls[i];

    for(int i = 0; i < prompts.size(); i++)
        delete prompts[i];


    inputControls.clear();
    prompts.clear();
    delete pEditArea;
    pEditArea = nullptr;
    }

