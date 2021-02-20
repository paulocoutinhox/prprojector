#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "QGuiApplication"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    timerToChangeLiveContent = nullptr;

    ui->setupUi(this);    

    videoPlayer = new QMediaPlayer;
    videoPlayer->setVideoOutput(ui->wiVideoPreview);
    videoPlayer->setVolume(0);

    playlist = new QMediaPlaylist();

    ui->tabWidget->setCurrentIndex(0);

    Configurations *config = Configurations::getInstance();
    config->load();

    if (!config->getProjectDir().isEmpty())
    {
        if (QDir(config->getProjectDir()).exists())
        {
            showSelectedProjectDir();
        }
    }

    updateTextConfigurations();
    getRegisterData();

    if (Configurations::getInstance()->getNeedRegister())
    {
        timerToRegister = new QTimer(this);
        timerToRegister->setInterval(1 * 60 * 1000);
        timerToRegister->setInterval(10 * 1000);
        QObject::connect(timerToRegister, SIGNAL(timeout()), this, SLOT(timeoutToTimerToRegister()));
        timerToRegister->start();
    }
    else
    {
        hideBtRegisterDialog();
    }

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if (timerToChangeLiveContent)
    {
        delete timerToChangeLiveContent;
    }
}

void MainWindow::on_edSongName_textEdited(const QString &arg1)
{
    refreshSongListBySongName(arg1);
}

void MainWindow::addSongToSelectedList()
{
    if (ui->lvSongList->count() > 0)
    {
        if (ui->lvSongList->selectedItems().count() > 0)
        {
            ui->lvSelectedSongs->setUpdatesEnabled(false);

            QModelIndexList indexes = ui->lvSongList->selectionModel()->selectedIndexes();

            for (int i = indexes.count() - 1; i > -1; --i)
            {
                QVariant selectedSongVariant;
                QString selectedSong;

                selectedSongVariant = ui->lvSongList->model()->data(indexes.at(i));
                selectedSong        = selectedSongVariant.toString();

                if (!selectedSong.isEmpty())
                {
                    Configurations *config = Configurations::getInstance();
                    QFile file(config->getProjectDirForSongs() + "/" + selectedSong);

                    if(!file.exists())
                    {
                        QMessageBox::information(0, "Mensagem", "O arquivo selecionado não existe.");
                    }
                    else
                    {
                        ui->lvSelectedSongs->addItem(selectedSong);
                    }
                }
            }

            ui->lvSelectedSongs->setUpdatesEnabled(true);
        }
    }
}

void MainWindow::on_lvSongList_doubleClicked(const QModelIndex &index)
{
    addSongToSelectedList();
}

void MainWindow::addPreviewSongPartsToLiveAndShow(int selectedRow)
{
    ui->lvLive->setUpdatesEnabled(false);

    ui->lvLive->clear();

    for (int x = 0; x < ui->lvPreview->count(); x++)
    {
        ui->lvLive->addItem(ui->lvPreview->item(x)->text());
    }

    if (ui->lvLive->count() > 0)
    {
        if (ui->lvLive->count() >= selectedRow)
        {
            ui->lvLive->setCurrentRow(selectedRow);
            sendSongTextToProjector(false);
        }

        activateWindow();
        raise();
        setFocus();
        ui->lvLive->setFocus();
    }

    ui->lvLive->setUpdatesEnabled(true);
}

void MainWindow::startTimerToChangeLiveContent()
{
    if (timerToChangeLiveContent)
    {
        stopTimerToChangeLiveContent();
    }

    timerToChangeLiveContent = new QTimer(this);
    timerToChangeLiveContent->setInterval((ui->sbIntervalToTimerToChangeLiveContent->value() * 1000));
    QObject::connect(timerToChangeLiveContent, SIGNAL(timeout()), this, SLOT(timeoutToTimerToChangeLiveContent()));
    timerToChangeLiveContent->start();

    ui->lbTimerToChangeLiveContentStatus->setText("Status: ativado");
}

