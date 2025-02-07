#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100
#define MAX_ACCOUNTS 250
#define MAX_DEPOSITS 250
#define MAX_WITHDRAWALS 750
#define MAX_TRANSACTIONS (MAX_DEPOSITS + MAX_WITHDRAWALS)
#define FIRST_NAME_SIZE 20
#define LAST_NAME_SIZE 20

// מבנה תאריך
struct Date {
    int day, month, year;
};

// מבנה לקוח
struct Customer {
    char first_name[FIRST_NAME_SIZE];
    char last_name[LAST_NAME_SIZE];
    int id;
    int num_accounts;
    int accounts[MAX_ACCOUNTS];
};

// מבנה חשבון בנק
struct Transaction {
    struct Date date;
    double amount;
    char description[50];
    double balance_after;
};

struct BankAccount {
    int account_number;
    int owner_id;
    double balance;
    int num_transactions;
    struct Transaction transactions[MAX_TRANSACTIONS];
};

// משתנים גלובליים
struct Customer customers[MAX_CUSTOMERS];
struct BankAccount accounts[MAX_ACCOUNTS];
int num_customers = 0;
int num_accounts = 0;

// יצירת תאריך
struct Date create_date(int day, int month, int year) {
    struct Date date = {day, month, year};
    return date;
}

void read_string(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';  // Set empty string on error
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character if present
}

void add_customer() {
    if (num_customers >= MAX_CUSTOMERS) {
        printf("Cannot add more customers!\n");
        return;
    }

    int new_id;
    printf("Enter unique ID number: ");
    if (scanf("%d", &new_id) != 1) {
        printf("Invalid ID input!\n");
        while (getchar() != '\n'); // Clear buffer
        return;
    }
    while (getchar() != '\n');

    // Check if ID already exists
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == new_id) {
            printf("Error: A customer with ID %d already exists!\n", new_id);
            return;
        }
    }

    printf("Enter first name: ");
    read_string(customers[num_customers].first_name, sizeof(customers[num_customers].first_name));

    printf("Enter last name: ");
    read_string(customers[num_customers].last_name, sizeof(customers[num_customers].last_name));

    customers[num_customers].id = new_id;
    customers[num_customers].num_accounts = 0;
    num_customers++;

    printf("Customer added successfully! ID: %d\n", new_id);
}

void create_account() {
    if (num_accounts >= MAX_ACCOUNTS) {
        printf("Cannot add more accounts!\n");
        return;
    }

    int owner_id;
    printf("Enter customer ID: ");
    if (scanf("%d", &owner_id) != 1) {
        printf("Invalid ID input!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int customer_index = -1;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == owner_id) {
            customer_index = i;
            break;
        }
    }

    if (customer_index == -1) {
        printf("Customer not found!\n");
        return;
    }

    if (customers[customer_index].num_accounts >= MAX_ACCOUNTS) {
        printf("This customer has reached the maximum number of accounts!\n");
        return;
    }

    accounts[num_accounts].account_number = num_accounts + 1;
    accounts[num_accounts].owner_id = owner_id;
    accounts[num_accounts].balance = 0;
    accounts[num_accounts].num_transactions = 0;

    customers[customer_index].accounts[customers[customer_index].num_accounts++] = num_accounts + 1;
    
    num_accounts++;

    printf("Bank account created successfully! Account number: %d\n", num_accounts);
}

