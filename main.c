#include "bank_system.h"

void display_menu() {
    printf("\nמערכת ניהול בנקאית\n");
    printf("1. רישום לקוח\n");
    printf("2. פתיחת חשבון\n");
    printf("3. הפקדה לחשבון\n");
    printf("4. משיכת כספים\n");
    printf("5. הצגת פרטי חשבון\n");
    printf("6. יציאה\n");
    printf("בחר אפשרות: ");
}

int main() {
    int choice;
    
    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_customer();
                break;
            case 2:
                open_account();
                break;
            case 3:
                deposit_money();
                break;
            case 4:
                withdraw_money();
                break;
            case 5:
                display_account();
                break;
            case 6:
                printf("יציאה מהמערכת.\n");
                return 0;
            default:
                printf("בחירה לא חוקית. נסה שוב.\n");
        }
    }

    return 0;
}
