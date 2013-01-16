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
    void setnumber(QString str);           //������� ����� Qstring
    void setnumber(int number_input);      //����� ����� int
    double getfullnumber();                //�������� ����� � ���� double
    double getsizenum();                   //���������� ���� � �����
    QString getsizenumstr();
    double sumnumber();
private:
    QVector<double> num;
};

#endif // NUMBER_H
