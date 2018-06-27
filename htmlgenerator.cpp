#include "htmlgenerator.h"


HtmlGenerator::HtmlGenerator(QString configFile):
    encodedFile(configFile, QSettings::IniFormat)
{
    encodedFile.sync();
    loadQuestions();
}

QVector<QuestionItem> HtmlGenerator::getQuestions() const
{
    return questionVector;
}

bool HtmlGenerator::saveToHtml(QString pathToFile)
{
    QFile output(pathToFile);
    if (!output.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "Can't create ouput file at " + pathToFile.toStdString() << std::endl;
        return false;
    }
    QTextStream out(&output);
    QTextCodec *codec = QTextCodec::codecForName("ISO-8859-1");

    QString textForHtml = "";

    textForHtml +=  QString("<head> <title>Deptest decoder 0.5</title> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head> <html> <body>");



    for (auto question : questionVector)
    {
        textForHtml += QString("<div>\n");
            textForHtml += QString("<p>%1</p>\n").arg(question.questionText);
            for (auto image : question.images)
            {
                textForHtml += QString("<img src=\"%1\">\n").arg(image);
            }
        textForHtml += QString("<p>Answers:</p>\n<ol>");
            for (auto answer : question.answers)
            {
                textForHtml += QString("<li><p>%1</p></li>\n").arg(answer);
            }
        textForHtml += QString("</ol>\n");
        textForHtml += QString("<p>Correct: %1</p>\n").arg(question.getCorrectAnswer());
        textForHtml += QString("<p>Difficult level: %1</p>\n").arg(question.difficultLevel);
        textForHtml += QString("</div><hr>\n");
    }
    textForHtml +=  QString("\n</body></html>");
    out << codec->fromUnicode(textForHtml.toUtf8());
    output.close();
}

void HtmlGenerator::loadQuestions()
{
    encodedFile.beginGroup("Main");
    int numberQuestions = encodedFile.value("NumberOfQuestions", -1).toInt();
    encodedFile.endGroup();
    questionVector.clear();
    for (int question=0; question < numberQuestions; ++question)
    {
        questionVector.append(loadQuestion(question));
    }

}

QuestionItem HtmlGenerator::loadQuestion(int questionNumber)
{
    QuestionItem ret;
    QString groupName = QString("Question") + QString::number(questionNumber);
    encodedFile.beginGroup(groupName);
    int questionLinesCount = encodedFile.value("QuestionsLinesCount", -1).toInt();
    QString questionString = QString("");
    for (int questionLine=0; questionLine < questionLinesCount; ++questionLine)
    {
        questionString += QString::fromLatin1(encodedFile.value(QString("QuestionLine") + QString::number(questionLine), "").toString().toLatin1());
    }

    int numberAnswers = encodedFile.value("NumberOfAnswers", -1).toInt();
    int numberImages = encodedFile.value("NumberOfImages", -1).toInt();
    QStringList answers;
    for (int answer=0; answer < numberAnswers; ++answer)
    {
        QString currentAnswer = "";
        int answerLines = encodedFile.value(QString("Answers")+ QString::number(answer) +QString("LinesCount"), -1).toInt();
        for (int answerLine = 0; answerLine < answerLines; ++answerLine)
        {
            currentAnswer += encodedFile.value(QString("Answers") + QString::number(answer) + QString("Line") + QString::number(answerLine), "").toString();

        }
        answers.append(currentAnswer);
    }
    int correctAnswer = encodedFile.value(QString("CorrectAnswers"), -1).toInt();
    int difficultLevel = encodedFile.value(QString("DifficultLevel"), -1).toInt();

    QStringList images;
    for (int image=1; image <= numberImages; ++image)
    {
        images.append(QString("%1%2.gif").arg(questionNumber+1).arg(image));
    }

    encodedFile.endGroup();

    ret.questionText = questionString;
    ret.answers = answers;
    ret.images = images;
    ret.correctAnswer = correctAnswer;
    ret.difficultLevel = difficultLevel;

    return ret;
}
