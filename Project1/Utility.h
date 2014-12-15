#ifndef UTILITY_H
#define UTILITY_H

#include "Record.h"
#include "Ordered_container.h"

#define TITLE_BUF_SIZE 64
#define NAME_BUF_SIZE 16
#define MEDIUM_BUF_SIZE 8
#define FILE_BUF_SIZE 32

/*Allocate memory of size and return the pointer
If there is no available memory, terminate the program*/
void* allocate_memory(size_t size);
/*Allocate the memory equals to source and copy source to the new position
Return the pointer of the string*/
char* string_copy(const char* source);
/*Compare two records based on their title.
Return negative, 0, positive if the title of record1 is smaller, equal or bigger than record2*/
int title_comp_fun(const struct Record* record1, const struct Record* record2);
/*Compare given tile with the title of the record
Return negative, 0, positive if the title is smaller, equal, or bigger than the title of record*/
int title_arg_comp_fun(const char* title, const struct Record* record);
/*Read all the characters in the stream up to the nextline*/
void read_to_newline(FILE* infile);
/*Read name from stream and store in buf*/
void read_name(char* buf, FILE* infile);
/*Read a medium type from stream and store in buf*/
void read_medium(char* buf, FILE* infile);
/*The functoin obtains the data which compare function returns zero when input arg
If the data doesn't exist, return NULL, else return the data pointer*/
void* get_data_pointer(const struct Ordered_container* container, void* arg, OC_find_item_arg_fp_t fun);




#endif
