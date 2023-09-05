#ifndef PROJECTORSCREEN_H
#define PROJECTORSCREEN_H

#include <QApplication>
#include <QObject>
#include <QResizeEvent>
#include <QGraphicsDropShadowEffect>
#include <QColor>
#include <QGuiApplication>
#include <QPalette>
#include <QGraphicsPixmapItem>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlContext>
#include <QVideoWidget>

namespace Ui {
class ProjectorScreen;
}

class ProjectorScreen : public QObject
{
    Q_OBJECT

public:
    explicit ProjectorScreen(QObject *parent = 0);
    ~ProjectorScreen();
    static ProjectorScreen *getInstance();
    void renderText(const QString &text);
    void showBlackScreen();
    void showScreen();
    void updateTextConfigurations(const QString &textColor, const QString &outlineColor, int size, bool textAutoFit);
    void setBackgroundImage(const QString &filePath);
    void setBackgroundVideo(const QString &filePath);
    void closeAll();
    void close();
    void updateVideoConfigurations(bool videoStretch);

private:
    static ProjectorScreen *instance;
    void positionObjects();
    bool textAutoFit;
    QQuickView *qmlView;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PROJECTORSCREEN_H
