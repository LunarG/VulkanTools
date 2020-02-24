#ifndef DLGCREATEPROFILE_H
#define DLGCREATEPROFILE_H

#include <QDialog>

#include <vulkanconfiguration.h>

namespace Ui {
class dlgcreateprofile;
}

class dlgcreateprofile : public QDialog
{
    Q_OBJECT

public:
    explicit dlgcreateprofile(QWidget *parent = nullptr);
    ~dlgcreateprofile();

private:
    Ui::dlgcreateprofile *ui;

    CVulkanConfiguration    *pVulkanConfig;

public	Q_SLOTS:
    void on_pushButtonBrowse_clicked();         // Pick the test application

};

#endif // DLGCREATEPROFILE_H
