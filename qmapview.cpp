#include "qmapview.h"

QMapView::QMapView(QString _filenameMap, QString _filenameVideo, QString _filenameXml, int _countTexture, int _dimention, QWidget *parent) :
    texture_map(_filenameMap),QGLWidget(parent),pitch(0),roll(0),course(0), coord_x(0),coord_y(0),coord_z(0.1)
{
    filenameMap=_filenameMap;
    filenameVideo=_filenameVideo;
    filenameXml=_filenameXml;
    countTexture=_countTexture;
    dimention=_dimention;
}
#define PAIR(top, bottom, param, step)\
    case top:\
    {\
        param += step;\
        updateGL();\
        \
        break;\
    }\
    case bottom:\
    {\
        param -= step;\
        updateGL();\
        \
        break;\
    }
void QMapView::initializeGL()
{
    QFile fileXml(filenameXml);
    QXmlInputSource source(&fileXml);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.parse(source);

    qglClearColor(Qt::blue);//цвет для очистки буфера изображения
    glEnable(GL_TEXTURE_2D);  // установить режим двумерных текстур
    glEnable(GL_DEPTH_TEST); // устанавливает режим проверки глубины пикселей
    genTextures();
    //glShadeModel(GL_FLAT);//отключение сглаживания цветов по умолчанию
    m_nMap=createMap();

}

void QMapView::resizeGL(int nWidth, int nHeight)
{
    glViewport(0,0,(GLint)nWidth, (GLint)nHeight);//размеры окна
    glMatrixMode(GL_PROJECTION);//текущая матрица проектирования
    glLoadIdentity();//присваивает матрице проектирования единичную матрицу
    glFrustum(-0.01,0.01,-0.01,0.01,0.01,50.0);//задает пространство видимости
}

void QMapView::paintGL()
{
    //pitch=parPitch;
    //roll=parRoll;
    //course=parCourse;
    //coord_x=parCoord_x;
    //coord_y=parCoord_y;
    //coord_z=parCoord_z;
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//очистить буфер изображения

    glMatrixMode(GL_MODELVIEW);//текущая матрица моделирования
    glLoadIdentity();
    //glTranslatef(0.0,0.0,-0.1);

    //glRotatef(m_xRotate,1.0,0.0,0.0);
    //glRotatef(m_yRotate,0.0,1.0,0.0);


    VideoCapture capture;
    double point[2];
    if(! capture.open(filenameVideo.toStdString()))
            throw 1;
    Mat frame;
    capture.read(frame);
    QMap<string,double> frameMap;

    int k=0;
    do
    {
        //cout<<"Frame "<<k+1<<endl;
        //imshow("frame",frame);
        frameMap.unite(handler.frames.at(k));

        QMap<string,double>::iterator it=frameMap.begin();
        for (;it != frameMap.end(); ++it)
        {
            //cout<<it.key()<<" "<<it.value()<<endl;
            if (it.key()=="x") {point[1]=(GLdouble)it.value();}
            if (it.key()=="y"){point[0]=(GLdouble)it.value();}
            if (it.key()=="z"){coord_z=(GLdouble)it.value();}
            if (it.key()=="course"){course=(GLdouble)it.value();}
            if (it.key()=="pitch"){pitch=(GLdouble)it.value();}
            if (it.key()=="roll"){roll=(GLdouble)it.value();}
        }
        cout<<"Точки"<<point[0]<<" "<<point[1]<<endl;
        //cout<<texture_map.env.MinX<<" "<<texture_map.env.MinY<<endl;
        /*int minXPixel=-1;
        int maxXPixel=-1;
        int minYPixel=1;
        int maxYPixel=1;
        float ax=(maxXPixel-minXPixel)/(texture_map.env.MaxX-texture_map.env.MinX);
        float bx=minXPixel-ax*texture_map.env.MinX;
        float ay=(maxYPixel-minYPixel)/(texture_map.env.MaxY-texture_map.env.MinY);
        float by=minYPixel-ay*texture_map.env.MinY;
        point[0]=ax*point[0]+bx;
        point[1]=ay*point[1]+by;*/
        texture_map.transformGCP(point,-1,-1,1,1);
        coord_x=point[0];
        coord_y=point[1];
        cout<<point[0]<<" "<<point[1]<<endl;
        frameMap.clear();
        k++;
        QTest::qWait(120);
        break;
    }
    while (capture.read(frame));

    glRotated(pitch, 1, 0, 0);
    glRotated(roll, 0, 1, 0);
    glRotated(course , 0, 0, 1);
    //glRotatef(course +90, 0, 0, 1);// Докрутка на север

    glTranslated(-coord_x, - coord_y, - coord_z);

//cout<<coord_x<<" "<<coord_y<<" "<<coord_z<<endl;
    glCallList(m_nMap);
}

