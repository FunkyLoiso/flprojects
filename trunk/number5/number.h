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
    number(int num);
    void setnum(int  number_input);
    void setnumber(QString str);           //вввести число Qstring
    void setnumber(int number_input);      //ввсти число int
    int  getfullnumber();                //получить число в типе int
    int  getsizenum();                   //количество цифр в числе
    QString getnumstr();
    int  sumnumber();
private:
    QVector<int> num;
};

#endif // NUMBER_H
