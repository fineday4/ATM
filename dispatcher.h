/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 07:00:48 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-06 07:55:52
 */
#pragma once

#include "messaging.h"
#include "TemplateDispatcher.h"

namespace messaging{
    class close_queue{};

    class dispatcher{
        
            messaging::queue *q;
            bool chained;
            dispatcher(dispatcher const &)=delete;
            dispatcher& operator=(dispatcher const &)=delete;

                        void wait_and_dispatch()
            {
                for(;;)
                {
                    auto msg = q->wait_and_pop();
                    dispatch(msg);
                }
            }

            bool dispatch(std::shared_ptr<message_base> const &msg)
            {
                if(dynamic_cast<wrapped_message<close_queue>* >(msg.get()))
                {
                    throw close_queue();
                }

                return false;
            }

            template<typename Dispather, typename Msg, typename Func>
            friend class TemplateDispatcher;

        public:

//! 实现的难点就在这里！！！看看这里是怎么实现的？？？
            template<typename Msg, typename Func>
            TemplateDispatcher<dispatcher, Msg, Func>
            handle(Func &&f){
                return TemplateDispatcher<dispatcher, Msg, Func>(q, this, std::forward<Func>(f));
            }
            
            dispatcher(dispatcher &&other)
            :q(other.q), chained(false)
            {
                other.chained = true;
            }

            explicit dispatcher(messaging::queue *q_)
            :q(q_), chained(false)
            {}

            ~dispatcher()
            {
                if(!chained)
                {
                    wait_and_dispatch();
                }
            }
    };
}