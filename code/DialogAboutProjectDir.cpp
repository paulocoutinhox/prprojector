#include "DialogAboutProjectDir.hpp"
#include "ui_DialogAboutProjectDir.h"
#include "QGuiApplication"
#include "QScreen"

DialogAboutProjectDir::DialogAboutProjectDir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutProjectDir)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    setFixedSize(width(), height());

    if (QGuiApplication::screens().count() > 0)
    {
        QScreen *screen = QGuiApplication::screens()[0];
        const QRect screenRes = screen->geometry();

        QSize size = this->size();
        int width  = screenRes.width();
        int height = screenRes.height();
        int mw = size.width();
        int mh = size.height();
        int centerW = (width/2) - (mw/2);
        int centerH = (height/2) - (mh/2);
        move(centerW, centerH);
    }
}

DialogAboutProjectDir::~DialogAboutProjectDir()
{
    delete ui;
}
