#ifndef QMAPVIEW_H
#define QMAPVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <texture.h>
#include <QDebug>
#include <parser.h>

class QMapView : public QGLWidget
{
    Q_OBJECT
private:
    GLuint* textureID;
    //GLuint textureID;
    GLuint m_nMap;
    //GLfloat m_xRotate;//поворот по х
    //GLfloat m_yRotate;//поворот по у
    bool cache;
    int countTexture;
    QString filenameMap;
    QString filenameVideo;
    QString filenameXml;
    int dimention;
    Parser handler;
    //QPoint m_ptPosition;//координата указателя мыши в момент нажатия

protected:

    //virtual void mousePressEvent(QMouseEvent *pe);
    //virtual void mouseMoveEvent(QMouseEvent *pe);
    void genTextures();
    GLuint createMap ();

public:
    virtual void initializeGL();
    virtual void resizeGL (int nWidth,int nHeight);
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    Texture texture_map;
    GLdouble pitch;
    GLdouble roll;
    GLdouble course;
    GLdouble coord_x;
    GLdouble coord_y;
    GLdouble coord_z;
    GLdouble aspect_x;
    GLdouble aspect_y;
    explicit QMapView(QString _filenameMap, QString _filenameVideo, QString _filenameXml, int _countTexture, int _dimention, bool _cache,QWidget *parent = 0);

signals:
    void doClassification();

public slots:

};

#endif // QMAPVIEW_H
