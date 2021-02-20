#ifndef CONFIGURATIONS_H
#define CONFIGURATIONS_H

#include <QObject>

class Configurations : public QObject
{
    Q_OBJECT
public:
    explicit Configurations(QObject *parent = 0);
    static Configurations *getInstance();

    void load();
    void save();

    QString getTextColor() const;
    void setTextColor(const QString &value);

    int getTextSize() const;
    void setTextSize(int value);

    QString getProjectDir() const;
    void setProjectDir(const QString &value);

    QString getProjectDirForSongs() const;
    void setProjectDirForSongs(const QString &value);

    QString getProjectDirForImages() const;
    void setProjectDirForImages(const QString &value);

    QString getBackgroundImage() const;
    void setBackgroundImage(const QString &value);

    QString getBackgroundImageFilePath() const;
    void setBackgroundImageFilePath(const QString &value);

    bool getTextAutoFit() const;
    void setTextAutoFit(bool value);

    QString getProjectDirForVideos() const;
    void setProjectDirForVideos(const QString &value);

    QString getBackgroundVideo() const;
    void setBackgroundVideo(const QString &value);

    QString getBackgroundVideoFilePath() const;
    void setBackgroundVideoFilePath(const QString &value);

    QString getTextOutlineColor() const;
    void setTextOutlineColor(const QString &value);

    bool getVideoStretch() const;
    void setVideoStretch(bool value);

    quint64 getRegisterPrivateNumber() const;
    void setRegisterPrivateNumber(const quint64 &value);

    QString getRegisterKeyword() const;
    void setRegisterKeyword(const QString &value);

    bool getNeedRegister() const;
    void setNeedRegister(bool value);

private:
    static Configurations *instance;
    QString textColor;
    QString textOutlineColor;
    int textSize;
    QString projectDir;
    QString projectDirForSongs;
    QString projectDirForImages;
    QString projectDirForVideos;
    QString backgroundImage;
    QString backgroundImageFilePath;
    QString backgroundVideo;
    QString backgroundVideoFilePath;
    bool textAutoFit;
    bool videoStretch;
    quint64 registerPrivateNumber;
    QString registerKeyword;
    bool needRegister;

signals:

public slots:

};

#endif // CONFIGURATIONS_H
