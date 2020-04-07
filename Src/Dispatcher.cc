/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-03 07:00:48 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 07:19:47
 */
#include "../Include/Dispatcher.h"
using namespace MESSAGE;
            
void Dispatcher::wait_and_dispatch()
{
    for(;;){
        auto msg = q->wait_and_pop();  //*实现阻塞等待
        dispatch(msg);
    }
}

bool Dispatcher::dispatch(std::shared_ptr<message_base> const& msg){
    if(dynamic_cast<wrapped_message<Close_queue>* >(msg.get())){
        throw Close_queue();
    }
    return false;
}

template<typename Message, typename Func>
TemplateDispatcher<Dispatcher, Message, Func>  Dispatcher::handle(Func &&f){
    return TemplateDispatcher<Dispatcher, Message, Func>(
        q, this, std::forward<Func>(f)
    );
}