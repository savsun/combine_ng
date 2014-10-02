#ifndef QMAPVIEW_H
#define QMAPVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <texture.h>
#include <QDebug>

class QMapView : public QGLWidget
{
    Q_OBJECT
private:
    GLuint* textureID;
    //GLuint textureID;
    GLuint m_nMap;
    //GLfloat m_xRotate;//поворот по х
    //GLfloat m_yRotate;//поворот по у
    GLfloat pitch;
    GLfloat roll;
    GLfloat course;
    GLfloat x;
    GLfloat y;
    GLfloat z;
    int count_texture;
    //QPoint m_ptPosition;//координата указателя мыши в момент нажатия

protected:
    virtual void initializeGL();
    virtual void resizeGL (int nWidth,int nHeight);
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent *keyEvent);
    //virtual void mousePressEvent(QMouseEvent *pe);
    //virtual void mouseMoveEvent(QMouseEvent *pe);
    void genTextures();
    GLuint createMap ();

public:
    explicit QMapView(QWidget *parent = 0);

signals:

public slots:

};

#endif // QMAPVIEW_H
