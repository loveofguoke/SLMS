#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
#include <QApplication>
#include <QSqlError>
#include<iostream>
#include"BRBook.h"

//构建借还书主界面
BRBook::BRBook()
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
    
    setWindowTitle("借/还书/借阅信息查询");
    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    cno = new QLineEdit;
    bno = new QLineEdit;
    
    FormLayout->addRow("Cno: ", cno);
    FormLayout->addRow("Bno: ", bno);

    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout* HBox = new QHBoxLayout;
    query = new QPushButton("Query");
    borrow = new QPushButton("Borrow");
    returnbook = new QPushButton("Return");
    cancel = new QPushButton("Cancel");
    HBox->addWidget(query);
    HBox->addWidget(borrow);
    HBox->addWidget(returnbook);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 4);
    VBox->addLayout(HBox, 1);
    this->setLayout(VBox);
    //点击确认按钮，将书籍信息加入数据库
    QObject::connect(query, &QPushButton::clicked, this, &BRBook::BorrowBookQuery); //检查是不是已经有了，没有就添加，否则提示已存在，并建议采用修改操作
    QObject::connect(borrow, &QPushButton::clicked, this, &BRBook::BorrowBook); //检查是不是已经有了，有就修改，否则提示不存在，建议采用添加操作
    QObject::connect(returnbook, &QPushButton::clicked, this, &BRBook::ReturnBook); //检查是不是已经有了，有就删除，否则提示不存在此card
    QObject::connect(cancel, &QPushButton::clicked, this, &BRBook::close);
}

