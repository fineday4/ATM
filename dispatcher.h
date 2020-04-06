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
            messaging::queue* q;
            bool chained;
            dispatcher(dispatcher const&) = delete;
            dispatcher& operator=(dispatcher const&) = delete;

            template<typename Dispatcher, typename Msg, typename Func>
            friend class TemplateDispatcher;
            
            void wait_and_dispatch()
            {
                for(;;){
                    auto msg = q->wait_and_pop();  //*实现阻塞等待
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

            ~dispatcher() noexcept(false)
            {
                if(!chained){//* chained的初值为false,chained如何变为true的?
                    wait_and_dispatch(); //* chained的值只有在dispatcher(dispatcher&& other)为true.该部分以抛出异常结束的
                }
            }
    };
}