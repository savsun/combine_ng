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
    _CentersFound = false;

    //Инициализация массива номеров кластеров для каждого пикселя
    _Labels=Mat::zeros(parFrame.rows,parFrame.cols,CV_8UC1);
}
//Расчет растояния между двумя пикселями
double Kmeans::distance(Vec3b parFirstPixel,
                        Vec3b parSecondPixel)
{
    return sqrt((parFirstPixel[0] - parSecondPixel[0])*(parFirstPixel[0] - parSecondPixel[0])
                + (parFirstPixel[1] - parSecondPixel[1])*(parFirstPixel[1] - parSecondPixel[1])
                + (parFirstPixel[2] - parSecondPixel[2])*(parFirstPixel[2] - parSecondPixel[2]));
}

void Kmeans::init()
{
    //Инициализация массива центроидов
    if (_ClusterCenters == NULL)
    {
        _ClusterCenters = new Vec3b[_ClusterCount];
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

            _ClusterCenters[i][0] = _Frame.at<Vec3b>(rowIndex,colIndex)[0];
            _ClusterCenters[i][1] = _Frame.at<Vec3b>(rowIndex,colIndex)[1];
            _ClusterCenters[i][2] = _Frame.at<Vec3b>(rowIndex,colIndex)[2];
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
                Vec3b currentPixel;
                currentPixel[0] = _Frame.at<Vec3b>(i,j)[0];
                currentPixel[1] = _Frame.at<Vec3b>(i,j)[1];
                currentPixel[2] = _Frame.at<Vec3b>(i,j)[2];

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
void *Kmeans::newCenterPositions()
{
    //Новые координаты центроида
    Vec3d* newCenterPositioins = new Vec3d[_ClusterCount];

    //Пикселей в кластере
    int* pixelInCluster = new int[_ClusterCount];

    //Инициализация новых центроидов
    for (int k = 0; k < _ClusterCount; k++)
    {
        pixelInCluster[k] = 0;

        newCenterPositioins[k][0] = 0;
        newCenterPositioins[k][1] = 0;
        newCenterPositioins[k][2] = 0;
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
            newCenterPositioins[numberCluster][0] += _Frame.at<Vec3b>(i,j)[0];
            newCenterPositioins[numberCluster][1] += _Frame.at<Vec3b>(i,j)[1];
            newCenterPositioins[numberCluster][2] += _Frame.at<Vec3b>(i,j)[2];
        }
    }

    //cout<<"New"<<endl;
    for (int k = 0; k < _ClusterCount; k++)
    {
        if (pixelInCluster[k] != 0)
        {
            newCenterPositioins[k][0] /= (double)pixelInCluster[k];
            newCenterPositioins[k][1] /= (double)pixelInCluster[k];
            newCenterPositioins[k][2] /= (double)pixelInCluster[k];
            //cout<<(int)newCenterPositioins[k][0]<<" "<<(int)newCenterPositioins[k][1]<<" "<<(int)newCenterPositioins[k][2]<<endl;
        }
    }

    /*cout<<"Old"<<endl;
    for (int k = 0; k < _ClusterCount; k++)
    {
        cout<<(int)_ClusterCenters[k][0]<<" "<<(int)_ClusterCenters[k][1]<<" "<<(int)_ClusterCenters[k][2]<<endl;
    };*/
    //Проверка условия выполнения точности
    bool flag=true;//истина,если нет изменения во всех класерах
    for (int k = 0; k < _ClusterCount; k++)
    {
        if (distance(newCenterPositioins[k],_ClusterCenters[k])>_Precision)
        {
             _ClusterCenters[k] = newCenterPositioins[k];
             flag=false;
        }
    }
    if (flag) {_CentersFound=true;}


    delete[] newCenterPositioins;
    delete [] pixelInCluster;
}
//реализация kmeans без opencv
Mat Kmeans::getFrame()
{
    //Датчик случайных чисел для центров кластеров
    //Заполняем _ClustersCenters
       init();

       int i=1;
       while ((!_CentersFound) && (i != _MaxIterationCount))
       {
            //Относим пиксели к кластерам
            pixelClustering();
            //Определение новых центров
            newCenterPositions();

            i++;
       }
       cout<<"Количество пересчетов "<<i<<endl;
    return _Labels;
}

