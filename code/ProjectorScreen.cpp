#include "ProjectorScreen.hpp"
#include "QGuiApplication"
#include "QScreen"
#include "QFile"

ProjectorScreen *ProjectorScreen::instance = nullptr;

ProjectorScreen::ProjectorScreen(QObject *parent) : QObject(parent)
{
    textAutoFit = false;

    qmlView = new QQuickView();
    qmlView->setSource(QUrl("qrc:/resources/qml/ProjectorScreen.qml"));
    qmlView->setFlags(Qt::FramelessWindowHint | Qt::WindowFullscreenButtonHint);
    qmlView->setResizeMode(QQuickView::SizeRootObjectToView);
}

ProjectorScreen::~ProjectorScreen()
{
    delete qmlView;
}

ProjectorScreen *ProjectorScreen::getInstance()
{
    if (!instance)
    {
        instance = new ProjectorScreen();
    }

    return instance;
}

void ProjectorScreen::renderText(const QString &text)
{
    QQuickItem *rootObject = qmlView->rootObject();
    QObject *renderText = rootObject->findChild<QObject *>("renderText");
    renderText->setProperty("text", text);

    QObject *renderTextGlow = rootObject->findChild<QObject *>("renderTextGlow");
    renderTextGlow->setProperty("visible", true);
}

void ProjectorScreen::showBlackScreen()
{
    renderText("");
    setBackgroundVideo("");
    setBackgroundImage("");

    showScreen();
}

void ProjectorScreen::showScreen()
{
    if (!qmlView->isVisible())
    {
        if (QGuiApplication::screens().count() > 1)
        {
            QScreen *screen = QGuiApplication::screens()[1];
            const QRect screenRes = screen->geometry();

            qmlView->setPosition(QPoint(screenRes.x(), screenRes.y()));
            qmlView->resize(screenRes.width(), screenRes.height());
            qmlView->show();
        }
    }
}

void ProjectorScreen::updateTextConfigurations(const QString &textColor, const QString &outlineColor, int size, bool autoFit)
{
    QQuickItem *rootObject = qmlView->rootObject();
    QObject *renderText = rootObject->findChild<QObject *>("renderText");

    textAutoFit = autoFit;
    renderText->setProperty("autoFit", textAutoFit);

    QFont currentFont = renderText->property("font").value<QFont>();
    currentFont.setFamily("Arial Black");

    if (textAutoFit)
    {
        currentFont.setPointSize(1000);
    }
    else
    {
        currentFont.setPointSize(size);
    }

    renderText->setProperty("font", currentFont);
    renderText->setProperty("color", textColor);

    QObject *renderTextGlow = rootObject->findChild<QObject *>("renderTextGlow");
    renderTextGlow->setProperty("color", outlineColor);
}

void ProjectorScreen::setBackgroundImage(const QString &filePath)
{
    QFile file(filePath);

    QQuickItem *rootObject = qmlView->rootObject();
    QVariant returnedValue;

    if (file.exists())
    {
        QObject *backgroundImage = rootObject->findChild<QObject *>("backgroundImage");

        QString fullFilePath = "file:" + filePath;

        qDebug("Nova imagem a ser exibida: " + fullFilePath.toUtf8());

        QMetaObject::invokeMethod(rootObject, "loadBackgroundImage", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, fullFilePath));
        backgroundImage->setProperty("visible", true);
    }
    else
    {
        QObject *backgroundImage = rootObject->findChild<QObject *>("backgroundImage");
        backgroundImage->setProperty("source", "");
        backgroundImage->setProperty("visible", false);
    }
}

void ProjectorScreen::setBackgroundVideo(const QString &filePath)
{
    QFile file(filePath);

    QQuickItem *rootObject = qmlView->rootObject();
    QVariant returnedValue;

    if (file.exists())
    {
        QMetaObject::invokeMethod(rootObject, "stopBackgroundVideo", Q_RETURN_ARG(QVariant, returnedValue));

        QString fullFilePath = "file:" + filePath;

        qDebug("Novo video a ser exibido: " + fullFilePath.toUtf8());

        QMetaObject::invokeMethod(rootObject, "loadBackgroundVideo", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, fullFilePath));
        QMetaObject::invokeMethod(rootObject, "startBackgroundVideo", Q_RETURN_ARG(QVariant, returnedValue));
    }
    else
    {
        QMetaObject::invokeMethod(rootObject, "stopBackgroundVideo", Q_RETURN_ARG(QVariant, returnedValue));
    }
}

void ProjectorScreen::closeAll()
{
    qmlView->close();
}

void ProjectorScreen::close()
{
    closeAll();
}

void ProjectorScreen::updateVideoConfigurations(bool videoStretch)
{
    QQuickItem *rootObject = qmlView->rootObject();
    QObject *backgroundVideoOutput = rootObject->findChild<QObject *>("backgroundVideoOutput");
    backgroundVideoOutput->setProperty("stretch", videoStretch);
}

void ProjectorScreen::closeEvent(QCloseEvent *event)
{
    closeAll();
}