void MainWindow::stopTimerToChangeLiveContent()
{
    if (timerToChangeLiveContent)
    {
        timerToChangeLiveContent->stop();
    }

    ui->lbTimerToChangeLiveContentStatus->setText("Status: desativado");
}

void MainWindow::selectNextSongPartOnLiveAndShow()
{
    if (canShowProjector(false))
    {
        if (ui->lvLive->count() > 0)
        {
            int selectedRow = ui->lvLive->currentRow();

            if (selectedRow < (ui->lvLive->count() - 1))
            {
                selectedRow++;
            }
            else
            {
                selectedRow = 0;
            }

            ui->lvLive->setCurrentRow(selectedRow);

            sendSongTextToProjector(false);
        }
    }
}

void MainWindow::on_lvPreview_doubleClicked(const QModelIndex &index)
{
    addPreviewSongPartsToLiveAndShow(index.row());
}

void MainWindow::on_btCloseProjector_released()
{
    stopTimerToChangeLiveContent();
    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
    projectorScreen->close();
}

void MainWindow::sendSongTextToProjector(bool showMessageOnError)
{
    if (canShowProjector(showMessageOnError))
    {
        ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
        projectorScreen->showScreen();

        if (ui->lvLive->count() > 0)
        {
            if (ui->lvLive->selectedItems().count() > 0)
            {
                QListWidgetItem *selectedItem = ui->lvLive->selectedItems().at(0);
                projectorScreen->renderText(selectedItem->text());
            }
        }
    }
}

bool MainWindow::canShowProjector(bool showMessageOnError)
{
    if (QGuiApplication::screens().count() > 1)
    {
        return true;
    }
    else
    {
        if (showMessageOnError)
        {
            QMessageBox::information(this, "Mensagem", "Você possui somente um monitor. Conecte o seu computador a mais um outro monitor e tente novamente.");
        }

        return false;
    }
}

void MainWindow::showSelectedTextColor()
{
    Configurations *config = Configurations::getInstance();
    ui->lbTextColor->setStyleSheet("background-color: " + config->getTextColor() + "; border: 1px solid #000;");
}

void MainWindow::showSelectedTextOutlineColor()
{
    Configurations *config = Configurations::getInstance();
    ui->lbTextOutlineColor->setStyleSheet("background-color: " + config->getTextOutlineColor() + "; border: 1px solid #000;");
}

void MainWindow::updateTextConfigurations()
{
    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
    Configurations *config           = Configurations::getInstance();

    // limpa tudo
    ui->edSongName->setText("");
    ui->lvSongList->clear();
    ui->lvPreview->clear();
    ui->lvSelectedSongs->clear();

    // cria pastas necessárias para a aplicação funcionar
    QString projectDir = config->getProjectDir();

    if (!projectDir.isEmpty() && QDir(projectDir).exists())
    {
        QDir dir(projectDir);
        dir.setPath(projectDir);

        if (dir.exists())
        {
            dir.mkpath("data/songs");
        }

        dir.setPath(projectDir);

        if (dir.exists())
        {
            dir.mkpath("data/images");
        }

        dir.setPath(projectDir);

        if (dir.exists())
        {
            dir.mkpath("data/videos");
        }

        config->setProjectDirForImages(projectDir + "/data/images");
        config->setProjectDirForSongs(projectDir + "/data/songs");
        config->setProjectDirForVideos(projectDir + "/data/videos");
    }

    // atualiza a lista de imagens
    updateImageList();

    // atualiza a lista de vídeos
    updateVideoList();

    // define atributos do texto
    projectorScreen->updateTextConfigurations(config->getTextColor(), config->getTextOutlineColor(), config->getTextSize(), config->getTextAutoFit());
    showSelectedTextColor();
    showSelectedTextOutlineColor();
    showSelectedAutoFitOption();
}

