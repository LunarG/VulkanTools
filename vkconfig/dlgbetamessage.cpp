#include <QStyle>
#include "dlgbetamessage.h"
#include "ui_dlgbetamessage.h"

dlgBetaMessage::dlgBetaMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgBetaMessage)
{
    ui->setupUi(this);
    ui->frame->setStyleSheet("background-color: rgb(0, 0, 0);");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

dlgBetaMessage::~dlgBetaMessage()
{
    delete ui;
}


void dlgBetaMessage::on_pushButtonDismiss_clicked()
    {
    close();
    }
