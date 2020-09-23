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
 * Authors:
 * - Richard S. Wright Jr. <richard@lunarg.com>
 * - Christophe Riccio <christophe@lunarg.com>
 */

#include "dlgvulkaninstallanalysis.h"

VulkanInstallAnalysisDialog::VulkanInstallAnalysisDialog(QObject *parent) : QAbstractItemModel(parent) {}

QVariant VulkanInstallAnalysisDialog::headerData(int section, Qt::Orientation orientation, int role) const {
    // FIXME: Implement me!
    return QVariant();
}

QModelIndex VulkanInstallAnalysisDialog::index(int row, int column, const QModelIndex &parent) const {
    // FIXME: Implement me!
    return QModelIndex();
}

QModelIndex VulkanInstallAnalysisDialog::parent(const QModelIndex &index) const {
    // FIXME: Implement me!
    return QModelIndex();
}

int VulkanInstallAnalysisDialog::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()) return 0;

    // FIXME: Implement me!
    return 0;
}

int VulkanInstallAnalysisDialog::columnCount(const QModelIndex &parent) const {
    if (!parent.isValid()) return 0;

    // FIXME: Implement me!
    return 0;
}

QVariant VulkanInstallAnalysisDialog::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
