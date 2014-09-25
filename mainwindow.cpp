#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    layout.addWidget(& gl_view);
    ui->centralWidget->setLayout(& layout);
}

MainWindow::~MainWindow()
{
    delete ui;
}
