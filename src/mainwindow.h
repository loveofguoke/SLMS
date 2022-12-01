#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QDataStream>
#include <QLabel>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include<QDebug>
#include"AddBooks.h"
#include"BookQuery.h"
#include"CardManage.h"
#include"BRBook.h"
#include"BatchAdd.h"
class MainWidget: public QWidget
{
    Q_OBJECT
public:
    //构建主窗口界面
    MainWidget();
    //当用户点击图书入库按钮，弹出图书入库输入框
    void ShowAddBooksBox();
    void ShowBookQueryBox();
    void ShowCardManageBox();
    void ShowCardQueryBox();
    void ShowBRBookBox();
    void ShowBatchAddBox();
    void ShowLoginBox();
    void ShowLogoutBox();
    void ShowRegisterBox();
    void Login(); //登录和注销
    void Logout();
    void Register(); //注册
//public slots:
    //更新表格中信息
    void BookTable();
    void BorrowTable();
    void CardTable();
    //更新列表中信息
    //void flushListWidget(int row);
    
private:
    //主窗口信息面板
    QGroupBox* createMess();
    //主窗口功能面板
    QGroupBox* createMenu();
    //主窗口Header
    QGroupBox* createHeader();
private:
    //主窗口中的表格控件
    QTabWidget* TabWidget;
    QTableWidget* BookWidget;
    QTableWidget* BorrowWidget;
    QTableWidget* CardWidget;
    //主窗口中的列表控件
    QListWidget* ListWidget;
    //主窗口中的图书及借书证操作控件
    QPushButton* AddBookBtn;
    QPushButton* BookQueryBtn;
    QPushButton* BRBookBtn;
    QPushButton* CardManageBtn;
    QPushButton* CardQueryBtn;
    QPushButton* BatchAddBtn;
    //主窗口中的登录，注册，当前登录用户，当前时间控件
    QLabel* LibHeader;
    QPushButton* LoginBtn;
    QPushButton* RegisterBtn;
    QLabel* CurrentTime;
    QLabel* CurrentUser;
    //为什么不把布局放进来？我认为界面的控件是基本的，但界面布局则可能会更换，所以在相应函数中申请即可

    //弹出的添加学生信息窗口
    AddBooks* AddBookBox;
    BookQuery* BookQueryBox;
    CardManage* CardManageBox;
    BRBook* BRBookBox;
    BatchAdd* BatchAddBox;
    //登录与注册窗口
    QDialog* LoginBox;
    QDialog* LogoutBox;
    QDialog* RegisterBox;
    QLineEdit* username;
    QLineEdit* password;

    QString username_str = "null";
    QString password_str = "null";

    bool IsLogin = false;
};


#endif 
