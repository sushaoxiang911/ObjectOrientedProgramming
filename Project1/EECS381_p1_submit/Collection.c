#include "Collection.h"
#include "Record.h"
#include "Ordered_container.h"
#include <stdlib.h>
#include <string.h>
#include "Utility.h"
#include "p1_globals.h"

struct Collection {
    char* name;
    struct Ordered_container* members;
};

void print_member_to_file(struct Record* record, FILE *outfile);



struct Collection* create_Collection(const char* name)
{
    struct Collection* collection_ptr = allocate_memory(sizeof(struct Collection));

    g_string_memory = g_string_memory + strlen(name) + 1;

    collection_ptr->name = string_copy(name);
    collection_ptr->members = OC_create_container((OC_comp_fp_t)title_comp_fun);
    return collection_ptr;
}

void destroy_Collection(struct Collection* collection_ptr)
{
    g_string_memory = g_string_memory - strlen(collection_ptr->name) - 1;

    free(collection_ptr->name);
    OC_destroy_container(collection_ptr->members);
    free(collection_ptr);
}

const char* get_Collection_name(const struct Collection* collection_ptr)
{
    return collection_ptr->name;
}

int Collection_empty(const struct Collection* collection_ptr)
{
    return OC_empty(collection_ptr->members);
}

int add_Collection_member(struct Collection* collection_ptr, const struct Record* record_ptr)
{
    int result = is_Collection_member_present(collection_ptr, record_ptr);
    if (!result)
    {
        OC_insert(collection_ptr->members, (void*)record_ptr);
    }
    return result;
}

int is_Collection_member_present(const struct Collection* collection_ptr, const struct Record* record_ptr)
{
    void* result_ptr = OC_find_item(collection_ptr->members, record_ptr);
    return (!result_ptr) ? 0 : 1;
}

int remove_Collection_member(struct Collection* collection_ptr, const struct Record* record_ptr)
{
    void* item_ptr = OC_find_item(collection_ptr->members, record_ptr);
    if (item_ptr) {
        OC_delete_item(collection_ptr->members, item_ptr);
        return 0;
    }
    return 1;
}

void print_Collection(const struct Collection* collection_ptr)
{
    printf("Collection %s contains:", collection_ptr->name);
    if (OC_empty(collection_ptr->members))
        printf(" None\n");
    else{
        printf("\n");
        OC_apply(collection_ptr->members, (OC_apply_fp_t)print_Record); 
    }
}

void save_Collection(const struct Collection* collection_ptr, FILE* outfile)
{   
    fprintf(outfile, "%s %d\n", collection_ptr->name, OC_get_size(collection_ptr->members));
    OC_apply_arg(collection_ptr->members, (OC_apply_arg_fp_t)print_member_to_file, outfile);
}

struct Collection* load_Collection(FILE* infile, const struct Ordered_container* records)
{
    char name_buf[NAME_BUF_SIZE];
    int number = -1;
    int i;
    char title_buf[TITLE_BUF_SIZE];
    struct Record* record;
    struct Collection* collection;
    read_name(name_buf, infile);
    if(fscanf(infile, "%d", &number) != 1)
        return NULL;
    read_to_newline(infile);
    collection = create_Collection(name_buf);
    for (i = 0; i < number; i++) {
        if (!fgets(title_buf, TITLE_BUF_SIZE, infile)) {
            destroy_Collection(collection);
            return NULL;
        }
        title_buf[strlen(title_buf) - 1] = '\0';
        record = get_data_pointer(records, title_buf, (OC_find_item_arg_fp_t)title_arg_comp_fun);
        if (record == NULL) {
            destroy_Collection(collection);
            return NULL;
        }
        OC_insert(collection->members, record);
    }
    return collection;
}

void print_member_to_file(struct Record* record, FILE *outfile)
{
    fprintf(outfile, "%s\n", get_Record_title(record));
}


