#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    dir_src = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit->setText(dir_src);
}

void MainWindow::on_pushButton_2_clicked()
{
    dir_trg = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_2->setText(dir_trg);
}

void MainWindow::on_pushButton_3_clicked()
{
    this->recursiveCopy(dir_src, dir_trg);
}

void MainWindow::recursiveCopy(QString path_src, QString path_trg)
{
    QDirIterator it(path_src);

    while(it.hasNext()) {
        QString temp_path = it.next();
        QFileInfo fi(temp_path);

            if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..") {
                this->recursiveCopy(temp_path, path_trg);
            } else if (fi.isFile() && fi.fileName() != "." && fi.fileName() != "..") {
                QString tmp_ext = "";

                if (ui->comboBox->currentText() == "hash") {
                    QFile f(temp_path);

                    if (f.open(QFile::ReadOnly)) {
                        QCryptographicHash hash(QCryptographicHash::Md5);
                        if (hash.addData(&f)) {
                            tmp_ext = hash.result().toHex();
                        }

                    }

                } else {
                    tmp_ext = QUuid::createUuid().toString();
                }

                QImage file(temp_path);
                QString file_name = fi.fileName();

                if (ui->checkBox->isChecked())
                    file_name = this->mapNumberToLetter(file_name);

                QFileInfo tmp_trg(path_trg + "/" + file_name + "." + tmp_ext);

                // if (file.format() && !tmp_trg.exists()) {
                if (!tmp_trg.exists()) {
                    if (!(QFile::copy(temp_path, path_trg + "/" + file_name + "." + tmp_ext))) {
                        qDebug () << "copy failed " << temp_path << " -> " << path_trg + "/" + file_name + "." + tmp_ext;
                     }
                }

            }
    }
}

QString MainWindow::mapNumberToLetter(QString input)
{
    QStringList output;

    foreach(QString character, input){
        if (character == "0")
            output << "a";
        else if (character == "1")
            output << "b";
        else if (character == "2")
            output << "c";
        else if (character == "3")
            output << "d";
        else if (character == "4")
            output << "e";
        else if (character == "5")
            output << "f";
        else if (character == "6")
            output << "g";
        else if (character == "7")
            output << "h";
        else if (character == "8")
            output << "i";
        else if (character == "9")
            output << "j";
        //numberString << QString::number(character.unicode(), 16);
    }

    return output.join("");

}
