/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:42:32 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-09 06:44:45
 */

#pragma once

#include<mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace MESSAGE{
    struct message_base
    {
        /* data */
        virtual ~message_base(){}
    };

    template<typename Msg>
    struct wrapped_message:
        message_base
    {
        /* data */
        Msg contents;
        explicit wrapped_message(Msg const & contents_):
            contents(contents_)
            {}
    };

    class Msg_queue{
        private:
            std::mutex m;
            std::condition_variable c;
            std::queue<std::shared_ptr<message_base> > q;
        public:
            
            template<typename T>
            void push(T const& msg){
                std::lock_guard<std::mutex> lk(m);
                q.push(std::make_shared<wrapped_message<T> > (msg) );
                c.notify_all();
            }

            std::shared_ptr<message_base> wait_and_pop();
            
    };
    
    
}