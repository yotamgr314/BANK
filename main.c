#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CUSTOMERS 100
#define MAX_ACCOUNTS 250
#define MAX_TRANSACTIONS 750

// מבנה תאריך
typedef struct {
    int day, month, year;
} Date;

// מבנה לקוח
typedef struct {
    char first_name[50];
    char last_name[50];
    char id[15];
    int num_accounts;
    int accounts[MAX_ACCOUNTS];
} Customer;

// מבנה חשבון בנק
typedef struct {
    int account_number;
    char owner_id[15];
    double balance;
    int num_transactions;
    struct Transaction {
        Date date;
        double amount;
        char description[50];
        double balance_after;
    } transactions[MAX_TRANSACTIONS];
} BankAccount;

// משתנים גלובליים
Customer customers[MAX_CUSTOMERS];
BankAccount accounts[MAX_ACCOUNTS];
int num_customers = 0;
int num_accounts = 0;

// יצירת תאריך
Date create_date(int day, int month, int year) {
    Date date = {day, month, year};
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

    printf("Enter first name: ");
    read_string(customers[num_customers].first_name, sizeof(customers[num_customers].first_name));

    printf("Enter last name: ");
    read_string(customers[num_customers].last_name, sizeof(customers[num_customers].last_name));

    printf("Enter unique ID number: ");
    read_string(customers[num_customers].id, sizeof(customers[num_customers].id));

    customers[num_customers].num_accounts = 0;
    num_customers++;

    printf("Customer added successfully! ID: %s\n", customers[num_customers - 1].id);
}


// יצירת חשבון בנק
void create_account() {
    if (num_accounts >= MAX_ACCOUNTS) {
        printf("Cannot add more accounts!\n");
        return;
    }

    char owner_id[15];
    printf("Enter customer ID: ");
    read_string(owner_id, sizeof(owner_id));

    int customer_index = -1;
    for (int i = 0; i < num_customers; i++) {
        if (strcmp(customers[i].id, owner_id) == 0) {
            customer_index = i;
            break;
        }
    }

    if (customer_index == -1) {
        printf("Customer not found!\n");
        return;
    }

    accounts[num_accounts].account_number = num_accounts + 1;
    strcpy(accounts[num_accounts].owner_id, owner_id);
    accounts[num_accounts].balance = 0;
    accounts[num_accounts].num_transactions = 0;
    customers[customer_index].accounts[customers[customer_index].num_accounts++] = num_accounts + 1;
    num_accounts++;

    printf("Bank account created successfully! Account number: %d\n", num_accounts);
}

// הפקדת כסף
void deposit() {
    int account_number;
    double amount;
    char description[50];
    int day, month, year;

    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }
    while (getchar() != '\n'); // ניקוי buffer אחרי scanf

    if (account_number < 1 || account_number > num_accounts) {
        printf("Invalid account number!\n");
        return;
    }

    printf("Enter amount to deposit: ");
    if (scanf("%lf", &amount) != 1) {
        printf("Invalid amount!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }
    while (getchar() != '\n'); // ניקוי buffer אחרי scanf

    printf("Enter description: ");
    read_string(description, sizeof(description));

    printf("Enter transaction date (DD MM YYYY): ");
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Invalid date input!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }

    BankAccount *acc = &accounts[account_number - 1];

    if (acc->num_transactions >= MAX_TRANSACTIONS) {
        printf("Cannot add more transactions!\n");
        return;
    }

    acc->balance += amount;
    acc->transactions[acc->num_transactions].date = create_date(day, month, year);
    acc->transactions[acc->num_transactions].amount = amount;
    strcpy(acc->transactions[acc->num_transactions].description, description);
    acc->transactions[acc->num_transactions].balance_after = acc->balance;
    acc->num_transactions++;

    printf("Deposit successful!\n");
}



