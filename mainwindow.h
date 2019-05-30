#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFile>
#include <QString>
#include <QFileInfo>
#include <QTextCodec>
#include <QTextStream>
#include <QFileSystemModel>
#include <string.h>
#include <stdlib.h>
#include <QInputDialog>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);

    void on_action_triggered();

    void on_action_2_triggered();

    void on_listView_clicked(const QModelIndex &index);

    void on_action_8_triggered();

    void on_listView_2_clicked(const QModelIndex &index);

    void on_action_9_triggered();

    void on_action_4_triggered();

    void on_action_7_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;
};


#endif // MAINWINDOW_H
