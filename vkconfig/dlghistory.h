#ifndef DLGHISTORY_H
#define DLGHISTORY_H

#include <QDialog>

namespace Ui {
class dlgHistory;
}

class dlgHistory : public QDialog
{
    Q_OBJECT

public:
    explicit dlgHistory(QWidget *parent = nullptr);
    ~dlgHistory();

private:
    Ui::dlgHistory *ui;
};

#endif // DLGHISTORY_H