void MainWindow::showSelectedProjectDir()
{
    Configurations *config = Configurations::getInstance();
    ui->lbProjectDir->setText(config->getProjectDir());
}

void MainWindow::updateImageList()
{
    ui->ltImages->clear();

    QStringList filters;
    filters << "*.png";
    filters << "*.jpg";
    filters << "*.jpeg";

    Configurations *config = Configurations::getInstance();

    QDir imagesDir(config->getProjectDirForImages());
    QStringList allFiles = imagesDir.entryList(filters, QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::DirsFirst);

    ui->ltImages->addItems(allFiles);
    ui->lbPreviewBackgroundImage->clear();
}

void MainWindow::updateVideoList()
{
    ui->ltVideos->clear();

    QStringList filters;
    filters << "*.avi";
    filters << "*.mpg";
    filters << "*.mpeg";
    filters << "*.mpe";
    filters << "*.mpeg-1";
    filters << "*.mpeg-2";
    filters << "*.divx";
    filters << "*.mov";
    filters << "*.rmvb";
    filters << "*.qt";
    filters << "*.asx";
    filters << "*.asf";
    filters << "*.wmv";
    filters << "*.rm";
    filters << "*.3gp";
    filters << "*.mp4";
    filters << "*.ogm";
    filters << "*.mkv";

    Configurations *config = Configurations::getInstance();

    QDir videosDir(config->getProjectDirForVideos());
    QStringList allFiles = videosDir.entryList(filters, QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::DirsFirst);

    ui->ltVideos->addItems(allFiles);
    videoPlayer->stop();
}

void MainWindow::showSelectedAutoFitOption()
{
    Configurations *config = Configurations::getInstance();
    ui->ckTextAutoFit->setChecked(config->getTextAutoFit());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    beforeQuit();
}

void MainWindow::on_lvLive_clicked(const QModelIndex &index)
{
    sendSongTextToProjector();
}

void MainWindow::on_lvLive_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    sendSongTextToProjector();
}

void MainWindow::on_btBlackScreen_released()
{
    stopTimerToChangeLiveContent();

    if (canShowProjector())
    {
        ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
        projectorScreen->showBlackScreen();
    }
}

void MainWindow::on_btDefaultScreenContent_released()
{
    stopTimerToChangeLiveContent();

    if (canShowProjector())
    {
        ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
        projectorScreen->showBlackScreen();
    }
}

void MainWindow::on_btApplyTextConfigurations_released()
{
    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
    Configurations *config           = Configurations::getInstance();

    projectorScreen->updateTextConfigurations(config->getTextColor(), config->getTextOutlineColor(), config->getTextSize(), config->getTextAutoFit());
}

void MainWindow::on_btSelectTextColor_released()
{
    Configurations *config = Configurations::getInstance();

    QColor color = QColorDialog::getColor(config->getTextColor());

    if (color.isValid())
    {
        config->setTextColor(color.name().toUtf8());
        showSelectedTextColor();
    }
}

void MainWindow::on_sbTextSize_valueChanged(const QString &arg1)
{
    Configurations *config = Configurations::getInstance();
    config->setTextSize(ui->sbTextSize->value());
}

void MainWindow::on_btSelectProjectDir_released()
{
    Configurations *config = Configurations::getInstance();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Selecionar pasta"),
                                                 config->getProjectDir(),
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty())
    {
        if (QDir(dir).exists())
        {
            config->setProjectDir(dir);
            config->save();

            showSelectedProjectDir();
            updateTextConfigurations();
            QMessageBox::information(this, "Mensagem", "O aplicativo está configurado ao seu projeto.");
        }
        else
        {
            QMessageBox::critical(this, "Mensagem", "A pasta selecionada é inválida.");
        }
    }
}

void MainWindow::on_btRefreshImageList_released()
{
    updateImageList();
}

