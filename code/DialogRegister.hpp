#ifndef DIALOGREGISTER_H
#define DIALOGREGISTER_H

#include <QDialog>

namespace Ui {
class DialogRegister;
}

class DialogRegister : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegister(QWidget *parent = 0);
    ~DialogRegister();
    void setRegisterInformationText(QString &text);
    static DialogRegister *getInstance();

private:
    Ui::DialogRegister *ui;
    static DialogRegister *instance;    

};

#endif // DIALOGREGISTER_H