void withdraw() {
    int account_number;
    double amount;
    char description[50];
    int day, month, year;

    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (account_number < 1 || account_number > num_accounts) {
        printf("Invalid account number!\n");
        return;
    }

    struct BankAccount *acc = &accounts[account_number - 1];

    // ספירת משיכות
    int withdraw_count = 0;
    for (int i = 0; i < acc->num_transactions; i++) {
        if (acc->transactions[i].amount < 0) {
            withdraw_count++;
        }
    }
    if (withdraw_count >= MAX_WITHDRAWALS) {
        printf("Withdrawal limit reached! You cannot make more than %d withdrawals.\n", MAX_WITHDRAWALS);
        return;
    }

    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Must be greater than zero.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (acc->balance < amount) {
        printf("Insufficient balance! Current balance: %.2f\n", acc->balance);
        return;
    }

    printf("Enter description: ");
    read_string(description, sizeof(description));

    printf("Enter transaction date (DD MM YYYY): ");
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Invalid date input!\n");
        while (getchar() != '\n');
        return;
    }

    acc->balance -= amount;
    acc->transactions[acc->num_transactions].date = create_date(day, month, year);
    acc->transactions[acc->num_transactions].amount = -amount;
    strcpy(acc->transactions[acc->num_transactions].description, description);
    acc->transactions[acc->num_transactions].balance_after = acc->balance;
    acc->num_transactions++;

    printf("Withdrawal successful! New balance: $%.2f\n", acc->balance);
}

void deposit() {
    int account_number;
    double amount;
    char depositor_name[FIRST_NAME_SIZE + LAST_NAME_SIZE + 2]; // Name of depositor
    int day, month, year;

    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number!\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (account_number < 1 || account_number > num_accounts) {
        printf("Invalid account number!\n");
        return;
    }

    struct BankAccount *acc = &accounts[account_number - 1];

    // Count deposits
    int deposit_count = 0;
    for (int i = 0; i < acc->num_transactions; i++) {
        if (acc->transactions[i].amount > 0) {
            deposit_count++;
        }
    }
    if (deposit_count >= MAX_DEPOSITS) {
        printf("Deposit limit reached! You cannot make more than %d deposits.\n", MAX_DEPOSITS);
        return;
    }

    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount! Must be greater than zero.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    printf("Enter depositor's full name: ");
    read_string(depositor_name, sizeof(depositor_name));  // Read full name

    printf("Enter transaction date (DD MM YYYY): ");
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Invalid date input!\n");
        while (getchar() != '\n');
        return;
    }

    acc->balance += amount;
    acc->transactions[acc->num_transactions].date = create_date(day, month, year);
    acc->transactions[acc->num_transactions].amount = amount;
    strcpy(acc->transactions[acc->num_transactions].description, depositor_name); // Store name instead of description
    acc->transactions[acc->num_transactions].balance_after = acc->balance;
    acc->num_transactions++;

    printf("Deposit successful! $%.2f deposited by %s. New balance: $%.2f\n", amount, depositor_name, acc->balance);
}




