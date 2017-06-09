#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
using namespace std;


double price = 0.0;
class Sales_data
{
    //friend bool compareIsbn(const Sales_data&, const Sales_data&);
    friend void check(const Sales_data&, const Sales_data&, const string);
    //friend Sales_data operator-(const Sales_data& lhs, const Sales_data& rhs);  //减法运算符
    //friend Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs);  //加法运算符
    friend istream& operator>>(istream&, Sales_data&);  //输入运算符
    friend ostream& operator<<(ostream&, const Sales_data&);    //输出运算符
public:
    Sales_data operator-=(const Sales_data&);
    Sales_data operator+=(const Sales_data&);
    Sales_data() = default;
    Sales_data(string b, size_t sz = 0, double r = 0.0) : units_sold(sz), revenue(r), bookNo(b) { }
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

//实现compareIsbn全局函数
bool compareIsbn(const Sales_data& lhs, const Sales_data& rhs)
{
    return lhs.isbn() < rhs.isbn();
}

typedef tuple<vector<Sales_data>::size_type,
                vector<Sales_data>::const_iterator,
                vector<Sales_data>::const_iterator> matches;
vector<matches> findBook(const vector<vector<Sales_data>> &files, const string& book)
{
    vector<matches> ret;
    for (vector<vector<Sales_data>>::const_iterator it = files.cbegin(); it != files.cend(); ++it)
    {
        pair<vector<Sales_data>::const_iterator, vector<Sales_data>::const_iterator> found = equal_range(it->cbegin(), it->cend(), Sales_data(book), &compareIsbn); //使用标准库算法equal_range()（函数模版）
        if (found.first != found.second)
            ret.push_back(make_tuple(it - files.cbegin(), found.first, found.second));
    }
    return ret;
}

void reportResults(istream& is, ostream& os, const vector<vector<Sales_data>> &files)
{
    string s;
    while (is >> s)
    {
        vector<matches> res = findBook(files, s);
        if (res.empty())
        {
            cout << "没有找到该类书籍" << endl;
            continue;
        }
        for (const auto &rr : res)
            os << "store " << get<0>(rr) << "sales " << accumulate(get<1>(rr), get<2>(rr), Sales_data(s)) << endl;;
    }
}

int main()
{
    Sales_data s1("BJ-010-99999", 10, 50);
    Sales_data s2("BJ-010-99999", 20, 80);
    Sales_data s3("BJ-010-88888", 30, 300);
    Sales_data s4("BJ-010-88888", 60, 600);
    Sales_data s5("BJ-010-88888", 90, 800);
    Sales_data s6("SH-021-66666", 40, 120);
    Sales_data s7("SH-021-66666", 50, 150);
    Sales_data s8("SH-021-88888", 50, 150);
    Sales_data s9("SH-021-88888", 50, 150);
    vector<Sales_data> v1 = {s1, s2, s3, s4, s5, s6, s7};
    vector<Sales_data> v2 = {s1, s3, s5, s7};
    vector<Sales_data> v3 = {s2, s4, s6, s7};
    vector<vector<Sales_data>> vv{v1, v2, v3};

    reportResults(cin, cout, vv);

    return 0;
}
