#include "AddBooks.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
#include <QApplication>
#include <QSqlError>

//构建图书入库主界面
AddBooks::AddBooks()
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
    
    setWindowTitle("单本入库");
    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    bno = new QLineEdit;
    category = new QLineEdit;
    title = new QLineEdit;
    press = new QLineEdit;
    year = new QLineEdit;
    author = new QLineEdit;
    price = new QLineEdit;
    
    FormLayout->addRow("Bno: ", bno);
    FormLayout->addRow("Category: ", category);
    FormLayout->addRow("Title: ", title);
    FormLayout->addRow("Press: ", press);
    FormLayout->addRow("Year: ", year);
    FormLayout->addRow("Author: ", author);
    FormLayout->addRow("Price: ", price);
    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout* HBox = new QHBoxLayout;
    add = new QPushButton("Add");
    update = new QPushButton("Update");
    cancel = new QPushButton("Cancel");
    HBox->addWidget(add);
    HBox->addWidget(update);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 4);
    VBox->addLayout(HBox, 1);
    this->setLayout(VBox);
    //点击确认按钮，将书籍信息加入数据库
    QObject::connect(add, &QPushButton::clicked, this, &AddBooks::AddBook2DB);
    QObject::connect(update, &QPushButton::clicked, this, &AddBooks::UpdateBook);
    QObject::connect(cancel, &QPushButton::clicked, this, &AddBooks::close);
}

//将书籍信息加入数据库
void AddBooks::AddBook2DB()
{
    //先确保要输入所有信息，否则提示用户将信息填写完整
    if(
        this->bno->text() != "" &&
        //this->category->text() != "" &&
        this->title->text() != "" //&&
        //this->press->text() != "" &&
        //this->year->text() != "" &&
        //this->author->text() != "" &&
        //this->price->text() != "" 
    )
    {
        Book newbook;

        newbook.bno = this->bno->text();        
        newbook.category = this->category->text();
        newbook.author = this->author->text();
        newbook.press = this->press->text();
        newbook.price = this->price->text();
        newbook.title = this->title->text();
        newbook.year = this->year->text();

        double price = newbook.price.toDouble();
        int year = newbook.year.toInt();

        //先更新
        QString Bookstr = QString("select * from book where bno = '%1' and title = '%2'").arg(newbook.bno).arg(newbook.title);
        QSqlQuery Bookquery;
        Bookquery.exec(Bookstr);
        if(Bookquery.next()) //已经存在此书，加库存和总量或更新信息
        {
            qDebug() << "此书已存在";
            QString BookStock = QString("update book set stock = stock + 1 , total = total + 1 where bno = '%1'").arg(newbook.bno);
            QSqlQuery AddStock;
            AddStock.exec(BookStock);
            flushBookTable(newbook.bno);
            this->close();
            emitCloseBox();
        }
        else //不存在此书
        {
            if(
                this->category->text() != "" &&
                this->press->text() != "" &&
                this->year->text() != "" &&
                this->author->text() != "" &&
                this->price->text() != "" 
            )
            {
                QString str = QString("insert into book values('%1','%2','%3','%4',%5,'%6',%7,1,1);")
                .arg(newbook.bno,newbook.category)
                .arg(newbook.title,newbook.press)
                .arg(year).arg(newbook.author)
                .arg(price)
                ;

                QSqlQuery query;
                bool a = query.exec(str);
                qDebug() << query.lastError();
                QMessageBox::about(this, "Success", "图书入库成功!");
                flushBookTable(newbook.bno);
                this->close();
                emitCloseBox();
            }
            else
            {
                QMessageBox::warning(this,"提示","首次入库请将信息填写完整",QMessageBox::Ok);
            }
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "您至少需要输入bno和title以防输入错误", QMessageBox::Ok);
    }
}

void AddBooks::UpdateBook()
{
    //确保至少输入书籍号，否则提示用户将信息填写完整
    if(
        this->bno->text() != "" 
    )
    {
        Book newbook;

        newbook.bno = this->bno->text();        
        newbook.category = this->category->text();
        newbook.author = this->author->text();
        newbook.press = this->press->text();
        newbook.price = this->price->text();
        newbook.title = this->title->text();
        newbook.year = this->year->text();

        double price = newbook.price.toDouble();
        int year = newbook.year.toInt();

        QString str = QString("update book set bno = '%1'")
        .arg(newbook.bno)
        ;

        if(newbook.category != "") str += QString(", category = '%1'").arg(newbook.category);
        if(newbook.author != "") str += QString(", author = '%1'").arg(newbook.author);
        if(newbook.press != "") str += QString(", press = '%1'").arg(newbook.press);
        if(newbook.price != "") str += QString(", price = %1").arg(price);
        if(newbook.title != "") str += QString(", title = '%1'").arg(newbook.title);
        if(newbook.year != "") str += QString(", year = %1").arg(year);
        str += QString(" where bno = '%1'").arg(newbook.bno);

        QSqlQuery query;
        bool a;
        if(a = query.exec(str))
        {
            if(!query.numRowsAffected()) 
            {
                QMessageBox::warning(this, "提示", "信息更新未进行，不存在此书或与原信息相同", QMessageBox::Ok);
            }
            else
            {
                QMessageBox::about(this, "Success","书籍信息更新成功！");
                flushBookTable(newbook.bno);
                this->close();
                emitCloseBox();
            }
        }
        else if(!a)
        {
            qDebug() << query.lastError();
            QMessageBox::critical(this, "错误", "信息更新失败，输入有误", QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请指明要更新的书籍编号", QMessageBox::Ok);
    }

}

//成功借书，还书后刷新表单
void AddBooks::flushBookTable(QString bno)
{
    QString str = QString("select * from book where bno = '%1'")
        .arg(bno);

    QSqlQuery query;
    query.exec(str);
        
    //将查询结果写到文件中
    QFile file("D:/Qt/projects/learn/BookQuery.txt"); //一定要绝对路径？
    if(file.open(QIODevice::Text|QIODevice::WriteOnly)){
        qDebug()<<"文件打开成功";
    }

    QTextStream out(&file);
    while(query.next())	//循环输出
    {
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
    emitAddSuccess();
}

void AddBooks::emitCloseBox()
{
    emit FinishAdd();
}

void AddBooks::emitAddSuccess()
{
    emit AddSuccess();
}