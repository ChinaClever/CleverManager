/*
 * usrjson.cpp
 *  用户信息的保存
 *  用户信息保存以json格式进行保存
 *  对用户信息新增、读取、修改、保存
 *
 *  Created on: 2016年10月11日
 *      Author: Lzy
 */
#include "usrjson.h"
#include "usrclass.h"

#define USR_JSON_FN  "usr.json"

extern UsrMaptype g_usrMap;

UsrJson::UsrJson()
{
    
}

UsrJson::~UsrJson()
{
    
}


bool UsrJson::readFile(QByteArray &json)
{
    bool ret;
    
    QFile file(cm::pathOfData(USR_JSON_FN));
    ret = file.open(QIODevice::ReadOnly);
    if(ret)
    {
        QDataStream in(&file);
        in >> json ;
        file.close();
    }
    else
        qDebug() << "ReadJsonFile:" << USR_JSON_FN <<"Err";
    
    return ret;
}

bool UsrJson::writeFile(QByteArray &json)
{
    bool ret;
    
    QFile file(cm::pathOfData(USR_JSON_FN));
    ret = file.open(QIODevice::WriteOnly);
    if(ret)
    {
        QDataStream out(&file);
        out << json;
        file.close();// 如果不想关闭文件，可以使用 file.flush();
    }
    else
        qDebug() << "WriteJsonFile:" << USR_JSON_FN <<"Err";
    
    return ret;
}

bool UsrJson::readJson(void)
{
    QByteArray json;
    
    bool ret = readFile(json);
    if(ret == false)
        return ret;
    
    QJsonParseError error;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json, &error);
    if (error.error == QJsonParseError::NoError) {
        if (!(jsonDocument.isNull() || jsonDocument.isEmpty())) {
            if (jsonDocument.isObject()) {
                
                QVariantMap result = jsonDocument.toVariant().toMap();
                
                for(int i=0; i<result.size(); ++i)
                {
                    QString str = QString::number(i);
                    QVariantMap nestedMap = result[str].toMap();
                    
                    UsrStruct *usr = new UsrStruct;
                    usr->jurisdiction = nestedMap["Jur"].toBool();
                    usr->name = nestedMap["name"].toString();
                    usr->pwd = nestedMap["pwd"].toString();
                    usr->email = nestedMap["email"].toString();
                    usr->telephone = nestedMap["telephone"].toString();
                    usr->remarks = nestedMap["remarks"].toString();
                    g_usrMap.insert(usr->name, usr);
                }
            }
        }
        
    } else {
        qDebug() << error.errorString().toUtf8().constData();// 检查错误类型
        return false;
    }
    
    return true;
}

/**
 * 功能：更新Json文档
 */
bool UsrJson::writeJson(void)
{
    int i=0;
    QMap<QString, QVariant> map;
    
    QMap<QString,UsrStruct*>::iterator it;
    for ( it = g_usrMap.begin(); it != g_usrMap.end(); ++it )
    {
        QVariantMap bob;
        
        bob.insert("name", it.value()->name);
        bob.insert("pwd", it.value()->pwd);
        bob.insert("Jur", it.value()->jurisdiction);
        bob.insert("telephone", it.value()->telephone);
        bob.insert("email", it.value()->email);
        bob.insert("remarks", it.value()->remarks);
        
        QString str = QString::number(i++);
        map.insert(str,bob);
    }
    
    QJsonDocument jsonDocument = QJsonDocument::fromVariant(QVariant(map));
    if (!jsonDocument.isNull()) {
        QByteArray json = jsonDocument.toJson();
        writeFile(json);
    }
    
    return true;
}
