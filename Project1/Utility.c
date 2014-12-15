#include "Utility.h"
#include <string.h>
#include "Ordered_container.h"
#include <stdio.h>
#include <stdlib.h>

void* allocate_memory(size_t size)
{
    void* ptr = malloc(size);
    if (!ptr)
        exit(1);
    return ptr;
}

char* string_copy(const char* source)
{
    char* destination = allocate_memory(strlen(source) + 1);
    strcpy(destination, source);
    return destination;
}

int title_comp_fun(const struct Record* record1, const struct Record* record2)
{
    return strcmp(get_Record_title(record1), get_Record_title(record2));
}

int title_arg_comp_fun(const char* title, const struct Record* record)
{
    return strcmp(title, get_Record_title(record));
}

void read_to_newline(FILE* infile)
{
    while (getc(infile) != '\n'){}
}

void read_name(char* buf, FILE* infile)
{
    char format[5];
    sprintf(format, "%%%ds", NAME_BUF_SIZE - 1);
    fscanf(infile, format, buf);
}

void read_medium(char* buf, FILE* infile)
{
    char format[4];
    sprintf(format, "%%%ds", MEDIUM_BUF_SIZE - 1);
    fscanf(infile, format, buf);
}

void* get_data_pointer(const struct Ordered_container* container, void* arg, OC_find_item_arg_fp_t fun)
{
    void* item = OC_find_item_arg(container, arg, fun);
    if (!item)
        return NULL;
    return OC_get_data_ptr(item);
}
