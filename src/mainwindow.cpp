#include "mainwindow.h"
#include<QDebug>

MainWidget::MainWidget()
{
    /*
     * 主界面利用 QHBoxLayout 布局为两个区域，左边区域用 Left_Table_Box 实现，右边由 Right_Table_Box 实现
    */
    
    setWindowTitle("图书管理系统");
    this->resize(1000,600);
    QVBoxLayout *VBoxLayout = new QVBoxLayout(this); //竖直分成两块

    QGroupBox *Header = createHeader();

    QGroupBox *Mess_Box = createMess();
    QGroupBox *Menu_Box = createMenu();

    VBoxLayout->addWidget(Header, 2);
    VBoxLayout->addWidget(Mess_Box, 6);
    VBoxLayout->addWidget(Menu_Box, 2);
}

QGroupBox* MainWidget::createHeader()
{
    QGroupBox* Header = new QGroupBox();
    Header->setAutoFillBackground(true);

    QDir dir;
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QPixmap(dir.currentPath()+"/title.png"))); // 修改为合适的路径
    Header->setPalette(palette);

    QHBoxLayout* HBoxLayout = new QHBoxLayout();
    
    LoginBtn = new QPushButton("登录");
    RegisterBtn = new QPushButton("注册");
    CurrentUser = new QLabel("当前用户: 未登录");
    CurrentUser->setStyleSheet("QLabel{color:white}");

    QDateTime current_time = QDateTime::currentDateTime();
    QString currenttime = current_time.toString("yyyy-MM-dd");
    CurrentTime = new QLabel("当前日期: " + currenttime);
    CurrentTime->setStyleSheet("QLabel{color:white}");

    CurrentUser->setFixedSize(QSize(150, 40));
	CurrentTime->setFixedSize(QSize(150, 40));
	RegisterBtn->setFixedSize(QSize(60, 40));
	LoginBtn->setFixedSize(QSize(60, 40));

	HBoxLayout->addStretch();//添加伸缩 居右
	HBoxLayout->addWidget(CurrentUser, 0, Qt::AlignBottom);
	HBoxLayout->addWidget(CurrentTime, 0, Qt::AlignBottom);
	HBoxLayout->addWidget(LoginBtn, 0, Qt::AlignBottom);
	HBoxLayout->addWidget(RegisterBtn, 0, Qt::AlignBottom);
	
	Header->setMaximumHeight(88);
	Header->setLayout(HBoxLayout);

    connect(LoginBtn,&QPushButton::clicked,this,&MainWidget::ShowLoginBox);
    connect(LoginBtn,&QPushButton::clicked,this,&MainWidget::ShowLogoutBox); 
    connect(RegisterBtn,&QPushButton::clicked,this,&MainWidget::ShowRegisterBox);
    return Header;
}

//额外功能，登录与注册，权限管理
void MainWidget::ShowLoginBox()
{
    if(LoginBtn->text() == "注销") return;
    IsLogin = true;
    LoginBox = new QDialog;
    
    LoginBox->setWindowTitle(LoginBtn->text());

    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    username = new QLineEdit;
    password = new QLineEdit;
    password->setEchoMode(QLineEdit::Password);
    FormLayout->addRow("username",username);
    FormLayout->addRow("password",password);

    QHBoxLayout* HBox = new QHBoxLayout;
    QPushButton* confirm = new QPushButton("Confirm");
    QPushButton* cancel = new QPushButton("Cancel");
    HBox->addWidget(confirm);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 2);
    VBox->addLayout(HBox, 1);
    LoginBox->setLayout(VBox);

    //确定登录还是取消
    connect(confirm,&QPushButton::clicked,this,&MainWidget::Login);
    connect(cancel,&QPushButton::clicked,LoginBox,&close);

    LoginBox->exec();
}

