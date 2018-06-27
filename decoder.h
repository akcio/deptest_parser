#ifndef DECODER_H
#define DECODER_H

#include <QString>
#include <QByteArray>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include <iostream>
//#include <string>

using namespace std;

class Decoder
{
public:
    Decoder();

    /**
     * @brief Translate encoded string
     * @param encoded string
     * @return decoded string
     */
    static QString decodeString(QString input);
    /**
     * @brief Decode encoded file
     * @param path to encoded file
     * @param decoded file
     * @return true - OK, false - some errors
     */
    static bool decodeFile(QString inFile, QString outFile);

};

#endif // DECODER_H
