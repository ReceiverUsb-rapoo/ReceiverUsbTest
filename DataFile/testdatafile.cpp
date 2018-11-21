#include "testdatafile.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

bool TestDataFile::GetTestRecordData(int &n_TestTotal,
                                     int &n_RetestTotal,
                                     int &n_PassTotal,
                                     int &n_FaileTotal,
                                     int &n_TestNumber)
{
    QString str_FilePath = QDir::currentPath() +
                           TestRecordDataPath;

    QDir o_QDir(str_FilePath);
    if(!o_QDir.exists()){
        o_QDir.mkpath(str_FilePath);
    }

    str_FilePath += "/" + TestRecordDataFileName;

    QFile o_QFileConfig(str_FilePath);

    if(!o_QFileConfig.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }

    QXmlStreamReader o_QXmlStreamReader(&o_QFileConfig);
    o_QXmlStreamReader.readNext();
    while(!o_QXmlStreamReader.atEnd()){
        QXmlStreamReader::TokenType Token = o_QXmlStreamReader.readNext();
        if(Token == QXmlStreamReader::StartDocument){
            continue;
        }

        if(Token == QXmlStreamReader::EndElement){
            continue;
        }

        if(Token == QXmlStreamReader::Characters){
            continue;
        }

        if(o_QXmlStreamReader.name() == "n_TestTotal"){
            o_QXmlStreamReader.readNext();
            n_TestTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_RetestTotal"){
            o_QXmlStreamReader.readNext();
            n_RetestTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_PassTotal"){
            o_QXmlStreamReader.readNext();
            n_PassTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_FaileTotal"){
            o_QXmlStreamReader.readNext();
            n_FaileTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_TestNumber"){
            o_QXmlStreamReader.readNext();
            n_TestNumber = o_QXmlStreamReader.text().toInt();
        }
    }

    if(o_QXmlStreamReader.hasError()){
        qDebug() << QString::fromLocal8Bit("GetTestRecordData 错误信息：%1  行号：%2  列号：%3  字符位移：%4").
                    arg(o_QXmlStreamReader.errorString()).
                    arg(o_QXmlStreamReader.lineNumber()).
                    arg(o_QXmlStreamReader.columnNumber()).
                    arg(o_QXmlStreamReader.characterOffset());
        return false;
    }

    return true;
}

bool TestDataFile::SaveTestRecordData(const int &n_TestTotal,
                                      const int &n_RetestTotal,
                                      const int &n_PassTotal,
                                      const int &n_FaileTotal,
                                      const int &n_TestNumber)
{
    QString str_FilePath = QDir::currentPath() +
                           TestRecordDataPath;

    QDir o_QDir(str_FilePath);
    if(!o_QDir.exists()){
        o_QDir.mkpath(str_FilePath);
    }

    str_FilePath += "/" + TestRecordDataFileName;

    QFile o_QFileConfig(str_FilePath);

    if(!o_QFileConfig.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }

    QXmlStreamWriter o_QXmlStreamWriter(&o_QFileConfig);
    o_QXmlStreamWriter.setAutoFormatting(true);
    o_QXmlStreamWriter.writeStartDocument();
    o_QXmlStreamWriter.writeStartElement("TestRecordData");

    o_QXmlStreamWriter.writeTextElement("n_TestTotal", QString::number(n_TestTotal));
    o_QXmlStreamWriter.writeTextElement("n_RetestTotal", QString::number(n_RetestTotal));
    o_QXmlStreamWriter.writeTextElement("n_PassTotal", QString::number(n_PassTotal));
    o_QXmlStreamWriter.writeTextElement("n_FaileTotal", QString::number(n_FaileTotal));
    o_QXmlStreamWriter.writeTextElement("n_TestNumber", QString::number(n_TestNumber));

    o_QXmlStreamWriter.writeEndElement();
    o_QXmlStreamWriter.writeEndDocument();
    o_QFileConfig.close();
    return true;
}
