/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 07:00:48 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-09 07:03:31
 */
#pragma once

#include "Msg_queue.h"
#include "TemplateDispatcher.h"

namespace MESSAGE{
    class Close_queue{};

    class Dispatcher{
            Msg_queue* q;
            bool chained;
            Dispatcher(Dispatcher const&) = delete;
            Dispatcher& operator=(Dispatcher const&) = delete;

            template<typename Dispatcher, typename Msg, typename Func>
            friend class TemplateDispatcher;
            
            void wait_and_dispatch();

            bool dispatch(std::shared_ptr<message_base> const& msg);

        public:
            Dispatcher(Dispatcher&& other);

            explicit Dispatcher(Msg_queue *q_);

        template<typename Message, typename Func>
        TemplateDispatcher<Dispatcher, Message, Func> handle(Func &&f)
        {
            return TemplateDispatcher<Dispatcher, Message, Func>(
                q, this, std::forward<Func>(f)
            );
        }

            ~Dispatcher() noexcept(false)
            {
                if(!chained){//* chained的初值为false,chained如何变为true的?
                    wait_and_dispatch(); //* chained的值只有在Dispatcher(Dispatcher&& other)为true.该部分以抛出异常结束的
                }
            }
    };
}