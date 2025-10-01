#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct User {
    char username[20];
    char password[20]; // encrypted
};

struct Account {
    int accNo;
    char name[50];
    float balance;
};

FILE *fp;

// ================= ENCRYPTION =================
void encrypt(char *str) {
    char key = 'X';  
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = str[i] ^ key;
    }
}

// ================= USER MANAGEMENT =================
void addUser() {
    struct User u;
    FILE *fp = fopen("users.dat", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n--- Add New User ---\n");
    printf("Enter username: ");
    scanf("%s", u.username);

    printf("Enter password: ");
    char temp[20];
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) { // Enter
            temp[i] = '\0';
            break;
        } else if (ch == 8 && i > 0) { // Backspace
            i--;
            printf("\b \b");
        } else {
            temp[i++] = ch;
            printf("*");
        }
    }

    strcpy(u.password, temp);
    encrypt(u.password); // store encrypted

    fwrite(&u, sizeof(u), 1, fp);
    fclose(fp);

    printf("\nUser added successfully!\n");
}

int login() {
    struct User u;
    char username[20], password[20];
    FILE *fp = fopen("users.dat", "rb");
    if (!fp) {
        printf("\nNo users found! Please add a user first.\n");
        return 0;
    }

    printf("\n====== Bank Login ======\n");
    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == 13) {
            password[i] = '\0';
            break;
        } else if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        } else {
            password[i++] = ch;
            printf("*");
        }
    }

    encrypt(password); // encrypt entered password

    while (fread(&u, sizeof(u), 1, fp)) {
        if (strcmp(username, u.username) == 0 && strcmp(password, u.password) == 0) {
            fclose(fp);
            printf("\n\nLogin successful! Welcome, %s.\n", username);
            return 1;
        }
    }

    fclose(fp);
    printf("\n\nInvalid login! Access denied.\n");
    return 0;
}

// ================= BANK MANAGEMENT =================
void createAccount() {
    struct Account acc;
    fp = fopen("accounts.dat", "ab"); 
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    printf("\n--- Create New Account ---\n");
    printf("Enter Account Number: ");
    scanf("%d", &acc.accNo);
    printf("Enter Name: ");
    scanf("%s", acc.name);
    acc.balance = 0.0;

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

void showAccount() {
    struct Account acc;
    int accNo, found = 0;

    fp = fopen("accounts.dat", "rb");
    if (!fp) {
        printf("No accounts found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("\n--- Account Details ---\n");
            printf("Account No: %d\n", acc.accNo);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void deposit() {
    struct Account acc;
    int accNo, found = 0;
    float amount;

    fp = fopen("accounts.dat", "rb+");
    if (!fp) {
        printf("No accounts found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;
            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            printf("Deposited successfully! New Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void withdraw() {
    struct Account acc;
    int accNo, found = 0;
    float amount;

    fp = fopen("accounts.dat", "rb+");
    if (!fp) {
        printf("No accounts found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (acc.balance >= amount) {
                acc.balance -= amount;
                fseek(fp, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, fp);
                printf("Withdrawal successful! Remaining Balance: %.2f\n", acc.balance);
            } else {
                printf("Insufficient balance!\n");
            }
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void checkBalance() {
    struct Account acc;
    int accNo, found = 0;

    fp = fopen("accounts.dat", "rb");
    if (!fp) {
        printf("No accounts found!\n");
        return;
    }

    printf("\nEnter Account Number: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("Current Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void updateAccount() {
    struct Account acc;
    int accNo, found = 0;

    fp = fopen("accounts.dat", "rb+");
    if (!fp) {
        printf("No accounts found!\n");
        return;
    }

    printf("\nEnter Account Number to Update: ");
    scanf("%d", &acc.accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == acc.accNo) {
            printf("Enter new name: ");
            scanf("%s", acc.name);
            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            printf("Account updated successfully!\n");
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void deleteAccount() {
    struct Account acc;
    int accNo, found = 0;

    FILE *temp;

    fp = fopen("accounts.dat", "rb");
    temp = fopen("temp.dat", "wb");
    if (!fp || !temp) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("Account deleted successfully!\n");
            found = 1;
        } else {
            fwrite(&acc, sizeof(acc), 1, temp);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (!found)
        printf("Account not found!\n");
}

void listAccounts() {
    struct Account acc;

    fp = fopen("accounts.dat", "rb");
    if (!fp) {
        printf("No accounts found!\n");
        return;
    }

    printf("\n--- All Accounts ---\n");
    printf("%-10s %-20s %-10s\n", "AccNo", "Name", "Balance");
    printf("-----------------------------------------\n");

    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("%-10d %-20s %-10.2f\n", acc.accNo, acc.name, acc.balance);
    }

    fclose(fp);
}

// ================= MAIN =================
int main() {
    int choice;

    while (1) {
        printf("\n====== MAIN MENU ======\n");
        printf("1. Add User\n");
        printf("2. Login to Bank System\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addUser(); break;
            case 2: 
                if (login()) {
                    while (1) {
                        printf("\n====== Bank Management System ======\n");
                        printf("1. Create Account\n");
                        printf("2. Show Account\n");
                        printf("3. Deposit\n");
                        printf("4. Withdraw\n");
                        printf("5. Check Balance\n");
                        printf("6. Update Account\n");
                        printf("7. Delete Account\n");
                        printf("8. List All Accounts\n");
                        printf("9. Logout\n");
                        printf("Enter choice: ");
                        scanf("%d", &choice);

                        switch (choice) {
                            case 1: createAccount(); break;
                            case 2: showAccount(); break;
                            case 3: deposit(); break;
                            case 4: withdraw(); break;
                            case 5: checkBalance(); break;
                            case 6: updateAccount(); break;
                            case 7: deleteAccount(); break;
                            case 8: listAccounts(); break;
                            case 9: goto endBank;
                            default: printf("Invalid choice!\n");
                        }
                    }
                    endBank:;
                }
                break;
            case 3: exit(0);
            default: printf("Invalid choice!\n");
        }
    }

    return 0;
}
