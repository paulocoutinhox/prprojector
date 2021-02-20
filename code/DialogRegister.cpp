#include "DialogRegister.hpp"
#include "ui_DialogRegister.h"

DialogRegister *DialogRegister::instance = 0;

DialogRegister::DialogRegister(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRegister)
{
    ui->setupUi(this);
    setModal(true);
}

DialogRegister::~DialogRegister()
{
    delete ui;
}

void DialogRegister::setRegisterInformationText(QString &text)
{
    ui->lbRegisterInformation->setText(text);
}

DialogRegister *DialogRegister::getInstance()
{
    if (!instance)
    {
        instance = new DialogRegister();
    }

    return instance;
}
