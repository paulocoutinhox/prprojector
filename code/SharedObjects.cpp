#include "SharedObjects.hpp"

SharedObjects *SharedObjects::instance = 0;

SharedObjects::SharedObjects(QObject *parent) : QObject(parent)
{

}

SharedObjects *SharedObjects::getInstance()
{
    if (!instance)
    {
        instance = new SharedObjects();
        instance->setDefaultData();
    }

    return instance;
}

void SharedObjects::setDefaultData()
{
    isRegistered      = false;
    registerEmail     = QString("");
    registerValidate  = QDate();
    isRegisterExpired = false;
}
bool SharedObjects::getIsRegistered() const
{
    return isRegistered;
}

void SharedObjects::setIsRegistered(bool value)
{
    isRegistered = value;
}
QString SharedObjects::getRegisterEmail() const
{
    return registerEmail;
}

void SharedObjects::setRegisterEmail(const QString &value)
{
    registerEmail = value;
}
QDate SharedObjects::getRegisterValidate() const
{
    return registerValidate;
}

void SharedObjects::setRegisterValidate(const QDate &value)
{
    registerValidate = value;
}
QString SharedObjects::getRegisterKeyword() const
{
    return registerKeyword;
}

void SharedObjects::setRegisterKeyword(const QString &value)
{
    registerKeyword = value;
}
bool SharedObjects::getIsRegisterExpired() const
{
    return isRegisterExpired;
}

void SharedObjects::setIsRegisterExpired(bool value)
{
    isRegisterExpired = value;
}







