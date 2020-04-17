/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:51:04 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 10:51:28
 */
#pragma once

#include "messaging.h"

namespace messaging{
    class sender{
            messaging::queue *q_;
        public:
            sender():q_(nullptr)
            {}

            explicit sender(messaging::queue *q):q_(q)
            {}

            template<typename Msg>
            void send(Msg const &msg)
            {
                q_->push(msg);
            }
    };
}