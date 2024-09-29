#include "mathanalygui.h"
#include "ui_mathanalygui.h"

double result;
mathanalygui::mathanalygui(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mathanalygui)
{
    ui->setupUi(this);
    ui->compute->setChecked(true);
}

mathanalygui::~mathanalygui()
{
    delete ui;
}

double functionanaly(QString fnName, double argu)
{
    if (fnName == "sba")
    {
        return abs(argu);
    }
    else if (fnName == "nis")
    {
        return sin(argu);
    }
    else if (fnName == "soc")
    {
        return cos(argu);
    }
    else if (fnName == "nat")
    {
        return tan(argu);
    }
    else if (fnName == "nl")
    {
        return log(argu);
    }
    else if (fnName == "" || fnName == "ep")
    {
        return argu;
    }
    else
    {
        return -5;
    }
}

QList<double> mathanalygui::analysisPer(psequ tempPsequ, QList<double> tempArguDL)
{
    QList<double> tempTrueArgu;
    for (int m = 0; m < tempPsequ.argumentList.length(); m++)
    {
        bool tifd = true;
        for (int n = 0; n < tempPsequ.argumentList[m].length(); n++)
        {
            if (!tempPsequ.argumentList[m][n].isDigit() && tempPsequ.argumentList[m][n] != '.')
            {
                tifd = false;
                break;
            }
        }
        if (tempPsequ.argumentList[m] == "X")
        {
            tempTrueArgu << ui->independentVariableIn->toPlainText().toDouble();
        }
        else if (tempPsequ.argumentList[m] == "N")
        {
            tempTrueArgu << tempArguDL[0];
            tempArguDL.removeAt(0);
        }
        else if (tifd)
        {
            tempTrueArgu << tempPsequ.argumentList[m].toDouble();
        }
        else
        {
            tempTrueArgu << 0;
        }
    }
    QList<double> tempReturn;
    QList<char> tempOperList = {'*','/'};
    while (true)
    {
        if (tempPsequ.operatorList[0] == '.')
        {
            tempReturn << functionanaly(tempPsequ.functionList[0],tempTrueArgu[0]);
            tempPsequ.functionList.removeAt(0);
            tempPsequ.operatorList.removeAt(0);
            tempTrueArgu.removeAt(0);
            if (tempPsequ.operatorList.length() == 0)
            {
                break;
            }
            continue;
        }
        int fa;
        for (int h = 0; h < tempPsequ.operatorList.length(); h++)
        {
            if (tempPsequ.operatorList[h] == '.')
            {
                break;
            }
            if (tempOperList.contains(tempPsequ.operatorList[h]))
            {
                fa = h;
                break;
            }
            else
            {
                fa = h;
            }
        }
        double tla = functionanaly(tempPsequ.functionList[fa],tempTrueArgu[fa]);
        double tra = functionanaly(tempPsequ.functionList[fa+1],tempTrueArgu[fa+1]);
        qDebug() << tla << tra << tempPsequ.operatorList[fa];
        if(fa - 1 >= 0&&tempPsequ.operatorList[fa-1] == '-')
        {
            tla = -tla;
            tempPsequ.operatorList[fa-1] = '+';
        }
        double ta;
        switch (tempPsequ.operatorList[fa])
        {
        case '+':
            ta = tla + tra;
            break;
        case '-':
            ta = tla - tra;
            break;
        case '*':
            ta = tla * tra;
            break;
        case '/':
            ta = tla / tra;
            break;
        default:
            ta = tla;
        }
        tempPsequ.functionList.removeAt(fa+1);
        tempPsequ.functionList[fa] = "";
        tempPsequ.operatorList.removeAt(fa);
        tempTrueArgu.removeAt(fa+1);
        tempTrueArgu[fa] = ta;
        if (tempPsequ.operatorList.length() == 0)
        {
            break;
        }
    }
    return tempReturn;
}