void withdraw() {
    int account_number;
    double amount;
    char description[50];
    int day, month, year;

    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }
    while (getchar() != '\n'); // ניקוי buffer אחרי scanf

    if (account_number < 1 || account_number > num_accounts) {
        printf("Invalid account number!\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1) {
        printf("Invalid amount!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }
    while (getchar() != '\n'); // ניקוי buffer אחרי scanf

    if (amount <= 0) {
        printf("Withdrawal amount must be greater than zero!\n");
        return;
    }

    BankAccount *acc = &accounts[account_number - 1];

    if (acc->balance < amount) {
        printf("Insufficient balance! Current balance: %.2f\n", acc->balance);
        return;
    }

    printf("Enter description: ");
    read_string(description, sizeof(description));

    printf("Enter transaction date (DD MM YYYY): ");
    if (scanf("%d %d %d", &day, &month, &year) != 3) {
        printf("Invalid date input!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }

    if (acc->num_transactions >= MAX_TRANSACTIONS) {
        printf("Cannot add more transactions!\n");
        return;
    }

    acc->balance -= amount;
    acc->transactions[acc->num_transactions].date = create_date(day, month, year);
    acc->transactions[acc->num_transactions].amount = -amount;  // סכום שלילי מייצג משיכה
    strcpy(acc->transactions[acc->num_transactions].description, description);
    acc->transactions[acc->num_transactions].balance_after = acc->balance;
    acc->num_transactions++;

    printf("Withdrawal successful! New balance: %.2f\n", acc->balance);
}



void display_account_details() {
    int account_number;

    printf("Enter account number: ");
    if (scanf("%d", &account_number) != 1) {
        printf("Invalid account number input!\n");
        while (getchar() != '\n'); // ניקוי buffer
        return;
    }
    while (getchar() != '\n'); // ניקוי buffer אחרי scanf

    if (account_number < 1 || account_number > num_accounts) {
        printf("Account not found!\n");
        return;
    }

    BankAccount *acc = &accounts[account_number - 1];

    printf("\n--- Account Details ---\n");
    printf("Account Number: %d\n", acc->account_number);
    printf("Owner ID: %s\n", acc->owner_id);
    printf("Current Balance: %.2f\n", acc->balance);
    printf("Number of Transactions: %d\n", acc->num_transactions);

    if (acc->num_transactions > 0) {
        printf("\nTransaction History:\n");
        printf("%-12s %-10s %-40s %-12s\n", "Date", "Amount", "Description", "Balance After");
        printf("-------------------------------------------------------------\n");
        for (int i = 0; i < acc->num_transactions; i++) {
            printf("%02d/%02d/%04d %-10.2f %-40s %-12.2f\n",
                   acc->transactions[i].date.day,
                   acc->transactions[i].date.month,
                   acc->transactions[i].date.year,
                   acc->transactions[i].amount,
                   acc->transactions[i].description,
                   acc->transactions[i].balance_after);
        }
    } else {
        printf("No transactions found for this account.\n");
    }
    printf("----------------------------\n");
}


void display_all_customers() {
    if (num_customers == 0) {
        printf("\nNo customers found in the system.\n");
        return;
    }

    printf("\n--- List of Bank Customers ---\n");
    printf("Full Name\t\tID\t\tAccounts\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < num_customers; i++) {
        printf("%s %s\t\t%s\t\t%d\n",
               customers[i].first_name,
               customers[i].last_name,
               customers[i].id,
               customers[i].num_accounts);
    }
    printf("------------------------------------------------------------\n");
}

void menu_display() {
    printf("\n Welcome to the Bank System\n");
    printf(" 1. Add a person to the bank\n");
    printf(" 2. Open a bank account\n");
    printf(" 3. Deposit money\n");
    printf(" 4. Withdraw money\n");
    printf(" 5. View account details\n");
    printf(" 6. View all customers\n"); // נוספה אופציה חדשה
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
            while (getchar() != '\n'); // ניקוי buffer
            continue;
        }
        while (getchar() != '\n'); // ניקוי buffer אחרי scanf

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
                display_all_customers(); // קריאה לפונקציה החדשה
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
