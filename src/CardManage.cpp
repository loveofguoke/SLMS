#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
#include <QApplication>
#include <QSqlError>
#include"CardManage.h"
#include<iostream>

//构建图书入库主界面
CardManage::CardManage(bool IsQuery) : IsQuery(IsQuery) 
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
    
    setWindowTitle("借书证管理");
    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    cno = new QLineEdit;
    name = new QLineEdit;
    department = new QLineEdit;
    type = new QLineEdit;
    
    FormLayout->addRow("Cno: ", cno);
    FormLayout->addRow("Name: ", name);
    FormLayout->addRow("Department: ", department);
    FormLayout->addRow("Type(S/T): ", type);

    FormLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

    QHBoxLayout* HBox = new QHBoxLayout;
    if(IsQuery)
    {
        query = new QPushButton("Query");
        HBox->addWidget(query);
        QObject::connect(query, &QPushButton::clicked, this, &CardManage::CardQuery);
    }
    else
    {
        add = new QPushButton("Add");
        update = new QPushButton("Update");
        deletecard = new QPushButton("Delete");
        HBox->addWidget(add);
        HBox->addWidget(update);
        HBox->addWidget(deletecard);
        QObject::connect(add, &QPushButton::clicked, this, &CardManage::AddCard2DB); //检查是不是已经有了，没有就添加，否则提示已存在，并建议采用修改操作
        QObject::connect(update, &QPushButton::clicked, this, &CardManage::UpdateCardinDB); //检查是不是已经有了，有就修改，否则提示不存在，建议采用添加操作
        QObject::connect(deletecard, &QPushButton::clicked, this, &CardManage::DeleteCardfDB); //检查是不是已经有了，有就删除，否则提示不存在此card
    }

    cancel = new QPushButton("Cancel");
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 4);
    VBox->addLayout(HBox, 1);
    this->setLayout(VBox);
    //点击确认按钮，将书籍信息加入数据库
    QObject::connect(cancel, &QPushButton::clicked, this, &CardManage::close);
}

//Add Card
void CardManage::AddCard2DB()
{
    //先确保要输入所有信息，否则提示用户将信息填写完整
    if(
        this->cno->text() != "" &&
        this->name->text() != "" &&
        this->department->text() != "" &&
        this->type->text() != "" 
    )
    {
        Card newcard;

        newcard.cno = this->cno->text();        
        newcard.name = this->name->text();
        newcard.department = this->department->text();
        newcard.type = this->type->text();

        QString str = QString("insert into card values('%1','%2','%3','%4');")
        .arg(newcard.cno,newcard.name)
        .arg(newcard.department,newcard.type)
        ;

        QSqlQuery query;
        bool a = query.exec(str);
        qDebug() << query.lastError();
        if(!a)
        {
            QMessageBox::critical(this, "错误", "借书证添加失败，输入有误或借书证已存在", QMessageBox::Ok);
        }
        else
        {
            QMessageBox::about(this, "Success","借书证添加成功！");
            flushCardTable(newcard.cno);
            this->close();
            emitCloseBox();
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请将信息填写完整", QMessageBox::Ok);
    }

}

//Update Card，要确保读书证已存在
void CardManage::UpdateCardinDB()
{
    //确保至少输入卡号，否则提示用户将信息填写完整
    if(
        this->cno->text() != "" 
    )
    {
        Card newcard;

        newcard.cno = this->cno->text();        
        newcard.name = this->name->text();
        newcard.department = this->department->text();
        newcard.type = this->type->text();

        QString str = QString("update card set cno = '%1'")
        .arg(newcard.cno)
        ;

        if(newcard.name != "") str += QString(", name = '%1'").arg(newcard.name);
        if(newcard.department != "") str += QString(", department = '%1'").arg(newcard.department);
        if(newcard.type != "") str += QString(", type = '%1'").arg(newcard.type);
        str += QString(" where cno = '%1'").arg(newcard.cno);

        QSqlQuery query;
        bool a;
        if(a = query.exec(str))
        {
            if(!query.numRowsAffected()) 
            {
                QMessageBox::warning(this, "提示", "信息更新未进行，不存在此借书证或与原信息相同", QMessageBox::Ok);
            }
            else
            {
                QMessageBox::about(this, "Success","借书证信息更新成功！");
                flushCardTable(newcard.cno);
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
        QMessageBox::warning(this, "提示", "请指明要更新的借书证编号", QMessageBox::Ok);
    }
}

//Delete Card
void CardManage::DeleteCardfDB()
{
    //删除时只关心卡号
    if(
        this->cno->text() != "" 
    )
    {
        Card newcard;

        newcard.cno = this->cno->text();        

        QString str = QString("delete from card where cno = '%1'")
        .arg(newcard.cno)
        ;

        QSqlQuery query;
        bool a;
        if(a = query.exec(str))
        {
            if(!query.numRowsAffected()) 
            {
                QMessageBox::warning(this, "提示", "删除未进行，不存在此借书证", QMessageBox::Ok);
            }
            else
            {
                QMessageBox::about(this, "Success","借书证删除成功！");
                flushCardTable(newcard.cno);
                this->close();
                emitCloseBox();
            }
        }
        else if(!a)
        {
            qDebug() << query.lastError();
            QMessageBox::critical(this, "错误", "删除失败，有未归还的图书", QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请指明要删除的借书证编号", QMessageBox::Ok);
    }

}

void CardManage::flushCardTable(QString cno)
{
    QString str = QString("select * from card where cno = '%1'")
        .arg(cno);

    QSqlQuery query;
    query.exec(str);
        
    //将查询结果写到文件中
    QFile file(".../CardQuery.txt"); // 修改为合适的路径
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

void CardManage::CardQuery()
{   
    Card newcard;

    newcard.cno = this->cno->text();        
    newcard.name = this->name->text();
    newcard.department = this->department->text();
    newcard.type = this->type->text();
    
    QString str = QString("select* from card where ( '%1' = '' or cno = '%1' ) and ( '%2' = '' or name = '%2' ) and ( '%3' = '' or department = '%3' ) "
        "and ( '%4' = '' or type = '%4' )")
        .arg(newcard.cno)
        .arg(newcard.name)
        .arg(newcard.department)
        .arg(newcard.type);

    //将查询得到的结果写到文件中
    QFile file(".../CardQuery.txt"); // 修改为合适的路径
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
                    <<query.value(3).toString() << "\n";
        }
        file.close();
        this->close();
        emitQuerySuccess();
    }
    else
    {
        qDebug() << "Query Failed!" << query.lastError().text();
        QMessageBox::critical(this,"error","查询失败,请检查输入",QMessageBox::Ok);
    }
}

void CardManage::emitQuerySuccess()
{
    emit QuerySuccess();
}

void CardManage::emitCloseBox()
{
    emit FinishCardOP();
}