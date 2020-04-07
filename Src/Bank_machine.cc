/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 10:18:39 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 06:20:29
 */

#include "../Include/Bank_machine.h"


using namespace MESSAGE;

void Bank_machine::run()
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
            );
        }
    }catch(MESSAGE::Close_queue const &){}
}
