#include "model.h"
#include "ui_model.h"

Model::Model(QString filenameMap, QString filenameVideo, QString filenameXml, int countTexture, int dimention, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Model),gl_view(filenameMap, filenameVideo,filenameXml, countTexture,dimention, this)
{
    ui->setupUi(this);

    /*Parser handler;
    QFile fileXml(filenameXml);
    QXmlInputSource source(&fileXml);
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.parse(source);*/

    layout.addWidget(& gl_view);
    setLayout(& layout);
    gl_view.setFocus();

    /*VideoCapture capture;
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
            if (it.key()=="x") {point[0]=(GLdouble)it.value();}
            if (it.key()=="y"){point[1]=(GLdouble)it.value();}
            if (it.key()=="z"){coord_z=(GLdouble)it.value();}
            if (it.key()=="course"){course=(GLdouble)it.value();}
            if (it.key()=="pitch"){pitch=(GLdouble)it.value();}
            if (it.key()=="roll"){roll=(GLdouble)it.value();}
        }
        cout<<"Точки"<<point[0]<<" "<<point[1]<<endl;
        cout<<texture_map.env.MinX<<" "<<texture_map.env.MinY<<endl;
        int minXPixel=-1;
        int maxXPixel=-1;
        int minYPixel=1;
        int maxYPixel=1;
        float ax=(maxXPixel-minXPixel)/(texture_map.env.MaxX-texture_map.env.MinX);
        float bx=minXPixel-ax*texture_map.env.MinX;
        float ay=(maxYPixel-minYPixel)/(texture_map.env.MaxY-texture_map.env.MinY);
        float by=minYPixel-ay*texture_map.env.MinY;
        coord_x=ax*point[0]+bx;
        coord_y=ay*point[1]+by;

        gl_view.paintGL();
        //texture_map.transformGCP(point,0,0,100,100);
        //coord_x=point[0];
        //coord_y=point[1];
        cout<<point[0]<<" "<<point[1]<<endl;
        frameMap.clear();
        k++;
        QTest::qWait(120);
    }
    while (capture.read(frame));*/
}

Model::~Model()
{
    delete ui;
}


