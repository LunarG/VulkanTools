#ifndef DLGBETAMESSAGE_H
#define DLGBETAMESSAGE_H

#include <QDialog>

namespace Ui {
class dlgBetaMessage;
}

class dlgBetaMessage : public QDialog
{
    Q_OBJECT

public:
    explicit dlgBetaMessage(QWidget *parent = nullptr);
    ~dlgBetaMessage();

private:
    Ui::dlgBetaMessage *ui;

public Q_SLOTS:
    void on_pushButtonDismiss_clicked();
};

#endif // DLGBETAMESSAGE_H
