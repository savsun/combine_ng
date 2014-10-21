#include "kmeans.h"

Kmeans::Kmeans()
{
}
Mat  Kmeans::getFrame(Mat parFrame)
{

        Mat resultFrame;
        resultFrame.create(parFrame.rows, parFrame.cols,CV_8U);

        int k=5;//количество кластеров

        int n = parFrame.rows*parFrame.cols;
        Mat img3xN(n,3,CV_8U);
        vector<Mat> imgRGB;

              split(parFrame,imgRGB);
              for(int p=0;p!=3;++p)
                 imgRGB[p].reshape(1,n).copyTo(img3xN.col(p));
              img3xN.convertTo(img3xN,CV_32F);
              Mat process;
              kmeans(img3xN,k,process,TermCriteria(),4,KMEANS_RANDOM_CENTERS);
              process= process.reshape(0,parFrame.rows);
              convertScaleAbs(process,process,int(255/k));
              process.copyTo(resultFrame);
        return resultFrame;
}
