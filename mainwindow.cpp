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
	connect(ui->checkBoxIsPosition, SIGNAL(toggled(bool)), this, SLOT(refreshUi()));

	checkMap("/home/amv/disser/data/Полет/maps/N3702/N3702.sxf");
	checkVideo("/home/amv/disser/data/Полет/video_full.avi");
	checkXml("/home/amv/disser/data/Полет/let2.xml");

	refreshUi();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refreshUi()
{
    ui->buttonRun->setEnabled(! filenameMap.isEmpty());
	ui->buttonRun->setEnabled(! filenameVideo.isEmpty());
    ui->buttonRun->setEnabled(! filenameXml.isEmpty());

	ui->spinBoxPosition->setEnabled
	(
		ui->checkBoxIsPosition->isChecked()
		&&
		! filenameVideo.isEmpty()
	);
}

//Обработка кнопки "Обзор" для карты
void MainWindow::LoadMap()
{
	checkMap(QFileDialog::getOpenFileName(this, QString::fromUtf8("Выбрать карту.."),".", trUtf8("Векторная карта (*.sxf)")));
}

void MainWindow::checkMap(QString filename)
{
	filenameMap = filename;
    ui->editLoadMap->setText(filenameMap);
	
	refreshUi();
}

//Обработка кнопки "Обзор" для видео
void MainWindow::LoadVideo()
{
	checkVideo(QFileDialog::getOpenFileName(this, QString::fromUtf8("Выбрать видео.."),".", trUtf8("Видео (*.avi *.mp4)")));
}

void MainWindow::checkVideo(QString filename)
{
	filenameVideo = filename;

	VideoCapture video(filenameVideo.toStdString());

	if(video.isOpened())
	{
		ui->editLoadVideo->setText(filenameVideo);
		ui->spinBoxPosition->setMaximum(video.get(CV_CAP_PROP_FRAME_COUNT));
	}
	else
	{
		ui->editLoadVideo->setText("");
	}
	
	refreshUi();
}
//Обработка кнопки "Обзор" для Xml
void MainWindow::LoadXml()
{
    checkXml(QFileDialog::getOpenFileName(this, QString::fromUtf8("Выбрать файл метаданных.."),".", trUtf8("XML-файл (*.xml)")));
}

void MainWindow::checkXml(QString filename)
{
	filenameXml = filename;
    ui->editLoadXml->setText(filenameXml);
	
	refreshUi();
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
    Model model(filenameMap,filenameVideo,filenameXml,countTexture,dimention,cash,ui->checkBoxIsPosition->isChecked() ? ui->spinBoxPosition->value() - 1 : -1, this);
    model.exec();
    cout<<"end"<<endl;
}

