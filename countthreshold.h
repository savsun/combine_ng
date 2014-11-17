#ifndef COUNTTHRESHOLD_H
#define COUNTTHRESHOLD_H
#include <all.h>

class CountThreshold
{
public:
    CountThreshold();
    int  otsuThreshold(Mat parFrame);
    int  gradThreshold(Mat parFrame);
};

#endif // COUNTTHRESHOLD_H
