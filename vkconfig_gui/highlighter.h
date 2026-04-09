/*
 * Copyright (c) 2020-2026 Valve Corporation
 * Copyright (c) 2020-2026 LunarG, Inc.
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
 * Authors:
 * - Christophe Riccio <christophe@lunarg.com>
 */

#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter {
    Q_OBJECT

   public:
    Highlighter(QTextDocument *parent = nullptr);

    void setColor(const QColor &color);
    void setSearch(const QString &text);
    void setCase(bool enabled);
    void setWhole(bool enabled);
    void setRegex(bool enabled);

   protected:
    void highlightBlock(const QString &text) override;

   private:
    void Update();

    QString search;
    bool search_case = false;
    bool search_whole = false;
    bool search_regex = false;

    QRegularExpression pattern;
    QTextCharFormat format;
};
