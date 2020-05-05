#include "khronossettings.h"
#include "ui_khronossettings.h"

KhronosSettings::KhronosSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KhronosSettings)
{
    ui->setupUi(this);
}

KhronosSettings::~KhronosSettings()
{
    delete ui;
}
