#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QGuiApplication>
#include <QListWidgetItem>
#include <QColorDialog>
#include <QFileDialog>
#include <QTimer>
#include <QDate>
#include <QMediaPlayer>

#include "ProjectorScreen.hpp"
#include "Configurations.hpp"
#include "DialogRegister.hpp"
#include "SharedObjects.hpp"
#include "SimpleCrypt.hpp"
#include "DialogAddOrUpdateSong.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void hideBtRegisterDialog();
private slots:

    void timeoutToTimerToChangeLiveContent();
    void timeoutToTimerToRegister();
    void onAboutToQuit();

    void on_edSongName_textEdited(const QString &arg1);
    void on_lvSongList_doubleClicked(const QModelIndex &index);
    void on_lvPreview_doubleClicked(const QModelIndex &index);
    void on_btCloseProjector_released();
    void on_lvLive_clicked(const QModelIndex &index);
    void on_lvLive_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_btBlackScreen_released();
    void on_btDefaultScreenContent_released();
    void on_btApplyTextConfigurations_released();
    void on_btSelectTextColor_released();
    void on_sbTextSize_valueChanged(const QString &arg1);
    void on_btSelectProjectDir_released();
    void on_btRefreshImageList_released();
    void on_ltImages_doubleClicked(const QModelIndex &index);
    void on_btNoBackgroundImage_released();
    void on_ckTextAutoFit_toggled(bool checked);
    void on_btNoBackgroundVideo_released();
    void on_btRefreshVideoList_released();
    void on_ltVideos_doubleClicked(const QModelIndex &index);
    void on_lvSelectedSongs_doubleClicked(const QModelIndex &index);
    void on_btRemoveSelectedSong_released();
    void on_btAddSongToSelectedList_released();
    void on_btAddSongToSelectedListAndShow_released();
    void on_btClearPreviewList_released();
    void on_btClearLiveList_released();
    void on_btAddSelectedSongtoPreview_released();
    void on_btAddPreviewSongToLiveAndShow_released();
    void on_btShowEmptyText_released();
    void on_btStartTimerToChangeLiveContent_released();
    void on_btStopTimerToChangeLiveContent_released();
    void on_btSelectTextOutlineColor_released();
    void on_ltImages_clicked(const QModelIndex &index);
    void on_ltImages_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_ckVideoStretch_toggled(bool checked);
    void on_btApplyVideoConfigurations_released();
    void on_btRegisterDialog_released();
    void on_btAddSong_released();
    void on_btUpdateSong_released();
    void on_btDeleteSong_released();
    void on_btRefreshSongList_released();
    void on_ltVideos_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_ltVideos_itemClicked(QListWidgetItem *item);

    void on_btStopPreviewVideo_released();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::MainWindow *ui;
    QTimer *timerToChangeLiveContent;
    QTimer *timerToRegister;
    QMediaPlayer *videoPlayer;

    void sendSongTextToProjector(bool showMessageOnError = true);
    bool canShowProjector(bool showMessageOnError = true);
    void showSelectedTextColor();
    void showSelectedTextOutlineColor();
    void updateTextConfigurations();
    void showSelectedProjectDir();
    void updateImageList();
    void updateVideoList();
    void showSelectedAutoFitOption();
    void addSongToSelectedList();
    void addSelectedSongToPreview();
    void selectLastSongOnSelectedList();
    void addPreviewSongPartsToLiveAndShow(int selectedRow);
    void startTimerToChangeLiveContent();
    void stopTimerToChangeLiveContent();
    void selectNextSongPartOnLiveAndShow();
    void showSelectedBackgroundImageOnPreview();
    void showSelectedVideoStretchOption();
    void updateVideoConfigurations();
    void getRegisterData();
    void setRegisterDataOnDialogRegister();
    void refreshSongListBySongName(QString songName);
    void showSelectedBackgroundVideoOnPreview();
    void beforeQuit();

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
