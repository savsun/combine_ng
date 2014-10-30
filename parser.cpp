#include "parser.h"

Parser::Parser()
{
    OGRSpatialReference src;
    OGRSpatialReference dst;
    src.importFromProj4("+no_defs +proj=longlat");
    dst.importFromProj4("+proj=tmerc +lat_0=0 +lon_0=39 +k=1 +x_0=7500000 +y_0=0 +ellps=krass +towgs84=23.92,-141.27,-80.9,-0,0.35,0.82,-0.12 +units=m +no_defs");

    result=OGRCreateCoordinateTransformation(&src,&dst);
}
bool Parser::characters(const QString& _strText)
{
   strText=_strText;
   return true;
}


bool Parser::endElement (const QString&, const QString&, const QString& str)
{
    if (str=="frame")
    {
        result->Transform(1,&frame["y"],&frame["x"],NULL);
        frames.push_back(frame);
    }
    else // str != frame
    if (str!="amv")
    {
      frame.insert(str.toStdString(),strText.toDouble());
    }
    return true;
}

bool Parser::fatalError (const QXmlParseException &exception)
{
    cout<<"Line: "<<exception.lineNumber()<<", Column: "<<exception.columnNumber()<<", Message: "<<exception.message().toStdString();
    return false;
}
