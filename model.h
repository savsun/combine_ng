#ifndef MODEL_H
#define MODEL_H
#include <all.h>
#include <QVBoxLayout>
#include <QGLWidget>
#include <qmapview.h>
#include <QDialog>
#include <QFileDialog>
#include <QWidget>

namespace Ui {
class Model;
}

class Model : public QWidget
{
    Q_OBJECT
    QVBoxLayout layout;
    QMapView gl_view;

public:
    explicit Model(QString filenameMap,QString filenameVideo,QString filenameXml,int countTexture,int dimention,QWidget *parent = 0);
    ~Model();

private:
    Ui::Model *ui;
};

#endif // MODEL_H