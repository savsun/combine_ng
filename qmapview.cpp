#include "qmapview.h"

QMapView::QMapView(QString _filenameMap, QString _filenameVideo, QString _filenameXml, int _countTexture, int _dimention, bool _cache, QWidget *parent) :
    texture_map(_filenameMap),QGLWidget(parent),pitch(0),roll(0),course(0), coord_x(0),coord_y(0),coord_z(0.1)
{
    filenameMap=_filenameMap;
    filenameVideo=_filenameVideo;
    filenameXml=_filenameXml;
    countTexture=_countTexture;
    dimention=_dimention;
    cache=_cache;
    coord_x=0.561648;
    coord_y=0.945259;
    coord_z=-0.997256;
    course=-3330.65;
    roll=5.06294;
    pitch=-19.427;
    aspect_x=65.5;
    aspect_y=64.4;
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
    qglClearColor(Qt::blue);//цвет для очистки буфера изображения
    glEnable(GL_TEXTURE_2D);  // установить режим двумерных текстур
    glEnable(GL_DEPTH_TEST); // устанавливает режим проверки глубины пикселей
    genTextures();
    m_nMap=createMap();
}

void QMapView::resizeGL(int nWidth, int nHeight)
{
    glViewport(0,0,(GLint)nWidth, (GLint)nHeight);//размеры окна
    glMatrixMode(GL_PROJECTION);//текущая матрица проектирования
    glLoadIdentity();//присваивает матрице проектирования единичную матрицу
    glOrtho(-1,1,-1,1,-1,1);
    gluPerspective(aspect_x,aspect_x/aspect_y,0, 2500000);
    //glFrustum(-0.01,0.01,-0.01,0.01,0.01,50.0);//задает пространство видимости
}

void QMapView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//очистить буфер изображения
    glMatrixMode(GL_PROJECTION);//текущая матрица проектирования
    glLoadIdentity();//присваивает матрице проектирования единичную матрицу
    glOrtho(-1,1,-1,1,-1,1);
    gluPerspective(aspect_x,aspect_x/aspect_y,0, 2500000);

    glMatrixMode(GL_MODELVIEW);//текущая матрица моделирования
    glLoadIdentity();
    //glTranslatef(0.0,0.0,-0.1);

    //glRotatef(m_xRotate,1.0,0.0,0.0);
    //glRotatef(m_yRotate,0.0,1.0,0.0);
    //glFrustum(-0.01,0.01,-0.01,0.01,0.01,50.0);//задает пространство видимости

    glRotated(-pitch-90, 1, 0, 0);
    glRotated(-roll, 0, 1, 0);
    glRotated(course, 0, 0, 1);
    cout<<coord_x<<" "<<coord_y<<" "<<coord_z<<" "<<course<<" "<<roll<<" "<<pitch<<" "<<aspect_x<<" "<<aspect_y<<endl;
    glTranslated(-coord_x, - coord_y, - coord_z);
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
        doClassification();
        break;
    }
    case Qt::Key_End:
    {
        toDo=false;
        break;
    }
    case Qt::Key_Enter:
    {
        toDo=true;
        break;
    }
    case Qt::Key_Right:
    {
        frame_count++;
        changeXmlFrame();
        break;
    }
    case Qt::Key_Left:
    {
        frame_count--;
        changeXmlFrame();
        break;
    }
        PAIR(Qt::Key_Q, Qt::Key_A, coord_x, 0.0001);
        PAIR(Qt::Key_W, Qt::Key_S, coord_y, 0.0001);
        PAIR(Qt::Key_E, Qt::Key_D, coord_z, 0.0001);
        PAIR(Qt::Key_R, Qt::Key_F, course, 1);
        PAIR(Qt::Key_T, Qt::Key_G, roll, 1);
        PAIR(Qt::Key_Y, Qt::Key_H, pitch, 1);
        PAIR(Qt::Key_U, Qt::Key_J, aspect_x, 0.1);
        PAIR(Qt::Key_I, Qt::Key_K, aspect_y, 0.1);
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
    textureID.reset(new GLuint[countTexture*countTexture],std::default_delete<GLuint[]>());
    QImage image[countTexture*countTexture];
    glGenTextures(countTexture*countTexture, textureID.get());

    for (int i=0; i<countTexture*countTexture;i++)
    {
        QString filenameImage=filenameMap;
        filenameImage.append(QString::number(dimention));
        filenameImage.append(QString::number(countTexture));
        filenameImage.append(QString::number(i));
        filenameImage.append(".png");
        fileImage.setFileName(filenameImage);
        if ((! fileImage.exists())||cache)
        {
            cache=false;
            cout<<"Пересчет текстуры"<<endl;
            texture_map.get(countTexture,dimention);
        }
        image[i].load(fileImage.fileName());
        image[i]=QGLWidget::convertToGLFormat(image[i]);
        glBindTexture(GL_TEXTURE_2D, textureID.get()[i]);
        // дополнительные параметры текстурного объекта
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // задана линейная фильтрация вблизи
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // задана линейная фильтрация вдали
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для s координаты
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // при фильтрации игнорируются тексели, выходящие за границу текстуры для t координаты
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // цвет текселя полностью замещает цвет фрагмента фигуры

        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,(GLsizei)image[i].width(),(GLsizei)image[i].height(),0,
                     GL_RGBA, GL_UNSIGNED_BYTE, image[i].bits());
    }
}
QMapView::~QMapView()
{
    glDeleteLists(m_nMap,1);
    glDeleteTextures(countTexture*countTexture,textureID.get());
}
GLuint QMapView::createMap()
{
    GLuint n = glGenLists(1);

    glNewList(n,GL_COMPILE);
    /*float maxH=74688;
    float minH=0;
    float zmax=1;
    float zmin=-1;
    float ah=(zmax-zmin)/(maxH-minH);
    float bh=zmin-minH*ah;
    int h=130;
    float z=ah*h+bh;*/
    float z=-1;

       float a=-1;
       float b=1;
       int k=0;
       float u=(b-a)/countTexture;
       for (int i=0; i<countTexture*countTexture;i++)
        {

            glBindTexture(GL_TEXTURE_2D, textureID.get()[i]);
            glBegin(GL_QUADS);//четырехугольник
            //qglColor(Qt::white);

            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(a,b,z);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(a,b-u,z);

            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(a+u,b-u,z);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(a+u,b,z);

            glEnd();

           if(k<countTexture-1)
            {
                k++;
                b-=u;
            }
            else
            {
                k=0;
                b=1;
                a+=u;
            }
          }

    glEndList();

  return n;
}
