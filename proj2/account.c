#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "account.h"


const user nullUser = {"", "", NULL, 0, 0};
const account nullAccount = {"", NULL, 0, 0};
void insertAccount(user *a, account acc)
{
    if (a->used == a->size)  // adaugam spatiu daca tot array-ul e folosit
    {
        a->size = a->size*2+5;
        void **tmp = realloc(a->accounts, a->size * sizeof(account));
        if(tmp) a->accounts = tmp;
        else printf("Error when trying to alocate more memory!");
    }
    a->accounts[a->used++] = acc; //adaugam elementul si crestem a->used
}

void recordTransaction(account *a, trs transaction)
{
    if (a->used == a->size)  // adaugam spatiu daca tot array-ul e folosit
    {
        a->size = a->size*2+5;
        void **tmp = realloc(a->transactions, a->size * sizeof(trs));
        if(tmp) a->transactions = tmp;
        else printf("Error when trying to alocate more memory!\n");
    }
    a->transactions[a->used++] = transaction; //adaugam elementul si crestem a->used
}

void deleteAccount(user *usr, account *acc)
{
    free(acc->transactions);
    for(int i = 0; i < usr->used; i++)
    {
        if(strcmp(usr->accounts[i].type, acc->type) == 0)
        {
            usr->accounts[i] = nullAccount;
            return;
        }
    }
}

int isDateValid(char *date)
{
    //functie care verifica daca data e valida
    int year, month, day;

    if (sscanf(date, "%d/%d/%d", &year, &month, &day) != 3) //se imparte data in an, luna, zi dupa /
    {
        return 0;
    }

    if (year < 1000 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) //se verifica ca acestea sa fie valide
    {
        return 0;
    }

    return 1;
}

trs readTransaction()
{
    //citeste o noua tranzactie
    trs newTransaction;
    char type[10];
    printf("Enter transaction data:\n");
    do
    {
        printf("Enter amount: ");
        scanf("%f", &newTransaction.amount);
        if(newTransaction.amount <= 0)
        {
            printf("Enter a positive amount!\n");
        }
    }while(newTransaction.amount <= 0);
    do
    {
        printf("Enter type (deposit, withdrawal, transfer, payment): ");
        scanf("%s", type);

        if(strcmp(type, "deposit") == 0 || strcmp(type, "payemeny") == 0)
        {
            break;
        }
        else if(strcmp(type, "withdrawal") == 0 || strcmp(type, "transfer") == 0 )
        {
            newTransaction.amount *= -1;
            break;
        }

        printf("Enter a valid type!\n");
    } while (1); //citeste pana cand inputul este valid (income sau expense)


    do
    {
        printf("Enter date(YYYY/MM/DD): ");
        scanf("%14s", newTransaction.date);
        if(!isDateValid(newTransaction.date))
            printf("Date is not valid!\n");
    }while(!isDateValid(newTransaction.date)); //citeste pana cand data e valida

    printf("Enter description: ");
    scanf(" %199[^\n]", newTransaction.description); //citeste un sir de caractere cu spatii de max 200 de caractere

    return newTransaction;
}

void printTransaction(trs transaction)
{
    //afiseaza o tranzactie
    printf("{\nAmount: %f\n", transaction.amount);
    printf("Type: %s\n", transaction.type);
    printf("Date: %s\n", transaction.date);
    printf("Description: %s\n}\n", transaction.description);
}

void displayTransactions(account account)
{
    //afiseaza toate tranzactiile
    printf("Transactions: ");
    for(int i = 0; i < account.used; i++)
    {
        printTransaction(account.transactions[i]);
    }
}

float calculateBalance(account account)
{
    //calculeaza balance
    float balance = 0;
    for(int i = 0; i < account.used; i++)
    {
        balance += account.transactions[i].amount;
    }
    return balance;
}

void saveAccountData(account account)
{
    //salveaza datele intr-un fisier de tipul [account.type].acc
    char fileName[100] = "";
    strcat(fileName, account.type);
    strcat(fileName, ".acc");
    FILE *saveFilePtr = fopen(fileName, "w");
    fprintf(saveFilePtr, "%d\n", &(account.used));

    for(int i = 0; i < account.used; i++)
    {
        fprintf(saveFilePtr, "%f,%s,%s,%s\n", account.transactions[i].amount, account.transactions[i].type,
            account.transactions[i].date, account.transactions[i].description); //se scrie in fisier
    }
    fclose(saveFilePtr);
}

void loadAccountData(account *account)
{
    //se citesc datele din fisier
    char fileName[100] = "";
    strcat(fileName, account->type);
    strcat(fileName, ".acc");

    FILE *loadFilePtr = fopen(fileName, "r");
    fscanf(loadFilePtr, "%d\n", &(account->used));
    account->size = account->used;
    account->transactions = malloc(account->size*sizeof(trs)); //se aloca memorie pentru a citi datele

    for(int i = 0; i < account->used; i++)
    {
        fscanf(loadFilePtr, "%f,%s,%s,%s\n", account->transactions[i].amount, account->transactions[i].type,
            account->transactions[i].date, account->transactions[i].description); //se citeste in fisier
    }
    fclose(loadFilePtr);
}

void saveUserAccounts(user user)
{
    //salveaza datele intr-un fisier de tipul [account.type].acc
    char fileName[100] = "";
    strcat(fileName, user.name);
    strcat(fileName, ".usr");
    FILE *saveFilePtr = fopen(fileName, "w");
    fprintf(saveFilePtr, "%d\n", &(user.used));

    for(int i = 0; i < user.used; i++)
    {
        fprintf(saveFilePtr, "%s,", user.accounts[i].type); //se scrie in fisier
    }
    fclose(saveFilePtr);
}

void loadUserAccounts(user *user)
{
    //se citesc datele din fisier
    char fileName[100] = "";
    strcat(fileName, user->name);
    strcat(fileName, ".usr");

    FILE *loadFilePtr = fopen(fileName, "r");
    fscanf(loadFilePtr, "%d\n", &(user->used));
    user->size = user->used;
    user->accounts = malloc(user->size*sizeof(account)); //se aloca memorie pentru a citi datele

    for(int i = 0; i < user->used; i++)
    {
        fscanf(loadFilePtr, "%s,", user->accounts[i].type); //se citeste in fisier
    }
    fclose(loadFilePtr);
}
