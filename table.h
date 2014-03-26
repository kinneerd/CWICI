/**
 * table.h
 *
 * @author Dr. Fenwick and class
 * version Spring 2014
 */

#ifndef TABLE_H_
#define TABLE_H_

#define TABLE_SIZE 20
#define KEY_SIZE 20

//Define the structs used for the tables.
//table entries
typedef struct{
      char key[KEY_SIZE+1];
      int value;
}tableEntry;

//table contains entries and the number of elements
typedef struct {
    int numItemsInUse;
    tableEntry entry[TABLE_SIZE];
} tableType;

void store(tableType *Xtable, char *key, int val);
int retrieve(tableType *Xtable, char *key);
void initializeTable(tableType *Xtable);
void printTable(tableType *Xtable);

#endif

