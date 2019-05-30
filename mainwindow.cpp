#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileInfo>
char sw = 0;

QString getPth(Ui::MainWindow* ui, MainWindow* that, QFileSystemModel* model);

QString getPthWD(Ui::MainWindow* ui, MainWindow* that, QFileSystemModel* model);

void recCopy(Ui::MainWindow* ui, QString pth, QString pth2);

bool copy_dir_recursive(QString from_dir,QString to_dir,bool replace_on_conflit);



MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QFileSystemModel(this);
    model->setFilter(QDir::AllEntries);
    model->setRootPath("");

    ui->listView->setModel(model);
    ui->listView_2->setModel(model);
    connect(ui->listView_2, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(on_listView_doubleClicked(QModelIndex)));
    ui->label->setText("C:\/");
    ui->label_2->setText("C:\/");

    setStyleSheet("QWidget{background: #1B1E2F; font: 12pt \"Liberation Mono\"; color:#01ACA4; text-align:center;}"
                  "QInputDialog{background: #1B1E2F;}"
                  "QPushButton{color:#01ACA4;font-size: 10pt; text-transform:lowercase; padding:5px; width:130px; border:0;}"
                  "QPushButton:hover{color:#01ACA4;font-size: 10pt; text-transform:lowercase; padding:5px; width:130px; border:0;}"
                  "QAction{background: #01ACA4;}"
                  "QListView{border:0; color:#01ACA4; font-size:12pt;}"
                  "QLabel{border: 0; color: #01ACA4;font: 10pt; border-bottom:1px solid #01ACA4;}");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{
    __asm
        {
        mov sw, 1
        }

    QFileInfo fileInfo = model->fileInfo(index);
    QString path;

    if(fileInfo.isDir())
    {
        path = fileInfo.filePath();
        ui->label->setText(path);
    }
    else if (fileInfo.isFile()) {
        path = fileInfo.filePath();
        ui->label->setText(path);
    }
}


void MainWindow::on_listView_2_clicked(const QModelIndex &index)
{
    __asm
        {
        mov sw, 0
        }
    QFileInfo fileInfo = model->fileInfo(index);
    QString path;
    if(fileInfo.isDir())
    {
        path = fileInfo.filePath();
        ui->label_2->setText(path);
    }
    else if (fileInfo.isFile())
    {
        path = fileInfo.filePath();
        ui->label_2->setText(path);
    }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QListView *listView = (QListView *)sender();
    QFileInfo fileInfo = model->fileInfo(index);

    if(fileInfo.fileName() == "..")
    {
        QDir dir = fileInfo.dir();
        dir.cdUp();
        listView->setRootIndex(model->index(dir.absolutePath()));
    }

    else if(fileInfo.fileName() == ".")
    {
        listView->setRootIndex(model->index(""));
    }

    else if(fileInfo.isDir())
    {
        listView->setRootIndex(index);
    }
    else if(fileInfo.isFile())
    {
        QString pth = getPthWD(ui, this, model);

        QString filepath = QObject::trUtf8(pth.toUtf8());

        QUrl fileurl = QUrl::fromLocalFile(filepath);

        bool result = QDesktopServices::openUrl(fileurl);
    }
}

//open
void MainWindow::on_action_triggered()
{
    QString pth = getPthWD(ui, this, model);
    if(pth.length() > 1 ){
        QString filepath = QObject::trUtf8(pth.toUtf8());

        QUrl fileurl = QUrl::fromLocalFile(filepath);

        bool result = QDesktopServices::openUrl(fileurl);

    }

}

//remove
void MainWindow::on_action_2_triggered()
{
    QString pth = getPthWD(ui, this, model);

    if(pth.length() > 3 ){

        if(QDir(pth).exists())
        {
            QDir(pth).removeRecursively();
        }
        else if(QFile(pth).exists())
        {
            QFile(pth).remove();
        }
    }
}



//create folder
void MainWindow::on_action_8_triggered()
{
    QString pth = getPth(ui, this, model);

    if(pth.length() > 1){
        if(!QDir(pth).exists())
        {
            QDir().mkdir(pth);
        }

    }
}

//create file
void MainWindow::on_action_9_triggered()
{
    QString pth = getPth(ui, this, model);
    if(pth.length() > 1){
        FILE *f = fopen(pth.toUtf8(), "w");
        fclose(f);
    }

}

