/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 10:18:39 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 10:59:16
 */

#include "receiver.h"
#include "withdraw.h"


using namespace messaging;

class bank_machine
{
private:
    /* data */
    receiver incoming;
    unsigned balance;

public:
    bank_machine(/* args */):
    balance(199)
    {}

    messaging::sender get_sender()
    {
        return incoming;
    }

    void done()
    {
        get_sender().send(close_queue());
    }

    void run()
    {
        try{
            for(;;){
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
                );   //! 这里创建了多个临时对象，这些临时对象的析构函数在遇到";"时才会执行，并不是创建后立刻执行，
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
            //wait_and_dispatch
            }
        }catch(messaging::close_queue const &){}
    }
    ~bank_machine(){}
};
