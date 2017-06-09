#include <iostream>
#include <string>

using namespace std;


double price = 0.0;
class Sales_data
{
    friend void check(const Sales_data&, const Sales_data&, const string);
    friend Sales_data operator-(const Sales_data& lhs, const Sales_data& rhs);  //减法运算符
    friend Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs);  //加法运算符
    friend istream& operator>>(istream&, Sales_data&);  //输入运算符
    friend ostream& operator<<(ostream&, const Sales_data&);    //输出运算符
public:
    Sales_data operator-=(const Sales_data&);
    Sales_data operator+=(const Sales_data&);
    Sales_data(size_t sz = 0, double r = 0.0, string b = "") : units_sold(sz), revenue(r), bookNo(b) { }
    string isbn() const { return this->bookNo; }
    double average_price() const { return revenue / units_sold; }

private:
    size_t units_sold;
    double revenue = 0.0;
    string bookNo;
};
void check(const Sales_data& lhs, const Sales_data& rhs, const string msg)
{
    if (lhs.bookNo != rhs.bookNo)
        throw std::runtime_error(msg);
}
//定义可以操作Sales_data的减法运算符（全局函数）; 同时 定义了Sales_data的成员函数Sales_data operator-=(const Sales_data&);
Sales_data operator-(const Sales_data& lhs, const Sales_data& rhs)
{
    Sales_data sub = lhs; //进行Sales_data的拷贝赋值 ： 调用Sales_data的拷贝构造函数 Sales_data(const Sales_data& r) : revenue(r.revenue), units_sold(r.units_sold) {}
    sub -= rhs; //调用Sales_data的operator-=(const Sales_data& r) 成员函数；
    return sub;
}

Sales_data Sales_data::operator-=(const Sales_data& lhs)
{
    this->units_sold -= lhs.units_sold;
    this->revenue -= lhs.revenue;
    return *this;
}

//定义可以操作Sales_data的加法运算符; 同时，定义了Sales_data的成员函数Sales_data operator+=(const Sales_data&)；
Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs)
{
    check(lhs, rhs, "不是同一种类的书籍不能相加！");
    Sales_data sub = lhs;   //将lhs逐成员拷贝给sub；即调用了Sales_data的拷贝构造函数Sales_data(const Sales_data &r) : units_sold(r.units_sold), revenue(r.revenue), bookNo(r.bookNo){}
    sub += rhs; //调用Sales_data的成员函数Sales_data operator+=(const Sales_data& lhs){units_sold += lhs->units_sold; revenue += lhs.revenue; return *this; }
    return sub;
}

Sales_data Sales_data::operator+=(const Sales_data& lhs)
{
    this->units_sold += lhs.units_sold;
    this->revenue += lhs.revenue;
    return *this;
}

//定义可以操作Sales_data输入运算符
istream& operator>>(istream& is, Sales_data& lhs)
{
    is >> lhs.bookNo >> lhs.units_sold >> price;
    if (is)
        lhs.revenue = lhs.units_sold * price;
    else
        lhs = Sales_data();
    return is;
}

//定义可以操作Sales_data的输出运算符
ostream& operator<<(ostream& os, const Sales_data& lhs)
{
    os << lhs.isbn() << " " << lhs.units_sold << " " << lhs.revenue << " " << lhs.average_price();
    return os;
}
int main()
{
    Sales_data r(2, 3, "BJ-010-999999");
    Sales_data rr(10, 100, "SH-021-666666");
    try
    {
        r + rr;
    }
    catch(std::runtime_error& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}
