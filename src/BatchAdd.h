#ifndef _BATCHADD_H_
#define _BATCHADD_H_

#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QLabel>
#include "Book.h"

class BatchAdd: public QDialog
{
    Q_OBJECT
public:
    //构建批量入库界面
    BatchAdd();
    //显示新加的书的信息
    void flushBookTable(QString url);
signals:
    //书籍入库操作完毕
    void FinishAdd();
    void AddSuccess();

public slots:
    //将信息插入到数据库表book中
    void AddBook2DB();
public:
    //发出closeBox信号
    void emitCloseBox();
    void emitAddSuccess();
private:
    QLabel* tips;
    QLineEdit* url;
    QPushButton* confirm;
    QPushButton* cancel;
};

#endif