#include "countthreshold.h"
#include <all.h>
CountThreshold::CountThreshold()
{
}

int  CountThreshold::otsuThreshold(Mat parFrame)
{
    //перевод в оттенки серого
    Mat Gray;
    /*Gray=Mat::zeros(parFrame.rows,parFrame.cols, CV_8UC1);
    for (int i=0; i < Gray.rows; i++)
    {
        for (int j=0; j < Gray.cols; j++)
        {
            Gray.at<uint8_t>(i,j)=0.2125*parFrame.at<Vec3b>(i,j)[0] +0.7154*parFrame.at<Vec3b>(i,j)[1]+0.0721*parFrame.at<Vec3b>(i,j)[2];
        }
    }*/

    cvtColor( parFrame, Gray, CV_RGB2GRAY);
    unsigned min = Gray.at<uint8_t>(0,0);
    unsigned max = Gray.at<uint8_t>(0,0);
    for (int i=0; i < Gray.rows; i++)
    {
        for (int j=0; j < Gray.cols; j++)
        {
            unsigned value = Gray.at<uint8_t>(i,j);
            if (value<min)
            {
                min = value;
            }
            if (value>max)
            {
                max = value;
            }
            //cout<<value<<" ";
        }
        //cout<<endl;
    }
    int histSize=max-min+1;
    //cout << max<<endl;
    //cout <<min<<endl;
    //cout<<histSize<<endl;
    int* hist = new int [histSize];

    for (int t=0; t<histSize;t++)
    {
        hist[t]=0;
    }

    //высота бинов
    for (int i=0; i<Gray.rows;i++)
    {
        for (int j=0; j<Gray.cols;j++)
        {
            int value = Gray.at<uint8_t>(i,j)-min;
            //cout<<value<<" ";
           hist[value]++;
           //cout<<hist[value]<<" ";
        }
     }

    int m=0; //сумма высот всех бинов, домноженных на положение их середины
    int n=0; //сумма высот всех бинов

    for( int t=0;t<=(max-min);t++)
    {
        m += t*hist[t];
        n += hist[t];
    }

    float maxSigma=-1;//максимальное значение межклассовой дисперсии
    int threshold=0;//Порог, соответствующий maxSigma
    int alpha1=0;//Сумма высот всех бинов для класса 1
    int beta1=0;//Сумма высот всех бинов для класса 1, доможенных на положение их середины

    for (int t=0;t<(max-min);t++)
    {
        alpha1 += t*hist[t];
        beta1 += hist[t];

        float w1=(float)beta1/n;
        float a=(float)alpha1/beta1-(float)(m-alpha1)/(n-beta1);
        float sigma=w1*(1-w1)*a*a;
        if (sigma>maxSigma)
        {
            maxSigma=sigma;
            threshold=t;
        }
    }
    threshold += min;
    return threshold;
}

int  CountThreshold::gradThreshold(Mat parFrame)
{
    Mat Gray;
    cvtColor( parFrame, Gray, CV_RGB2GRAY);
    Mat Gradient;
    Gradient.create(Gray.rows, Gray.cols,CV_8UC1);
    for (int i=0; i < Gray.rows; i++)
    {
        for (int j=0; j < Gray.cols; j++)
        {
            int value1 = abs((int)Gray.at<uint8_t>(i+1,j)-(int)Gray.at<uint8_t>(i-1,j));
            int value2 = abs((int)Gray.at<uint8_t>(i,j+1)-(int)Gray.at<uint8_t>(i,j-1));
            if (value1>value2)
            {
              Gradient.at<uint8_t>(i,j)= value1;
            }
            else
            {
              Gradient.at<uint8_t>(i,j)= value2;
            }
            //cout<<value1<<" ";
            //cout<<value2<<endl;
        }
    }
    int nominator=0;
    int denominator=0;
    for (int i=0; i < Gray.rows; i++)
    {
        for (int j=0; j < Gray.cols; j++)
        {
           nominator+=(int)Gray.at<uint8_t>(i,j)*(int)Gradient.at<uint8_t>(i,j);
        }
    }
    for (int i=0; i < Gray.rows; i++)
    {
        for (int j=0; j < Gray.cols; j++)
        {
           denominator+=(int)Gradient.at<uint8_t>(i,j);
        }
    }
    int threshold=nominator/denominator;
    return threshold;
}
