#ifndef KOHONEN_H
#define KOHONEN_H
#include <all.h>

class Kohonen
{
public:
    Kohonen();
    Mat  getFrame(Mat parFrame);

    QMap<unsigned,vector<Point> > collection_clusters;
};

#endif // KOHONEN_H
