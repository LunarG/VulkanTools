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

#include "dlgabout.h"
#include "ui_dlgabout.h"

dlgAbout::dlgAbout(QWidget *parent) : QDialog(parent), ui_(new Ui::dlgAbout) {
    ui_->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui_->pbAboutQt, SIGNAL(clicked()), this, SLOT(aboutQt()));
}

dlgAbout::~dlgAbout() { delete ui_; }

void dlgAbout::aboutQt(void) { qApp->aboutQt(); }
