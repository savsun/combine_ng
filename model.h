#ifndef MODEL_H
#define MODEL_H
#include <all.h>
#include <QVBoxLayout>
#include <QGLWidget>
#include <qmapview.h>
#include <QDialog>
#include <QFileDialog>
#include <QWidget>
#include <QTimer>
#include <kohonen.h>
#include <kmeans.h>

namespace Ui {
class Model;
}

class Model : public QWidget
{
    Q_OBJECT
    QVBoxLayout layout;
    QMapView gl_view;
    Parser handler;
    VideoCapture capture;
    int countFrame;
    Mat frame;
    Mat marks;
    void separation (unsigned cls, vector<Point> obj, Mat img, Mat marks, int x, int y);
public slots:
    void updateView();
public:
    vector<vector<Point> > collectionClusters;
    void getClusters(Mat frame);
    explicit Model(QString filenameMap,QString filenameVideo,QString filenameXml,int countTexture,int dimention,bool cache,QWidget *parent = 0);
    ~Model();
private:
    Ui::Model *ui;
};

#endif // MODEL_H
