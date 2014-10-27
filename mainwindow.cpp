#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->buttonRun->setEnabled(false);
    connect(ui->buttonLoadMap, SIGNAL(clicked()), this, SLOT(LoadMap()));
    connect(ui->buttonLoadVideo, SIGNAL(clicked()), this, SLOT(LoadVideo()));
    connect(ui->buttonLoadXml, SIGNAL(clicked()), this, SLOT(LoadXml()));
    connect(ui->buttonRun, SIGNAL(clicked()), this, SLOT(Run()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
//Обработка кнопки "Обзор" для карты
void MainWindow::LoadMap()
{
    filenameMap=QFileDialog::getOpenFileName(this, QString::fromUtf8("Выбрать карту.."),".", trUtf8("Векторная карта (*.sxf)"));
    ui->editLoadMap->setText(filenameMap);
    //ui->buttonRun->setEnabled(! filenameMap.toStdString().empty());
}
//Обработка кнопки "Обзор" для видео
void MainWindow::LoadVideo()
{
    filenameVideo=QFileDialog::getOpenFileName(this, QString::fromUtf8("Выбрать видео.."),".", trUtf8("Видео (*.avi *.mp4)"));
    ui->editLoadVideo->setText(filenameVideo);
    //ui->buttonRun->setEnabled(! filenameVideo.toStdString().empty());
}
//Обработка кнопки "Обзор" для Xml
void MainWindow::LoadXml()
{
    filenameXml=QFileDialog::getOpenFileName(this, QString::fromUtf8("Выбрать файл метаданных.."),".", trUtf8("XML-файл (*.xml)"));
    ui->editLoadXml->setText(filenameXml);
    //ui->buttonRun->setEnabled(! filenameXml.toStdString().empty());
}

void MainWindow::Run()
{
    //ui->buttonRun->setEnabled(false);
    //ui->buttonLoadMap->setEnabled(false);
    countTexture=ui->editCountTexture->text().toInt();
    dimention=ui->editDimentionTexture->text().toInt();
    filenameMap=ui->editLoadMap->text();
    filenameVideo=ui->editLoadVideo->text();
    filenameXml=ui->editLoadXml->text();
    bool cash=ui->checkBoxCash->isChecked();
    Model* model = new Model(filenameMap,filenameVideo,filenameXml,countTexture,dimention,cash);
    model->show();
}
