#include <iostream>
#include <QCoreApplication>
#include <QTextCodec>
#include <QMap>
#include <QFile>
#include <QTextStream>
#include <stdlib.h>
#include <QSettings>
#include <QStringList>
#include <math.h>

using namespace std;

constexpr int rus_first = 192;
constexpr int rus_last = 255;

bool isRus(int val) { if (val % 300 < rus_first or val % 300  > rus_last) return false; return true; }

char getRusSimbol(int val)
{
    val = val % 300;
//    if (val < rus_first or val  > rus_last) return '';
    return char('А' + (val - rus_first));
}

string encodeStr(string input)
{
//    QString inStr;
//    inStr = QString::fromStdString(input);
    string inStr = input;
    QString res = "";
//        cout << "Result: ";
//        for (int i = 0; i < inStr.length(); i+=3) {
//            int q = inStr.mid(i, 3).toInt();
//            if (rus.contains(q))
//                res += rus[q];
//            else
//                res += QChar(q % 300);
//        }
    int i = 0;
    while(i < inStr.length()) {
        if (inStr[i] < '0' or inStr[i] > '9') {
            res += inStr[i];
            i++;
            continue;
        }
        int q = atoi(inStr.substr(i, 3).c_str());
//        if (isRus(q))
//            res += getRusSimbol(q);
//        else
            res += QChar(q % 300);
        i += 3;
    }

//        cout << endl;
//    QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
//    QByteArray encodedString = codec->fromUnicode(res);
//    return encodedString.data();
    return res.toLocal8Bit().data();
}

QByteArray encodeStr2(string input)
{
    QByteArray res = QByteArray();
//    QString inStr;
//    inStr = QString::fromStdString(input);
    string inStr = input;
//    QString res = "";
//        cout << "Result: ";
//        for (int i = 0; i < inStr.length(); i+=3) {
//            int q = inStr.mid(i, 3).toInt();
//            if (rus.contains(q))
//                res += rus[q];
//            else
//                res += QChar(q % 300);
//        }
    int i = 0;
    while(i < inStr.length()) {
        if (inStr[i] < '0' or inStr[i] > '9') {
            res.append(inStr[i]);
            i++;
            continue;
        }
        int q = atoi(inStr.substr(i, 3).c_str());
//        if (isRus(q))
//            res += getRusSimbol(q);
//        else
            res.append(q % 300);
        i += 3;
    }

//        cout << endl;
//    QTextCodec *codec = QTextCodec::codecForName("Windows-1250");
//    QByteArray encodedString = codec->fromUnicode(res);
//    return encodedString.data();
    return res;
}

void prepareFile()
{
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    string d;



    QTextCodec *codec1 = QTextCodec::codecForName("Windows-1251");

    QFile file("in.txt");
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;
    QFile file2("out.txt");
    file2.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file2);
       while (!file.atEnd()) {
           QByteArray line = file.readLine();
           out << (codec1->toUnicode(encodeStr2(line.toStdString())));
       }

       file2.close();
       file.close();
}

int main()
{
    prepareFile();

    QTextCodec *codec = QTextCodec::codecForName("ISO-8859-1");

    QSettings settings("out.txt",QSettings::IniFormat);
    settings.sync();

    QFile file2("out.html");
    file2.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file2);
    out << "<head> <title>Разборщик тестов 0.5</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head> <html> <body><div>";


    settings.beginGroup("Main");
    int numberQuestion = settings.value("NumberOfQuestions", -1).toInt();
    settings.endGroup();
    for (int question=0; question < numberQuestion; ++question)
    {
        QString groupName = QString("Question") + QString::number(question);
        settings.beginGroup(groupName);
        int questionLinesCount = settings.value("QuestionsLinesCount", -1).toInt();
        QString questionString = QString("");
        for (int questionLine=0; questionLine < questionLinesCount; ++questionLine)
        {
            questionString += QString::fromLatin1(settings.value(QString("QuestionLine") + QString::number(questionLine), "").toString().toLatin1());
        }
        int numberAnswers = settings.value("NumberOfAnswers", -1).toInt();
        int numberImages = settings.value("NumberOfImages", -1).toInt();
        QStringList answers;
        for (int answer=0; answer < numberAnswers; ++answer)
        {
            QString currentAnswer = "";
            int answerLines = settings.value(QString("Answers")+ QString::number(answer) +QString("LinesCount"), -1).toInt();
            for (int answerLine = 0; answerLine < answerLines; ++answerLine)
            {
                currentAnswer += settings.value(QString("Answers") + QString::number(answer) + QString("Line") + QString::number(answerLine), "").toString();

            }
            answers.append(currentAnswer);
        }
        int correctAnswer = settings.value(QString("CorrectAnswers"), -1).toInt();
        int difficultLevel = settings.value(QString("DifficultLevel"), -1).toInt();

        settings.endGroup();

        QString textForHTml = "Question: " + questionString + "\n<br> Answers:\n<br>";
        for (int image=1; image <= numberImages; ++image)
        {
            textForHTml += QString("<img src=\"%1%2.gif\" />\n<br>").arg(question+1).arg(image);
        }
        for (auto item : answers)
        {
            textForHTml += item + "\n<br>";
        }
        QString correctAnswerString = QString::number(log2(correctAnswer));
        try{
            textForHTml += QString("Correct: ") + answers[correctAnswerString.split(".")[0].toInt()] + QString("\n<br>");
        }
        catch(exception &e)
        {

        }

        textForHTml += QString("Correct number(debug): ") + QString::number(log2(correctAnswer)) + QString("\n<br>") + QString("Difficul:") + QString::number(difficultLevel) + QString("\n<br><hr>");

        out << codec->fromUnicode(textForHTml.toUtf8());


    }
    out << "/n<//div><//body><//html>";
//    while (true) {
//        cout << ">>> ";
//        cin >> d;
//        if (d == "exit") break;
//        QByteArray encod2 = encodeStr2(d);

//        cout << "Result: " << codec1->toUnicode(encod2).toStdString() << endl;

//    }
    return 0;
}
