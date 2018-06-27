#include <iostream>
#include <QCoreApplication>
#include <decoder.h>
#include <htmlgenerator.h>
#include <QCommandLineParser>
#include <QCommandLineOption>

using namespace std;

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("deptest_parser");
    QCoreApplication::setApplicationVersion("0.5");
    QCommandLineParser parser;
    parser.setApplicationDescription("Program for parsing *.deptest files. UrFU 2018");
    parser.addVersionOption();
    parser.addHelpOption();

    QCommandLineOption modeExport(QStringList() << "m" << "export_mode", QCoreApplication::translate("main", "Set mode of export HTML or INI"), QCoreApplication::translate("main", "export_mode"));
    QCommandLineOption inputFile(QStringList() << "if" << "input_file", QCoreApplication::translate("main", "Path to input file *.deptest"), QCoreApplication::translate("main", "input_file"));
    QCommandLineOption outputFile(QStringList() << "of" << "output_file", QCoreApplication::translate("main", "Path to ouput file"), QCoreApplication::translate("main", "output_file"));
    QCommandLineOption decodeStr(QStringList() << "s" << "string", QCoreApplication::translate("main", "String to decode"), QCoreApplication::translate("main", "strings"));

    parser.addOption(decodeStr);
    parser.addOption(modeExport);
    parser.addOption(inputFile);
    parser.addOption(outputFile);

    parser.process(a);

    if (parser.isSet(decodeStr))
    {
        cout << "Decoded:" << endl << Decoder::decodeString(parser.value(decodeStr)).toStdString() << endl;
        return 0;
    }

    if (!parser.isSet(inputFile))
    {
        cout << "input_file parameter not found!" << endl;
        return 0;
    }
    QString inputFilePath = parser.value(inputFile);
    QString outputFilePath = "out";
    bool isHtmlMode = false;
    bool noOutputFile = false;
    if (!parser.isSet(outputFile))
    {
        cout << "Output file not set" << endl;
    }
    else
    {
        outputFilePath = parser.value(outputFile);
    }

    if (parser.isSet(modeExport))
    {
        if ("html" == parser.value(modeExport).toLower())
        {
            isHtmlMode = true;
        }
        else if ("ini" == parser.value(modeExport).toLower()) {
            isHtmlMode = false;
        }
        else
        {
            cout << "Export mode set to INI";
            noOutputFile = true;
        }
    }
    else
    {
        noOutputFile = true;
    }

    if (noOutputFile)
    {
        if (Decoder::decodeFile(inputFilePath, "tmp"))
        {
            if (isHtmlMode)
            {
                HtmlGenerator htmlGen("tmp");
                htmlGen.saveToHtml(outputFilePath+".html");
                QFile::remove("tmp");
            }
            else
            {
                QFile::rename("tmp", "out.ini");
                cout << "Saved to out.ini" << endl;
            }
        }
    }
    else
    {
        if (Decoder::decodeFile(inputFilePath, "tmp"))
        {
            if (isHtmlMode)
            {
                HtmlGenerator htmlGen("tmp");
                htmlGen.saveToHtml(outputFilePath);
                QFile::remove("tmp");
            }
            else
            {
                QFile::rename("tmp", outputFilePath);
                cout << "Saved to " << outputFilePath.toStdString() << endl;
            }
        }
    }
    return 0;
}
