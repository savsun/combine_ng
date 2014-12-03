#ifndef KMEANSOPENCV_H
#define KMEANSOPENCV_H
#include <all.h>
#include "countthreshold.h"

class KMeansOpenCV
{
public:
    KMeansOpenCV();
    Mat getFrameOCV (Mat parframe);
};

#endif // KMEANSOPENCV_H
