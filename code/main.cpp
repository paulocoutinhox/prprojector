#include "MainWindow.hpp"
#include "QApplication"
#include "QGuiApplication"
#include "QScreen"

int main(int argc, char *argv[])
{
    // cria a aplicação com a janela principal
    QApplication a(argc, argv);

    // define estilo global
    QString globalStyle;
    globalStyle.append("QPushButton { max-height: 32px }");
    a.setStyleSheet(globalStyle);

    // posiciona a janela no centro do desktop
    MainWindow w;

    if (QGuiApplication::screens().count() > 0)
    {
        QScreen *screen = QGuiApplication::screens()[0];
        const QRect screenRes = screen->geometry();

        QSize size = w.size();
        int width  = screenRes.width();
        int height = screenRes.height();
        int mw = size.width();
        int mh = size.height();
        int centerW = (width/2) - (mw/2);
        int centerH = (height/2) - (mh/2);
        w.move(centerW, centerH);
    }

    // exibe a janela principal
    w.showMaximized();

    return a.exec();
}
