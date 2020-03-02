#ifndef DLGCUSTOMPATHS_H
#define DLGCUSTOMPATHS_H

#include <QDialog>

namespace Ui {
class dlgCustomPaths;
}

class dlgCustomPaths : public QDialog
{
    Q_OBJECT

public:
    explicit dlgCustomPaths(QWidget *parent = nullptr);
    ~dlgCustomPaths();

private:
    Ui::dlgCustomPaths *ui;
};

#endif // DLGCUSTOMPATHS_H
