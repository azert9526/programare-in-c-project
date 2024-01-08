#pragma once

#ifndef ACCOUNT_H_INCLUDED
#define ACCOUNT_H_INCLUDED

#include "users.h"


void insertAccount(user *a, account element);

void recordTransaction(account *a, trs transaction);

void deleteAccount(user *usr, account *a);

int isDateValid(char *date);

trs readTransaction();

void printTransaction(trs transaction);

void displayTransactions(account account);

float calculateBalance(account account);

void saveAccountData(account account);

void loadAccountData(account *account);

void saveUserAccounts(user user);

void loadUserAccounts(user *user);

#endif