void MainWindow::on_ltImages_doubleClicked(const QModelIndex &index)
{
    if (canShowProjector())
    {
        if (ui->ltImages->selectedItems().count() > 0)
        {
            QListWidgetItem *selectedItem = ui->ltImages->selectedItems().at(0);
            QString selectedBackGroundImage = selectedItem->text();

            if (!selectedBackGroundImage.isEmpty())
            {
                Configurations *config = Configurations::getInstance();
                QFile selectedBackGroundFile(config->getProjectDirForImages() + "/" + selectedBackGroundImage);
                QString selectedBackGroundImageFilePath = selectedBackGroundFile.fileName();

                if (selectedBackGroundFile.exists())
                {
                    config->setBackgroundImage(selectedBackGroundImage);
                    config->setBackgroundImageFilePath(selectedBackGroundImageFilePath);

                    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
                    projectorScreen->setBackgroundVideo("");
                    projectorScreen->setBackgroundImage(config->getBackgroundImageFilePath());
                    projectorScreen->showScreen();
                }
                else
                {
                    QMessageBox::critical(this, "Mensagem", "O arquivo selecionado é inválido.");
                }
            }
        }
    }
}

void MainWindow::on_btNoBackgroundImage_released()
{
    if (canShowProjector())
    {
        Configurations *config = Configurations::getInstance();
        config->setBackgroundImage("");
        config->setBackgroundImageFilePath("");

        ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
        projectorScreen->setBackgroundImage(config->getBackgroundImageFilePath());

        projectorScreen->showScreen();
    }
}

void MainWindow::on_ckTextAutoFit_toggled(bool checked)
{
    Configurations *config = Configurations::getInstance();
    config->setTextAutoFit(ui->ckTextAutoFit->isChecked());
}

void MainWindow::on_btNoBackgroundVideo_released()
{
    if (canShowProjector())
    {
        Configurations *config = Configurations::getInstance();
        config->setBackgroundVideo("");
        config->setBackgroundVideoFilePath("");

        ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
        projectorScreen->setBackgroundVideo(config->getBackgroundVideoFilePath());

        projectorScreen->showScreen();
    }
}

void MainWindow::on_btRefreshVideoList_released()
{
    updateVideoList();
}

void MainWindow::on_ltVideos_doubleClicked(const QModelIndex &index)
{
    if (canShowProjector())
    {
        if (ui->ltVideos->selectedItems().count() > 0)
        {
            QListWidgetItem *selectedItem = ui->ltVideos->selectedItems().at(0);
            QString selectedBackGroundVideo = selectedItem->text();

            if (!selectedBackGroundVideo.isEmpty())
            {
                Configurations *config = Configurations::getInstance();
                QFile selectedBackGroundFile(config->getProjectDirForVideos() + "/" + selectedBackGroundVideo);
                QString selectedBackGroundVideoFilePath = selectedBackGroundFile.fileName();

                if (selectedBackGroundFile.exists())
                {
                    config->setBackgroundVideo(selectedBackGroundVideo);
                    config->setBackgroundVideoFilePath(selectedBackGroundVideoFilePath);

                    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
                    projectorScreen->setBackgroundImage("");
                    projectorScreen->setBackgroundVideo(config->getBackgroundVideoFilePath());
                    projectorScreen->showScreen();
                }
                else
                {
                    QMessageBox::critical(this, "Mensagem", "O arquivo selecionado é inválido.");
                }
            }
        }
    }
}

