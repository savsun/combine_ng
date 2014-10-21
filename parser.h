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

public:
    bool characters(const QString& _strText);
    bool endElement (const QString&, const QString&, const QString& str);
    bool fatalError (const QXmlParseException &exception);
    shared_ptr<double> transform(double y, double x);
};

#endif // PARSER_H
