#ifndef MATHANALYGUI_H
#define MATHANALYGUI_H

#include <QMainWindow>
#include <time.h>
//#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class mathanalygui; }
QT_END_NAMESPACE

class psequ
{
public:
    QStringList argumentList;    //参数序列
    QStringList functionList;    //函数代号序列
    QList<char> operatorList;   //运算符序列
};
/*
class formulaStrInt
{
public:

};
*/
class mathanalygui : public QMainWindow
{
    Q_OBJECT

public:
    mathanalygui(QWidget *parent = nullptr);
    ~mathanalygui();

private slots:
    void on_Run_clicked();

    QList<double> analysisPer(psequ tempPsequ, QList<double> tempArguDL);
    void on_pushButton_clicked();


private:
    Ui::mathanalygui *ui;
};
#endif // MATHANALYGUI_H
