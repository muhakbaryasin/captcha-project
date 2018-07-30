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
    QDirIterator it(dir_src);

    while(it.hasNext()) {
        QString temp_path = it.next();
        QFileInfo fi(temp_path);

            if (fi.isFile() && fi.fileName() != "." && fi.fileName() != "..") {
                QString file_name = fi.fileName();

                QImage file(temp_path);
                QFileInfo tmp_trg(dir_trg + "/" + file_name);

                // if (file.format() && !tmp_trg.exists()) {
                if (!tmp_trg.exists()) {
                    try {
                        cv::Mat source = cv::imread(QString(temp_path).toLatin1().data());
                        //cv::imshow("anu", source);
                        prep->eroding(source);
                        QString full_path = dir_trg + "/" + file_name + ".png";
                        QString rm_ext = dir_trg + "/" + file_name;

                        if (!cv::imwrite(QString(full_path).toLatin1().data(), source)) {
                            qDebug() << "cant save image " << full_path;
                            continue;
                        }

                        if (!QFile::copy(full_path, rm_ext)) {
                            qDebug() << "cant copy " << full_path << " -> " << rm_ext;
                            continue;
                        }

                        if (!QFile(full_path).remove()) {
                            qDebug() << "cant remove " << full_path;
                            continue;
                        }

                    } catch (...) {
                        qDebug () << "error load or save " << temp_path << " -> " << dir_trg + "/" + file_name;
                    }


                }
                else qDebug() << "path target exists -> " << tmp_trg.absoluteFilePath();

            }

    }
}
