#include "usermodel.hpp"
#include "db.h"
#include<iostream>
using namespace std;

bool UserModel::insert(User& user)
{
    //增刪改查之其增
    // 1.組裝sql語句
    char sql[1024] = {0};
    // ALTER TABLE USER MODIFY column id int AUTO_INCREMENT
    sprintf(sql, "insert into USER(name, password, state) values('%s', '%s', '%s')",
     user.getName().c_str(), user.getPwd().c_str(), user.getState().c_str());
    
    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            // 獲取插入成功的用戶數據生成的主鍵id
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
}

// 根据用户id查询用户信息
User UserModel::query(int id)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from USER where id = %d", id);

    MySQL mysql;
    if (mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if (res != nullptr) {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }

    return User();
}

bool UserModel::updateState(User user)
{
    // 1.組裝sql語句
    char sql[1024] = {0};
    // ALTER TABLE USER MODIFY column id int AUTO_INCREMENT
    sprintf(sql, "update USER set state = '%s' where id = %d",
     user.getState().c_str(), user.getId());
    
    MySQL mysql;
    if (mysql.connect()) {
        if (mysql.update(sql)) {
            return true;
        } else {
            return false;
        }
    }
}

// 重置用户的状态信息
void UserModel::resetState()
{
    // 1.組裝sql語句
    char sql[1024] = "update USER set state = 'offline' where id = 'online'";
    
    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}