#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->checkBox->setChecked(true);
    display_label.append(ui->label);
    display_label.append(ui->label_2);
    display_label.append(ui->label_3);
    display_label.append(ui->label_4);
    display_label.append(ui->label_5);
    display_label.append(ui->label_6);
    display_label.append(ui->label_7);
    display_label.append(ui->label_8);
    display_label.append(ui->label_9);
    display_label.append(ui->label_10);
    display_label.append(ui->label_11);
    display_label.append(ui->label_12);
    display_label.append(ui->label_13);
    display_label.append(ui->label_14);
    display_label.append(ui->label_15);
    display_label.append(ui->label_16);
    display_label.append(ui->label_17);
    display_label.append(ui->label_18);
    display_label.append(ui->label_19);
    display_label.append(ui->label_20);

    label.append(ui->lineEdit);
    label.append(ui->lineEdit_2);
    label.append(ui->lineEdit_3);
    label.append(ui->lineEdit_4);
    label.append(ui->lineEdit_5);
    label.append(ui->lineEdit_6);
    label.append(ui->lineEdit_7);
    label.append(ui->lineEdit_8);
    label.append(ui->lineEdit_9);
    label.append(ui->lineEdit_10);
    label.append(ui->lineEdit_11);
    label.append(ui->lineEdit_12);
    label.append(ui->lineEdit_13);
    label.append(ui->lineEdit_14);
    label.append(ui->lineEdit_15);
    label.append(ui->lineEdit_16);
    label.append(ui->lineEdit_17);
    label.append(ui->lineEdit_18);
    label.append(ui->lineEdit_19);
    label.append(ui->lineEdit_20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize size = ui->groupBox->size();

    QSize tes;
    if (size.height() > 100) {
        tes.setHeight(size.height()/10/25*25); tes.setWidth(tes.height()/25*60);
        ui->label->setFixedSize(tes);
        ui->label_2->setFixedSize(tes);
        ui->label_3->setFixedSize(tes);
        ui->label_4->setFixedSize(tes);
        ui->label_5->setFixedSize(tes);
        ui->label_6->setFixedSize(tes);
        ui->label_7->setFixedSize(tes);
        ui->label_8->setFixedSize(tes);
        ui->label_9->setFixedSize(tes);
        ui->label_10->setFixedSize(tes);
        ui->label_16->setFixedSize(tes);
    }
}

void MainWindow::on_pushButton_clicked()
{
    dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "/home",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    ui->line_source->setText(dir);
    image_name_list.clear();
    ui->listWidget->clear();
    dir_list.clear();

    QDirIterator it(dir);

    ui->label_total->setText("");
    while(it.hasNext()){
        QString temp_path = it.next();
        QFileInfo fi(temp_path);

        if (fi.isDir() && fi.fileName() != "." && fi.fileName() != "..") {
            dir_list.append(temp_path);
            QDir tmp_dir(temp_path);

            foreach(QString file_name, tmp_dir.entryList())
            {
                QString full_path = temp_path + "/" + file_name;
                QFileInfo t_fi(full_path);

                if (t_fi.isFile()) {
                    QImage file(full_path);
                    if (!image_name_list.contains(file_name)){
                        // qDebug () << file_name;
                        image_name_list.append(file_name);
                        ui->listWidget->addItem(file_name);
                    }

                }

            }

        }
    }
    ui->label_total->setText(QString::number(ui->listWidget->count()));


}

