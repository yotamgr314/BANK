#include "bank_system.h"

Customer customers[MAX_CUSTOMERS];
BankAccount accounts[MAX_CUSTOMERS];
int customer_count = 0;
int account_count = 0;

// פונקציה לרישום לקוח חדש
void register_customer() {
    if (customer_count >= MAX_CUSTOMERS) {
        printf("לא ניתן לרשום לקוחות נוספים.\n");
        return;
    }

    printf("הכנס מספר תעודת זהות: ");
    scanf("%s", customers[customer_count].id);
    printf("הכנס שם פרטי: ");
    scanf("%s", customers[customer_count].first_name);
    printf("הכנס שם משפחה: ");
    scanf("%s", customers[customer_count].last_name);

    printf("לקוח נוסף בהצלחה!\n");
    customer_count++;
}

// פונקציה לפתיחת חשבון בנק חדש
void open_account() {
    if (account_count >= MAX_CUSTOMERS) {
        printf("לא ניתן לפתוח חשבונות נוספים.\n");
        return;
    }

    printf("הכנס מספר תעודת זהות של הלקוח: ");
    char id[MAX_NAME_LENGTH];
    scanf("%s", id);

    if (find_customer_index(id) == -1) {
        printf("לקוח לא נמצא.\n");
        return;
    }

    printf("הכנס מספר חשבון חדש: ");
    scanf("%s", accounts[account_count].account_number);
    strcpy(accounts[account_count].owner_id, id);
    accounts[account_count].balance = 0.00; // יתרת פתיחה ברירת מחדל

    printf("חשבון נפתח בהצלחה!\n");
    account_count++;
}

// פונקציה להפקדה לחשבון בנק
void deposit_money() {
    printf("הכנס מספר חשבון להפקדה: ");
    char account_number[MAX_NAME_LENGTH];
    scanf("%s", account_number);

    int index = find_account_index(account_number);
    if (index == -1) {
        printf("חשבון לא נמצא.\n");
        return;
    }

    double amount;
    printf("הכנס סכום להפקדה: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("סכום ההפקדה חייב להיות חיובי.\n");
        return;
    }

    accounts[index].balance += amount;
    printf("ההפקדה בוצעה בהצלחה! יתרה נוכחית: $%.2f\n", accounts[index].balance);
}

// פונקציה למשיכת כסף מחשבון בנק
void withdraw_money() {
    printf("הכנס מספר חשבון למשיכה: ");
    char account_number[MAX_NAME_LENGTH];
    scanf("%s", account_number);

    int index = find_account_index(account_number);
    if (index == -1) {
        printf("חשבון לא נמצא.\n");
        return;
    }

    double amount;
    printf("הכנס סכום למשיכה: ");
    scanf("%lf", &amount);

    if (amount <= 0) {
        printf("סכום המשיכה חייב להיות חיובי.\n");
        return;
    }

    if (accounts[index].balance - amount < -750.00) {
        printf("חריגה מהמגבלה! לא ניתן למשוך יותר מ-$750 במינוס.\n");
        return;
    }

    accounts[index].balance -= amount;
    printf("המשיכה בוצעה בהצלחה! יתרה נוכחית: $%.2f\n", accounts[index].balance);
}

// פונקציה להצגת פרטי חשבון
void display_account() {
    printf("הכנס מספר חשבון לתצוגה: ");
    char account_number[MAX_NAME_LENGTH];
    scanf("%s", account_number);

    int index = find_account_index(account_number);
    if (index == -1) {
        printf("חשבון לא נמצא.\n");
        return;
    }

    printf("פרטי החשבון:\n");
    printf("מספר חשבון: %s\n", accounts[index].account_number);
    printf("בעלים (תעודת זהות): %s\n", accounts[index].owner_id);
    printf("יתרה נוכחית: $%.2f\n", accounts[index].balance);
}

// פונקציה למציאת אינדקס לקוח לפי תעודת זהות
int find_customer_index(const char* id) {
    for (int i = 0; i < customer_count; i++) {
        if (strcmp(customers[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

// פונקציה למציאת אינדקס חשבון לפי מספר חשבון
int find_account_index(const char* account_number) {
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_number, account_number) == 0) {
            return i;
        }
    }
    return -1;
}
