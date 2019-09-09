#ifndef LANDINGUSER_H
#define LANDINGUSER_H
#include "dbuserlog.h"

QString land_user_name();

class LandingUser
{
    LandingUser();
public:
    static LandingUser* get();
    sUserItem user;
    bool land;

protected:
    void init();

};

#endif // LANDINGUSER_H
