#ifndef DLGCUSTOMALERT_H
#define DLGCUSTOMALERT_H
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

#include <QDialog>

namespace Ui {
class dlgCustomAlert;
}

class dlgCustomAlert : public QDialog
{
    Q_OBJECT

public:
    explicit dlgCustomAlert(QWidget *parent = nullptr);
    ~dlgCustomAlert();

    void SetMessage(QString qsMessage);
    bool DontShowMeAgain(void) { return bDontShowMe; }

protected:
    bool bDontShowMe;

private:
    Ui::dlgCustomAlert *ui;

public Q_SLOTS:
    void on_checkBoxDontShow_clicked();
};

#endif // DLGCUSTOMALERT_H
