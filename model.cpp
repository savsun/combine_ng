#include "model.h"
#include "ui_model.h"

Model::Model(QString filenameMap, QString filenameVideo, QString filenameXml, int countTexture, int dimention, bool cash, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Model),gl_view(filenameMap, filenameVideo,filenameXml, countTexture,dimention,cash, this)
{
    setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this);

    QFile fileXml(filenameXml);
    QXmlInputSource source(&fileXml);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.parse(source);

    if(! capture.open(filenameVideo.toStdString()))
            throw 1;
    capture.read(frame);
    countFrame=0;

    layout.addWidget(& gl_view);
    setLayout(& layout);
    gl_view.setFocus();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
    timer->start(4000);
}

void Model::updateView()
{
    QMap<string,double> frameMap;
    double point[2];
    GLdouble coord_z;
    if ((this->isVisible())&&(capture.read(frame)))
    {
        //Kohonen kohonen;
        //Mat frame_kohonen=kohonen.getFrame(frame);
        //imshow( "kohonen", frame_kohonen );

        frameMap.unite(handler.frames.at(countFrame));

        QMap<string,double>::iterator it=frameMap.begin();
        for (;it != frameMap.end(); ++it)
        {
            //cout<<it.key()<<" "<<it.value()<<endl;
            //В Xml координаты x и у поменяны местами
            if (it.key()=="x") {point[1]=(GLdouble)it.value();}
            if (it.key()=="y"){point[0]=(GLdouble)it.value();}
            if (it.key()=="h"){coord_z=(GLdouble)it.value();}
            if (it.key()=="course"){gl_view.course=(GLdouble)it.value();}
            if (it.key()=="pitch"){gl_view.pitch=(GLdouble)it.value();}
            if (it.key()=="roll"){gl_view.roll=(GLdouble)it.value();}
            if (it.key()=="aspect_x"){gl_view.aspect_x=(GLdouble)it.value();}
            if (it.key()=="aspect_y"){gl_view.aspect_y=(GLdouble)it.value();}
        }
        GLdouble maxH=74688;
        GLdouble minH=0;
        GLdouble zmax=1;
        GLdouble zmin=-1;
        GLdouble ah=(zmax-zmin)/(maxH-minH);
        GLdouble bh=zmin-minH*ah;

        gl_view.coord_z=ah*(coord_z+200)+bh;
        //cout<<gl_view.coord_z<<endl;
        gl_view.texture_map.transformGCP(point,-1,-1,1,1);
        gl_view.coord_x=point[0];
        gl_view.coord_y=point[1];
        frameMap.clear();
        countFrame++;
        //imshow("frame",frame);
        //gl_view.paintGL();
        gl_view.repaint();

        //Kmeans kmeans;
        //Mat frame_kmeans=kmeans.getFrame(frame);
        //imshow( "kmeans", frame_kmeans );
        capture.release();
    }
}
Model::~Model()
{
    delete ui;
}


