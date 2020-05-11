#ifndef DLGHELP_H
#define DLGHELP_H

#include <QWidget>

namespace Ui {
class dlgHelp;
}

class dlgHelp : public QWidget
{
    Q_OBJECT

public:
    explicit dlgHelp(QWidget *parent = nullptr);
    ~dlgHelp();

private:
    Ui::dlgHelp *ui;
};

#endif // DLGHELP_H
