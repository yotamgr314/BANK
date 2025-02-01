#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100
#define MAX_NAME_LENGTH 40

// מבנה נתונים ללקוח
typedef struct {
    char id[MAX_NAME_LENGTH];
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
} Customer;

// מבנה נתונים לחשבון בנק
typedef struct {
    char account_number[MAX_NAME_LENGTH];
    char owner_id[MAX_NAME_LENGTH];
    double balance;
} BankAccount;

// פונקציות לניהול לקוחות
void register_customer();
void display_customers();

// פונקציות לניהול חשבונות בנק
void open_account();
void deposit_money();
void withdraw_money();
void display_account();

// פונקציות עזר
int find_customer_index(const char* id);
int find_account_index(const char* account_number);

#endif