void display_account_details() {
    int account_number;
    struct Date start_date, end_date;
    double opening_balance = 0.0;

    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number input!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }
    while (getchar() != '\n');

    if (account_number < 1 || account_number > num_accounts) {
        printf("Account not found!\n");
        return;
    }

    struct BankAccount *acc = &accounts[account_number - 1];

    // מציאת שם הלקוח המתאים
    char owner_name[FIRST_NAME_SIZE + LAST_NAME_SIZE + 2]; // +2 בשביל רווח ונול-טרמינציה
    int found = 0;
    for (int i = 0; i < num_customers; i++) {
        if (customers[i].id == acc->owner_id) {
            // שימוש ב-strcpy ו-strcat במקום snprintf/sprintf
            strcpy(owner_name, customers[i].first_name);
            strcat(owner_name, " ");
            strcat(owner_name, customers[i].last_name);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Error: Customer not found for this account!\n");
        return;
    }

    printf("Enter start date (DD MM YYYY): ");
    if (scanf("%d %d %d", &start_date.day, &start_date.month, &start_date.year) != 3) {
        printf("Invalid date input!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Enter end date (DD MM YYYY): ");
    if (scanf("%d %d %d", &end_date.day, &end_date.month, &end_date.year) != 3) {
        printf("Invalid date input!\n");
        while (getchar() != '\n');
        return;
    }

    // חישוב יתרת הפתיחה הנכונה
    for (int i = 0; i < acc->num_transactions; i++) {
        struct Transaction *t = &acc->transactions[i];
        if (t->date.year < start_date.year ||
            (t->date.year == start_date.year && t->date.month < start_date.month) ||
            (t->date.year == start_date.year && t->date.month == start_date.month && t->date.day < start_date.day)) {
            opening_balance = t->balance_after;
        }
    }

    printf("\n--- Account Statement ---\n");
    printf("Account Holder: %s\n", owner_name); // הדפסת שם הלקוח במקום Owner ID
    printf("Account Number: %d\n", acc->account_number);
    printf("Statement Period: %02d/%02d/%04d - %02d/%02d/%04d\n", 
           start_date.day, start_date.month, start_date.year, 
           end_date.day, end_date.month, end_date.year);
    printf("Opening Balance: $%.2f\n", opening_balance);
    printf("\nDate\t\tDeposits\tWithdrawals\tBalance After\n");
    printf("--------------------------------------------------------------\n");

    double current_balance = opening_balance;

    for (int i = 0; i < acc->num_transactions; i++) {
        struct Transaction *t = &acc->transactions[i];

        if ((t->date.year > start_date.year ||
            (t->date.year == start_date.year && t->date.month > start_date.month) ||
            (t->date.year == start_date.year && t->date.month == start_date.month && t->date.day >= start_date.day)) &&
            (t->date.year < end_date.year ||
            (t->date.year == end_date.year && t->date.month < end_date.month) ||
            (t->date.year == end_date.year && t->date.month == end_date.month && t->date.day <= end_date.day))) {

            current_balance = t->balance_after;
            printf("%02d/%02d/%04d\t", t->date.day, t->date.month, t->date.year);
            if (t->amount > 0) {
                 printf("$%.2f\t\t-\t\t$%.2f\n", t->amount, current_balance);
            } else {
                 printf("-\t\t$%.2f\t\t$%.2f\n", -t->amount, current_balance);
            }
        }
    }

    printf("--------------------------------------------------------------\n");
    printf("Closing Balance: $%.2f\n", current_balance);
}



void display_all_customers() {
    if (num_customers == 0) {
        printf("\nNo customers found in the system.\n");
        return;
    }

    printf("\n--- List of Bank Customers ---\n");
    printf("%-20s %-15s %-30s\n", "Full Name", "Customer Id", "[Account ID: Balance]");
    printf("-----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < num_customers; i++) {
        printf("%-20s %-15d ", customers[i].first_name, customers[i].id);
        
        for (int j = 0; j < customers[i].num_accounts; j++) {
            int acc_index = customers[i].accounts[j] - 1;
            printf("[%d: $%.2f] ", accounts[acc_index].account_number, accounts[acc_index].balance);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------------------------------------------\n");
}






void menu_display() {
    printf("\n Welcome to the Bank System\n");
    printf(" 1. Add a person to the bank\n");
    printf(" 2. Open a bank account\n");
    printf(" 3. Deposit money\n");
    printf(" 4. Withdraw money\n");
    printf(" 5. View account details\n");
    printf(" 6. View all customers\n");
    printf(" 7. Exit\n");
}

int main() {
    int user_choice;
    int run = 1;

    while (run) {
        menu_display();
        printf("Enter your choice: ");

        if (scanf("%d", &user_choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (user_choice) {
            case 1:
                add_customer();
                break;
            case 2:
                create_account();
                break;
            case 3:
                deposit();
                break;
            case 4:
                withdraw();
                break;
            case 5:
                display_account_details();
                break;
            case 6:
                display_all_customers();
                break;
            case 7:
                printf("Exiting the bank system. Goodbye!\n");
                run = 0;
                break;
            default:
                printf("Invalid choice! Please enter a number between 1-7.\n");
        }
    }
    return 0;
}
