#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QtWidgets>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_listWidget_itemSelectionChanged();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QString dir;
    QStringList dir_list, image_name_list;
    QList<QLabel*> display_label;
    QList<QLineEdit *> label;

    void displayImage(cv::Mat argI, QLabel *labelToShow);
};

#endif // MAINWINDOW_H
