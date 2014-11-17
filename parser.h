#ifndef PARSER_H
#define PARSER_H
#include <all.h>

class Parser: public QXmlDefaultHandler
{
public:
    Parser();
    vector<QMap<string,double> > frames;
private:
    QString strText;
    QMap<string,double> frame;
    OGRCoordinateTransformation *result;

public:
    bool characters(const QString& _strText);
    bool endElement (const QString&, const QString&, const QString& str);
    bool fatalError (const QXmlParseException &exception);
};

#endif // PARSER_H
