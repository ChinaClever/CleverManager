#include "iconpath.h"

/**
 * @brief 设备状态图标
 * @param id
 * @param path
 * @return
 */
bool icon_get_devStatus(int id, QString &path)
{
    bool ret = true;

    switch (id)
    {
    case 0: // 正常图标
        path = ":/devstatus/dev/devnormal.jpg";
        break;

    case 1: // 报警图标
        path = ":/devstatus/dev/devalarm.jpg";
        break;

    case 2: // 离线
        path = ":/devstatus/dev/devoffline.jpg";
        break;

    default:
        ret = false;
        break;
    }

    return ret;
}

QString icon_get_devType(void)
{
    return ":/images/devType.png";
}

QString icon_get_group(void)
{
    return ":/images/group.png";
}

QString icon_get_devIcon(void)
{
//    return ":/devstatus/dev/dev.jpg";
    return 0;
}


/**
 * @brief 输出位状态
 * @param id
 * @param path
 * @return
 */
bool icon_get_opStatus(int id, QString &path)
{
    bool ret = true;

    switch (id)
    {
    case 0: // 正常图标
        path = ":/output/output/opnormal.jpg";
        break;

    case 1: // 报警图标
        path = ":/output/output/opalarm.jpg";
        break;

    case 2: // 打开
        path = ":/output/output/opopen.jpg";
        break;

    case 3: // 关闭
        path = ":/output/output/opclose.jpg";
        break;

    default:
        ret = false;
        break;
    }

    return ret;
}

