#ifndef NUMBER_H
#define NUMBER_H

#include <QVector>
#include <qmath.h>
#include <QString>
#include <sstream>

class number
{
public:
    number();
    void setnum(double number_input);
    void setnumber(QString str);           //вввести число Qstring
    void setnumber(int number_input);      //ввсти число int
    double getfullnumber();                //получить число в типе double
    double getsizenum();                   //количество цифр в числе
    QString getsizenumstr();
    double sumnumber();
private:
    QVector<double> num;
};

#endif // NUMBER_H
