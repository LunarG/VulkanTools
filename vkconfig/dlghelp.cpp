#include "dlghelp.h"
#include "ui_dlghelp.h"

dlgHelp::dlgHelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dlgHelp)
{
    ui->setupUi(this);
}

dlgHelp::~dlgHelp()
{
    delete ui;
}
