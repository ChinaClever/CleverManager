#define SOFTWAREREGISTRATION_H

#include "registerlib_global.h"

class REGISTERLIBSHARED_EXPORT SoftwareRegiste
{
public:
    SoftwareRegiste();
    QString getWMIHWInfo(int type);
};

#endif // SOFTWAREREGISTRATION_H