void MainWindow::addSelectedSongToPreview()
{
    if (ui->lvSelectedSongs->count() > 0)
    {
        if (ui->lvSelectedSongs->selectedItems().count() > 0)
        {
            ui->lvPreview->setUpdatesEnabled(false);

            QModelIndexList indexes = ui->lvSelectedSongs->selectionModel()->selectedIndexes();

            for (int i = indexes.count() - 1; i > -1; --i)
            {
                QVariant selectedSongVariant;
                QString selectedSong;
                QStringList previewStringList;

                selectedSongVariant = ui->lvSelectedSongs->model()->data(indexes.at(i));
                selectedSong        = selectedSongVariant.toString();

                if (!selectedSong.isEmpty())
                {
                    Configurations *config = Configurations::getInstance();
                    QFile file(config->getProjectDirForSongs() + "/" + selectedSong);

                    if(!file.open(QIODevice::ReadOnly))
                    {
                        QMessageBox::information(0, "Mensagem", "Ocorreu um erro ao abrir o arquivo selecionado. \n\nDescrição: " + file.errorString());
                    }
                    else
                    {
                        QTextStream in(&file);
                        in.setCodec("UTF-8");
                        QString verse = QString();

                        while(!in.atEnd())
                        {
                            QString line = QString(in.readLine());

                            if (line.isEmpty())
                            {
                                if (!verse.isEmpty())
                                {
                                    previewStringList.append(verse);

                                    qDebug("Linha adicionada: ");
                                    qDebug(verse.toUtf8());
                                    qDebug("Fim da linha!");
                                }
                                verse.clear();
                            }
                            else
                            {
                                if (verse.isEmpty())
                                {
                                    verse.append(line);
                                }
                                else
                                {
                                    verse.append("\n" + line);
                                }

                                qDebug("Linha adicionada no verso: " + line.toUtf8());
                            }
                        }

                        if (!verse.isEmpty())
                        {
                            previewStringList.append(verse);

                            qDebug("Linha adicionada: ");
                            qDebug(verse.toUtf8());
                            qDebug("Fim da linha!");
                        }

                        file.close();

                        ui->lvPreview->clear();
                        ui->lvPreview->addItems(previewStringList);
                    }
                }
            }

            ui->lvPreview->setUpdatesEnabled(true);
        }
    }
}

void MainWindow::selectLastSongOnSelectedList()
{
    if (ui->lvSelectedSongs->count() > 0)
    {
        ui->lvSelectedSongs->setCurrentRow(ui->lvSelectedSongs->count() - 1);
    }
}

void MainWindow::on_lvSelectedSongs_doubleClicked(const QModelIndex &index)
{
    addSelectedSongToPreview();
}

void MainWindow::on_btRemoveSelectedSong_released()
{
    if (ui->lvSelectedSongs->count() > 0)
    {
        if (ui->lvSelectedSongs->selectedItems().count() > 0)
        {
            ui->lvSelectedSongs->setUpdatesEnabled(false);
            QModelIndexList indexes = ui->lvSelectedSongs->selectionModel()->selectedIndexes();
            qSort(indexes.begin(), indexes.end());

            for(int i = indexes.count() - 1; i > -1; --i)
            {
                ui->lvSelectedSongs->model()->removeRow(indexes.at(i).row());
            }

            ui->lvSelectedSongs->setUpdatesEnabled(true);
        }
    }
}

void MainWindow::on_btAddSongToSelectedList_released()
{
    addSongToSelectedList();
}

void MainWindow::on_btAddSongToSelectedListAndShow_released()
{
    addSongToSelectedList();
    selectLastSongOnSelectedList();
    addSelectedSongToPreview();
    addPreviewSongPartsToLiveAndShow(0);
}

void MainWindow::on_btClearPreviewList_released()
{
    ui->lvPreview->clear();
}

void MainWindow::on_btClearLiveList_released()
{
    ui->lvLive->clear();
}

void MainWindow::on_btAddSelectedSongtoPreview_released()
{
    addSelectedSongToPreview();
}

void MainWindow::on_btAddPreviewSongToLiveAndShow_released()
{
    int selectedRow = ui->lvPreview->currentRow();
    addPreviewSongPartsToLiveAndShow(selectedRow);
}

void MainWindow::on_btShowEmptyText_released()
{
    if (canShowProjector())
    {
        ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
        projectorScreen->showScreen();
        projectorScreen->renderText("");
    }
}

