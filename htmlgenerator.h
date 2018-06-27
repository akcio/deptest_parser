#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H
#include <QString>
#include <QSettings>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <iostream>

struct QuestionItem
{
    QString questionText;
    QStringList answers;
    QStringList images;
    int difficultLevel;
    int correctAnswer;

    QString getCorrectAnswer()
    {
        if (answers.length() > log2(correctAnswer))
            return answers.at(int(log2(correctAnswer)));
        return QString("Correct number from file: ") + QString::number(correctAnswer);
    }
};

class HtmlGenerator
{
public:
    HtmlGenerator(QString configFile);
    /**
     * @brief return vector of questions from decoded file
     * @return
     */
    QVector<QuestionItem> getQuestions() const;
    bool saveToHtml(QString pathToFile);

private:
    QSettings encodedFile;
    QVector<QuestionItem> questionVector;
    void loadQuestions();
    QuestionItem loadQuestion(int questionNumber);
};

#endif // HTMLGENERATOR_H