void QMapView::keyPressEvent(QKeyEvent* keyEvent)
{
    switch(keyEvent->key())
     {
       case Qt::Key_Space:
       {
        /*cout<<width()<<endl;
        cout<<height()<<endl;
        Mat result;
        result.create(height(),width(),CV_8UC3);
        GLbyte *buf= new GLbyte[height()*width()*3];

        //GLbyte *buf=(GLbyte *)malloc (height()*width()*3);
        glPixelStorei(GL_PACK_ALIGNMENT,1);
        glReadPixels(0,0,width(),height(),GL_BGR,GL_UNSIGNED_BYTE,buf);
        int t=0;
        for (int i=0; i<height();i++)
        {
            for (int j=0; j<width();j++)
            {
                result.at<Vec3b>(i,j)[0]=buf[t];
                result.at<Vec3b>(i,j)[1]=buf[t+1];
                result.at<Vec3b>(i,j)[2]=buf[t+2];
                t+=3;
            }
        }
        imshow("result",result);
        delete buf[];*/
        QImage image=renderPixmap().toImage();
        Mat result1(height(),width(),CV_8UC4,image.bits(),image.bytesPerLine());
        imshow("result1",result1);
        break;
    }
        PAIR(Qt::Key_Q, Qt::Key_A, coord_x, 0.01);
        PAIR(Qt::Key_W, Qt::Key_S, coord_y, 0.01);
        PAIR(Qt::Key_E, Qt::Key_D, coord_z, 0.01);
        PAIR(Qt::Key_R, Qt::Key_F, course, 10);
        PAIR(Qt::Key_T, Qt::Key_G, roll, 10);
        PAIR(Qt::Key_Y, Qt::Key_H, pitch, 10);
      default:
      QGLWidget::keyPressEvent(keyEvent);
      break;
    }
}

/*void QMapView::mousePressEvent(QMouseEvent* pe)
{
    m_ptPosition = pe->pos();
}*/

/*void QMapView::mouseMoveEvent(QMouseEvent* pe)
{
    pitch += 180*(GLfloat)(pe->y() - m_ptPosition.y())/height();//углы поворота
    roll += 180*(GLfloat)(pe->x() - m_ptPosition.x())/width();
    updateGL();

    m_ptPosition = pe->pos();
}*/
void QMapView::genTextures()
{
    QFile fileImage;
    textureID=new GLuint[countTexture*countTexture];
    QImage image[countTexture*countTexture];
    glGenTextures(countTexture*countTexture, textureID);


    for (int i=0; i<countTexture*countTexture;i++)
    {
        QString filenameImage=filenameMap;
        filenameImage.append(QString::number(dimention));
        filenameImage.append(QString::number(countTexture));
        filenameImage.append(QString::number(i));
        filenameImage.append(".png");
        fileImage.setFileName(filenameImage);
        if (! fileImage.exists())
        {
            cout<<"Пересчет текстуры"<<endl;
            texture_map.get(countTexture,dimention);
        }
        image[i].load(fileImage.fileName());
        image[i]=QGLWidget::convertToGLFormat(image[i]);
        glBindTexture(GL_TEXTURE_2D, textureID[i]);
        // дополнительные параметры текстурного объекта
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // задана линейная фильтрация вблизи
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // задана линейная фильтрация вдали
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // цвет текселя полностью замещает цвет фрагмента фигуры
        /* textureID=bindTexture(QPixmap(QString(texture_map.get("/home/alexandra/N3716.sxf"))), GL_TEXTURE_2D);
        // далее параметры текстурного объекта
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);*/ // цвет текселя полностью замещает цвет фрагмента фигуры
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,(GLsizei)image[i].width(),(GLsizei)image[i].height(),0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image[i].bits());
    }

}

GLuint QMapView::createMap()
{
    GLuint n = glGenLists(1);

    glNewList(n,GL_COMPILE);

       float a=-1;
       float b=1;
       int k=0;
       float u=(b-a)/countTexture;
       for (int i=0; i<countTexture*countTexture;i++)
        {

            glBindTexture(GL_TEXTURE_2D, textureID[i]);
            glBegin(GL_QUADS);//четырехугольник
            //qglColor(Qt::white);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(a,b,-1);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(a+u,b,-1);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(a+u,b-u,-1);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(a,b-u,-1);

            glEnd();

           if(k<countTexture-1)
            {
                k++;
                a+=u;
            }
            else
            {
                k=0;
                a=-1;
                b-=u;
            }
          }

    glEndList();

  return n;
}
