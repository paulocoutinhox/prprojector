#ifndef DIALOGADDORUPDATESONG_H
#define DIALOGADDORUPDATESONG_H

#include <QDialog>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QTextStream>
#include <QStringList>

#include "Configurations.hpp"

namespace Ui {
class DialogAddOrUpdateSong;
}

class DialogAddOrUpdateSong : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddOrUpdateSong(QWidget *parent = 0);
    ~DialogAddOrUpdateSong();
    void setFilename(QString &filename);
    void setMethod(int method);
    void updateFile();    
    void formatText();

private slots:
    void on_btCancelar_released();
    void on_btConfirmar_released();

    void on_btFormatar_released();

private:
    Ui::DialogAddOrUpdateSong *ui;
    QString filename;
    int method; // 1 = add, 2 = update
    QString title;

};

#endif // DIALOGADDORUPDATESONG_H
