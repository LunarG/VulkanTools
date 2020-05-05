#ifndef KHRONOSSETTINGS_H
#define KHRONOSSETTINGS_H

#include <QWidget>

namespace Ui {
class KhronosSettings;
}

class KhronosSettings : public QWidget
{
    Q_OBJECT

public:
    explicit KhronosSettings(QWidget *parent = nullptr);
    ~KhronosSettings();

private:
    Ui::KhronosSettings *ui;
};

#endif // KHRONOSSETTINGS_H
