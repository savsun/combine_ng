#include "parser.h"

Parser::Parser()
{
}
shared_ptr<double> Parser::transform(double y, double x)
{
    shared_ptr<double> coord(new double[2],std::default_delete<double[]>());
    OGRSpatialReference src;
    OGRSpatialReference dst;
    src.importFromProj4("+no_defs +proj=longlat");
    dst.importFromProj4("+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=7500000 +y_0=0 +ellps=krass +towgs84=23.92,-141.27,-80.9,-0,0.35,0.82,-0.12 +units=m +no_defs");

    OGRCoordinateTransformation *result=OGRCreateCoordinateTransformation(&src,&dst);
    if (result->Transform(1,&y,&x,NULL)==TRUE)
    {
        coord.get()[0]=y;
        coord.get()[1]=x;
    }
    return coord;
}

bool Parser::characters(const QString& _strText)
{
   strText=_strText;
   return true;
}


bool Parser::endElement (const QString&, const QString&, const QString& str)
{
    shared_ptr<double> xy;
    if (str=="frame")
    {
        xy=transform(frame["y"],frame["x"]);
        frame["y"]=xy.get()[0];
        frame["x"]=xy.get()[1];
        frames.push_back(frame);
    }
    if (str!="amv"&&str!="frame")
    {
      //cout<<str.toStdString()<<" "<<strText.toDouble()<<endl;
      frame.insert(str.toStdString(),strText.toDouble());
    }
    return true;
}

bool Parser::fatalError (const QXmlParseException &exception)
{
    cout<<"Line: "<<exception.lineNumber()<<", Column: "<<exception.columnNumber()<<", Message: "<<exception.message().toStdString();
    return false;
}
