#include <iostream>

using namespace std;

class out_of_stock : public std::runtime_error
{
public:
    explicit out_of_stock(const string &s) : runtime_error(s){}
};
class isbn_mismatch : public logic_error
{
public:
    explicit isbn_mismatch(const std::string& s) : std::logic_error(s) {}

    isbn_mismatch(const string& s, const string& lhs, const string& rhs) : std::logic_error(s), left(lhs), right(rhs) {}

    const string left, right;
};

double price = 0.0;
class Sales_data
{
    friend Sales_data operator-(const Sales_data&, const Sales_data&);
    friend Sales_data operator+(const Sales_data&, const Sales_data&);
    friend istream& operator>>(istream&, Sales_data&);
    friend ostream& operator<<(ostream&, Sales_data&);
public:
    Sales_data operator-=(const Sales_data&);
    Sales_data operator+=(const Sales_data&);
    Sales_data(const string book = "", size_t sz = 0, double r = 0.0) : bookNo(book), units_sold(sz), revenue(r){}
    string isbn() const { return this->bookNo; }
    double average_price() const { return revenue / units_sold; }

private:
    size_t units_sold;
    string bookNo;
    double revenue = 0.0;
};
void check(const Sales_data&, const Sales_data&, const string&);
Sales_data Sales_data::operator-=(const Sales_data& lhs)
{
   // if (isbn() != lhs.isbn())
   //     throw std::runtime_error("不是同一种书籍不能相减！");
    this->units_sold -= lhs.units_sold;
    this->revenue -= lhs.revenue;
    return *this;
}
Sales_data Sales_data::operator+=(const Sales_data& lhs)
{
   // if (isbn() != lhs.isbn())
    //    throw std::runtime_error("不是同一种书籍不能相加！");
    units_sold += lhs.units_sold;
    revenue += lhs.revenue;
    return *this;
}

Sales_data operator-(const Sales_data& lhs, const Sales_data& rhs)
{
    check(lhs, rhs, "不是同一种书籍不能相减！");
    Sales_data sub = lhs;
    sub -= rhs;
    return sub;
}

Sales_data operator+(const Sales_data& lhs, const Sales_data& rhs)
{
    check(lhs, rhs, "不是同一种书籍不能相加！");
    Sales_data sub = lhs;
    sub += rhs;
    return sub;
}

istream& operator>>(istream& is, Sales_data& lhs)
{
    is >> lhs.bookNo >> lhs.units_sold >> price;
    if (is)
        lhs.revenue = lhs.units_sold * price;
    else
        lhs = Sales_data();
    return is;
}

ostream& operator<<(ostream& os, Sales_data& lhs)
{
    os << lhs.bookNo << " " << lhs.units_sold << " " << lhs.revenue << " " << lhs.average_price();
    return os;
}

void check(const Sales_data& lhs, const Sales_data& rhs, const string& msg)
{
    if (lhs.isbn() != rhs.isbn())
        throw isbn_mismatch(msg, lhs.isbn(), rhs.isbn());
}


int main()
{
    Sales_data s1("SH-021-66666", 10, 88);
    Sales_data s2("SH-021-88888", 20, 66);
    Sales_data s3("BJ-010-99999", 30, 99);
    cin >> s2;
    try
    {
        Sales_data s = s1 + s2;
        cout << s << endl;
    }
    catch(isbn_mismatch& e)
    {
        cout << e.what() <<endl;
    }

    return 0;
}
