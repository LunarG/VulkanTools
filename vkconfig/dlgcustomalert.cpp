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
 * A custom message box with a "do not show again" feature.
 *
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include "dlgcustomalert.h"
#include "ui_dlgcustomalert.h"

dlgCustomAlert::dlgCustomAlert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgCustomAlert)
    {
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    bDontShowMe = false;
    }

dlgCustomAlert::~dlgCustomAlert()
    {
    delete ui;
    }

void dlgCustomAlert::SetMessage(QString qsMessage)
    {
    ui->label->setText(qsMessage);
    }

void dlgCustomAlert::on_checkBoxDontShow_clicked()
    {
    bDontShowMe = ui->checkBoxDontShow->isChecked();
    }
