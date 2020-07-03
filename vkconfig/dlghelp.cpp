#include "dlghelp.h"
#include "ui_dlghelp.h"

dlgHelp::dlgHelp(QWidget *parent) : QWidget(parent), ui_(new Ui::dlgHelp) {
    ui_->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
}

dlgHelp::~dlgHelp() { delete ui_; }
