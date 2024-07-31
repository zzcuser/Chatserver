#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "redis.hpp"
#include <unordered_map>
#include <functional>
#include <mutex>

using namespace std;
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

using MsgHandler = function<void(const TcpConnectionPtr& conn, json& js, Timestamp time)>;

// 聊天服務器業務類
class ChatService
{
public:
    // 暴露一個接口，該接口是獲取單例對象的接口
    static ChatService* instance();
    // 處理登錄業務
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 處理注冊業務
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // 獲取消息對應的handler處理器
    MsgHandler getHandler(int msgid);
    // 从redis消息队列中获取订阅的消息
    void handleRedisSubscribeMessage(int, string);
    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);

    // 一对一聊天业务
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 添加好友业务
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 加入群组业务
    void addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 群组聊天业务
    void groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time);

    void loginout(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 服务器异常，业务重置方法
    void reset();
private:
    // 單例
    ChatService();
    // 存儲消息id和對應業務事件的處理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 定义互斥锁，保证_userConnMap的线程安全
    mutex _connMutex;

    // 存储在线用户的连接
    unordered_map<int, TcpConnectionPtr> _userConnectionMap;

    // 數據操作類對象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    Redis _redis;
};


#endif