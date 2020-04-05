/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 07:00:48 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 10:51:44
 */
#pragma once

#include "messaging.h"

namespace messaging{
    class close_queue{};

    class dispatcher{
        private:
            queue* q;
            bool chained;
            dispatcher(dispatcher const&) = delete;
            dispatcher& operator=(dispatcher const&) = delete;

            template<typename Dispatch, typename Msg, typename Func>
            friend class TemplateDispatcher;
            void wait_and_dispatch()
            {
                for(;;){
                    auto msg = q->wait_and_pop();
                    dispatch(msg);
                }
            }

            bool dispatch(std::shared_ptr<message_base> const& msg){
                if(dynamic_cast<wrapped_message<close_queue>* >(msg.get())){
                    throw close_queue();
                }
                return false;
            }

        public:
            dispatcher(dispatcher&& other):
            q(other.q),chained(other.chained){
                other.chained = true;
            }

            explicit dispatcher(queue *q_):
            q(q_), chained(false){}

            template<typename Message, typename Func>
            TemplateDispatcher<dispatcher, Message, Func>
            handle(Func &&f){
                return TemplateDispatcher<dispatcher, Message, Func>(
                    q, this, std::forward<Func>(f)
                );
            }
    };
}