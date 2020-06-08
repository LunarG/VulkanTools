#include "settingstreemanager.h"

CSettingsTreeManager::CSettingsTreeManager()
    {
    pEditorTree = nullptr;
    pProfile = nullptr;

    }

////////////////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::CreateGUI(QTreeWidget *pBuildTree, CProfileDef *pProfileDef)
    {
    CleanupGUI();

    pEditorTree = pBuildTree;
    pProfile = pProfileDef;

    pBuildTree->clear();

    // There will be one top level item for each layer
    for(int i = 0; i < pProfile->layers.size(); i++) {
        QTreeWidgetItem *pItem = new QTreeWidgetItem();
        pItem->setText(0, pProfileDef->layers[i]->name);
        pEditorTree->addTopLevelItem(pItem);
        }



    }

////////////////////////////////////////////////////////////////////////////////////
void CSettingsTreeManager::CleanupGUI(void)
    {
    if(pEditorTree == nullptr)
        return;

    pEditorTree->clear();
    pEditorTree = nullptr;
    pProfile = nullptr;
    }
