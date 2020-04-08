/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 06:54:57 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-09 07:04:05
 */
#pragma once

#include "Sender.h"
#include "Dispatcher.h"

namespace MESSAGE{
    class Receiver{
        private:
            MESSAGE::Msg_queue q;
        
        public:
            operator Sender(){
                return Sender(&q);
            }

            Dispatcher wait(){  //* Receiver函数如何实现阻塞等待的?
                return Dispatcher(&q);  //* Dispatcher(&q)创建了一个临时对象，它创建后就会调用Dispatcher的析构函数。disppatcher的析构函数是执行阻塞等待的逻辑部分
            }
    };
}