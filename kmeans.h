#ifndef KMEANS_H
#define KMEANS_H
#include <all.h>

class Kmeans{

    static double distance(Vec3b parFirstPixel, Vec3b parSecondPixel);//Расчет растояния между двумя пикселями
    void init();//Инициализация массивов центроидов и пикселей сегментируемого изображения
    void pixelClustering();//Отнесение пикселя к сегментам
    void *newCenterPositions();//Получение новых позиций центроидов

    Mat _Frame;
    Vec3b* _ClusterCenters;//Центры кластеров
    bool _CentersFound;//Центры кластеров были найдены
    int _ClusterCount;//Количество кластеров
    int _MaxIterationCount;//Максимальное количество итераций сегментации
    Mat _Labels;//Принадлежность пикселей к кластерам
    double _Precision;//Точность вычисления целевой функции

public:
    Kmeans(int parClusterCount,Mat parFrame,int parMaxIterationCount,double parPrecision);
    Mat  getFrame();
};

#endif // KMEANS_H