void MainWindow::on_btStartTimerToChangeLiveContent_released()
{
    startTimerToChangeLiveContent();
}

void MainWindow::timeoutToTimerToChangeLiveContent()
{
    selectNextSongPartOnLiveAndShow();
}

void MainWindow::timeoutToTimerToRegister()
{
    getRegisterData();
    setRegisterDataOnDialogRegister();

    SharedObjects *so = SharedObjects::getInstance();

    if (so->getIsRegistered())
    {
        timerToRegister->stop();
    }
    else
    {
        DialogRegister *dialog = DialogRegister::getInstance();
        dialog->show();
    }
}

void MainWindow::onAboutToQuit()
{
    beforeQuit();
}

void MainWindow::on_btStopTimerToChangeLiveContent_released()
{
    stopTimerToChangeLiveContent();
}

void MainWindow::on_btSelectTextOutlineColor_released()
{
    Configurations *config = Configurations::getInstance();

    QColor color = QColorDialog::getColor(config->getTextOutlineColor());

    if (color.isValid())
    {
        config->setTextOutlineColor(color.name().toUtf8());
        showSelectedTextOutlineColor();
    }
}

void MainWindow::showSelectedBackgroundImageOnPreview()
{
    if (ui->ltImages->selectedItems().count() > 0)
    {
        QListWidgetItem *selectedItem = ui->ltImages->selectedItems().at(0);
        QString selectedBackGroundImage = selectedItem->text();

        if (!selectedBackGroundImage.isEmpty())
        {
            Configurations *config = Configurations::getInstance();
            QFile selectedBackGroundFile(config->getProjectDirForImages() + "/" + selectedBackGroundImage);
            QString selectedBackGroundImageFilePath = selectedBackGroundFile.fileName();

            if (selectedBackGroundFile.exists())
            {
                QImage image;
                image.load(selectedBackGroundImageFilePath);

                ui->lbPreviewBackgroundImage->setPixmap(QPixmap::fromImage(image).scaled(
                    ui->lbPreviewBackgroundImage->geometry().width(),
                    ui->lbPreviewBackgroundImage->geometry().height(),
                    Qt::AspectRatioMode::KeepAspectRatio
                ));
            }
            else
            {
                QMessageBox::critical(this, "Mensagem", "O arquivo selecionado é inválido.");
            }
        }
    }
}

void MainWindow::showSelectedBackgroundVideoOnPreview()
{
    if (ui->ltVideos->selectedItems().count() > 0)
    {
        QListWidgetItem *selectedItem = ui->ltVideos->selectedItems().at(0);
        QString selectedBackGroundVideo = selectedItem->text();

        if (!selectedBackGroundVideo.isEmpty())
        {
            Configurations *config = Configurations::getInstance();
            QFile selectedBackGroundFile(config->getProjectDirForVideos() + "/" + selectedBackGroundVideo);
            QString selectedBackGroundImageFilePath = selectedBackGroundFile.fileName();

            if (selectedBackGroundFile.exists())
            {
                videoPlayer->stop();

                playlist->clear();
                playlist->addMedia(QUrl::fromLocalFile(selectedBackGroundImageFilePath));
                playlist->setPlaybackMode(QMediaPlaylist::Loop);

                videoPlayer->setPlaylist(playlist);
                videoPlayer->play();
            }
            else
            {
                QMessageBox::critical(this, "Mensagem", "O arquivo selecionado é inválido.");
            }
        }
    }
}

void MainWindow::beforeQuit()
{
    stopTimerToChangeLiveContent();

    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
    projectorScreen->close();

    DialogRegister *dialog = DialogRegister::getInstance();
    dialog->close();

    Configurations *config = Configurations::getInstance();
    config->save();
}

void MainWindow::showSelectedVideoStretchOption()
{
    Configurations *config = Configurations::getInstance();
    ui->ckVideoStretch->setChecked(config->getVideoStretch());
}

