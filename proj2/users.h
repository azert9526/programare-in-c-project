#pragma once

#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

typedef struct {
    float amount; // + = income, - = expense
    char type[50]; //deposit, withdrawal, transfer, payment
    char date[15], description[200];
} trs;

typedef struct {
    char type[50];
    trs *transactions;
    size_t used, size;
} account;

typedef struct {
    char name[50], pass[50];
    account *accounts;
    size_t used, size;
} user;

typedef struct {
  user *array;
  size_t used, size;
} userArray;

void insertUser(userArray *a, user element);

void deleteUser(); //todo

user readUserNamePass();

void addUser(userArray *users, user usr);

user logInUser(userArray users, user user);

void saveUsersLogIn(userArray *users);

void loadUsersLogIn(userArray *users);

#endif