void mathanalygui::on_Run_clicked()
{
    QString formulaStr = ui->formulaIn->toPlainText();
    QString preStr = formulaStr;
    QList<int> parenthesisIndex; // 括号索引
    QList<int> sequence;         // 等级序列
    QList<psequ> psequList;
    formulaStr.remove(' ');
    formulaStr.replace('X', "(X)");
    int seqnum = 0;
    int tsum = 0;
    for (int i = 0; i < formulaStr.length(); i++)
    {
        if (formulaStr[i].isDigit() && !tsum)
        {
            formulaStr.insert(i, '(');
            tsum = 1;
        }
        else if (!(formulaStr[i].isDigit() || formulaStr[i] == '.') && tsum)
        {
            formulaStr.insert(i, ')');
            tsum = 0;
        }
        if (i == formulaStr.length() - 1 && tsum)
        {
            formulaStr += ')';
            tsum = 0;
        }
        if (formulaStr[i] == '(')
        {
            parenthesisIndex << i;
            sequence << seqnum;
            seqnum += 1;
        }
        else if (formulaStr[i] == ')')
        {
            parenthesisIndex << i;
            seqnum -= 1;
            sequence << seqnum;
        }
        if (seqnum < 0)
        {
            ui->Output->insertPlainText("检测到输入公式错误：括号不匹配！");
            return;
        }
    }
    QList<int> depth;
    QList<QList<int>> tempBeginAndEnd;
    QString tempArguStr;
    for (int s = 0; s < sequence.length(); s++)
    {
        bool tempSt = false;
        if (depth.length() - 1 < sequence[s])
        {
            depth << 1;
            tempBeginAndEnd << QList<int>();
            tempBeginAndEnd[sequence[s]] << parenthesisIndex[s];
            tempSt = true;
            goto process1;
        }
        if (depth[sequence[s]])
        {
            depth[sequence[s]] = 0;
            tempBeginAndEnd[sequence[s]] << parenthesisIndex[s];
            if (parenthesisIndex[s] != formulaStr.length() - 1)
            {
                QList<char> tempOperChar = {'+', '-', '*', '/'};
                if (tempOperChar.contains(formulaStr[parenthesisIndex[s] + 1]))
                {
                    psequList[sequence[s]].operatorList << formulaStr[parenthesisIndex[s] + 1].toLatin1();
                }
                else if (formulaStr[parenthesisIndex[s] + 1] == ')')
                {
                    psequList[sequence[s]].operatorList << '.';
                }
            }
            else
            {
                psequList[sequence[s]].operatorList << '.';
            }
            continue;
        }
        else
        {
            depth[sequence[s]] = 1;
            tempBeginAndEnd[sequence[s]] << parenthesisIndex[s];
        }
    process1:
        QString tempFuncStr;
        if (s >= 1)
        {
            for (int a = parenthesisIndex[s] - 1; a > parenthesisIndex[s - 1]; a--)
            {
                if (!formulaStr[a].isLetter())
                {
                    break;
                }
                tempFuncStr += formulaStr[a];
            }
        }
        else
        {
            for (int b = parenthesisIndex[0] - 1; b >= 0; b--)
            {
                if (!formulaStr[b].isLetter())
                {
                    break;
                }
                tempFuncStr += formulaStr[b];
            }
        }
        if (tempSt)
        {
            psequList << psequ();
        }
        psequList[sequence[s]].functionList << tempFuncStr;
    }
    for (int d = 0; d < tempBeginAndEnd.length(); d++)
    {
        for (int f = 0; f < tempBeginAndEnd[d].length(); f += 2)
        {
            tempArguStr = formulaStr.mid(tempBeginAndEnd[d][f]+1, tempBeginAndEnd[d][f+1] - tempBeginAndEnd[d][f] - 1);
            if (tempArguStr == 'X')
            {
                psequList[d].argumentList << "X";
                continue;
            }
            bool ch = true;
            for (int e = 0; e < tempArguStr.length(); e++)
            {
                qDebug() << tempArguStr[e];
                if (!tempArguStr[e].isDigit() && tempArguStr[e] != '.')
                {
                    ch = false;
                }
            }
            if (ch)
            {
                psequList[d].argumentList << tempArguStr;
            }
            else
            {
                psequList[d].argumentList << "N";
            }
        }
    }
    QList<double> FinalArguList;
    for (int t = psequList.length() - 1; t >= 0; t--)
    {
        qDebug() << "现在是第" << t << "级";
        if (t == psequList.length() - 1)
        {
            for (int p = 0; p < psequList[t].argumentList.length(); p++)
            {
                FinalArguList << psequList[t].argumentList[p].toDouble();
            }
        }
        FinalArguList = analysisPer(psequList[t],FinalArguList);

        qDebug() << psequList[t].argumentList;
        qDebug() << psequList[t].functionList;
        qDebug() << psequList[t].operatorList;

    }
    //qDebug() << FinalArguList;
    time_t t = time(0);
    char time[64]={};
    strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S",localtime(&t));
    result = FinalArguList[0];
    ui->Output->insertPlainText(QString(time)+"\n"+"输入: "+preStr+"\n"+"计算结果为: "+QString::number(FinalArguList[0])+"\n");
}
void mathanalygui::on_pushButton_clicked(){

    int e = 1;
    on_Run_clicked();
    double y = result;
    while(abs(y) > 0.0001 && e <= 10000)
    {
        ui->Output->insertPlainText("误差为："+QString::number(abs(y))+"\n");
        double t=ui->independentVariableIn->toPlainText().toDouble();
        ui->independentVariableIn->setPlainText(QString::number(t-0.001));
        on_Run_clicked();
        double t1=result;
        ui->independentVariableIn->setPlainText(QString::number(t+0.001));
        on_Run_clicked();
        double t2=result;
        double t3 = (t2-t1)/0.002;
        ui->independentVariableIn->setPlainText(QString::number(t-y/t3));

        on_Run_clicked();

        y = result;
        e++;
    }
    ui->Output->insertPlainText(QString::number(y));
}

