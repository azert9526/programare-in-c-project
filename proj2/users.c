#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "users.h"



user getNullUser()
{
    user nul = {"", "", NULL, 0, 0};
    return nul;
}

account getNullAccount()
{
    account nul = {"", NULL, 0, 0};
    return nul;
}

void insertUser(userArray *a, user element)
{
    if(a->size == 0)
    {
        a->size = 16;
        user *tmp = malloc(a->size * sizeof(user));
        if(tmp) a->array = tmp;
        else {
            printf("Error when trying to alocate more memory! (%d)\n",a->size * sizeof(user));
            return;
        }
    }
    else if (a->used == a->size)  // adaugam spatiu daca tot array-ul e folosit
    {
        a->size = a->size*2;
        user *tmp = realloc(a->array, a->size * sizeof(user));
        if(tmp) a->array = tmp;
        else {
            printf("Error when trying to alocate more memory! (%d, %d)\n", a->array, a->size * sizeof(user));
            return;
        }
    }
    a->array[a->used++] = element; //adaugam elementul si crestem a->used
}

user readUserNamePass()
{
    //citeste numele si parola unui user si returneaza user-ul creat
    user newUser = getNullUser();
    printf("Enter user name:\n");
    scanf("%49s", newUser.name);
    printf("Enter user password:\n");
    scanf("%49s", newUser.pass);
    return newUser;
}

void addUser(userArray *users, user usr) //creaza un nou user
{
    for(int i = 0; i < users->used; i++)
    {
        if(strcmp(users->array[i].name, usr.name) == 0)
        {
            printf("This name is already used!\n");
            return;
        }
    }
    insertUser(users, usr);
}

user logInUser(userArray users, user user)
{
    for(int i = 0; i < users.used; i++)
    {
        if(strcmp(users.array[i].name, user.name) == 0 && strcmp(users.array[i].pass, user.pass) == 0)
        {
            return users.array[i];
        }
    }
    return getNullUser();
}

void saveUsersLogIn(userArray *users)
{
    //salveaza datele intr-un fisierul users.save
    FILE *saveFilePtr = fopen("users.save", "w");
    fprintf(saveFilePtr, "%d\n", users->used);

    for(int i = 0; i < users->used; i++)
    {
        fprintf(saveFilePtr, "%s\n%s\n", users->array[i].name, users->array[i].pass); //se scrie in fisier
    }
    fclose(saveFilePtr);
}

void loadUsersLogIn(userArray *users)
{
    //se citesc datele din fisierul users.save
    FILE *loadFilePtr = fopen("users.save", "r");
    if(loadFilePtr)
    {
        fscanf(loadFilePtr, "%d\n", &(users->used));
        users->size = users->used;
        users->array = malloc(users->size*sizeof(userArray)); //se aloca memorie pentru a citi datele
        for(int i = 0; i < users->used; i++)
        {
            fscanf(loadFilePtr, "%s\n%s\n", users->array[i].name, users->array[i].pass);
        }
        fclose(loadFilePtr);
    }
    else
    {
        users->size = users->used = 0;
    }
}
