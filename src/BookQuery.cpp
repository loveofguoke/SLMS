#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
#include <QApplication>
#include <QSqlError>
#include"BookQuery.h"
#include<iostream>

//构建图书入库主界面
BookQuery::BookQuery()
{
    /*
     * QVBoxLayout 局部工具中放置 QFormLayout 和 QHBoxLayout
     * QFormLayout 中放置多个单行输入框
     * QHBoxLayout 中放置两个按钮
     * QVBoxLayout: 垂直布局
     * QFormLayout: 表单布局
     * QHBoxLayout: 水平布局
     *  
    */
    
    setWindowTitle("图书查询");
    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    bno = new QLineEdit;
    category = new QLineEdit;
    title = new QLineEdit;
    press = new QLineEdit;
    lyear = new QLineEdit;
    uyear = new QLineEdit;
    author = new QLineEdit;
    lprice = new QLineEdit;
    uprice = new QLineEdit;

    
    FormLayout->addRow("Bno: ", bno);
    FormLayout->addRow("Category: ", category);
    FormLayout->addRow("Title: ", title);
    FormLayout->addRow("Press: ", press);
    //FormLayout->addRow("Year: ", year);
    QGridLayout* Year = new QGridLayout;
    Year->addWidget(lyear,0,0);
    Year->addWidget(uyear,0,1);
    FormLayout->addRow("Year: ", Year);

    FormLayout->addRow("Author: ", author);
    //FormLayout->addRow("Price: ", price);
    QGridLayout* Price = new QGridLayout;
    Price->addWidget(lprice,0,0);
    Price->addWidget(uprice,0,1);
    FormLayout->addRow("Price: ", Price);

    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout* HBox = new QHBoxLayout;
    confirm = new QPushButton("Confirm");
    cancel = new QPushButton("Cancel");
    HBox->addWidget(confirm);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 4);
    VBox->addLayout(HBox, 1);
    this->setLayout(VBox);
    //点击确认按钮，将书籍信息加入数据库
    QObject::connect(confirm, &QPushButton::clicked, this, &BookQuery::QueryBookfDB);
    QObject::connect(cancel, &QPushButton::clicked, this, &BookQuery::close);
}

//将书籍信息加入数据库
void BookQuery::QueryBookfDB()
{
    //先确保要输入所有信息，否则提示用户将信息填写完整
    if(
        this->bno->text() != "" ||
        this->category->text() != "" ||
        this->title->text() != "" ||
        this->press->text() != "" ||
        this->lyear->text() != "" ||
        this->uyear->text() != "" ||
        this->author->text() != "" ||
        this->lprice->text() != "" ||
        this->uprice->text() != "" 
    )
    {
        Book newbook;

        newbook.bno = this->bno->text();       
        newbook.category = this->category->text(); 
        newbook.author = this->author->text(); 
        newbook.press = this->press->text(); 
        newbook.lprice = this->lprice->text();
        newbook.uprice = this->uprice->text();
        newbook.title = this->title->text();
        newbook.lyear = this->lyear->text();
        newbook.uyear = this->uyear->text();

        double lprice = newbook.lprice.toDouble();
        double uprice = newbook.uprice.toDouble();
        int lyear = newbook.lyear.toInt();
        int uyear = newbook.uyear.toInt();

        QString str = QString("select* from book where ( '%1' = '' or bno = '%1' ) and ( '%2' = '' or category = '%2' ) and ( '%3' = '' or title = '%3' ) "
        "and ( '%4' = '' or press = '%4' ) "
        "and ((%5 = 0 and %6 = 0) or (%5 = 0 and year <= %6) or (%6 = 0 and year >= %5) or (year >= %5 and year <= %6)) "
        "and ( '%7' = '' or author = '%7' ) "
        "and ((%8 = 0 and %9 = 0) or (%8 = 0 and price <= %9) or (%9 = 0 and price >= %8) or (price >= %8 and price <= %9))")
        .arg(newbook.bno,newbook.category)
        .arg(newbook.title,newbook.press)
        .arg(lyear).arg(uyear).arg(newbook.author)
        .arg(lprice).arg(uprice)
        ;

        //将查询得到的结果写到文件中
        QFile file(".../BookQuery.txt"); // 修改为合适的路径
        if(file.open(QIODevice::Text|QIODevice::WriteOnly)){
            qDebug()<<"文件打开成功";
        }

        QTextStream out(&file);

        QSqlQuery query;
        if(query.exec(str))
        {
            qDebug()<<"Search Sucess!";
            while(query.next())	//循环输出
            {
                qDebug() << "in ";
                out<<query.value(0).toString() << ","
                        <<query.value(1).toString() << ","
                        <<query.value(2).toString() << ","
                        <<query.value(3).toString() << ","
                        <<query.value(4).toString() << ","
                        <<query.value(5).toString() << ","
                        <<query.value(6).toString() << ","
                        <<query.value(7).toString() << ","
                        <<query.value(8).toString() <<"\n";
            }
            file.close();
        }
        else
        {
            qDebug() << "Query Failed!" << query.lastError().text();
        }

        this->close();
        emitCloseBox();
    }
    else
    {
        QMessageBox::warning(this, "提示", "请输入至少一项查询信息", QMessageBox::Ok);
    }
}

void BookQuery::emitCloseBox()
{
    emit FinishQuery();
}