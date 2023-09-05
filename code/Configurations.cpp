#include "Configurations.hpp"
#include "QStandardPaths"
#include "QSettings"
#include "QDebug"

Configurations *Configurations::instance = 0;

Configurations::Configurations(QObject *parent) : QObject(parent)
{
    registerPrivateNumber = 3291652751;
    registerKeyword       = "prprojector";
}

Configurations *Configurations::getInstance()
{
    if (!instance)
    {
        instance = new Configurations();
        instance->setNeedRegister(false);
    }

    return instance;
}

void Configurations::load()
{
    const QString dataFolder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString settingsFile = dataFolder + "/settings.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);

    qDebug("Arquivo de configuração para carregar: " + settingsFile.toUtf8());

    setTextColor("#FFFFFF");
    setTextOutlineColor("#000000");
    setTextSize(40);
    setProjectDir(settings.value("Project/path").toString());
    setTextAutoFit(true);
    setVideoStretch(true);
}

void Configurations::save()
{
    const QString dataFolder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString settingsFile = dataFolder + "/settings.ini";
    QSettings settings(settingsFile, QSettings::IniFormat);

    qDebug("Arquivo de configuração para salvar: " + settingsFile.toUtf8());

    settings.setValue("Project/path", projectDir);
}

QString Configurations::getTextColor() const
{
    return textColor;
}

void Configurations::setTextColor(const QString &value)
{
    textColor = value;
}
int Configurations::getTextSize() const
{
    return textSize;
}

void Configurations::setTextSize(int value)
{
    textSize = value;
}
QString Configurations::getProjectDir() const
{
    return projectDir;
}

void Configurations::setProjectDir(const QString &value)
{
    projectDir = value;
}
QString Configurations::getProjectDirForSongs() const
{
    return projectDirForSongs;
}

void Configurations::setProjectDirForSongs(const QString &value)
{
    projectDirForSongs = value;
}
QString Configurations::getProjectDirForImages() const
{
    return projectDirForImages;
}

void Configurations::setProjectDirForImages(const QString &value)
{
    projectDirForImages = value;
}
QString Configurations::getBackgroundImage() const
{
    return backgroundImage;
}

void Configurations::setBackgroundImage(const QString &value)
{
    backgroundImage = value;
}
QString Configurations::getBackgroundImageFilePath() const
{
    return backgroundImageFilePath;
}

void Configurations::setBackgroundImageFilePath(const QString &value)
{
    backgroundImageFilePath = value;
}
bool Configurations::getTextAutoFit() const
{
    return textAutoFit;
}

void Configurations::setTextAutoFit(bool value)
{
    textAutoFit = value;
}
QString Configurations::getProjectDirForVideos() const
{
    return projectDirForVideos;
}

void Configurations::setProjectDirForVideos(const QString &value)
{
    projectDirForVideos = value;
}
QString Configurations::getBackgroundVideo() const
{
    return backgroundVideo;
}

void Configurations::setBackgroundVideo(const QString &value)
{
    backgroundVideo = value;
}
QString Configurations::getBackgroundVideoFilePath() const
{
    return backgroundVideoFilePath;
}

void Configurations::setBackgroundVideoFilePath(const QString &value)
{
    backgroundVideoFilePath = value;
}
QString Configurations::getTextOutlineColor() const
{
    return textOutlineColor;
}

void Configurations::setTextOutlineColor(const QString &value)
{
    textOutlineColor = value;
}
bool Configurations::getVideoStretch() const
{
    return videoStretch;
}

void Configurations::setVideoStretch(bool value)
{
    videoStretch = value;
}
quint64 Configurations::getRegisterPrivateNumber() const
{
    return registerPrivateNumber;
}

void Configurations::setRegisterPrivateNumber(const quint64 &value)
{
    registerPrivateNumber = value;
}
QString Configurations::getRegisterKeyword() const
{
    return registerKeyword;
}

void Configurations::setRegisterKeyword(const QString &value)
{
    registerKeyword = value;
}
bool Configurations::getNeedRegister() const
{
    return needRegister;
}

void Configurations::setNeedRegister(bool value)
{
    needRegister = value;
}







