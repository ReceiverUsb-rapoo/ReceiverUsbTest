#include "testdatafile.h"
#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include "logfile.h"

bool TestDataFile::GetCurrentTestRecordData(RECORDDATA &CurrentData)
{
    QString str_FilePath = QDir::currentPath() +
                           TestRecordDataPath;

    QDir o_QDir(str_FilePath);
    if(!o_QDir.exists()){
        o_QDir.mkpath(str_FilePath);
    }

    str_FilePath += "/" + CurrentTestRecordDataFileName;

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
            CurrentData.n_TestTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_RetestTotal"){
            o_QXmlStreamReader.readNext();
            CurrentData.n_RetestTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_PassTotal"){
            o_QXmlStreamReader.readNext();
            CurrentData.n_PassTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_FaileTotal"){
            o_QXmlStreamReader.readNext();
            CurrentData.n_FaileTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_TestNumber"){
            o_QXmlStreamReader.readNext();
            CurrentData.n_TestNumber = o_QXmlStreamReader.text().toInt();
        }
    }

    if(o_QXmlStreamReader.hasError()){
        QString str_ErrorInfo = QString::fromLocal8Bit("GetCurrentTestRecordData xml 错误信息：%1  行号：%2  列号：%3  字符位移：%4").
                                arg(o_QXmlStreamReader.errorString()).
                                arg(o_QXmlStreamReader.lineNumber()).
                                arg(o_QXmlStreamReader.columnNumber()).
                                arg(o_QXmlStreamReader.characterOffset());
        qDebug()<<str_ErrorInfo;

        LogFile::Addlog(str_ErrorInfo);

        return false;
    }

    return true;
}

bool TestDataFile::SaveCurrentTestRecordData(const RECORDDATA &CurrentData)
{
    QString str_FilePath = QDir::currentPath() +
                           TestRecordDataPath;

    QDir o_QDir(str_FilePath);
    if(!o_QDir.exists()){
        o_QDir.mkpath(str_FilePath);
    }

    str_FilePath += "/" + CurrentTestRecordDataFileName;

    QFile o_QFileConfig(str_FilePath);

    if(!o_QFileConfig.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }

    QXmlStreamWriter o_QXmlStreamWriter(&o_QFileConfig);
    o_QXmlStreamWriter.setAutoFormatting(true);
    o_QXmlStreamWriter.writeStartDocument();
    o_QXmlStreamWriter.writeStartElement("TestRecordData");

    o_QXmlStreamWriter.writeTextElement("n_TestTotal", QString::number(CurrentData.n_TestTotal));
    o_QXmlStreamWriter.writeTextElement("n_RetestTotal", QString::number(CurrentData.n_RetestTotal));
    o_QXmlStreamWriter.writeTextElement("n_PassTotal", QString::number(CurrentData.n_PassTotal));
    o_QXmlStreamWriter.writeTextElement("n_FaileTotal", QString::number(CurrentData.n_FaileTotal));
    o_QXmlStreamWriter.writeTextElement("n_TestNumber", QString::number(CurrentData.n_TestNumber));

    o_QXmlStreamWriter.writeEndElement();
    o_QXmlStreamWriter.writeEndDocument();
    o_QFileConfig.close();
    return true;
}

bool TestDataFile::GetTestTotalRecordData(RECORDDATA &TotalData)
{
    QString str_FilePath = QDir::currentPath() +
                           TestRecordDataPath;

    QDir o_QDir(str_FilePath);
    if(!o_QDir.exists()){
        o_QDir.mkpath(str_FilePath);
    }

    str_FilePath += "/" + TestTotalRecordDataFileName;

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
            TotalData.n_TestTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_RetestTotal"){
            o_QXmlStreamReader.readNext();
            TotalData.n_RetestTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_PassTotal"){
            o_QXmlStreamReader.readNext();
            TotalData.n_PassTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_FaileTotal"){
            o_QXmlStreamReader.readNext();
            TotalData.n_FaileTotal = o_QXmlStreamReader.text().toInt();
        }
        else if(o_QXmlStreamReader.name() == "n_TestNumber"){
            o_QXmlStreamReader.readNext();
            TotalData.n_TestNumber = o_QXmlStreamReader.text().toInt();
        }
    }

    if(o_QXmlStreamReader.hasError()){
        QString str_ErrorInfo = QString::fromLocal8Bit("GetTestTotalRecordData xml 错误信息：%1  行号：%2  列号：%3  字符位移：%4").
                                arg(o_QXmlStreamReader.errorString()).
                                arg(o_QXmlStreamReader.lineNumber()).
                                arg(o_QXmlStreamReader.columnNumber()).
                                arg(o_QXmlStreamReader.characterOffset());
        qDebug()<<str_ErrorInfo;

        LogFile::Addlog(str_ErrorInfo);

        return false;
    }

    return true;
}

bool TestDataFile::SaveTestTotalRecordData(const RECORDDATA &TotalData)
{
    QString str_FilePath = QDir::currentPath() +
                           TestRecordDataPath;

    QDir o_QDir(str_FilePath);
    if(!o_QDir.exists()){
        o_QDir.mkpath(str_FilePath);
    }

    str_FilePath += "/" + TestTotalRecordDataFileName;

    QFile o_QFileConfig(str_FilePath);

    if(!o_QFileConfig.open(QIODevice::WriteOnly | QIODevice::Text)){
        return false;
    }

    QXmlStreamWriter o_QXmlStreamWriter(&o_QFileConfig);
    o_QXmlStreamWriter.setAutoFormatting(true);
    o_QXmlStreamWriter.writeStartDocument();
    o_QXmlStreamWriter.writeStartElement("TestRecordData");

    o_QXmlStreamWriter.writeTextElement("n_TestTotal", QString::number(TotalData.n_TestTotal));
    o_QXmlStreamWriter.writeTextElement("n_RetestTotal", QString::number(TotalData.n_RetestTotal));
    o_QXmlStreamWriter.writeTextElement("n_PassTotal", QString::number(TotalData.n_PassTotal));
    o_QXmlStreamWriter.writeTextElement("n_FaileTotal", QString::number(TotalData.n_FaileTotal));
    o_QXmlStreamWriter.writeTextElement("n_TestNumber", QString::number(TotalData.n_TestNumber));

    o_QXmlStreamWriter.writeEndElement();
    o_QXmlStreamWriter.writeEndDocument();
    o_QFileConfig.close();
    return true;
}

void TestDataFile::PooledData()
{
    RECORDDATA CurrentData;
    RECORDDATA TotalData;

    GetCurrentTestRecordData(CurrentData);
    GetTestTotalRecordData(TotalData);

    TotalData.n_TestTotal += CurrentData.n_TestTotal;
    TotalData.n_RetestTotal += CurrentData.n_RetestTotal;
    TotalData.n_PassTotal += CurrentData.n_PassTotal;
    TotalData.n_FaileTotal += CurrentData.n_FaileTotal;
    TotalData.n_TestNumber += CurrentData.n_TestNumber;

    CurrentData.n_TestTotal = 0;
    CurrentData.n_RetestTotal = 0;
    CurrentData.n_PassTotal = 0;
    CurrentData.n_FaileTotal = 0;
    CurrentData.n_TestNumber = 0;

    SaveCurrentTestRecordData(CurrentData);
    SaveTestTotalRecordData(TotalData);
}
