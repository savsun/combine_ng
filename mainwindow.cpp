#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),gl_view(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    layout.addWidget(& gl_view);
    ui->centralWidget->setLayout(& layout);
    gl_view.setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}
