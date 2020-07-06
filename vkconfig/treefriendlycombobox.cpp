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
 * This class creates a combobox that can be used in a tree widget.
 * The new functionality from this derived class is the signal emitted
 * will contain a pointer ot the tree item from which it came.
 * Author: Richard S. Wright Jr. <richard@lunarg.com>
 */

#include "treefriendlycombobox.h"
TreeFriendlyComboBox::TreeFriendlyComboBox(QTreeWidgetItem *pItem) : QComboBox() {
    tree_widget_ = pItem;
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));
}

///////////////////////////////////////////////////////////////////
void TreeFriendlyComboBox::indexChanged(int nIndex) { emit selectionMade(tree_widget_, nIndex); }
