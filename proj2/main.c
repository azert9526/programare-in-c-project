#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "users.h"
#include "account.h"

const user nullUser = {"", "", NULL, 0, 0};
const account nullAccount = {"", NULL, 0, 0};
void accountloop(account acc)
{
    //pentru a face operatii intr-un cont
    loadAccountData(&acc);
    for(;;) //bucla infinita, se opreste cand utilizatorul alege optiunea exit/ logout
    {
        printf("Options:\n1. Record transaction\n2. Calculate balance\n3. Transaction registers\n4.Expense reports\n5. Exit account\n6. Exit\n");
        char option;
        scanf(" %c", &option);
        switch (option) //citirea si procesarea optiunii
        {
            case '1':
                recordTransaction(&acc, readTransaction());
                saveAccountData(acc);
                break;

            case '2':
                printf("The account balance is: %f", calculateBalance(acc));
                break;

            case '3':
                displayTransactions(acc);
                break;

            case '4':
                break;

            case '5':
                return;
                break;

            case '6':
                exit(0);
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }
}

void userloop(user usr)
{
    char type[50];
    loadUserAccounts(&usr);
    //pentru a face operatii pentru un user
    for(;;) //bucla infinita, se opreste cand utilizatorul alege optiunea exit/ logout
    {
        printf("Options:\n1. Display accounts\n2. Enter in account\n3. Create new account\n5. Log out\n6. Exit\n");
        char option;
        scanf(" %c", &option);
        switch (option) //citirea si procesarea optiunii
        {
            case '1':
                printf("The available accounts are: ");
                for(int i = 0; i < usr.used; i++)
                {
                    printf("%s, ", usr.accounts[i].type);
                }
                printf("\n");
                break;

            case '2':
                do{
                    scanf("%49s", type);
                    for(int i = 0; i < usr.used; i++)
                    {
                        if(strcmp(type, usr.accounts[i].type) == 0)
                        {
                            account acc = nullAccount;
                            strcpy(acc.type, type);
                            accountloop(acc);
                            break;
                        }
                    }
                    printf("This type of account does not exist!\n");
                }while(1);

                break;

            case '3':
                scanf("%49s", type);
                account acc = nullAccount;
                strcpy(acc.type, type);
                insertAccount(&usr, acc); //adaugam contul
                saveUserAccounts(usr);
                accountloop(acc); //pornim loop account
                break;

            case '5':
                return;
                break;

            case '6':
                exit(0);
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }
}

void uiloop()
{
    userArray users;
    loadUsersLogIn(&users);
    user usr;
    //partea principala a programului
    for(;;) //bucla infinita, se opreste cand utilizatorul alege optiunea exit
    {
        printf("Options:\n1. Log in to user\n2. Create new user\n3. Exit\n");
        char option;
        scanf(" %c", &option);
        switch (option) //citirea si procesarea optiunii
        {
            case '1':
                usr = logInUser(users, readUserNamePass());
                userloop(usr);
                break;

            case '2':
                addUser(&users, readUserNamePass());
                saveUsersLogIn(&users);
                userloop(usr);
                break;

            case '3':
                exit(0);
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }
}

int main()
{
    uiloop(); //se ruleaza partea principala a programului
    return 0;
}
