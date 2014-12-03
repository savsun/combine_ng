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
#include <kmeansopencv.h>

namespace Ui {
class Model;
}

class Model : public QDialog
{
    Q_OBJECT
    QVBoxLayout layout;
    QMapView gl_view;
    Parser handler;
    VideoCapture capture;
    const int countFrame=58000;
    Mat frame;
    Mat marks;
    vector<unsigned> nonclassifier;
    vector<pair<uint32_t,unsigned>> classifier;
    QTimer timer;
    void separation (unsigned cls, vector<Point> &obj, Mat img, Mat marks, int x, int y);
    void classification(vector<vector<Point> > collectionClusters, Mat perspective, int T);
public slots:
    //void updateView();
    void doClassification();
    void changeXmlFrame();
public:
    vector<vector<Point> > collectionClusters;
    void getClusters(Mat frame);
    explicit Model(QString filenameMap,QString filenameVideo,QString filenameXml,int countTexture,int dimention,bool cache,QWidget *parent = 0);
    ~Model();
private:
    Ui::Model *ui;
};

#endif // MODEL_H
