#ifndef KMEANS_H
#define KMEANS_H
#include <all.h>
//Структура для описание цветного пикселя
struct PixelRgb
{
    //unsigned X;
    //unsigned Y;
    unsigned Red;
    unsigned Green;
    unsigned Blue;
};

class Kmeans{

    static double distance(PixelRgb parFirstPixel,PixelRgb parSecondPixel);//Расчет растояния между двумя пикселями
    double distance(int parClusterIndex, int parCol, int parRow);//Расчет растояния между центром кластера с текущим пикселем
    void init();//Инициализация массивов центроидов и пикселей сегментируемого изображения
    void pixelClustering();//Отнесение пикселя к сегментам
    PixelRgb* newCenterPositions();//Получение новых позиций центроидов

    Mat _Frame;
    PixelRgb* _ClusterCenters;//Центры кластеров
    bool _CentersFound;//Центры кластеров были найдены
    int _ClusterCount;//Количество кластеров
    int _LastIterationCount;//Количество итераций при последней сегментации
    int _MaxIterationCount;//Максимальное количество итераций сегментации
    bool _PixelsClustered;//Пиксели были распределены по кластерам
    Mat _Labels;//Принадлежность пикселей к кластерам
    double _Precision;//Точность вычисления целевой функции

public:
    Kmeans(int parClusterCount,Mat parFrame,int parMaxIterationCount,double parPrecision);
    Mat  getFrame();
};

#endif // KMEANS_H
