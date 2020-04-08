/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:42:32 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-09 06:44:14
 */

#include"../Include/Msg_queue.h"

using namespace MESSAGE;


std::shared_ptr<message_base> Msg_queue::wait_and_pop()
{
    std::unique_lock<std::mutex> lk(m);
    c.wait(lk, [&](){
        return !q.empty();
    });
    auto res = q.front();
    q.pop();
    return res;
}