#include "DialogAddOrUpdateSong.hpp"
#include "ui_DialogAddOrUpdateSong.h"

DialogAddOrUpdateSong::DialogAddOrUpdateSong(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddOrUpdateSong)
{
    ui->setupUi(this);
    setModal(true);
}

DialogAddOrUpdateSong::~DialogAddOrUpdateSong()
{
    delete ui;
}

void DialogAddOrUpdateSong::setFilename(QString &filename)
{
    QFile file(filename);

    this->filename = filename;
    this->title    = QFileInfo(filename).baseName();

    ui->edContent->clear();
    ui->edTitle->setText(title);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString lines = in.readAll();
        ui->edContent->insertPlainText(lines);
        file.close();
    }
}

void DialogAddOrUpdateSong::setMethod(int method)
{
    this->method = method;

    if (this->method == 1)
    {
        setWindowTitle("Criar nova música");
    }
    else if (this->method == 2)
    {
        setWindowTitle("Alterar música");
    }
}

void DialogAddOrUpdateSong::updateFile()
{
    Configurations *config = Configurations::getInstance();
    title = ui->edTitle->text();

    bool store = false;

    if (this->title == NULL || this->title.isEmpty())
    {
        QMessageBox::critical(this, "Mensagem", "Informe o título da música. Este título também será usado como o nome do arquivo.");
        return;
    }
    else
    {
        store = true;
    }

    if (store)
    {
        QFile file(config->getProjectDirForSongs() + "/" + this->title + ".txt");

        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << ui->edContent->toPlainText();
            file.close();
            close();
        }
        else
        {
            QMessageBox::critical(this, "Mensagem", "Não foi possível salvar o arquivo da música, verifique se você possui permissão de escrita na pasta onde as músicas ficam localizadas.");
        }
    }
}

void DialogAddOrUpdateSong::formatText()
{
    QString originalText = ui->edContent->toPlainText();
    QStringList originalTextLines = originalText.split(QRegExp("\n|\r\n|\r"));
    QStringList originalWords = QStringList();
    QStringList newTextLines = QStringList();
    QString newLine = "[NEW-LINE]";

    qDebug("Linhas: %i", originalTextLines.count());

    // obtém todas as palavras e adiciona as quebras de linhas obrigatórias
    foreach (QString originalTextLine, originalTextLines)
    {
        qDebug("Frase: ");
        qDebug(originalTextLine.toUtf8());

        if (originalTextLine == "" || originalTextLine == "\n" || originalTextLine == "\r" || originalTextLine == "\r\n" || originalTextLine == " ")
        {
            originalWords.append(newLine);
        }
        else
        {
            QStringList words = originalTextLine.split(QRegExp("(\\s)+"), QString::SkipEmptyParts);
            originalWords.append(words);
        }
    }

    // cria novas frases com as regras de limitação
    int maxWordsPerLine = 3;
    int maxPhrasePerBlock = 3;
    QString newPhrase = "";
    int wordCount = 0;
    int phraseCount = 0;
    bool breakLineInserted = false;
    bool newLineInserted = false;

    qDebug("Palavras: %i", originalWords.count());

    foreach (QString word, originalWords)
    {
        qDebug("Palavra:");
        qDebug(word.toUtf8());

        newLineInserted = false;

        if ((wordCount+1) > maxWordsPerLine)
        {
            wordCount = 0;
            newTextLines.append(newPhrase.toUtf8().trimmed());
            newPhrase = "";
            phraseCount++;
            newLineInserted = true;
        }

        if ((phraseCount+1) > maxPhrasePerBlock)
        {
            if (!breakLineInserted)
            {
                newTextLines.append("\r\n");
                phraseCount = 0;
                breakLineInserted = true;
            }
        }
        else
        {
            breakLineInserted = false;
        }

        if (word == newLine)
        {
            if (!newLineInserted)
            {
                newTextLines.append(newPhrase.toUtf8().trimmed());
            }

            wordCount = 0;

            if (!breakLineInserted)
            {
                newTextLines.append("\r\n");
                breakLineInserted = true;
            }

            newPhrase = "";
            phraseCount = 0;
            newLineInserted = true;
        }
        else
        {
            newPhrase += (" " + word.toUtf8().trimmed());
            wordCount++;
        }
    }

    if (!newLineInserted)
    {
        newTextLines.append(newPhrase.toUtf8().trimmed());
    }

    // exibe novo texto
    ui->edContent->setPlainText("");

    foreach (QString newTextLine, newTextLines)
    {
        qDebug("Nova frase:");
        qDebug(newTextLine.toUtf8().trimmed());

        ui->edContent->appendPlainText(newTextLine.toUtf8().trimmed());
    }

    ui->edContent->setPlainText(ui->edContent->toPlainText().toUtf8().trimmed());
}

void DialogAddOrUpdateSong::on_btCancelar_released()
{
    close();
}

void DialogAddOrUpdateSong::on_btConfirmar_released()
{
    updateFile();
}

void DialogAddOrUpdateSong::on_btFormatar_released()
{
    formatText();
}
