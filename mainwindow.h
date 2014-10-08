#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QGLWidget>
#include <QDialog>
#include <QFileDialog>
#include <model.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void LoadMap();
    void LoadVideo();
    void LoadXml();
    void Run();

private:
    Ui::MainWindow *ui;

    QString filenameMap;
    QString filenameVideo;
    QString filenameXml;
    int countTexture;
    int dimention;

};

#endif // MAINWINDOW_H
