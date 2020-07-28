#ifndef DUCK_DB_H
#define DUCK_DB_H

#include "bpt.h"
#include "TextTable.h"
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace bpt;

const char *errorMessage = "> your input is invalid,print \".help\" for more infomation!\n";
const char *nextLineHeader ="> ";
const char *exitMessage = "> bye!\n";
const char *dbFileName = "./data/db.bin";

clock_t startTime,finishTime; 

bplus_tree *duck_db_ptr;

void initialSystem();
void printHelpMess();
void readCommand();
void exitCommand();
void helpCommand();
void resetCommand();
void insertCommand(const char *userCommand);
bool is_file_exist(const char *fileName);
void convertIntToKeyT(bpt::key_t *keyPtr, int *number);
double durationTime(clock_t *f, clock_t *s);
int insertRecord(bplus_tree *treePtr, int *index, value_t *values);
void updateCommand(const char *userCommand);
int updateRecord(bplus_tree *treePtr,int *index, value_t *values);
void deleteCommand(const char *userCommand);
int deleteRecord(bplus_tree *treePtr, int *index);
void selectCommand(const char *userCommand);
int searchRecord(bplus_tree *treePtr,int *index, value_t *return_val);
void printTable(int *index, value_t *values);

#endif 
