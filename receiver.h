/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:54:57 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 10:51:56
 */
#pragma once

#include "sender.h"
#include "dispatcher.h"

namespace messaging{
    class receiver{
        private:
            queue q;
        
        public:
            operator sender(){
                return sender(&q);
            }

            dispatcher wait(){
                return dispatcher(&q);
            }
    };
}