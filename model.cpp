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
    QXmlSimpleReader reader;
    reader.setContentHandler(&handler);
    reader.parse(source);

    if(! capture.open(filenameVideo.toStdString()))
            throw 1;
    capture.read(frame);
    countFrame=50000;

    layout.addWidget(& gl_view);
    setLayout(& layout);
    gl_view.setFocus();

    //connect(&gl_view,SIGNAL(doClassification()),this,SLOT(doClassification()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateView()));
    timer->start(4000);
}
//x-столбцы, y-строки
void Model::separation(unsigned cls, vector<Point> &obj, Mat img, Mat marks, int x, int y)
{
    if ((marks.at<unsigned>(y,x)==0)&&((obj.size()==0) || (img.at<unsigned>(y,x)==cls)))
    {
     obj.push_back(Point(x,y));
     cls=img.at<unsigned>(y,x);
     marks.at<unsigned>(y,x)=1;
     int from_x = max(0, x - 1);
     int from_y = max(0, y - 1);
     int to_x = min(img.cols/2 - 1, x + 1);
     int to_y = min(img.rows/2 - 1, y + 1);
     for(int i=from_x; i<=to_x; i++)
        for (int j=from_y; j<=to_y; j++)
        {
            separation(cls,obj,img,marks,i,j);
        }
    }
}

void Model::getClusters(Mat frame)
{
    cout<<"Разделение на кластеры"<<endl;
    marks=Mat::zeros(frame.rows,frame.cols,CV_8UC1);
    for (int u=0;u<frame.rows/2;u++)
        for (int v=0;v<frame.cols/2;v++)
        {
            if (marks.at<unsigned>(u,v)==0)
            {
                vector<Point> cluster;
                separation(0,cluster,frame,marks,v,u);
                //cout<<"Н";
                /*for (int i=0;i<cluster.size();i++)
                {
                    cout<<cluster[i]<<" ";
                }*/
                collectionClusters.push_back(cluster);
            }
        }
}
void Model::classification(vector<vector<Point> > collectionClusters, Mat perspective,int T)
{
    cout<<"Классификация"<<endl;
    Mat classificated=Mat::zeros(perspective.rows,perspective.cols, CV_8UC1);
    uint32_t key;
    //Для каждого кластера
    for (int i=0; i<collectionClusters.size();i++)
    {
        QMap<uint32_t,int> countPoint;
        int index=-1;

        for (int j=0; j<collectionClusters.at(i).size();j++)
        {
          key=perspective.at<uint32_t>(collectionClusters[i][j].y,collectionClusters[i][j].x);
          cout<<key<<" ";
          if (countPoint.contains(key))
          {
              countPoint[key]++;
          }
          else
          {
             countPoint[key]=1;
          }
        }
        int numberPoint=collectionClusters[i].size();
        QMap<uint32_t,int>::iterator it=countPoint.begin();
        for (;it != countPoint.end(); ++it)
        {
            if (it.value()*100./numberPoint>T)
            {
                if (index == -1)
                {
                    index = it.key();
                }
                else
                {
                    index = -1;
                    break;
                }
            }
        }
        if (index ==-1)
        {
             nonclassifier.push_back(i);
        }
        else
        {
            classifier.push_back(make_pair(index,i));
            Point * ptrCluster=collectionClusters[i].data();
            fillPoly(classificated,(const Point**)&ptrCluster,&numberPoint,1,Scalar::all(255),8,0,Point());
        }
    }
    imshow("classificated",classificated);
}

void Model::doClassification()
{
    cout<<"1"<<endl;
    QImage image=gl_view.renderPixmap().toImage();
    cout<<"2"<<endl;
    Mat perspective(height(),width(),CV_8UC4,image.bits(),image.bytesPerLine());
    //classification(collectionClusters,perspective,50);
    classification(collectionClusters,Mat::zeros(frame.rows,frame.cols,CV_32SC1),50);
    cout<<"Классификация закончена"<<endl;
    collectionClusters.clear();
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
        //cout<<"Кохонен завершен"<<endl;
        //imshow( "kohonen", frame_kohonen );

        //Kmeans kmeans;
        //Mat frame_kmeans=kmeans.getFrame(frame);
        //imshow( "kmeans", frame_kmeans );

        //getClusters(frame_kohonen);
        //cout<<"Разделение на кластеры окончено"<<endl;
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
        capture.release();
    }
}
Model::~Model()
{
    delete ui;
}
