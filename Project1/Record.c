#include <stdlib.h>
#include <string.h>
#include "Record.h"
#include "Utility.h"
#include "p1_globals.h"
#include <stdio.h>

int global_ID = 0;

struct Record {
    char* title;
    int ID;
    char* medium;
    int rating;
};

struct Record* create_Record(const char* medium, const char* title) 
{
    struct Record* record_ptr = allocate_memory(sizeof(struct Record));

    g_string_memory = g_string_memory + strlen(title) + strlen(medium) + 2;

    record_ptr->title = string_copy(title);
    record_ptr->medium = string_copy(medium);
    record_ptr->ID = ++global_ID;
    record_ptr->rating = 0;
    return record_ptr;
}

void destroy_Record(struct Record* record_ptr)
{
    g_string_memory = g_string_memory - 
        strlen(record_ptr->title) - strlen(record_ptr->medium) - 2;

    free(record_ptr->title);
    free(record_ptr->medium);
    free(record_ptr);
}

int get_Record_ID(const struct Record* record_ptr)
{
    return record_ptr->ID;
}

const char* get_Record_title(const struct Record* record_ptr) 
{
    return record_ptr->title;
}

void set_Record_rating(struct Record* record_ptr, int new_rating)
{
    record_ptr->rating = new_rating;
}

void print_Record(const struct Record* record_ptr)
{
    if (!record_ptr->rating)
        printf("%d: %s u %s\n", record_ptr->ID, record_ptr->medium, record_ptr->title);
    else
        printf("%d: %s %d %s\n", record_ptr->ID, record_ptr->medium, 
                                                record_ptr->rating, record_ptr->title);
}

void save_Record(const struct Record* record_ptr, FILE* outfile)
{
    fprintf(outfile, "%d %s %d %s\n", record_ptr->ID, record_ptr->medium, record_ptr->rating, 
            record_ptr->title);
}

struct Record* load_Record(FILE* infile)
{
    char format[10];
    char title_buf[TITLE_BUF_SIZE];
    char medium_buf[MEDIUM_BUF_SIZE];
    int rating = -1;
    int id = 0;
    struct Record* new_record;
    sprintf(format, "%%d %%%ds %%d", MEDIUM_BUF_SIZE - 1);
    if (fscanf(infile, format, &id, medium_buf, &rating) != 3)
        return NULL;
    fgetc(infile);
    if (!fgets(title_buf, TITLE_BUF_SIZE, infile))
        return NULL;
    title_buf[strlen(title_buf) - 1] = '\0';
    new_record = create_Record(medium_buf, title_buf);
    new_record->rating = rating;
    new_record->ID = id;
    global_ID--;
    if (id > global_ID)
        global_ID = id;
    return new_record;
}

void reset_Record_ID_counter(void)
{
    global_ID = 0;
}
