/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 21:51:47 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-05 22:08:24
 */

#include <thread>
#include "bank_machine.cpp"
#include "interface_machine.cpp"
#include "atm.cpp"

int main()
{
    bank_machine bank;
    interface_machine interface_hardware;
    atm atm_machine(bank.get_sender(), interface_hardware.get_sender());
    
    std::thread bank_thread(&bank_machine::run, &bank);   //* 
    std::thread if_thread(&interface_machine::run, &interface_hardware);  //* 界面线程
    std::thread atm_thread(&atm::run, &atm_machine);//* 

    messaging::sender atmqueue(atm_machine.get_sender());

    bool quit_pressed = false;
    while(!quit_pressed){
        char c = getchar();
        switch (c)
        {
        case '0':
            /* code */
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