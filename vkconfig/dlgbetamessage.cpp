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
 * This is a temporary message that we are currently in "Beta" for this tool
 */

#include <QStyle>
#include "dlgbetamessage.h"
#include "ui_dlgbetamessage.h"

dlgBetaMessage::dlgBetaMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgBetaMessage)
    {
    ui->setupUi(this);
    ui->frame->setStyleSheet("background-color: rgb(0, 0, 0);");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    bDontShow = false;

    connect(ui->checkBoxStopIt, SIGNAL(toggled(bool)), this, SLOT(dontShowToggled(bool)));
    }

dlgBetaMessage::~dlgBetaMessage()
    {
    delete ui;
    }


void dlgBetaMessage::on_pushButtonDismiss_clicked()
    {
    close();
    }

void dlgBetaMessage::dontShowToggled(bool bToggled)
    {
    (void)bToggled;
    bDontShow = ui->checkBoxStopIt->isChecked();
    }