void MainWindow::updateVideoConfigurations()
{
    ProjectorScreen *projectorScreen = ProjectorScreen::getInstance();
    Configurations *config           = Configurations::getInstance();

    projectorScreen->updateVideoConfigurations(config->getVideoStretch());

}

void MainWindow::getRegisterData()
{
    SharedObjects *so = SharedObjects::getInstance();
    so->setDefaultData();

    QString filename = QCoreApplication::applicationDirPath() + "/license.dat";
    qDebug("Arquivo de licença: " + filename.toUtf8());

    QFile file(filename);

    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            QString licenseData;

            while(!in.atEnd())
            {
                QString line = in.readLine();
                licenseData.append(line);
            }

            file.close();

            if (!licenseData.isEmpty())
            {
                Configurations *config = Configurations::getInstance();
                quint64 privateNumber = config->getRegisterPrivateNumber();

                SimpleCrypt crypto(privateNumber);
                QString decryptResult = crypto.decryptToString(licenseData);

                QStringList resultData = decryptResult.split("|");

                if (resultData.count() == 3)
                {
                    QString keyword     = resultData.at(0);
                    QString email       = resultData.at(1);
                    QString validateStr = resultData.at(2);
                    QDate validate      = QDate::fromString(validateStr, "dd-MM-yyyy");

                    so->setRegisterEmail(email);
                    so->setRegisterValidate(validate);
                    so->setRegisterKeyword(keyword);

                    if (!so->getRegisterEmail().isEmpty() && !so->getRegisterKeyword().isEmpty() && so->getRegisterValidate().isValid())
                    {
                        if (so->getRegisterKeyword() == config->getRegisterKeyword())
                        {
                            if (so->getRegisterValidate() >= QDate::currentDate())
                            {
                                so->setIsRegistered(true);
                            }
                            else
                            {
                                so->setIsRegisterExpired(true);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::on_ltImages_clicked(const QModelIndex &index)
{
    showSelectedBackgroundImageOnPreview();
}

void MainWindow::on_ltImages_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    showSelectedBackgroundImageOnPreview();
}

void MainWindow::on_ckVideoStretch_toggled(bool checked)
{
    Configurations *config = Configurations::getInstance();
    config->setVideoStretch(ui->ckVideoStretch->isChecked());
}

void MainWindow::on_btApplyVideoConfigurations_released()
{
    updateVideoConfigurations();
}

void MainWindow::setRegisterDataOnDialogRegister()
{
    DialogRegister *dialog = DialogRegister::getInstance();
    SharedObjects *so      = SharedObjects::getInstance();

    QString registerInformation;

    if (so->getIsRegistered())
    {
        registerInformation.append("Este software está registrado para o email '" + so->getRegisterEmail().toUtf8() + "' e sua licença expira no dia '" + so->getRegisterValidate().toString("dd/MM/yyyy")) + "'.";
    }
    else
    {
        if (so->getIsRegisterExpired())
        {
            registerInformation.append("Este software está registrado para o email '" + so->getRegisterEmail().toUtf8() + "' e sua licença expirou no dia '" + so->getRegisterValidate().toString("dd/MM/yyyy") + "'.");
            registerInformation.append("\n");
            registerInformation.append("Por favor, renove sua licença para que você continue usando este software.");
            registerInformation.append("\n");
        }
        else
        {
            registerInformation.append("Este software não está registrado.");
            registerInformation.append("\n");
        }

        registerInformation.append("Você pode comprar uma nova licença ou renovar uma licença existente que você possua, através do site http://www.prsolucoes.com. Em caso de dúvidas acesse o formulário de contato em nosso site.");
    }

    dialog->setRegisterInformationText(registerInformation);
}

void MainWindow::refreshSongListBySongName(QString songName)
{
    if (songName.isEmpty())
    {
        ui->lvSongList->clear();
        return;
    }

    QStringList filters;
    filters << "*" + songName + "*.txt";

    Configurations *config = Configurations::getInstance();

    QDir songsDir(config->getProjectDirForSongs());
    QStringList allFiles = songsDir.entryList(filters, QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::Files, QDir::DirsFirst);

    ui->lvSongList->clear();
    ui->lvSongList->addItems(allFiles);
}

void MainWindow::on_btRegisterDialog_released()
{
    getRegisterData();
    setRegisterDataOnDialogRegister();

    DialogRegister *dialog = DialogRegister::getInstance();
    dialog->show();
}

void MainWindow::on_btAddSong_released()
{
    Configurations *config = Configurations::getInstance();

    if (config->getProjectDirForSongs().isEmpty())
    {
        QMessageBox::critical(this, "Mensagem", "Você ainda não selecionou um projeto. Faça isso na aba de 'configurações'.");
    }
    else
    {
        DialogAddOrUpdateSong *dialog = new DialogAddOrUpdateSong();
        dialog->setMethod(1);
        dialog->show();
    }
}

void MainWindow::on_btUpdateSong_released()
{
    if (ui->lvSongList->count() > 0)
    {
        if (ui->lvSongList->selectedItems().count() > 0)
        {
            QVariant selectedSongVariant;
            QString selectedSong;

            QModelIndexList indexes = ui->lvSongList->selectionModel()->selectedIndexes();

            selectedSongVariant = ui->lvSongList->model()->data(indexes.at(0));
            selectedSong        = selectedSongVariant.toString();

            if (!selectedSong.isEmpty())
            {
                Configurations *config = Configurations::getInstance();
                QString filename = config->getProjectDirForSongs() + "/" + selectedSong;

                QFile file(filename);

                if(!file.exists())
                {
                    QMessageBox::information(0, "Mensagem", "O arquivo selecionado não existe.");
                }
                else
                {
                    DialogAddOrUpdateSong *dialog = new DialogAddOrUpdateSong();
                    dialog->setMethod(2);
                    dialog->setFilename(filename);
                    dialog->show();                    
                }
            }
        }
    }
}

void MainWindow::on_btDeleteSong_released()
{
    if (ui->lvSongList->count() > 0)
    {
        if (ui->lvSongList->selectedItems().count() > 0)
        {
            QVariant selectedSongVariant;
            QString selectedSong;

            QModelIndexList indexes = ui->lvSongList->selectionModel()->selectedIndexes();

            selectedSongVariant = ui->lvSongList->model()->data(indexes.at(0));
            selectedSong        = selectedSongVariant.toString();

            if (!selectedSong.isEmpty())
            {
                Configurations *config = Configurations::getInstance();
                QString filename = config->getProjectDirForSongs() + "/" + selectedSong;

                QFile file(filename);

                if(!file.exists())
                {
                    QMessageBox::information(0, "Mensagem", "O arquivo selecionado não existe.");
                }
                else
                {
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Mensagem", "Tem certeza que seja apagar esta música?", QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes)
                    {
                        file.remove();
                        refreshSongListBySongName(ui->edSongName->text());
                    }
                }
            }
        }
    }
}

void MainWindow::on_btRefreshSongList_released()
{
    refreshSongListBySongName(ui->edSongName->text());
}


void MainWindow::hideBtRegisterDialog()
{
    ui->btRegisterDialog->setHidden(false);
    ui->btRegisterDialog->setFixedSize(0, 0);
}

void MainWindow::on_ltVideos_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    showSelectedBackgroundVideoOnPreview();
}

void MainWindow::on_ltVideos_itemClicked(QListWidgetItem *item)
{
    showSelectedBackgroundVideoOnPreview();
}

void MainWindow::on_btStopPreviewVideo_released()
{
    videoPlayer->stop();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index != 2)
    {
        if (videoPlayer != nullptr)
        {
            // precisa disso?
            //videoPlayer->stop();
            //qDebug("Preview de video cancelado pela troca de aba");
        }
    }
}
