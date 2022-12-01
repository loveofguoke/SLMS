#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
#include <QApplication>
#include <QSqlError>
#include "BatchAdd.h"

//构建图书入库主界面
BatchAdd::BatchAdd()
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
    
    setWindowTitle("批量入库");
    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    tips = new QLabel("请输入本文链接");
    url = new QLineEdit; 
    
    FormLayout->addRow(tips);
    FormLayout->addRow("Url: ", url);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout* HBox = new QHBoxLayout;
    confirm = new QPushButton("Confirm");
    cancel = new QPushButton("Cancel");
    HBox->addWidget(confirm);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 4);
    VBox->addLayout(HBox, 1);
    this->setLayout(VBox);
    //点击确认按钮，将所给文本中书籍信息批量加入数据库
    QObject::connect(confirm, &QPushButton::clicked, this, &BatchAdd::AddBook2DB);
    QObject::connect(cancel, &QPushButton::clicked, this, &BatchAdd::close);
}

//将书籍信息加入数据库
void BatchAdd::AddBook2DB()
{
    //先确保要输入所有信息，否则提示用户将信息填写完整
    if(
        this->url->text() != ""
    )
    {
        QString txt_url = this->url->text();

        QFile file(txt_url);
        bool a = file.open(QIODevice::ReadOnly);
        qDebug() << "Now Addbooks: " << a;
        QTextStream dataStr(&file);
        QString line;
        QStringList bookinf;

        while(!dataStr.atEnd()){
            //通过readline解决读入回车的问题
            line = dataStr.readLine();
            bookinf = line.split(",");

            qDebug() << bookinf.at(0);

            double price = bookinf.at(priceindex).toDouble();
            int year = bookinf.at(yearindex).toInt();
            int amount = bookinf.at(7).toInt();

            //先看看有没有
            QString Bookstr = QString("select * from book where bno = '%1'").arg(bookinf.at(bnoindex));
            QSqlQuery Bookquery;
            Bookquery.exec(Bookstr);
            if(Bookquery.next()) //已经存在此书，加库存和总量
            {
                QString BookStock = QString("update book set stock = stock + %2 , total = total + %2 where bno = '%1'").arg(bookinf.at(bnoindex)).arg(amount);
                QSqlQuery AddStock;
                AddStock.exec(BookStock);
            }
            else //不存在此书,加此书
            {
                QString str = QString("insert into book values('%1','%2','%3','%4',%5,'%6',%7,%8,%8);")
                .arg(bookinf.at(bnoindex),bookinf.at(cateindex))
                .arg(bookinf.at(titleindex),bookinf.at(pressindex))
                .arg(year).arg(bookinf.at(authorindex))
                .arg(price).arg(amount)
                ;

                QSqlQuery query;
                bool a = query.exec(str);
                qDebug() << query.lastError();
            }
        }

        file.close();
        this->close();
        flushBookTable(txt_url);
        emitCloseBox();
    }
    else
    {
        QMessageBox::warning(this, "提示", "请输入文本链接", QMessageBox::Ok);
    }
}

void BatchAdd::flushBookTable(QString url)
{
    QFile file(url);
    file.open(QIODevice::ReadOnly);
    QTextStream dataStr(&file);
    QString line;
    QStringList bookinf;

    QFile outfile(".../BookQuery.txt"); // 修改为合适的路径
        if(outfile.open(QIODevice::Text|QIODevice::WriteOnly)){
            qDebug()<<"文件打开成功";
        }

    QTextStream out(&outfile);

    while(!dataStr.atEnd()){
        //通过readline解决读入回车的问题
        line = dataStr.readLine();
        bookinf = line.split(",");

        QString Bookstr = QString("select * from book where bno = '%1'").arg(bookinf.at(bnoindex));
        QSqlQuery Bookquery;
        Bookquery.exec(Bookstr);

        //将查询得到的结果写到文件中
        qDebug() << "in ";
        if(Bookquery.next())
        {
            out<<Bookquery.value(0).toString() << ","
                <<Bookquery.value(1).toString() << ","
                <<Bookquery.value(2).toString() << ","
                <<Bookquery.value(3).toString() << ","
                <<Bookquery.value(4).toString() << ","
                <<Bookquery.value(5).toString() << ","
                <<Bookquery.value(6).toString() << ","
                <<Bookquery.value(7).toString() << ","
                <<Bookquery.value(8).toString() <<"\n";
        }
        
    }
    file.close();
    outfile.close();    
    emitAddSuccess();
}

void BatchAdd::emitAddSuccess()
{
    emit AddSuccess();
} 

void BatchAdd::emitCloseBox()
{
    emit FinishAdd();
}