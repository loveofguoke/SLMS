#ifndef _ADDBOOKS_H_
#define _ADDBOOKS_H_

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include "Book.h"

class AddBooks: public QDialog
{
    Q_OBJECT
public:
    //构建图书入库界面
    AddBooks();
    //成功入库后显示此书当前信息
    void flushBookTable(QString bno);
signals:
    //书籍入库操作完毕
    void FinishAdd();
    void AddSuccess();

public slots:
    //将信息插入到数据库表book中
    void AddBook2DB();
    void UpdateBook();
public:
    //发出closeBox信号
    void emitCloseBox();
    void emitAddSuccess();
private:
    QLineEdit* bno;
    QLineEdit* category;
    QLineEdit* title;
    QLineEdit* press;
    QLineEdit* year;
    QLineEdit* author;
    QLineEdit* price;
    QPushButton* add;
    QPushButton* update;
    QPushButton* cancel;
};

#endif