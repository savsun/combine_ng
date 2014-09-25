#ifndef QMAPVIEW_H
#define QMAPVIEW_H

#include <QGLWidget>
#include <QMouseEvent>
#include <texture.h>

class QMapView : public QGLWidget
{
    Q_OBJECT
private:
    GLuint textureID;
    GLuint m_nPyramid;
    GLfloat m_xRotate;//поворот по х
    GLfloat m_yRotate;//поворот по у
    QPoint m_ptPosition;//координата указателя мыши в момент нажатия

protected:
    virtual void initializeGL();
    virtual void resizeGL (int nWidth,int nHeight);
    virtual void paintGL ();
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    void genTextures();
    GLuint createPyramid ();

public:
    explicit QMapView(QWidget *parent = 0);

signals:

public slots:

};

#endif // QMAPVIEW_H
