#ifndef DLGCUSTOMPATHS_H
#define DLGCUSTOMPATHS_H

#include <QDialog>

#include <vulkanconfiguration.h>

namespace Ui {
class dlgCustomPaths;
}

class dlgCustomPaths : public QDialog
{
    Q_OBJECT

public:
    explicit dlgCustomPaths(QWidget *parent = nullptr);
    ~dlgCustomPaths();

private:
    Ui::dlgCustomPaths *ui;

    CVulkanConfiguration *pVulkanConfig;

public Q_SLOTS:
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();
    void on_treeWidget_itemSelectionChanged();
};

#endif // DLGCUSTOMPATHS_H
