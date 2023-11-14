#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define trs struct transaction
#define MAXTRANSACTIONS 1000

struct transaction{
    float amount;
    short type; // 1 = income, -1 = expense
    char date[15], description[200];
};

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

void testIsDateValid()
{
    assert(isDateValid("2023/10/12"));
    assert(isDateValid("20sasdda dsad20/12") == 0);
    assert(isDateValid("2023//12") == 0);
    assert(isDateValid("2023/1012") == 0);
}

trs readTransaction()
{
    //citeste o noua tranzactie
    trs newTransaction;
    float amount;
    char type[10];
    printf("Enter transaction data:\n");
    do
    {
        printf("Enter amount: ");
        scanf("%f", &newTransaction.amount);
    }while(newTransaction.amount <= 0);
    do
    {
        printf("Enter type(income or expense): ");
        scanf("%s", &type);
        
        if(strcmp(type, "income") == 0)
        {
            newTransaction.type = 1;
            break;
        }
        else if(strcmp(type, "expense") == 0)
        {
            newTransaction.type = -1;
            break;
        }
    } while (1); //citeste pana cand inputul este valid (income sau expense)
    
    
    do
    {
        printf("Enter date(YYYY/MM/DD): ");       
        scanf("%s",&newTransaction.date);
        if(!isDateValid(newTransaction.date))
            printf("Date is not valid!");
    }while(!isDateValid(newTransaction.date)); //citeste pana cand data e valida

    printf("Enter description: ");
    scanf(" %200[^\n]", newTransaction.description); //citeste un sir de caractere cu spatii de max 200 de caractere

    return newTransaction;
}

trs createTransaction(float amount, short type, char *date, char *description)
{
    //creaza o noua tranzactie
    trs newTransaction;
    newTransaction.amount = amount;
    newTransaction.type = type;
    strcpy(newTransaction.date, date);
    strcpy(newTransaction.description, description);
    return newTransaction;
}

void printTransaction(trs transaction)
{
    //afiseaza o tranzactie
    printf("{\nAmount: %f\n", transaction.amount);
    printf("Type: %s\n", transaction.type == 1 ? "income" : "expense");
    printf("Date: %s\n", transaction.date);
    printf("Description: %s\n}\n", transaction.description);
}

void recordTransaction(trs *transactionsList, int *numberOfTransactions, trs newTransaction)
{
    //adauga o tranzactie noua
    transactionsList[(*numberOfTransactions)++] = newTransaction; 
}

void displayTransactions(trs *transactionsList, int *numberOfTransactions)
{
    //afiseaza toate tranzactiile
    printf("Transactions: ");
    for(int i = 0; i < *numberOfTransactions; i++)
    {
        printTransaction(transactionsList[i]);
    }
}

float calculateBalance(trs *transactionsList, int *numberOfTransactions)
{
    //calculeaza balance
    float balance = 0;
    for(int i = 0; i < *numberOfTransactions; i++)
    {
        balance += transactionsList[i].amount * transactionsList[i].type;
    }
    return balance;
}

void testCalculateBalance()
{
    trs transactionsList[MAXTRANSACTIONS];
    int numberOfTransactions = 0;
    recordTransaction(transactionsList, &numberOfTransactions, createTransaction(10, 1, "2022/02/03", "-"));
    recordTransaction(transactionsList, &numberOfTransactions, createTransaction(1220, -1, "2012/02/03", "-"));
    recordTransaction(transactionsList, &numberOfTransactions, createTransaction(1, 1, "2023/02/03", "-"));
    assert(calculateBalance(transactionsList, &numberOfTransactions) == 10-1220+1);
}

void financialReport(trs *transactionsList, int *numberOfTransactions, char *dateStart, char *dateEnd,
                        float *income, float *expenses)
{
    //genereaza un raport financiar
    *income = 0; *expenses = 0;
    for(int i = 0; i < *numberOfTransactions; i++)
    {
        if(strcmp(dateStart, transactionsList[i].date) <= 0 && 
           strcmp(dateEnd, transactionsList[i].date) >= 0) //verifica daca data se afla intre cea de inceput si cea de final
        {
            if(transactionsList[i].type == 1) //verifica daca e income
                *income += transactionsList[i].amount;
            else if(transactionsList[i].type == -1) //verifica daca e expense
                *expenses += transactionsList[i].amount;
        }
    }

}

