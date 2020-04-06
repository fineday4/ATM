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
            messaging::queue q;
        
        public:
            operator sender(){
                return sender(&q);
            }

            dispatcher wait(){  //* receiver函数如何实现阻塞等待的?
                return dispatcher(&q);  //* dispatcher(&q)创建了一个临时对象，它创建后就会调用dispatcher的析构函数。disppatcher的析构函数是执行阻塞等待的逻辑部分
            }
    };
}