// get path with dialog
QString getPth(Ui::MainWindow* ui, MainWindow* that, QFileSystemModel* model){


    QString name = QInputDialog::getText(that, "Имя папки/файла", "");



    QString pth;

    QDir dDir;

    if(sw == 1)
    {
        dDir = QDir(model->filePath(ui->listView->rootIndex()));
        pth = ui->label->text();
        if (pth.toStdString()[pth.length()-1] == '.') {
            pth = pth.remove(pth.length()-1, 1);
            if (pth.toStdString()[pth.length()-1] == '.')
            {
                pth = pth.remove(pth.length()-1, 1);
            }

        }
    }
    else if(sw == 0)
    {
        dDir = QDir(model->filePath(ui->listView_2->rootIndex()));
        pth = ui->label_2->text();
        if (pth.toStdString()[pth.length()-1] == '.')
        {
            pth = pth.remove(pth.length()-1, 1);
            if (pth.toStdString()[pth.length()-1] == '.')
            {
                pth = pth.remove(pth.length()-1, 1);
            }
        }
    }

    pth.append("/");
    pth.append(name);

    return pth;
}

//get path without dialog
QString getPthWD(Ui::MainWindow* ui, MainWindow* that, QFileSystemModel* model){
    QString pth;

    QDir dDir;

    if(sw == 1)
    {
        dDir = QDir(model->filePath(ui->listView->rootIndex()));
        pth = ui->label->text();
    }
    else if(sw == 0)
    {
        dDir = QDir(model->filePath(ui->listView_2->rootIndex()));
        pth = ui->label_2->text();
    }
    return pth;
}
//copy
void MainWindow::on_action_4_triggered()
{
    QString pth = getPthWD(ui, this, model);

    if(pth.length() > 1){

        QString pth2 = ui->label_2->text();

        recCopy(ui, pth, pth2);
    }

}

void recCopy(Ui::MainWindow* ui, QString pth, QString pth2)
{
    if(pth.length() > 1){

        if(QFileInfo(pth).isDir())
        {
            copy_dir_recursive(pth, pth2, true);

        }
        else if (QFileInfo(pth).isFile())
        {
            if(QFileInfo(pth2).isFile())
            {
                pth2 = pth2.remove(pth2.length() - QFileInfo(pth2).fileName().length(), QFileInfo(pth2).fileName().length());
            }
            if(QFileInfo(pth2).isDir())
            {
                pth2.append('/');
                pth2.append(QFileInfo(pth).fileName());

                QFile::copy(pth, pth2);
            }
        }
    }


}


bool copy_dir_recursive(QString from_dir, QString to_dir, bool replace_on_conflit)
{
    QDir dir;
    dir.setPath(from_dir);

    from_dir += QDir::separator();
    to_dir += QDir::separator();

    foreach (QString copy_file, dir.entryList(QDir::Files))
    {
        QString from = from_dir + copy_file;
        QString to = to_dir + copy_file;

        if (QFile::exists(to))
        {
            if (replace_on_conflit)
            {
                if (QFile::remove(to) == false)
                {
                    return false;
                }
            }
            else
            {
                continue;
            }
        }

        if (QFile::copy(from, to) == false)
        {
            return false;
        }
    }

    foreach (QString copy_dir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
    {
        QString from = from_dir + copy_dir;
        QString to = to_dir + copy_dir;

        if (dir.mkpath(to) == false)
        {
            return false;
        }

        if (copy_dir_recursive(from, to, replace_on_conflit) == false)
        {
            return false;
        }
    }

    return true;
}

void MainWindow::on_action_7_triggered()
{
    QString pth = getPthWD(ui, this, model);

    QString pth2 = ui->label_2->text();

    if(pth.length() > 3 && pth2.length() > 3){


        recCopy(ui, pth, pth2);

        if(QDir(pth).exists())
        {
            QDir(pth).removeRecursively();
        }
        else if(QFile(pth).exists())
        {
            QFile(pth).remove();
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QString pth = getPth(ui, this, model);
    if(pth.length() > 1){

        if(!QDir(pth).exists())
        {
            QDir().mkdir(pth);
        }

    }
}

void MainWindow::on_pushButton_clicked()
{

    QString pth = getPth(ui, this, model);

    if(pth.length() > 1){

        QFile file(pth);

        file.open(QIODevice::ReadWrite);

        file.close();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString pth = getPthWD(ui, this, model);
    QString pth2 = ui->label_2->text();
    if(pth.length() > 1 && pth2.length() > 1){

        recCopy(ui, pth, pth2);
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QString pth = getPthWD(ui, this, model);

    if(pth.length() > 3){

        if(QDir(pth).exists())
        {
            QDir(pth).removeRecursively();
        }
        else if(QFile(pth).exists())
        {
            QFile(pth).remove();
        }

    }
}
