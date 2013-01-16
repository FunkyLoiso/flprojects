#include "number.h"

number::number()
{
    //    number::num.resize(size);
}

void number::setnum(double number_input)
{
//    num.prepend();
}

void number::setnumber(QString str)
{
    num.resize(str.size());
//    num.clear();
    for (int i = 0 ;i<str.size();i++)
    {
        num[i]=(double)str[i].digitValue();
    }
}

void number::setnumber(int number_input)
{
//    std::stringstream ss;
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss<<number_input;
    QString str = str.fromStdString(ss.str());
    number::setnumber(str);
}

double number::getfullnumber()
{
    double tmp = 0;
    for (int i = 0 ,out=num.size()-1;i>=0;i++,out--)
    {
        tmp += num[i]*qPow(10,out);
    }
    return tmp;
}

double number::getsizenum()
{
    return number::num.size();
}

QString number::getsizenumstr()
{
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss<<(int)getfullnumber();
    QString stri = stri.fromStdString(ss.str());
    return stri;
}

double number::sumnumber()
{
    double sum;
    for (int i = 0;i<num.size();i++)
    {
        sum += num[i];
    }
    return sum;
}
