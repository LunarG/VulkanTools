#include "dlghistory.h"
#include "ui_dlghistory.h"

dlgHistory::dlgHistory(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgHistory)
{
    ui->setupUi(this);
}

dlgHistory::~dlgHistory()
{
    delete ui;
}
