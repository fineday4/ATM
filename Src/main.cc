/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 21:51:47 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-08 06:59:51
 */

#include <thread>
#include "../Include/Bank_machine.h"
#include "../Include/Interface_machine.h"
#include "../Include/Atm.h"

int main()
{
    Bank_machine bank;
    Interface_machine interface_hardware;
    Atm atm_machine(bank.get_sender(), interface_hardware.get_sender());
    
    std::thread bank_thread(&Bank_machine::run, &bank);   //* 
    std::thread if_thread(&Interface_machine::run, &interface_hardware);  //* 界面线程
    std::thread atm_thread(&Atm::run, &atm_machine);//* 

    MESSAGE::Sender atmqueue(atm_machine.get_sender());

    bool quit_pressed = false;
    while(!quit_pressed){
        char c = getchar();
        switch (c)
        {
            /* code */
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                atmqueue.send(digit_pressed(c));
                break;

            case 'b':
                atmqueue.send(balance_pressed());
                break;

            case 'w':
                atmqueue.send(withdraw_pressed(50));
                break;

            case 'c':
                atmqueue.send(cancel_pressed());
                break;

            case 'q':
                quit_pressed = true;
                break;

            case 'i':
                atmqueue.send(card_inserted("acc1234"));
                break;
            
        }
    }

    bank.done();
    atm_machine.done();
    interface_hardware.done();
    atm_thread.join();
    bank_thread.join();
    if_thread.join();
}