#include "kmeansopencv.h"

KMeansOpenCV::KMeansOpenCV()
{
}
//реализация kmeans c помощью opencv
Mat  KMeansOpenCV::getFrameOCV(Mat parFrame)
{

        Mat resultFrame;
        resultFrame.create(parFrame.rows, parFrame.cols,CV_8U);

        int k=5;//количество кластеров
        //cv::threshold(parFrame,parFrame,0,255,CV_THRESH_TRUNC|CV_THRESH_OTSU);
        CountThreshold threshold1;
        int t=threshold1.gradThreshold(parFrame);
        threshold(parFrame,parFrame,t,250,CV_THRESH_TRUNC);
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
