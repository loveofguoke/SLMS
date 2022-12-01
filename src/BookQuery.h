#ifndef _BOOKQUERY_H_
#define _BOOKQUERY_H_

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "Book.h"

class BookQuery: public QDialog
{
    Q_OBJECT
public:
    //构建图书查询界面
    BookQuery();
signals:
    //查询操作完毕
    void FinishQuery();

public slots:
    //从数据库中查询数据
    void QueryBookfDB();

public:
        //发出closeBox信号
    void emitCloseBox();
private:
    QLineEdit* bno;
    QLineEdit* category;
    QLineEdit* title;
    QLineEdit* press;
    QLineEdit* lyear;
    QLineEdit* uyear;
    QLineEdit* author;
    QLineEdit* lprice;
    QLineEdit* uprice;
    QLineEdit* total;
    QLineEdit* stock; 
    QPushButton* confirm;
    QPushButton* cancel;
};

#endif