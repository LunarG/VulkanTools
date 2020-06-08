#ifndef CSETTINGSTREEMANAGER_H
#define CSETTINGSTREEMANAGER_H

#include <QObject>
#include <QTreeWidget>


#include "profiledef.h"

class CSettingsTreeManager : QObject
{
    Q_OBJECT
public:
    CSettingsTreeManager();

    void CreateGUI(QTreeWidget *pBuildTree, CProfileDef *pProfileDef);
    void CleanupGUI(void);

protected:
    QTreeWidget *pEditorTree;

    CProfileDef *pProfile;

};

#endif // CSETTINGSTREEMANAGER_H
