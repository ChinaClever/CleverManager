#ifndef USRJSON_H
#define USRJSON_H
#include <QtCore>
#include "common.h"

class UsrJson
{
public:
    UsrJson();
    ~UsrJson();

    bool writeJson(void);
    bool readJson(void);

protected:
    bool writeFile(QByteArray &);
    bool readFile(QByteArray &);

};

#endif // USRJSON_H
