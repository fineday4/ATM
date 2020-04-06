/*
 * @Author: xuhuanhuan(hhxu@robvision) 
 * @Date: 2020-04-05 08:35:59 
 * @Last Modified by: xuhuanhuan(hhxu@robvision.cn)
 * @Last Modified time: 2020-04-06 06:03:57
 */
#pragma once

#include "sender.h"


struct withdraw
{
    /* data */
    std::string account;
    unsigned amount;
    mutable messaging::sender atm_queue;
    
    withdraw(std::string const &account_, unsigned amount_, messaging::sender atm_queue_):
    account(account_), amount(amount_), atm_queue(atm_queue_)
    {}
};

struct withdraw_ok
{
    /* data */
};

struct withdraw_denied
{
    /* data */
};

struct cancel_withdrawal
{
    /* data */
    std::string account;
    unsigned amount;
    cancel_withdrawal(std::string const &account_, unsigned amount_):
    account(account_), amount(amount_)
    {}
};


struct withdrawal_processed
{
    /* data */
    std::string account;
    unsigned amount;
    withdrawal_processed(std::string const &account_, unsigned amount_):
    account(account_), amount(amount_)
    {}
};

struct card_inserted
{
    /* data */
    std::string account;

    explicit card_inserted(std::string const &account_):
    account(account_)
    {}
};

struct digit_pressed
{
    /* data */
    char digit;

    explicit digit_pressed(char digit_):
    digit(digit_)
    {}
};

struct clear_last_pressed
{
    /* data */
};

struct eject_card
{
    /* data */
};


struct withdraw_pressed
{
    /* data */
    unsigned amount;
    explicit withdraw_pressed(unsigned amount_):
    amount(amount_)
    {}
};

struct cancel_pressed
{
    /* data */
};

struct issue_money
{
    /* data */
    unsigned amount;
    issue_money(unsigned amount_):
    amount(amount_)
    {}
};

struct verify_pin
{
    /* data */
    std::string account;
    std::string pin;
    mutable messaging::sender atm_queue;
    verify_pin(std::string const &account_, std::string const &pin_, messaging::sender atm_queue_):
    account(account_), pin(pin_), atm_queue(atm_queue_)
    {}
};

struct pin_verified
{
    /* data */
};

struct pin_incorrect
{
    /* data */
};

struct display_enter_pin
{
    /* data */
};

struct display_enter_card
{
    /* data */
};

struct display_insufficient_funds
{
    /* data */
};

struct display_withdrawal_cancelled
{
    /* data */
};

struct display_pin_incorrect_message
{
    /* data */
};

struct display_withdrawal_options
{
    /* data */
};

struct get_balance
{
    /* data */
    std::string account;
    mutable messaging::sender atm_queue;
    get_balance(std::string const &account_, messaging::sender atm_queue_):
    account(account_), atm_queue(atm_queue_)
    {}
};

struct balance
{
    /* data */
    unsigned amount;
    explicit balance(unsigned amount_):
    amount(amount_)
    {}
};

struct display_balance
{
    /* data */
    unsigned amount;
    explicit display_balance(unsigned amount_):
    amount(amount_)
    {}
};

struct balance_pressed
{
    /* data */
};






















