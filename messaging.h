/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:42:32 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-09 07:21:25
 */

#pragma once

#include<mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace messaging{

    struct message_base
    {
        virtual ~message_base(){}
    };

    template<typename Msg>
    struct wrapped_message:
        message_base
    {
        Msg contents;
        explicit wrapped_message(Msg const &content):
            contents(content)
        {}
    };

    class queue{
        private:
            std::mutex m_Mtx;
            std::condition_variable m_Cv;
            std::queue<std::shared_ptr<message_base> > m_MsgQueue;

        public:
            template<typename T>
            void push(T const &msg)
            {
                std::lock_guard<std::mutex> lk(m_Mtx);
                m_MsgQueue.push(std::make_shared<wrapped_message<T> >(msg));
                m_Cv.notify_all();
            }

             std::shared_ptr<message_base> wait_and_pop()
             {
                 std::unique_lock<std::mutex> lk(m_Mtx);

                 m_Cv.wait(lk,[&](){
                     return !m_MsgQueue.empty();
                 });

                 auto ret = m_MsgQueue.front();
                 m_MsgQueue.pop();
                 return ret;
             }
            
    };
}