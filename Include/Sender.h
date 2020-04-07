/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:51:04 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 06:51:04
 */
#pragma once

#include "Msg_queue.h"

namespace MESSAGE{
    class Sender{
        private:
            MESSAGE::Msg_queue* q;
        public:
            Sender():
                q(nullptr)
            {}

            explicit Sender(MESSAGE::Msg_queue *q_)
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