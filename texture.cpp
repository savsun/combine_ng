#include "texture.h"

Texture::Texture(QString filenameMap)
{
    _filenameMap=filenameMap;

    OGRRegisterAll();
    OGRDataSource   *poDataset;
    poDataset = OGRSFDriverRegistrar::Open(_filenameMap.toStdString().c_str());
    if( poDataset == NULL )
        {
            printf( "Open failed.\n" );
            exit( 1 );
        }

    system=poDataset->GetLayerByName("SYSTEM");
    //Получение размеров рамки
    system->GetExtent(& env);
    OGRDataSource::DestroyDataSource( poDataset );
}
 void Texture::transformGCP(double point[],int minXPixel,int minYPixel,int maxXPixel,int maxYPixel)
{
     /*float ax=(maxXPixel-minXPixel)/(env.MaxX-env.MinX);
     float bx=minXPixel-ax*env.MinX;
     float ay=(maxYPixel-minYPixel)/(env.MaxY-env.MinY);
     float by=minYPixel-ay*env.MinY;
     point[0]=ax*point[0]+bx;
     point[1]=ay*point[1]+by;*/
    //cout<<"minXPixel "<<minXPixel<<"minYPixel "<<minYPixel<<"maxXPixel"<<maxXPixel<<"maxYPixel "<<maxYPixel<<endl;
    int *panSuccess = (int *) CPLCalloc(sizeof(int),1);
    GDAL_GCP array_GCP[4];
    array_GCP[0].pszId="1";
    array_GCP[0].pszInfo="";
    array_GCP[0].dfGCPPixel=minXPixel;
    array_GCP[0].dfGCPLine=minYPixel;
    array_GCP[0].dfGCPX=env.MinX;
    array_GCP[0].dfGCPY=env.MinY;
    array_GCP[0].dfGCPZ=0;

    array_GCP[1].pszId="2";
    array_GCP[1].pszInfo="";
    array_GCP[1].dfGCPPixel=maxXPixel;
    array_GCP[1].dfGCPLine=maxYPixel;
    array_GCP[1].dfGCPX=env.MaxX;
    array_GCP[1].dfGCPY=env.MaxY;
    array_GCP[1].dfGCPZ=0;

    array_GCP[2].pszId="3";
    array_GCP[2].pszInfo="";
    array_GCP[2].dfGCPPixel=maxXPixel;
    array_GCP[2].dfGCPLine=minYPixel;
    array_GCP[2].dfGCPX=env.MaxX;
    array_GCP[2].dfGCPY=env.MinY;
    array_GCP[2].dfGCPZ=0;

    array_GCP[3].pszId="4";
    array_GCP[3].pszInfo="";
    array_GCP[3].dfGCPPixel=minXPixel;
    array_GCP[3].dfGCPLine=maxYPixel;
    array_GCP[3].dfGCPX=env.MinX;
    array_GCP[3].dfGCPY=env.MaxY;
    array_GCP[3].dfGCPZ=0;

    void *pTransformArg = GDALCreateGCPTransformer(4,array_GCP,0,TRUE);
    GDALGCPTransform(pTransformArg, FALSE,1,&point[0],&point[1], NULL, panSuccess);
    CPLFree( panSuccess );
}

void Texture::get(int countTexture, int dimention)
{
    OGRRegisterAll();
    OGRDataSource   *poDataset;
    poDataset = OGRSFDriverRegistrar::Open(_filenameMap.toStdString().c_str());
    if( poDataset == NULL )
        {
            printf( "Open failed.\n" );
            exit( 1 );
        }

    OGRLayer * water;
    water=poDataset->GetLayerByName("water");

    //Размеры изображения
    int mat_rows=dimention;
    int mat_cols=dimention;
    vector<Mat> result;
    //Mat mat;
    //mat=Mat::zeros(mat_rows,mat_cols, CV_8UC3);
    //Количество текстур N^2
    int N=countTexture;
    result.resize(N*N);
    for (int k=0;k<N*N;k++)
    {
        result[k]=Mat::zeros(mat_rows,mat_cols, CV_8UC3);
        //result.push_back(mat.clone());
    }
    //Размеры растра
    int rastr_rows=N*mat_rows;
    int rastr_cols=N*mat_cols;

    //счетчик
    uint32_t sc;

    int k=0;

    OGRFeature *poFeature;

    for (int u=0;u<N;u++)
    {
     for(int v=0;v<N;v++)
     {
         water->ResetReading();
         sc=1;
            while ((poFeature = water->GetNextFeature()) != NULL )
            {
                OGRGeometry *poGeometry;
                poGeometry = poFeature->GetGeometryRef();
                if (poGeometry != NULL && poGeometry->getGeometryType() == wkbPolygon)
                {
                    OGRPolygon *poPolygon = (OGRPolygon *)  poGeometry;

                    OGRLinearRing * ringE1;
                    ringE1=poPolygon->getExteriorRing();
                    //Количество точек во внешнем контуре
                    int ne1=ringE1->getNumPoints();
                    //Количество дырок
                    int numRings1=poPolygon->getNumInteriorRings();
                    int n[numRings1+1];
                    Point ** pts1=new Point* [numRings1+1];
                    pts1[0]=new Point[ne1];
                    n[0]=ne1;
                    double point[2];
                    for (int i=0; i<ne1;i++)
                    {
                        point[0]=ringE1->getX(i);
                        point[1]=ringE1->getY(i);
                        //В SXF min,min в левом нижнем углу, а в CV в правом верхнем
                        transformGCP(point,0,rastr_cols-1,rastr_rows-1,0);
                        //GDALGCPTransform(pTransformArg, FALSE,1 ,&X,&Y, NULL, panSuccess);
                        pts1[0][i]=Point(point[0]-u*mat_cols,point[1]-v*mat_rows);
                    }
                    int ni1;
                    OGRLinearRing * ringI1;
                    for (int i=0;i<numRings1;i++)
                    {
                      ringI1=poPolygon->getInteriorRing(i);
                      ni1=ringI1->getNumPoints();
                      pts1[i+1]=new Point[ni1];

                      n[i+1]=ni1;
                      for (int j=0; j<ni1;j++)
                       {
                         point[0]=ringI1->getX(j);
                         point[1]=ringI1->getY(j);
                         transformGCP(point,0,rastr_cols-1,rastr_rows-1,0);
                         //GDALGCPTransform(pTransformArg, FALSE,1 ,&X,&Y, NULL, panSuccess);
                         pts1[i+1][j]=Point(point[0]-u*mat_cols,point[1]-v*mat_rows);
                       }
                     }
                    //fillPoly(result[k], (const Point**) pts1,n,numRings1+1,Scalar::all(255),8,0,Point());
                    fillPoly(result[k], (const Point**) pts1,n,numRings1+1,Scalar(((0xFF0000&sc)>>16),((0x00FF00&sc)>>8),(0x0000FF&sc)),8,0,Point());

                    for (int i=0;i<numRings1+1;i++)
                    {
                        delete[] pts1[i];
                    }
                    delete[] pts1;
                  }
                sc+=0xFFFFFF/3000;

                //sc++;
                OGRFeature::DestroyFeature( poFeature );
             }
            QString filename=_filenameMap;
            filename.append(QString::number(dimention));
            filename.append(QString::number(N));
            filename.append(QString::number(k));
            filename.append(".png");
            imwrite(filename.toStdString(),result[k]);
            k++;
            }
    }
    OGRDataSource::DestroyDataSource( poDataset );
}
