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

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}

void Highlighter::setColor(const QColor &color) {
    this->format.setBackground(color);
    this->Update();
}

void Highlighter::setSearch(const QString &text) {
    this->search = text;
    this->Update();
}

void Highlighter::setCase(bool enabled) {
    this->search_case = enabled;
    this->Update();
}

void Highlighter::setWhole(bool enabled) {
    this->search_whole = enabled;
    this->Update();
}

void Highlighter::setRegex(bool enabled) {
    this->search_regex = enabled;
    this->Update();
}

void Highlighter::Update() {
    this->pattern = QRegularExpression(
        this->search, this->search_case ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
    this->rehighlight();
}

void Highlighter::highlightBlock(const QString &text) {
    this->format.setFontWeight(QFont::Normal);

    QRegularExpressionMatchIterator matchIterator = this->pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), this->format);
    }
}
