#include "model.h"
#include "ui_model.h"

Model::Model(QString filenameMap, QString filenameVideo, QString filenameXml, int countTexture, int dimention, bool cache, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Model),gl_view(filenameMap, filenameVideo,filenameXml, countTexture,dimention,cache, this)
{
    setWindowModality(Qt::ApplicationModal);
    ui->setupUi(this);

    QFile fileXml(filenameXml);
    QXmlInputSource source(&fileXml);
    QXmlSimpleReader reader;\
    reader.setContentHandler(&handler);
    reader.parse(source);

    if(! capture.open(filenameVideo.toStdString()))
            throw 1;
    capture.read(frame);
    countFrame=50000;

    layout.addWidget(& gl_view);
    setLayout(& layout);
    gl_view.setFocus();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
    timer->start(4000);
}

void Model::separation(unsigned cls, vector<Point> obj, Mat img, Mat marks, int x, int y)
{
    if ((marks.at<unsigned>(x,y)==0)&&((obj.size()==0) || (img.at<unsigned>(x,y)=cls)))
    {
     obj.push_back(Point(x,y));
     cls=img.at<unsigned>(x,y);
     marks.at<unsigned>(x,y)=1;
     for(int i=x-1; i<=x+1; i++)
        for (int j=y-1; j<=y+1; j++)
     {
         separation(cls,obj,img,marks,i,j);
     }
    }
}

void Model::getClusters(Mat frame)
{
    marks=Mat::zeros(frame.rows,frame.cols,CV_8UC1);
    for (int u=0;u<frame.rows;u++)
        for (int v=0;v<frame.cols;v++)
        {
            if (marks.at<unsigned>(u,v)==0)
            {
                int cls=frame.at<unsigned>(u,v);
                vector<Point> cluster;
                separation(cls,cluster,frame,marks,u,v);
                collectionClusters.push_back(cluster);
            }
        }
}

void Model::updateView()
{
    QMap<string,double> frameMap;
    double point[2];
    GLdouble coord_z;
    if ((this->isVisible())&&(capture.read(frame)))
    {
        Kohonen kohonen;
        Mat frame_kohonen=kohonen.getFrame(frame);
        imshow( "kohonen", frame_kohonen );
        getClusters(frame_kohonen);

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
        imshow("frame",frame);
        //gl_view.paintGL();
        gl_view.repaint();

        Kmeans kmeans;
        Mat frame_kmeans=kmeans.getFrame(frame);
        imshow( "kmeans", frame_kmeans );
        collectionClusters.clear();
        capture.release();
    }
}
Model::~Model()
{
    delete ui;
}


