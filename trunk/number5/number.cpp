#include "number.h"

number::number()
{
}

number::number(int num)
{
    //    number::num.resize(size);
    number::setnumber(num);
}

void number::setnum(int number_input)
{
    num.append(number_input);
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

int number::getfullnumber()
{
    int  tmp = 0;
    for (int i = 0 ,out=num.size()-1;num.size()>i;i++,out--)
    {
        tmp += num[i]*qPow(10,out);
    }
    return tmp;
}

int number::getsizenum()
{
    return number::num.size();
}

QString number::getnumstr()
{
    std::stringstream ss (std::stringstream::in | std::stringstream::out);
    ss<<(int)getfullnumber();
    QString stri = stri.fromStdString(ss.str());
    return stri;
}

int number::sumnumber()
{
    int  sum=0;
    for (int i = 0;i<num.size();i++)
    {
        sum += num[i];
    }
    return sum;
}
