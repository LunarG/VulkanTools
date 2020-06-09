#ifndef CENUMSETTINGWIDGET_H
#define CENUMSETTINGWIDGET_H

#include <QObject>
#include <QWidget>
#include <QComboBox>
#include <QTreeWidgetItem>
#include "layerfile.h"

class CEnumSettingWidget : public QComboBox
{
    Q_OBJECT
public:
    CEnumSettingWidget(QTreeWidgetItem* pTreeItem, TLayerSettings *pLayerSetting);

private:
    TLayerSettings *pSetting;

public Q_SLOTS:
    void indexChanged(int nIndex);

};

#endif // CENUMSETTINGWIDGET_H
