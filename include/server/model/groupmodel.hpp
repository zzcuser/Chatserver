#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "group.hpp"
#include <string>
#include <vector>
using namespace std;

class GroupModel
{
public:
    // 创建群组
    bool createGroup(Group& group);
    
    // 加入群组
    void addGroup(int userid, int groupid, string role);

    // 查询用户所在群组信息
    vector<Group> queryGroups(int userid);

    // 根据指定的群组id，查询该群组用户id列表
    vector<int> queryGroupUsers(int userid, int groupid);
};

#endif