//借书查询
void BRBook::BorrowBookQuery()
{
    //先确保输入借书证编号
    if(
        this->cno->text() != "" 
    )
    {
        Borrow newborrow;

        newborrow.cno = this->cno->text();        

        QString cardstr = QString("select * from card where cno = '%1'")
        .arg(newborrow.cno);
        QSqlQuery cardquery;
        cardquery.exec(cardstr);

        QString str = QString("select * from borrow where cno = '%1'")
        .arg(newborrow.cno)
        ;

        QSqlQuery query;
        bool a;
        if(a = query.exec(str))
        {
            if(!cardquery.next()) 
            {
                QMessageBox::warning(this, "提示", "不存在此借书证", QMessageBox::Ok);
            }
            else
            {
                //将查询结果写到文件中
                QFile file(".../BorrowQuery.txt"); // 修改为合适的路径
                if(file.open(QIODevice::Text|QIODevice::WriteOnly)){
                    qDebug()<<"文件打开成功";
                }

                QTextStream out(&file);

                while(query.next())	//循环输出
                {
                    out << query.value(0).toString() << ","
                    <<query.value(1).toString() << ","
                    <<query.value(2).toString() << ","
                    <<query.value(3).toString() << "\n";
                }
                file.close();
                this->close();
                emitQuerySuccess();
            }
        }
        else
        {
            qDebug() << query.lastError();
            QMessageBox::critical(this, "错误", "查询失败，输入有误", QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请填写借书证编号", QMessageBox::Ok);
    }
}

//借书
void BRBook::BorrowBook()
{
    //先确保输入借书证编号和书籍编号
    if(
        this->cno->text() != "" &&
        this->bno->text() != ""
    )
    {
        Borrow newborrow;

        newborrow.cno = this->cno->text();
        newborrow.bno = this->bno->text();

        QString cardstr = QString("select * from card where cno = '%1'")
        .arg(newborrow.cno);
        QSqlQuery cardquery;
        cardquery.exec(cardstr);

        QString bookstr = QString("select * from book where bno = '%1'")
        .arg(newborrow.bno);
        QSqlQuery bookquery;
        bookquery.exec(bookstr);

        bool findbook = false;

        if(!cardquery.next()) 
        {
            QMessageBox::warning(this, "提示", "请输入正确的借书证", QMessageBox::Ok);
        }
        else
        {
            while(bookquery.next())
            {
                findbook = true;
                qDebug() << "in here";
                //看看此书有无库存，有的话借书成功，库存减一，添加借书信息，没有的话，告知该书无库存，即最近归还时间(对borrow进行查询)
                if(bookquery.value(8).toInt() >= 1)
                {

                    QDate current_date = QDate::currentDate();
                    QString currentdate = current_date.toString("yyyyMMdd");
                    qDebug() << "current date " << currentdate;
                    int borrow_date = currentdate.toInt();
                    int return_date = current_date.addDays(30).toString("yyyyMMdd").toInt(); //借一个月

                    QString borrowstr = QString("insert into borrow values('%1', '%2', %3, %4)")
                    .arg(newborrow.cno).arg(newborrow.bno).arg(borrow_date).arg(return_date);
                    QSqlQuery addborrow;
                    bool a = addborrow.exec(borrowstr);
                    qDebug() << "addborrow: " << addborrow.lastError();
                    
                    if(a) //确保添加借书记录成功后再减库存
                    {
                        QString stockstr = QString("update book set stock = stock - 1 where bno = '%1'").arg(newborrow.bno);

                        QSqlQuery minusstock;
                        minusstock.exec(stockstr);
                        qDebug() << minusstock.lastError();

                        QMessageBox::about(this, "Success", "借书成功");
                        flushBorrowTable(newborrow.cno);
                        this->close();
                        emitCloseBox();
                    }
                    else
                    {
                        QMessageBox::warning(this, "提示", "请先归还此书再续借",QMessageBox::Ok);
                    }
                    
                }
                else //没库存了
                {
                    QString retdatestr = QString("select min(return_date) from borrow where bno = '%1'")
                    .arg(newborrow.bno);
                    QSqlQuery retdate;
                    retdate.exec(retdatestr);
                    qDebug() << "retdate: " << retdate.lastError();
                    QString mess;
                    if(retdate.first()) //一开始停在第一条记录前，通过调用first来使其定位到第一条记录
                    {
                        mess = "此书暂无库存,最近归还时间为: " + retdate.value(0).toString(); 
                    } 
                    QMessageBox::warning(this, "提示", mess,QMessageBox::Ok);
                    this->close();
                    emitCloseBox();
                } 
            }
            if(!findbook)
            {
                QMessageBox::warning(this, "提示", "库中暂无此书", QMessageBox::Ok);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请将借书证编号和书籍编号填写完整", QMessageBox::Ok);
    }
}

//还书
void BRBook::ReturnBook()
{
    //先确保输入借书证编号和书籍编号
    if(
        this->cno->text() != "" &&
        this->bno->text() != ""
    )
    {
        Borrow newborrow;

        newborrow.cno = this->cno->text();
        newborrow.bno = this->bno->text();

        QString cardstr = QString("select * from card where cno = '%1'")
        .arg(newborrow.cno);
        QSqlQuery cardquery;
        cardquery.exec(cardstr);

        QString bookstr = QString("select * from book where bno = '%1'")
        .arg(newborrow.bno);
        QSqlQuery bookquery;
        bookquery.exec(bookstr);

        bool findbook = false;

        if(!cardquery.next()) 
        {
            QMessageBox::warning(this, "提示", "请输入正确的借书证", QMessageBox::Ok);
        }
        else
        {
            if(bookquery.next())
            {
                findbook = true;
                qDebug() << "in here";
                //看看有没有借此书，有的话就归还，库存加一，否则输出错误信息
                QString Isborrowstr = QString("delete from borrow where cno = '%1' and bno = '%2'")
                .arg(newborrow.cno).arg(newborrow.bno);

                QSqlQuery Isborrow;
                Isborrow.exec(Isborrowstr);
                qDebug() << "Isborrow: " << Isborrow.lastError();

                if(Isborrow.numRowsAffected()) //借了
                {   
                    QString stockstr = QString("update book set stock = stock + 1 where bno = '%1'").arg(newborrow.bno);

                    QSqlQuery plusstock;
                    plusstock.exec(stockstr);
                    qDebug() << plusstock.lastError();

                    QMessageBox::about(this, "Success", "还书成功");
                    flushBorrowTable(newborrow.cno);
                    this->close();
                    emitCloseBox();

                }
                else //当前没借这本书
                {
                    QMessageBox::critical(this, "错误", "还书错误,您并没有借这本书",QMessageBox::Ok);
                    this->close();
                    emitCloseBox();
                } 
            }
            if(!findbook)
            {
                QMessageBox::warning(this, "提示", "请输入正确的书籍编号", QMessageBox::Ok);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请将借书证编号和书籍编号填写完整", QMessageBox::Ok);
    }
}

//成功借书，还书后刷新表单
void BRBook::flushBorrowTable(QString cno)
{
    QString str = QString("select * from borrow where cno = '%1'")
        .arg(cno);

        QSqlQuery query;
        query.exec(str);
            
        //将查询结果写到文件中
        QFile file(".../BorrowQuery.txt"); // 修改为合适的路径
        if(file.open(QIODevice::Text|QIODevice::WriteOnly)){
            qDebug()<<"文件打开成功";
        }

        QTextStream out(&file);
        while(query.next())	//循环输出
        {
            out << query.value(0).toString() << ","
            <<query.value(1).toString() << ","
            <<query.value(2).toString() << ","
            <<query.value(3).toString() << "\n";
        }
        file.close();
        emitQuerySuccess();
}

void BRBook::emitCloseBox()
{
    emit FinishBR();
}

void BRBook::emitQuerySuccess()
{
    emit QuerySuccess();
}