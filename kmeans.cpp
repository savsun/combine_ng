#include "kmeans.h"

Kmeans::Kmeans(int parClusterCount,Mat parFrame,int parMaxIterationCount,double parPrecision)
{
    if (parClusterCount > 0)
    {
        _ClusterCount = parClusterCount;
    }

    _Frame=parFrame;
    _MaxIterationCount = parMaxIterationCount;
    _Precision = parPrecision;

    _ClusterCenters = NULL;

    _LastIterationCount = 0;
    //_CentersFound = false;
    //_PixelsClustered = false;

    //Инициализация массива номеров кластеров для каждого пикселя
    _Labels=Mat::zeros(parFrame.rows,parFrame.cols,CV_8UC1);
}
//Расчет растояния между двумя пикселями
double Kmeans::distance(PixelRgb parFirstPixel,
                        PixelRgb parSecondPixel)
{
    /*return sqrt((parFirstPixel.X - parSecondPixel.X)*(parFirstPixel.X - parSecondPixel.X)
                + (parFirstPixel.Y - parSecondPixel.Y)*(parFirstPixel.Y - parSecondPixel.Y)
                + (parFirstPixel.Red - parSecondPixel.Red)*(parFirstPixel.Red - parSecondPixel.Red)
                + (parFirstPixel.Green - parSecondPixel.Green)*(parFirstPixel.Green - parSecondPixel.Green)
                + (parFirstPixel.Blue - parSecondPixel.Blue)*(parFirstPixel.Blue - parSecondPixel.Blue));*/

    return sqrt((parFirstPixel.Red - parSecondPixel.Red)*(parFirstPixel.Red - parSecondPixel.Red)
                + (parFirstPixel.Green - parSecondPixel.Green)*(parFirstPixel.Green - parSecondPixel.Green)
                + (parFirstPixel.Blue - parSecondPixel.Blue)*(parFirstPixel.Blue - parSecondPixel.Blue));
}

void Kmeans::init()
{
    //Инициализация массива центроидов
    if (_ClusterCenters == NULL)
    {
        _ClusterCenters = new PixelRgb[_ClusterCount];
        for (int i = 0; i < _ClusterCount; i++)
        {
            //переменная для получения целой части числа
            double* intPart = new double;
            *intPart = 0;

            int colIndex;
            int rowIndex;

            modf(qrand() / (double) INT_MAX * _Frame.cols, intPart);
            colIndex = (int) *intPart;

            modf(qrand() / (double) INT_MAX * _Frame.rows, intPart);
            rowIndex = (int) *intPart;

            //_ClusterCenters[i].X = colIndex;
           // _ClusterCenters[i].Y = rowIndex;
            _ClusterCenters[i].Red = _Frame.at<Vec3b>(rowIndex,colIndex)[0];
            _ClusterCenters[i].Green = _Frame.at<Vec3b>(rowIndex,colIndex)[1];
            _ClusterCenters[i].Blue = _Frame.at<Vec3b>(rowIndex,colIndex)[2];
        }
    }
}

void Kmeans::pixelClustering()
{
    //Распределение пикселей по кластерам

        double minDistance; //Минимальное расстояние до центроида

        //Номер ближайшего центроида
        int numberNearestCluster;

        //Проходим по всем пикселям
        for (int i = 0; i < _Frame.rows; i++)
        {
            for (int j = 0; j < _Frame.cols; j++)
            {
                //Текущий обрабатываемый пиксель
                PixelRgb currentPixel;
                //currentPixel.X = j;
                //currentPixel.Y = i;
                currentPixel.Red = _Frame.at<Vec3b>(i,j)[0];
                currentPixel.Green = _Frame.at<Vec3b>(i,j)[1];
                currentPixel.Blue = _Frame.at<Vec3b>(i,j)[2];

                //Ишем ближайший центроид
                minDistance = distance(currentPixel,_ClusterCenters[0]);
                numberNearestCluster = 0;

                double currentDistance;
                for(unsigned k = 1; k < _ClusterCount; k++)
                {
                    currentDistance = distance(currentPixel, _ClusterCenters[k]);
                    if (currentDistance < minDistance)
                    {
                        numberNearestCluster = k;
                        minDistance = currentDistance;
                    }
                }

                //Приписываем пиксель кластеру
                _Labels.at<uint8_t>(i,j) = numberNearestCluster*20;
            }
        }
        //_PixelsClustered = true;
}

//Получение новых позиций центроидов
PixelRgb * Kmeans::newCenterPositions()
{
    //Новые координаты центроида
    PixelRgb* newCenterPositioins = new PixelRgb[_ClusterCount];

    //Пикселей в кластере
    int* pixelInCluster = new int[_ClusterCount];

    //Инициализация новых центроидов
    for (int k = 0; k < _ClusterCount; k++)
    {
        pixelInCluster[k] = 0;

        //newCenterPositioins[k].X = 0;
        //newCenterPositioins[k].Y = 0;
        newCenterPositioins[k].Red = 0;
        newCenterPositioins[k].Green = 0;
        newCenterPositioins[k].Blue = 0;
    }


    //Расчет новых значений центроидов
    unsigned numberCluster;
    for (int i = 0; i <_Frame.rows; i++)
    {
        for (int j = 0; j < _Frame.cols; j++)
        {
            numberCluster =_Labels.at<uint8_t>(i,j)/20;
            pixelInCluster[numberCluster]++;
            //Значение нового центра - среднее арифметическое
            //newCenterPositioins[numberCluster].X += j;
            //newCenterPositioins[numberCluster].Y += i;
            newCenterPositioins[numberCluster].Red += _Frame.at<Vec3b>(i,j)[0];
            newCenterPositioins[numberCluster].Green += _Frame.at<Vec3b>(i,j)[1];
            newCenterPositioins[numberCluster].Blue +=_Frame.at<Vec3b>(i,j)[2];
        }
    }

    for (int k = 0; k < _ClusterCount; k++)
    {
        if (pixelInCluster[k] != 0)
        {
            //newCenterPositioins[k].X /= (double)pixelInCluster[k];
            //newCenterPositioins[k].Y /= (double)pixelInCluster[k];
            newCenterPositioins[k].Red /= (double)pixelInCluster[k];
            newCenterPositioins[k].Green /= (double)pixelInCluster[k];
            newCenterPositioins[k].Blue /= (double)pixelInCluster[k];
        }
    }

    return newCenterPositioins;
}
//реализация kmeans без opencv
Mat Kmeans::getFrame()
{
    //Датчик случайных чисел для центров кластеров
    //Заполняем _ClustersCenters
       init();

    for (int i=1;i != _MaxIterationCount;i++)
     {
        //Относим пиксели к кластерам
        pixelClustering();

        //Определение новых центров
        newCenterPositions();
    }
    return _Labels;
}