void MainWindow::displayImage(cv::Mat argI, QLabel *labelToShow)
{
    QString image_type = NULL;

    // resize the image to fit with the label.
    cv::resize(argI, argI, cv::Size(labelToShow->width(), labelToShow->height()), 0, 0, CV_INTER_AREA);

    QImage img;

    // Convert to RGB format
    if(argI.channels() == 1) {
        cv::cvtColor(argI, argI, CV_GRAY2RGB);
        image_type = "GRAY";
    } else if(argI.channels() == 3) {
        cv::cvtColor(argI, argI, CV_BGR2RGB);
        image_type = "RGB";
    }

    //convert to QImage
    img = QImage((const unsigned char*)(argI.data), argI.cols, argI.rows, QImage::Format_RGB888);

    int w = labelToShow->width();
    int h = labelToShow->height();

    // display input on label
    labelToShow->setPixmap(QPixmap::fromImage(img).scaled(w,h,Qt::KeepAspectRatio,Qt::SmoothTransformation));

    // revert the color type
    if (image_type == "GRAY")
        cv::cvtColor(argI, argI, CV_RGB2GRAY);
    else
        cv::cvtColor(argI, argI, CV_RGB2BGR);

    // wait the image be ready
    cv::waitKey(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    if (ui->listWidget->count() < 1 ||  ui->listWidget->selectedItems().count() < 1) return;

    QString selected_text = ui->listWidget->selectedItems().at(0)->text();

    if (dir == "" || selected_text == "") return;

    QString path = dir + "/../labeled";
    QDir target(path);

    if (!target.exists())
        QDir().mkdir(path);

    int dir_list_len = dir_list.length();
    bool deleted = true;

    for(int i=0; i < dir_list_len; i++){
        QString source = dir_list.at(i) + "/" + selected_text;

        QString selected_label = label.at(i)->text();
        QString tmp_dirname = dir_list.at(i).split("/").last();
        QString full_dir = path + "/" + tmp_dirname ;

        if (!QDir(full_dir).exists())
            QDir().mkdir(full_dir);

        QString full_path = full_dir + "/" + selected_label;

        int sec_idx = dir_list_len + 1;
        while(QFile(full_path).exists()) {
            full_dir = path + "/" + QString::number(sec_idx);

            if (!QDir(full_dir).exists())
                QDir().mkdir(full_dir);

            full_path = full_dir + "/" + selected_label;
            sec_idx++;
        }

        qDebug () << source << "--->" << full_path;

        if (!(QFile::copy(source, full_path) && QFile(source).remove())){
            deleted += false;
         }
    }

    if (deleted) {
        QListWidgetItem *item = ui->listWidget->selectedItems().at(0);
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
        ui->label_total->setText(QString::number(ui->listWidget->count()));
    }

    ui->lineEdit->setText("");ui->lineEdit_2->setText("");ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");ui->lineEdit_6->setText("");ui->lineEdit_5->setText("");
    ui->lineEdit_7->setText("");ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");ui->lineEdit_12->setText("");
    ui->lineEdit_13->setText("");ui->lineEdit_14->setText("");
    ui->lineEdit_15->setText("");ui->lineEdit_16->setText("");
    ui->lineEdit_17->setText("");ui->lineEdit_18->setText("");
    ui->lineEdit_19->setText("");ui->lineEdit_20->setText("");
    ui->lineEdit->setFocus();
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    if (ui->checkBox->isChecked()) {
        ui->lineEdit_2->setText(arg1);
        ui->lineEdit_3->setText(arg1);
        ui->lineEdit_4->setText(arg1);
        ui->lineEdit_5->setText(arg1);
        ui->lineEdit_6->setText(arg1);
        ui->lineEdit_7->setText(arg1);
        ui->lineEdit_8->setText(arg1);
        ui->lineEdit_9->setText(arg1);
        ui->lineEdit_10->setText(arg1);
        ui->lineEdit_11->setText(arg1);
        ui->lineEdit_12->setText(arg1);
        ui->lineEdit_13->setText(arg1);
        ui->lineEdit_14->setText(arg1);
        ui->lineEdit_15->setText(arg1);
        ui->lineEdit_16->setText(arg1);
        ui->lineEdit_17->setText(arg1);
        ui->lineEdit_18->setText(arg1);
        ui->lineEdit_19->setText(arg1);
        ui->lineEdit_20->setText(arg1);
    }
}

void MainWindow::on_listWidget_itemSelectionChanged()
{
    if (ui->listWidget->count() < 1 ||  ui->listWidget->selectedItems().count() < 1) return;

    QListWidgetItem *item = ui->listWidget->selectedItems().at(0);

    int index = 0;

    foreach(QString dir_name, dir_list){
        //qDebug() << dir_name + "/" + item->text();
        try {
            QString file_path = dir_name + "/" + item->text();
            if (QFile::exists(file_path)) {
                cv::Mat tmp_image_file = cv::imread(QString(dir_name + "/" + item->text()).toLatin1().data());
                displayImage(tmp_image_file, display_label.at(index));
            } else display_label.at(index)->clear();
        } catch (...) {
            display_label.at(index)->clear();

        }

        index++;
    }

}

void MainWindow::on_lineEdit_returnPressed()
{
    if (ui->checkBox->isChecked()) {
        this->on_pushButton_2_clicked();
    }
}