void saveData(trs *transactionsList, int *numberOfTransactions, char *fileName)
{
    //salveaza datele intr-un fisier de tipul [fileName].fin
    strcat(fileName, ".fin");
    FILE *saveFilePtr = fopen(fileName, "w");
    fprintf(saveFilePtr, "%d\n", *numberOfTransactions);

    for(int i = 0; i < *numberOfTransactions; i++)
    {
        if(transactionsList[i].description[0] == '\0')
            strcpy(transactionsList[i].description, "-"); //daca nu este descriere, se inlocuieste cu -
        fprintf(saveFilePtr, "%f\n%d\n%s\n%s\n", transactionsList[i].amount, transactionsList[i].type,
            transactionsList[i].date, transactionsList[i].description); //se scrie in fisier
    }
    fclose(saveFilePtr);
}

void loadData(trs *transactionsList, int *numberOfTransactions, char *fileName)
{
    //se citesc datele din fisier
    strcat(fileName, ".fin");
    FILE *loadFilePtr = fopen(fileName, "r");
    fscanf(loadFilePtr, "%d\n", numberOfTransactions);

    for(int i = 0; i < *numberOfTransactions; i++)
    {
        fscanf(loadFilePtr, "%f\n%d\n%s\n%200[^\n]\n", &transactionsList[i].amount, &transactionsList[i].type,
            &transactionsList[i].date, &transactionsList[i].description);
    }
    fclose(loadFilePtr);
}


void uiloop()
{
    //partea principala a programului
    trs transactionsList[MAXTRANSACTIONS];
    int numberOfTransactions = 0;
    char fileName[100]; //folosit pentru save/load
    for(;;) //bucla infinita, se opreste cand utilizatorul alege optiunea exit
    {
        printf("Options:\n1. Record new transaction\n2. Display previous transactions\n3. Calculate account balance\n"
        "4. Generate financial report for specific period\n5. Save\n6. Load\n7. Exit\n");
        char option;
        scanf(" %c", &option); 
        switch (option) //citirea si procesarea optiunii
        {
            case '1':
                if(numberOfTransactions > MAXTRANSACTIONS)
                    printf("Too many transactions!\n");
                else
                    recordTransaction(transactionsList, &numberOfTransactions, readTransaction());
                break;

            case '2':
                displayTransactions(transactionsList, &numberOfTransactions);
                break;

            case '3':
                printf("Total balance: %f\n", calculateBalance(transactionsList, &numberOfTransactions));
                break;
            
            case '4':
                char dateStart[15], dateEnd[15];
                do
                {
                    printf("Enter the start date(YYYY/MM/DD): ");       
                    scanf("%s", &dateStart);
                    if(!isDateValid(dateStart))
                        printf("Date is not valid!");
                }while(!isDateValid(dateStart));
                do
                {
                    printf("Enter the end date(YYYY/MM/DD): ");
                    scanf("%s", &dateEnd);
                    if(!isDateValid(dateEnd))
                            printf("Date is not valid!\n");
                }while(!isDateValid(dateEnd)); 
                //se citesc datele pana sunt valide

                float income, expenses;
                financialReport(transactionsList, &numberOfTransactions, dateStart, dateEnd, &income, &expenses);
                printf("Income for given period: %f \nExpenses for give period: %f\n", income, expenses);
                break;

            case '5':
                printf("Input save file name: ");
                scanf("%s", &fileName);
                saveData(transactionsList, &numberOfTransactions, fileName);
                printf("Saved\n");
                break;
                
            case '6':
                printf("Input load file name: ");
                scanf("%s", &fileName);
                loadData(transactionsList, &numberOfTransactions, fileName);
                printf("Loaded\n");
                break;

            case '7':
                exit(0);
                break;

            default:
                printf("Invalid option!\n");
                break;
        }
    }
}

int main (int argc, char **argv)
{
    testIsDateValid();
    testCalculateBalance(); //se testeaza
    uiloop(); //se ruleaza partea principala a programului
    return 0;
}
