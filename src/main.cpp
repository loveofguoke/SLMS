//main.cpp
#include <QSqlDatabase>
#include <QSqlQuery>
#include<QDebug>
#include <QApplication>
#include <QSqlError>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include<QInputDialog>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget mWidget;
    
    
    //qDebug()<<QSqlDatabase::drivers();
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("DB lab5"); //连接数据源及对应的数据库
    db.setUserName("root");
    db.setPassword("123456");
    if (!db.open())
       qDebug()<<"can't open!"<<db.lastError();
    else
        qDebug()<<"open!";    

    QSqlQuery result = db.exec("select * from book;");   //注意，因为上面ODBC数据源配置时选择的是mysql这个自带系统库，所以可以访问user这个表。
    while (result.next())
        qDebug() << result.value("title").toString();

    mWidget.show();

    return a.exec();
}