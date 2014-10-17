#ifndef TEXTURE_H
#define TEXTURE_H
#include <all.h>
#include <QString>
class Texture
{
     //OGREnvelope env;
     QString _filenameMap;
     OGRLayer * system;
public:
    OGREnvelope env;
    Texture(QString filenameMap);
    void get( int countTexture, int dimention);
    void transformGCP(double point[2], int minXPixel, int minYPixel, int maxXPixel, int maxYPixel);
};

#endif // TEXTURE_H
