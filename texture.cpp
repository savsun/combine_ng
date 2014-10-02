#include "texture.h"

Texture::Texture()
{
}
void Texture::get(QString filenameMap,int count_texture)
{
    OGRRegisterAll();
    OGRDataSource   *poDataset;
    poDataset = OGRSFDriverRegistrar::Open(filenameMap.toStdString().c_str());
    if( poDataset == NULL )
        {
            printf( "Open failed.\n" );
            exit( 1 );
        }

    OGRLayer * water;
    OGRLayer * system;
    water=poDataset->GetLayerByName("LAYER4");
    system=poDataset->GetLayerByName("LAYER13");

    //Размеры изображения
    int mat_rows=4096;
    int mat_cols=4096;
    vector<Mat> result;
    //Mat mat;
    //mat=Mat::zeros(mat_rows,mat_cols, CV_8UC3);
    //Количество текстур N^2
    int N=count_texture;
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

    //Получение размеров рамки
    OGREnvelope env;
    system->GetExtent(& env);

    int *panSuccess = (int *) CPLCalloc(sizeof(int),1);

    GDAL_GCP* array_GCP=new GDAL_GCP[4];
    array_GCP[0].pszId="1";
    array_GCP[0].pszInfo="";
    array_GCP[0].dfGCPPixel=0;
    array_GCP[0].dfGCPLine=0;
    array_GCP[0].dfGCPX=env.MinX;
    array_GCP[0].dfGCPY=env.MaxY;
    array_GCP[0].dfGCPZ=0;

    array_GCP[1].pszId="2";
    array_GCP[1].pszInfo="";
    array_GCP[1].dfGCPPixel=rastr_cols-1;
    array_GCP[1].dfGCPLine=rastr_rows-1;
    array_GCP[1].dfGCPX=env.MaxX;
    array_GCP[1].dfGCPY=env.MinY;
    array_GCP[1].dfGCPZ=0;

    array_GCP[2].pszId="3";
    array_GCP[2].pszInfo="";
    array_GCP[2].dfGCPPixel=rastr_cols-1;
    array_GCP[2].dfGCPLine=0;
    array_GCP[2].dfGCPX=env.MaxX;
    array_GCP[2].dfGCPY=env.MaxY;
    array_GCP[2].dfGCPZ=0;

    array_GCP[3].pszId="4";
    array_GCP[3].pszInfo="";
    array_GCP[3].dfGCPPixel=0;
    array_GCP[3].dfGCPLine=rastr_rows-1;
    array_GCP[3].dfGCPX=env.MinX;
    array_GCP[3].dfGCPY=env.MinY;
    array_GCP[3].dfGCPZ=0;

    double X;
    double Y;

    void *pTransformArg = GDALCreateGCPTransformer(4,array_GCP,0,TRUE);
    int k=0;

    OGRFeature *poFeature;
    for (int v=0;v<N;v++)
    {
     for(int u=0;u<N;u++)
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

                    int* n=new int[numRings1+1];
                    Point ** pts1=new Point* [numRings1+1];
                    pts1[0]=new Point[ne1];
                    n[0]=ne1;
                    for (int i=0; i<ne1;i++)
                    {
                        X=ringE1->getX(i);
                        Y=ringE1->getY(i);
                        GDALGCPTransform(pTransformArg, FALSE,1 ,&X,&Y, NULL, panSuccess);
                        pts1[0][i]=Point(X-u*mat_cols,Y-v*mat_rows);
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
                         X=ringI1->getX(j);
                         Y=ringI1->getY(j);
                         GDALGCPTransform(pTransformArg, FALSE,1 ,&X,&Y, NULL, panSuccess);
                         pts1[i+1][j]=Point(X-u*mat_cols,Y-v*mat_rows);
                       }
                     }
                    fillPoly(result[k], (const Point**) pts1,n,numRings1+1,Scalar(((0xFF0000&sc)>>16),((0x00FF00&sc)>>8),(0x0000FF&sc)),8,0,Point());
                }
                sc+=0xFFFFFF/3000;

                //sc++;
                OGRFeature::DestroyFeature( poFeature );
             }
            QString filename=filenameMap;
            filename.append(QString::number(N));
            filename.append(QString::number(k));
            filename.append(".png");
            //imshow(filename.toStdString(),result[k]);
            //waitKey();
            imwrite(filename.toStdString(),result[k]);
            k++;
            }
    }
    CPLFree( panSuccess );
    //cv::imwrite("texture.png",result);
    OGRDataSource::DestroyDataSource( poDataset );
}
