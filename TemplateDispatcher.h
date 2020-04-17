/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-04 22:18:35 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-06 07:50:23
 */
#pragma once

#include <iostream>
#include "messaging.h"

namespace messaging{
    
    template<typename PreviousDispatcher, typename Msg, typename Func>
    class TemplateDispatcher
    {
            messaging::queue *q;
            PreviousDispatcher *prev;
            Func f;
            bool chained;
            
            TemplateDispatcher(TemplateDispatcher const &) = delete;
            TemplateDispatcher &operator=(TemplateDispatcher const &) = delete;
            
            template<typename Dispatcher, typename OtherMsg, typename OtherFunc>
            friend class TemplateDispatcher;

            void wait_and_dispatch()
            {
                for(;;)
                {
                    auto msg = q->wait_and_pop();
                    if(dispatch(msg))
                        break;
                }
            }

            bool dispatch(std::shared_ptr<message_base> const &msg)
            {
                if(wrapped_message<Msg> *wrapper = 
                dynamic_cast<wrapped_message<Msg>*>(msg.get()))
                {
                    f(wrapper->contents);
                    return true;
                }else{
                    return prev->dispatch(msg);  //TODO: 当前没有数据时执行？？？？
                }
            }

        public:
            TemplateDispatcher(TemplateDispatcher &&other)
            :q(other.q), prev(other.prev), f(std::move(other.f)),chained(other.chained)
            {
                other.chained = true;
            }

            TemplateDispatcher(messaging::queue *q_, PreviousDispatcher *prev_, Func &&f_):
            q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false)
            {
                prev_->chained = true;
            }

            template<typename OtherMsg, typename OtherFunc>
            TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
            handle(OtherFunc &&of)
            {
                return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(
                    q, this, std::forward<OtherFunc>(of));
            }
            
            ~TemplateDispatcher() noexcept(false)
            {
                if(!chained){  //TODO:析构函数中chained = true才结束，单这并不是循环而是判断。所以prev并不是析够掉了，而是一直在wait_and_dispatch中
                    wait_and_dispatch();
                }
            }
    };
} // namespace messaging