void MainWidget::Login()
{
    QString userstr,passstr;
    userstr = username->text();
    passstr = password->text();
    if(userstr != "" && passstr != "")
    {
        QString usernamestr = QString("select * from account where username = '%1'").arg(userstr);
        QSqlQuery query;
        bool a = query.exec(usernamestr);
        qDebug() << query.lastError();
        if(query.next())
        {
            if(query.value(1) == passstr) //登陆成功
            {
                username_str = userstr;
                password_str = passstr;
                LoginBtn->setText("注销");
                CurrentUser->setText("当前用户: " + username_str);
                LoginBox->close();
            }
            else
            {
                QMessageBox::critical(this,"error","密码错误",QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::critical(this,"error","用户名不存在",QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::warning(this,"提示","请输入完整的用户名和密码",QMessageBox::Ok);
    }
}

void MainWidget::ShowLogoutBox()
{
    if(IsLogin){
        IsLogin = false;
        return;
    } 
    LogoutBox = new QDialog;
    
    LogoutBox->setWindowTitle(LoginBtn->text());

    QVBoxLayout* VBox = new QVBoxLayout;
    QLabel* sure = new QLabel("注销当前账户?");
    VBox->addWidget(sure);

    QHBoxLayout* HBox = new QHBoxLayout;
    QPushButton* confirm = new QPushButton("Confirm");
    QPushButton* cancel = new QPushButton("Cancel");
    HBox->addWidget(confirm);
    HBox->addWidget(cancel);

    VBox->addLayout(HBox, 1);
    LogoutBox->setLayout(VBox);

    //确定注销还是取消
    connect(confirm,&QPushButton::clicked,this,&MainWidget::Logout);
    connect(cancel,&QPushButton::clicked,LogoutBox,&close);

    LogoutBox->exec();
}

void MainWidget::Logout()
{
    username_str = "null";
    password_str = "null";
    CurrentUser->setText("当前用户: 未登录");
    LoginBtn->setText("登录");

    int rowNum = BookWidget->rowCount();
    for(int i = 0 ; i < rowNum ; i++)//清空列表
    {
        BookWidget->removeRow(0);
    }

    rowNum = BorrowWidget->rowCount();
    for(int i = 0 ; i < rowNum ; i++)//清空列表
    {
        BorrowWidget->removeRow(0);
    }

    rowNum = CardWidget->rowCount();
    for(int i = 0 ; i < rowNum ; i++)//清空列表
    {
        CardWidget->removeRow(0);
    }
    
    LogoutBox->close();
}

void MainWidget::ShowRegisterBox()
{
    RegisterBox = new QDialog;
    
    RegisterBox->setWindowTitle("注册");

    QVBoxLayout* VBox = new QVBoxLayout;
    QFormLayout* FormLayout = new QFormLayout;

    username = new QLineEdit;
    password = new QLineEdit;
    password->setEchoMode(QLineEdit::Password);
    FormLayout->addRow("username",username);
    FormLayout->addRow("password",password);

    QHBoxLayout* HBox = new QHBoxLayout;
    QPushButton* confirm = new QPushButton("Confirm");
    QPushButton* cancel = new QPushButton("Cancel");
    HBox->addWidget(confirm);
    HBox->addWidget(cancel);

    VBox->addLayout(FormLayout, 2);
    VBox->addLayout(HBox, 1);
    RegisterBox->setLayout(VBox);

    //确定登录还是取消
    connect(confirm,&QPushButton::clicked,this,&MainWidget::Register);
    connect(cancel,&QPushButton::clicked,RegisterBox,&close);

    RegisterBox->exec();
}

void MainWidget::Register()
{
    QString userstr,passstr;
    userstr = username->text();
    passstr = password->text();
    if(userstr != "" && passstr != "")
    {
        QString usernamestr = QString("select * from account where username = '%1'").arg(userstr);
        QSqlQuery query;
        query.exec(usernamestr);
        qDebug() << query.lastError();
        if(query.next()) //已存在
        {
            QMessageBox::critical(this,"error","用户名已存在",QMessageBox::Ok);
        }
        else //不存在，新建用户
        {
            QString newaccount = QString("insert into account values('%1','%2')").arg(userstr).arg(passstr);
            QSqlQuery addaccount;
            addaccount.exec(newaccount);
            qDebug() << addaccount.lastError();
            RegisterBox->close();
        }
    }
    else
    {
        QMessageBox::warning(this,"提示","请输入完整的用户名和密码",QMessageBox::Ok);
    }
}


//构建信息面板，Mess_Box 中只包含一个 QTableWidget 表格控件，为了使表格尺寸自适应整个窗口，将表格添加到了 QHBoxLayout 布局工具中。
QGroupBox* MainWidget::createMess()
{
    QGroupBox* Mess_Box = new QGroupBox("查询结果界面"); 
    TabWidget = new QTabWidget;

    BookWidget = new QTableWidget; //表格
    BookWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    CardWidget = new QTableWidget;
    CardWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    BorrowWidget = new QTableWidget;
    BorrowWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    //flushTable();
    TabWidget->addTab(BookWidget, "图书查询");
    TabWidget->addTab(BorrowWidget, "借阅信息查询");
    TabWidget->addTab(CardWidget,"借书证查询");
    QHBoxLayout * AutoHBoxLayout = new QHBoxLayout;
    AutoHBoxLayout->addWidget(TabWidget);
    Mess_Box->setLayout(AutoHBoxLayout);

    return Mess_Box;

} 

void MainWidget::BookTable()
{
    BookWidget->setColumnCount(9);
    BookWidget->setHorizontalHeaderLabels(QStringList() <<"bno"<< "category"<<"title"<<"press"<<"year"<<"author"<<"price"<<"total"<<"stock");   
    QFile file(".../BookQuery.txt"); // 修改为合适的路径
    bool a = file.open(QIODevice::ReadOnly);
    qDebug() << "a: " << a;
    QTextStream textstr(&file);
    QString line;
    QStringList bookinf;
    BookWidget->setRowCount(0);
    BookWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    while(!textstr.atEnd()){
        BookWidget->setRowCount(BookWidget->rowCount()+1);
        line = textstr.readLine();
        bookinf = line.split(",");

        qDebug() << bookinf.at(0);
        BookWidget->setItem(BookWidget->rowCount()-1,0,new QTableWidgetItem(bookinf.at(bnoindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,1,new QTableWidgetItem(bookinf.at(cateindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,2,new QTableWidgetItem(bookinf.at(titleindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,3,new QTableWidgetItem(bookinf.at(pressindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,4,new QTableWidgetItem(bookinf.at(yearindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,5,new QTableWidgetItem(bookinf.at(authorindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,6,new QTableWidgetItem(bookinf.at(priceindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,7,new QTableWidgetItem(bookinf.at(totalindex)));
        BookWidget->setItem(BookWidget->rowCount()-1,8,new QTableWidgetItem(bookinf.at(stockindex)));
    }

    for(int i = 0;i < BookWidget->rowCount(); i++)
    {
        for(int j = 0; j < BookWidget->columnCount(); j++)
        {
            BookWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }

    file.close();
}

void MainWidget::BorrowTable()
{
   BorrowWidget->setColumnCount(4);
   BorrowWidget->setHorizontalHeaderLabels(QStringList() <<"cno"<< "bno"<<"borrow_date"<<"return_date");   
    QFile file(".../BorrowQuery.txt"); // 修改为合适的路径
    bool a = file.open(QIODevice::ReadOnly);
    qDebug() << "a: " << a;
    QTextStream textstr(&file);
    QString line;
    QStringList borrowinf;
   BorrowWidget->setRowCount(0);
   BorrowWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    while(!textstr.atEnd()){
        line = textstr.readLine();
        borrowinf = line.split(",");
       BorrowWidget->setRowCount(BorrowWidget->rowCount()+1);
       BorrowWidget->setItem(BorrowWidget->rowCount()-1,0,new QTableWidgetItem(borrowinf.at(br_cno)));
       BorrowWidget->setItem(BorrowWidget->rowCount()-1,1,new QTableWidgetItem(borrowinf.at(br_bno)));
       BorrowWidget->setItem(BorrowWidget->rowCount()-1,2,new QTableWidgetItem(borrowinf.at(br_date)));
       BorrowWidget->setItem(BorrowWidget->rowCount()-1,3,new QTableWidgetItem(borrowinf.at(br_rdate)));
    }

    for(int i = 0;i <BorrowWidget->rowCount(); i++)
    {
        for(int j = 0; j <BorrowWidget->columnCount(); j++)
        {
           BorrowWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }

    file.close();
}

void MainWidget::CardTable()
{
    CardWidget->setColumnCount(4);
    CardWidget->setHorizontalHeaderLabels(QStringList() <<"cno"<< "name"<<"department"<<"type");   
    QFile file(".../CardQuery.txt"); // 修改为合适的路径
    bool a = file.open(QIODevice::ReadOnly);
    qDebug() << "a: " << a;
    QTextStream textstr(&file);
    QString line;
    QStringList cardinf;
    CardWidget->setRowCount(0);
    CardWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    while(!textstr.atEnd()){
        line = textstr.readLine();
        cardinf = line.split(",");
        CardWidget->setRowCount(CardWidget->rowCount()+1);
        CardWidget->setItem(CardWidget->rowCount()-1,0,new QTableWidgetItem(cardinf.at(0)));
        CardWidget->setItem(CardWidget->rowCount()-1,1,new QTableWidgetItem(cardinf.at(1)));
        CardWidget->setItem(CardWidget->rowCount()-1,2,new QTableWidgetItem(cardinf.at(2)));
        CardWidget->setItem(CardWidget->rowCount()-1,3,new QTableWidgetItem(cardinf.at(3)));
    }

    for(int i = 0;i <CardWidget->rowCount(); i++)
    {
        for(int j = 0; j <CardWidget->columnCount(); j++)
        {
           CardWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }

    file.close();

}

QGroupBox* MainWidget::createMenu()
{
    QGroupBox* Menu_Box = new QGroupBox("操作界面");
    QVBoxLayout* VBoxLayout = new QVBoxLayout; //垂直布局

    QGridLayout* Buts = new QGridLayout; //按钮布局
    AddBookBtn = new QPushButton("单本入库");
    BatchAddBtn = new QPushButton("批量入库");
    BookQueryBtn = new QPushButton("图书查询");
    BRBookBtn = new QPushButton("借/还书/借阅信息查询");
    CardManageBtn = new QPushButton("借书证管理");
    CardQueryBtn = new QPushButton("借书证查询");

    Buts->addWidget(AddBookBtn,0,0);
    Buts->addWidget(BatchAddBtn,0,1);
    Buts->addWidget(BookQueryBtn,1,0);
    Buts->addWidget(BRBookBtn,1,1);
    Buts->addWidget(CardQueryBtn,2,0);
    Buts->addWidget(CardManageBtn,2,1);

    VBoxLayout->addLayout(Buts,2);

    Menu_Box->setLayout(VBoxLayout); //设置总体布局
    //点击图书入库按钮，执行 AddBookBox()方法
    connect(AddBookBtn,&QPushButton::clicked,this,&MainWidget::ShowAddBooksBox);
    connect(BookQueryBtn,&QPushButton::clicked,this,&MainWidget::ShowBookQueryBox);
    connect(CardManageBtn,&QPushButton::clicked,this,&MainWidget::ShowCardManageBox);
    connect(CardQueryBtn,&QPushButton::clicked,this,&MainWidget::ShowCardQueryBox);
    connect(BRBookBtn,&QPushButton::clicked,this,&MainWidget::ShowBRBookBox);
    connect(BatchAddBtn,&QPushButton::clicked,this,&MainWidget::ShowBatchAddBox);

    return Menu_Box;
}
//当点击添加按钮时，弹出添加学生信息的子窗口
void MainWidget::ShowAddBooksBox()
{
    if(username_str != "root")
    {
        QMessageBox::warning(this,"提示","您没有此权限",QMessageBox::Ok);
        return;
    }
    AddBookBox = new AddBooks;
    connect(AddBookBox,&AddBooks::AddSuccess,this,&MainWidget::BookTable);
    AddBookBox->exec();
}

void MainWidget::ShowBookQueryBox()
{
    BookQueryBox = new BookQuery;
    connect(BookQueryBox,&BookQuery::FinishQuery,this,&MainWidget::BookTable); //先建立关联再执行
    BookQueryBox->exec();
}

void MainWidget::ShowCardQueryBox()
{
    if(username_str != "root")
    {
        QMessageBox::warning(this,"提示","您没有此权限",QMessageBox::Ok);
        return;
    }
    CardManageBox = new CardManage(true);
    connect(CardManageBox,&CardManage::QuerySuccess,this,&MainWidget::CardTable);
    CardManageBox->exec();
}

void MainWidget::ShowCardManageBox()
{
    if(username_str != "root")
    {
        QMessageBox::warning(this,"提示","您没有此权限",QMessageBox::Ok);
        return;
    }
    CardManageBox = new CardManage(false);
    connect(CardManageBox,&CardManage::QuerySuccess,this,&MainWidget::CardTable);
    CardManageBox->exec();
}

void MainWidget::ShowBRBookBox()
{
    BRBookBox = new BRBook;
    connect(BRBookBox,&BRBook::QuerySuccess,this,&MainWidget::BorrowTable);
    BRBookBox->exec();
}

void MainWidget::ShowBatchAddBox()
{
    if(username_str != "root")
    {
        QMessageBox::warning(this,"提示","您没有此权限",QMessageBox::Ok);
        return;
    }
    BatchAddBox = new BatchAdd;
    connect(BatchAddBox,&BatchAdd::AddSuccess,this,&MainWidget::BookTable);
    BatchAddBox->exec();
}








