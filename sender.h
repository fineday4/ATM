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
        private:
            messaging::queue* q;
        public:
            sender():
                q(nullptr)
            {}

            explicit sender(messaging::queue *q_)
            :q(q_)
            {}

            template<typename Message>
            void send(Message const & msg)
            {
                if(q){
                    q->push(msg);
                }
            }            
    };
}