#include "decoder.h"

Decoder::Decoder()
{
}

QString Decoder::decodeString(QString input)
{
    QByteArray res = QByteArray();
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
//    string inStr = input;
    int i = 0;
    while(i < input.length()) {
        if (input.at(i) < '0' or input.at(i) > '9') {
            res.append(input.at(i));
            i++;
            continue;
        }
        int q = input.mid(i, 3).toInt();
        res.append(q % 300);
        i += 3;
    }

    return codec->toUnicode(res);
}

bool Decoder::decodeFile(QString inFile, QString outFile)
{
    if (!QFile::exists(inFile)) return false;
    QFile inputFile(inFile);
    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "Input file not found at" + inFile.toStdString() << std::endl;
        return false;
    }
    QFile outputFile(outFile);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "Can't create output file at" + outFile.toStdString() << std::endl;
        return false;
    }
    QTextStream out(&outputFile);
    while (!inputFile.atEnd()) {
        QByteArray line = inputFile.readLine();
        out << decodeString(line);
    }
    outputFile.close();
}
