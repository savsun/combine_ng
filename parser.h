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

    double *xy=new double[2];

public:
    bool characters(const QString& _strText);
    bool endElement (const QString&, const QString&, const QString& str);
    bool fatalError (const QXmlParseException &exception);
    double* transform(double y, double x);
};

#endif // PARSER_H
