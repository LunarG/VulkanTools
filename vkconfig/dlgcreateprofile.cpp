

#include <QFileDialog>

#include "dlgcreateprofile.h"
#include "ui_dlgcreateprofile.h"


dlgcreateprofile::dlgcreateprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgcreateprofile)
{
    ui->setupUi(this);

    pVulkanConfig = CVulkanConfiguration::getVulkanConfig();

    for(int i = 1; i < pVulkanConfig->profileList.size(); i++)
        ui->comboBoxProfiles->addItem(pVulkanConfig->profileList[i]->profileName);

}

dlgcreateprofile::~dlgcreateprofile()
{
    delete ui;
}


void dlgcreateprofile::on_pushButtonBrowse_clicked()         // Pick the test application
    {
    QString fileWildcard = ("Applications (*)");    // Linux default

#ifdef TARGET_OS_OSX
    fileWildcard = QString("Applications (*.app, *");
#endif

#ifdef TARGET_OS_WIN32
    fileWildcard = QString("Applications (*.exe)");
#endif

    QString appWithPath =  QFileDialog::getOpenFileName(this,
        tr("Select executable to pair with "), "/", fileWildcard);



    }
