#ifndef _BOOK_H_
#define _BOOK_H_
#define bnoindex 0 
#define cateindex 1 
#define titleindex 2 
#define pressindex 3
#define yearindex 4
#define authorindex 5
#define priceindex 6
#define totalindex 7    
#define stockindex 8

#include<QString>

class Book
{
public:
    QString getbno() const;
    void setbno(const QString &value);

    QString getcategory() const;
    void setcategory(const QString &value);

    QString gettitle() const;
    void settitle(const QString &value);

    QString getpress() const;
    void setpress(const QString &value);

    QString getyear() const;
    void setyear(const QString &value);

    QString getauthor() const;
    void setauthor(const QString &value);

    QString getprice() const;
    void setprice(const QString &value); 

    QString gettotal() const;
    void settotal(const QString &value); 

    QString getstock() const;
    void setstock(const QString &value); 

public:
    QString bno; 
    QString category; 
    QString title; 
    QString press;
    QString year;
    QString lyear;
    QString uyear;
    QString author;
    QString price;
    QString lprice;
    QString uprice;
    QString total;    
    QString stock;

};


#endif