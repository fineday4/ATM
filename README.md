## 编译：
    g++ main.cpp -std=c++11 -lpthread

## 文件间的依赖关系
messaging.h <--依赖--sender.h <--依赖-- withdraw.h
TemplateDispatcher.h <--依赖--dispatcher.h

## handle的实现机制
''' dispatcher类的handle定义

    template<typename Msg, typename Func>
    TemplateDispatcher<dispatcher, Msg, Func>
    handle(Func &&f){
        return TemplateDispatcher<dispatcher, Msg, Func>(q, this, std::forward<Func>(f));
    }

''' TemplateDispatcher类的handle定义

    template<typename OtherMsg, typename OtherFunc>
    TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>
    handle(OtherFunc &&of)
    {
        return TemplateDispatcher<TemplateDispatcher, OtherMsg, OtherFunc>(
            q, this, std::forward<OtherFunc>(of));
    }

‘’‘

'''使用
    incoming.wait()
    .handle<card_inserted>(
        [&](card_inserted const &msg)
        {
            account = msg.account;
            pin = "";
            interface_hardware.send(display_enter_pin());
            state = &atm::getting_pin;
        }
    );
’‘’
handle的调用incoming.wait.handle.handle.handle...
dispatcher的handle使用过是incoming.wait.handle的第一个handle调用的。TemplateDispatcher的handle是由incoming.wait.handle.handle第一个之后的hangle调用的。
handle的作用就是创建一个新的dispacther/TemplaterDispatcher处理新收到的数据，这个的创建意味着旧的数据处理完了，新的数据需要处理。

            
------------------2020.04.17 理解-----------------
bank_machine.cpp代码摘要：  
                incoming.wait()
                .handle<verify_pin>(
                    [&](verify_pin const &msg){
                        if(msg.pin == "1937"){
                            msg.atm_queue.send(pin_verified());
                        }else{
                            msg.atm_queue.send(pin_incorrect());
                        }
                    }
                )
                .handle<withdraw>(
                    [&](withdraw const &msg)
                    {
                        if(balance >= msg.amount){
                            msg.atm_queue.send(withdraw_ok());
                            balance -= msg.amount;
                        }else{
                            msg.atm_queue.send(withdraw_denied());
                        }
                    }
                )
                .handle<get_balance>(
                    [&](get_balance const &msg)
                    {
                        msg.atm_queue.send(::balance(balance));
                    }
                )
                .handle<withdrawal_processed>(
                    [&](withdrawal_processed const &msg)
                    {
                    }
                )
                .handle<cancel_withdrawal>(
                    [&](cancel_withdrawal)
                    {}
                );  
                //! 这里创建了多个临时对象，这些临时对象的析构函数在遇到";"时才会执行，并不是创建后立刻执行，
                //! 创建的多个临时对象是通过：第一个对象receiver->wait创建的，第二个对象通过dispatcher->handle，之后的对象都是
                //!通过TemplateDispatcher->handle创建的。这些创建的临时对象时候最后一个对象的chained = false，之前的所有临时对象
                //!都是chained = true()，
                //!-----------------------下面的代码就是证据------------------------
                //? TemplateDispatcher对象创建临时对象的时候，将上一个临时对象的chained赋值为true了。

                            //*TemplateDispatcher(messaging::queue *q_, PreviousDispatcher *prev_, Func &&f_):
                            //*q(q_), prev(prev_), f(std::forward<Func>(f_)), chained(false)
                            //* {
                            //*     prev_->chained = true;
                            //* }

                //? dispatcher对象创建临时对象是，将上一个临时对象的chained赋值为true了。

                            //*  dispatcher(dispatcher &&other)
                            //* :q(other.q), chained(false)
                            //* {
                            //*     other.chained = true;
                            //* }
            //!-----------------------证据结束-----------------------------------
            //!遇到“;”之前就完成了所有临时对象的创建，遇到“;”后就开始执行析构函数。各个对象执行析构函数的顺序
            //!和创建的顺序相反——是一个压栈（创建）和出栈（析构）的过程。所以先执行最后一个临时对象的析构函数，
            //!chained = false。所以进入TemplateDispatcher->wait_and_dispatch。
            //wait_and_dispatch的实现：
                for(;;)
                {
                    auto msg = q->wait_and_pop();
                    if(dispatch(msg))
                        break;
                }
            所以最后一个临时对象阻塞取出消息后进入dispatch函数处理数据。
            //dispatch函数的实现：
                if(wrapped_message<Msg> *wrapper = 
                dynamic_cast<wrapped_message<Msg>*>(msg.get()))
                {
                    f(wrapper->contents);  //第一种情况 -》 126行
                    return true;
                }else{
                    return prev->dispatch(msg);  //第二种情况 -》131行
                }
            即：将取到的消息动态类型转换为该模板对象创建时传入的消息类型Msg的指针，
            第一种情况：可以转换，说明是当前处理的消息类型，则调用传入的函数处理完后
                return true.返回wait_and_dispatch函数，break跳出循环，析构函
                数运行结束，该临时对象析构掉。在该临时对象之前的所有临时对象也依次执行
                析构函数，又出它们的chained = true，所以没有进入wait_and_dispatch就
                直接析构掉了。for循环的一次循环结束。
            第二种情况：不可以转换，说明当前对象无法处理传入的消息，只能递归调用之前的临时对象的
                dispatch函数是否有可处理的当前消息的临时对象。如果有就会在依次在dispatch中返回
                true,然后实现了临时对象出栈析构。