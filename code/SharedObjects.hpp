#ifndef SHAREDOBJECTS_H
#define SHAREDOBJECTS_H

#include <QObject>
#include <QDate>

class SharedObjects : public QObject
{
    Q_OBJECT

public:
    explicit SharedObjects(QObject *parent = 0);
    static SharedObjects *getInstance();
    void setDefaultData();

    bool getIsRegistered() const;
    void setIsRegistered(bool value);

    QString getRegisterEmail() const;
    void setRegisterEmail(const QString &value);

    QDate getRegisterValidate() const;
    void setRegisterValidate(const QDate &value);

    QString getRegisterKeyword() const;
    void setRegisterKeyword(const QString &value);

    bool getIsRegisterExpired() const;
    void setIsRegisterExpired(bool value);

signals:

public slots:

private:
    static SharedObjects *instance;
    bool isRegistered;
    QString registerEmail;
    QDate registerValidate;
    QString registerKeyword;
    bool isRegisterExpired;

};

#endif // SHAREDOBJECTS_H
