#include "dlgcustompaths.h"
#include "ui_dlgcustompaths.h"

dlgCustomPaths::dlgCustomPaths(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgCustomPaths)
{
    ui->setupUi(this);
}

dlgCustomPaths::~dlgCustomPaths()
{
    delete ui;
}
