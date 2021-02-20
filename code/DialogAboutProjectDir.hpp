#ifndef DIALOGABOUTPROJECTDIR_H
#define DIALOGABOUTPROJECTDIR_H

#include <QDialog>
#include <QDesktopWidget>

namespace Ui {
class DialogAboutProjectDir;
}

class DialogAboutProjectDir : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAboutProjectDir(QWidget *parent = 0);
    ~DialogAboutProjectDir();

private:
    Ui::DialogAboutProjectDir *ui;
};

#endif // DIALOGABOUTPROJECTDIR_H
