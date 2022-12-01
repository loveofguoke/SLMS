#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include<QDateTime>
#include"Borrow.h"

class BRBook: public QDialog
{
    Q_OBJECT
public:
    //构建借还书界面
    BRBook();
    //在成功借书，还书后刷新查询结果
    void flushBorrowTable(QString cno);
signals:
    //对借书证的操作完毕
    void FinishBR();
    //查询成功
    void QuerySuccess();

public slots:
    //借书记录查询
    void BorrowBookQuery();
    //借书
    void BorrowBook();
    //还书    
    void ReturnBook();

public:
    //发出closeBox信号
    void emitCloseBox();
    //发出查询成功信号
    void emitQuerySuccess();
private:
    QLineEdit* cno;
    QLineEdit* bno;
    QPushButton* query;
    QPushButton* borrow;
    QPushButton* returnbook;
    QPushButton* cancel;
};