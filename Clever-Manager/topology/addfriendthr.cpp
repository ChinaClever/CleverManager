#include "addfriendthr.h"
#include "zebra/zebra_client.h"

AddFriendThr::AddFriendThr(const QString invite_code, const QString friend_name)
    : m_code(invite_code)
    , m_name(friend_name)
{

}

void AddFriendThr::run()
{
    int ret = zebra_client::get_instance()->add_friend(m_code, m_name);
    if(ret < 0)
    {
        emit(add_friend_result(false));
    }
    else
    {
        emit(add_friend_result(true));
    }
}
