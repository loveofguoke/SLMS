#include <QWidget>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "Card.h"

class CardManage: public QDialog
{
    Q_OBJECT
public:
    //构建借书证管理界面
    CardManage(bool IsQuery);
    //添加，删除，修改后
    void flushCardTable(QString cno);
signals:
    //对借书证的操作完毕
    void FinishCardOP();
    void QuerySuccess();

public slots:
    //添加借书证
    void AddCard2DB();
    //删除现有的借书证，根据输入的cno
    void DeleteCardfDB();
    //修改已有借书证信息
    void UpdateCardinDB();
    //查询借书证
    void CardQuery();

public:
    bool IsQuery = true;
    //发出closeBox信号
    void emitCloseBox();
    void emitQuerySuccess();
private:
    QLineEdit* cno;
    QLineEdit* name;
    QLineEdit* department;
    QLineEdit* type;
    QPushButton* add;
    QPushButton* deletecard;
    QPushButton* update;
    QPushButton* query;
    QPushButton* cancel;
};