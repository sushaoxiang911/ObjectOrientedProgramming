#include "Ordered_container.h"
#include "Record.h"
#include <stdio.h>
#include "Collection.h"
#include <string.h>
#include "Utility.h"
#include "p1_globals.h"
#include <ctype.h>

#define COMMAND_ERROR 1
#define TITLE_ERROR 2
#define TITLE_NO_FOUND 3
#define NUMBER_ERROR 4
#define ID_NO_FOUND 5
#define NAME_NO_FOUND 6
#define TITLE_EXIST 7
#define NAME_EXIST 8
#define MEMBER_PRESENT 9
#define RATING_OUT_OF_RANGE 10
#define RECORD_IN_COLLECTION 11
#define RECORD_NOT_MEMBER 12
#define COLLECTION_NOT_EMPTY 13
#define FILE_ERROR 14
#define INVALID_FILE_DATA 15

#define RATING_MINIMUM 1
#define RATING_MAXIMUM 5

int id_comp_fun(const struct Record* record1, const struct Record* record2);

int id_arg_comp_fun(const int* id, const struct Record* record);

void read_title(char* buf, FILE* infile);

int title_available(char* buf);

int name_comp_fun(const struct Collection* collection1, 
                                    const struct Collection* conllection2);

int name_arg_comp_fun(const char* name, const struct Collection* collection);

int has_member (struct Collection* collection);

void print_error_message(int code);

void unrecognized_command(void);

void print_record_title(struct Ordered_container* title_library);

void print_record_id(struct Ordered_container* id_library);

void print_collection(struct Ordered_container* catalog);

void print_library(struct Ordered_container* title_library);

void print_catalog(struct Ordered_container* catalog);

void print_allocation(struct Ordered_container* title_library, 
        struct Ordered_container* id_library, struct Ordered_container* catalog);

void add_record(struct Ordered_container* title_library, 
                                        struct Ordered_container* id_library);

void add_collection(struct Ordered_container* catalog);

void add_record_to_collection(struct Ordered_container* catalog, 
                                            struct Ordered_container* id_library);

void modify_record_rating(struct Ordered_container* id_library);

void delete_record_from_library(struct Ordered_container* title_library, 
            struct Ordered_container* id_library, struct Ordered_container* catalog);

void delete_collection_from_catalog(struct Ordered_container* catalog);

void delete_record_from_collection(struct Ordered_container* catalog, 
                                            struct Ordered_container* id_library);

int clear_library (struct Ordered_container* title_library, 
            struct Ordered_container* id_library, struct Ordered_container* catalog);

void clear_catalog(struct Ordered_container* catalog);

void clear_all(struct Ordered_container* catalog, 
        struct Ordered_container* title_library, struct Ordered_container* id_library);

void read_filename(char* buf);

void save_all(struct Ordered_container* title_library, struct Ordered_container* catalog);

int read_data_from_file (FILE* file, struct Ordered_container *title_library, 
                struct Ordered_container *id_library, struct Ordered_container *catalog);

void restore_all(struct Ordered_container* title_library, 
                struct Ordered_container* id_library, struct Ordered_container* catalog);

void quit(struct Ordered_container* title_library, struct Ordered_container* id_library, 
                                                        struct Ordered_container* catalog);

int main () {

    struct Ordered_container *id_library = OC_create_container((OC_comp_fp_t)id_comp_fun);
    struct Ordered_container *title_library = OC_create_container((OC_comp_fp_t)title_comp_fun);
    struct Ordered_container *catalog = OC_create_container((OC_comp_fp_t)name_comp_fun);    

    char action;
    char object;

    while(1) {
        printf("\nEnter command: ");
        scanf(" %c", &action);
        scanf(" %c", &object);
        switch (action) {
            case 'f':
                switch (object) {
                    case 'r':
                       	print_record_title(title_library);
                        break;
                    default:
                        unrecognized_command();
                        break;  
                }    
                break;
            case 'p':
                switch(object) {
                    case 'r':
                        print_record_id(id_library);
                        break;
                    case 'c':
                        print_collection(catalog);
                        break;
                    case 'L':
                        print_library(title_library);
                        break;
                    case 'C':
                        print_catalog(catalog);
                        break;
                    case 'a':
                        print_allocation(title_library, id_library, catalog);
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 'a':
                switch(object) {
                    case 'r':
                        add_record(title_library, id_library);
                        break;
                    case 'c':
                       	add_collection(catalog);
                        break;
                    case 'm':
                        add_record_to_collection(catalog, id_library);
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 'm':
                switch(object) {
                    case 'r':
                        modify_record_rating(id_library);
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 'd':
                switch (object) {
                    case 'r':
                        delete_record_from_library(title_library, id_library, catalog);
                        break;
                    case 'c':
                        delete_collection_from_catalog(catalog);
                        break;
                    case 'm':
                        delete_record_from_collection(catalog, id_library);
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 'c':
                switch (object) {
                    case 'L':
                        if (clear_library(title_library, id_library, catalog))
                        printf("All records deleted\n");
                        break;
                    case 'C':
                        clear_catalog(catalog);
                        printf("All collections deleted\n");
                        break;
                    case 'A':
                        clear_all(catalog, title_library, id_library);
                        printf("All data deleted\n");
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 's':
                switch(object) {
                    case 'A':
                        save_all(title_library, catalog);
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 'r':
                switch(object) {
                    case 'A':
                        restore_all(title_library, id_library, catalog);
                        break;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            case 'q':
                switch(object) {
                    case 'q':
                        quit(title_library, id_library, catalog);
                        return 0;
                    default:
                        unrecognized_command();
                        break;
                }
                break;
            default:
                unrecognized_command();
                break;
        }
    }
    return 0;
}
/*The comparison function to compare two records based on their id.
Return negative, 0, positive if record1 is smaller, equal or bigger
than record2*/
int id_comp_fun(const struct Record* record1, const struct Record* record2)
{
    return get_Record_ID(record1) - get_Record_ID(record2);
}
/*The comparison function to compare one record with a given id number.
Return negative, 0, positive if the given id is smaller, equal, or bigger
than the id number of record*/
int id_arg_comp_fun(const int* id, const struct Record* record)
{
    return *id - get_Record_ID(record);
}
/*Read the tile from FILE and save in the buf
All the leading and tailing whitespaces are eliminated. Eliminate inside duplicate whitesapces*/
void read_title(char* buf, FILE* infile)
{
    int i, j;
    char backup[TITLE_BUF_SIZE];
    memset(buf, 0, TITLE_BUF_SIZE);
    memset(backup, 0, TITLE_BUF_SIZE);
    fgets(buf, TITLE_BUF_SIZE, infile);

    i = strlen(buf) - 1;
    while (isspace(buf[i])) {
        buf[i--] = '\0';
    }

    i = 0;
    j = 0;
    while (i < TITLE_BUF_SIZE) {
        while (isspace(buf[i]))
            i++;
        while (!isspace(buf[i]) && buf[i] != '\0')
            backup[j++] = buf[i++];
        if (buf[i] == '\0')
            break;
        backup[j++] = ' ';
    }
    strcpy(buf, backup);
}

/*Check if the title length is bigger than 0.
Return 1 if the length is bigger than 0, return 0 and print an error message
of TITLE_ERROR if the length is 0*/
int title_available(char* buf)
{
    if (strlen(buf) > 0)
        return 1;
    else {
        print_error_message(TITLE_ERROR);
        return 0;
    }
}
/*The comparison function to compare two collections based on their name.
Return negative, 0, or positive if the name of collection1 is smaller, equal, or
bigger than the name of collection2*/
int name_comp_fun(const struct Collection* collection1, const struct Collection* collection2)
{
    return strcmp(get_Collection_name(collection1), get_Collection_name(collection2));
}
/*Then comparison function to compare the name of a collection with a given name.
Return negative, 0, positive if the given name is smaller, equal or bigger than
the name of collection*/
int name_arg_comp_fun(const char* name, const struct Collection* collection)
{
    return strcmp(name, get_Collection_name(collection));
}
/*Check if a given collection has members.
Return 1 is it is not empty, 0 if it is empty*/
int has_member(struct Collection* collection)
{
    return !Collection_empty(collection);
}
/*Read the file name into the given buf*/
void read_filename(char* buf)
{
    char format[5];
    sprintf(format, "%%%ds", FILE_BUF_SIZE - 1);
    scanf(format, buf);
}
/*Print a user friendly error message based on the error code*/
void print_error_message(int code)
{
    switch (code) {
        case COMMAND_ERROR:
            printf("Unrecognized command!\n");
            break;
        case TITLE_ERROR:
            printf("Could not read a title!\n");
            break;
        case TITLE_NO_FOUND:
            printf("No record with that title!\n");
            break;
        case NUMBER_ERROR:
            printf("Could not read an integer value!\n");
            break;
        case ID_NO_FOUND:
            printf("No record with that ID!\n");
            break;
        case NAME_NO_FOUND:
            printf("No collection with that name!\n");
            break;
        case TITLE_EXIST:
            printf("Library already has a record with this title!\n");
            break;
        case NAME_EXIST:
            printf("Catalog already has a collection with this name!\n");
            break;
        case MEMBER_PRESENT:
            printf("Record is already a member in the collection!\n");
            break;
        case RATING_OUT_OF_RANGE:
            printf("Rating is out of range!\n");
            break;
        case RECORD_IN_COLLECTION:
            printf("Cannot delete a record that is a member of a collection!\n");
            break;
        case RECORD_NOT_MEMBER:
            printf("Record is not a member in the collection!\n");
            break;
        case COLLECTION_NOT_EMPTY:
            printf("Cannot clear all records unless all collections are empty!\n");
            break;
        case FILE_ERROR:
            printf("Could not open file!\n");
            break;
        case INVALID_FILE_DATA:
            printf("Invalid data found in file!\n");
            break;
        default:
            printf("Wrong code!\n");
            break;
    }
}
/*Print the error message for COMMAND_ERROR*/
void unrecognized_command(void)
{
    print_error_message(COMMAND_ERROR);
    read_to_newline(stdin);
}
/*Obtain the title from input and print the corresponding record.
Error message would be printed based on the error type*/
void print_record_title(struct Ordered_container* title_library)
{
	char title_buf[TITLE_BUF_SIZE];
	struct Record* record_ptr;
	read_title(title_buf, stdin);
    if (title_available(title_buf)) {
        record_ptr = get_data_pointer(title_library, 
                title_buf, (OC_find_item_arg_fp_t)title_arg_comp_fun);
        if (!record_ptr){
            print_error_message(TITLE_NO_FOUND);
            return;
        }
        print_Record(record_ptr);                       
    }
}
/*Obtain the id from input and print the corresponding record.
Error message would be printed based on the error type*/
void print_record_id(struct Ordered_container* id_library)
{
	int number;
	struct Record* record_ptr;
	if (!scanf("%d", &number)){
        print_error_message(NUMBER_ERROR);
        read_to_newline(stdin);
        return;
    }
    record_ptr = get_data_pointer(id_library, &number, 
                            (OC_find_item_arg_fp_t)id_arg_comp_fun);
    if (!record_ptr) {
        print_error_message(ID_NO_FOUND);
        read_to_newline(stdin);
        return;
    }
    print_Record(record_ptr);
}
/*Obtained the name from input and print the corresponding collection
Error message would be printed based on the error type*/
void print_collection(struct Ordered_container* catalog)
{
	char name_buf[NAME_BUF_SIZE];
	struct Collection* collection_ptr;
	read_name(name_buf, stdin);
    collection_ptr = get_data_pointer(catalog, name_buf,
                                (OC_find_item_arg_fp_t)name_arg_comp_fun);
    if (!collection_ptr) {
        print_error_message(NAME_NO_FOUND);
        read_to_newline(stdin);
        return;
    }
    print_Collection(collection_ptr);
}
/*Print all the record in the library. Print empty message if there is no record inside*/
void print_library(struct Ordered_container* title_library)
{
	if (OC_empty(title_library))
        printf("Library is empty\n");
    else {
        printf("Library contains %d records:\n", OC_get_size(title_library));
	    OC_apply(title_library, (OC_apply_fp_t)print_Record);
	}
}
/*Print all the collection in the catalog. Print empty message if there is no collection*/
void print_catalog(struct Ordered_container* catalog)
{
	if (OC_empty(catalog))
        printf("Catalog is empty\n");
    else {
        printf("Catalog contains %d collections:\n", OC_get_size(catalog));
        OC_apply(catalog, (OC_apply_fp_t)print_Collection);
    }
}
/*Print all the memory allocation status. Message includs record number, collection number
container number, container items in use and container items allocated*/
void print_allocation(struct Ordered_container* title_library, 
        struct Ordered_container* id_library, struct Ordered_container* catalog)
{
	printf("Memory allocations:\n");
    printf("Records: %d\n", OC_get_size(title_library));
    printf("Collections: %d\n", OC_get_size(catalog));
    printf("Containers: %d\n", g_Container_count);
    printf("Container items in use: %d\n", g_Container_items_in_use);
    printf("Container items allocated: %d\n", g_Container_items_allocated);
    printf("C-strings: %d bytes total\n", g_string_memory);
}
/*Add a new record with input title and medium.
Error message would be printed based on error type*/
void add_record(struct Ordered_container* title_library, struct Ordered_container* id_library)
{
	char medium_buf[MEDIUM_BUF_SIZE];
	char title_buf[TITLE_BUF_SIZE];
	struct Record* record_ptr;
	read_medium(medium_buf, stdin);
    read_title(title_buf, stdin);
    if (title_available(title_buf)) {
        record_ptr = get_data_pointer(title_library, 
        	title_buf, (OC_find_item_arg_fp_t)title_arg_comp_fun);
    	if (record_ptr) {
        	print_error_message(TITLE_EXIST);
        	return;
        }
        record_ptr = create_Record(medium_buf, title_buf);
    	OC_insert(id_library, record_ptr);
    	OC_insert(title_library, record_ptr);
    	printf("Record %d added\n", get_Record_ID(record_ptr));	
    }
}
/*Add a new collection with input name.
Error message would be printed based on the error type*/
void add_collection(struct Ordered_container* catalog)
{
	char name_buf[NAME_BUF_SIZE];
	struct Collection* collection_ptr;
	read_name(name_buf, stdin);
    collection_ptr = get_data_pointer(catalog, name_buf, 
       			             (OC_find_item_arg_fp_t)name_arg_comp_fun);
    if (collection_ptr){
       	print_error_message(NAME_EXIST);
	   	read_to_newline(stdin);
	   	return;
    }
    collection_ptr = create_Collection(name_buf);
    OC_insert(catalog, collection_ptr);
    printf("Collection %s added\n", get_Collection_name(collection_ptr));	
}
/*add a record into a given collection. record id number and collection name are input
Error message would be printed based on the error type*/
void add_record_to_collection(struct Ordered_container* catalog, 
                                    struct Ordered_container* id_library)
{
	char name_buf[NAME_BUF_SIZE];
	int number;
	struct Collection* collection_ptr;
	struct Record* record_ptr;
	read_name(name_buf, stdin);
    collection_ptr = get_data_pointer(catalog, name_buf,
			                (OC_find_item_arg_fp_t)name_arg_comp_fun);
    if (!collection_ptr) {
        print_error_message(NAME_NO_FOUND);
        read_to_newline(stdin);
        return;
    }
    if (!scanf("%d", &number)) {
        print_error_message(NUMBER_ERROR);
        read_to_newline(stdin);
        return;
    }
    record_ptr = get_data_pointer(id_library, &number, 
		       			(OC_find_item_arg_fp_t)id_arg_comp_fun);
    if (!record_ptr) {
        print_error_message(ID_NO_FOUND);
        read_to_newline(stdin);
        return;
    }
    if (add_Collection_member(collection_ptr, record_ptr)) {
        print_error_message(MEMBER_PRESENT);
        read_to_newline(stdin);
        return;
    }
    printf("Member %d %s added\n", get_Record_ID(record_ptr), get_Record_title(record_ptr));
}
/*Modify the rating number of a given record. New rating and record id number
are input.
Error message would be printed based on the error type.*/
void modify_record_rating(struct Ordered_container* id_library)
{
	int number;
	struct Record* record_ptr;
	if (!scanf("%d", &number)) {
        print_error_message(NUMBER_ERROR);
        read_to_newline(stdin);
        return;
    }
    record_ptr = get_data_pointer(id_library, &number, 
                            (OC_find_item_arg_fp_t)id_arg_comp_fun);
    if (!record_ptr){
        read_to_newline(stdin);
        print_error_message(ID_NO_FOUND);
        return;
    }
    if (!scanf("%d", &number)) {
        print_error_message(NUMBER_ERROR);
        read_to_newline(stdin);
        return;
    }
    if (number < RATING_MINIMUM || number > RATING_MAXIMUM) {
        print_error_message(RATING_OUT_OF_RANGE);
        read_to_newline(stdin);
        return;
    }
    set_Record_rating(record_ptr, number);
    printf("Rating for record %d changed to %d\n", 
                                get_Record_ID(record_ptr), number);
}
/*Delete a record from the library. The title of the record is input.
Error message would be printed based on the error type.*/
void delete_record_from_library(struct Ordered_container* title_library, 
					struct Ordered_container* id_library, struct Ordered_container* catalog)
{
	char title_buf[TITLE_BUF_SIZE];
	struct Record* record_ptr;
	read_title(title_buf, stdin);
    if (!title_available(title_buf))
        return;
    record_ptr = get_data_pointer(title_library, title_buf,
    (OC_find_item_arg_fp_t)title_arg_comp_fun);
    if (!record_ptr) {
        print_error_message(TITLE_NO_FOUND);
        return;
    }
    if (OC_apply_if_arg(catalog, (OC_apply_if_arg_fp_t)
    								is_Collection_member_present, record_ptr)) {
        print_error_message(RECORD_IN_COLLECTION);
        return;
    }
    printf("Record %d %s deleted\n", get_Record_ID(record_ptr), get_Record_title(record_ptr));
    OC_delete_item(title_library, OC_find_item(title_library, record_ptr));
    OC_delete_item(id_library, OC_find_item(id_library, record_ptr));
    destroy_Record(record_ptr);
}
/*Delete a collection from the catalog. The name of the collection is input.
Error message would be printed based on the error type.*/
void delete_collection_from_catalog(struct Ordered_container* catalog)
{
	char name_buf[NAME_BUF_SIZE];
	struct Collection* collection_ptr;
	read_name(name_buf, stdin);
    collection_ptr = get_data_pointer(catalog, name_buf,
			                (OC_find_item_arg_fp_t)name_arg_comp_fun);
    if (!collection_ptr) {
        read_to_newline(stdin);
        print_error_message(NAME_NO_FOUND);
        return;
    }
    printf("Collection %s deleted\n", get_Collection_name(collection_ptr));
    OC_delete_item(catalog, OC_find_item(catalog, collection_ptr));
    destroy_Collection(collection_ptr);
}
/*Delete a record from the collection. The id of the record and the name of the collection
are input. 
Error message would be printed based on the error type.*/
void delete_record_from_collection(struct Ordered_container* catalog, 
                                                struct Ordered_container* id_library)
{
	char name_buf[NAME_BUF_SIZE];
	int number;
	struct Collection* collection_ptr;
	struct Record* record_ptr;
	read_name(name_buf, stdin);
    collection_ptr = get_data_pointer(catalog, name_buf, 
		                    (OC_find_item_arg_fp_t)name_arg_comp_fun);
    if (!collection_ptr){
        read_to_newline(stdin);
        print_error_message(NAME_NO_FOUND);
        return;
    }
    if (!scanf("%d", &number)) {
        print_error_message(NUMBER_ERROR);
        read_to_newline(stdin);
        return;
    }
    record_ptr = get_data_pointer(id_library, &number,
				            (OC_find_item_arg_fp_t)id_arg_comp_fun);
    if (!record_ptr) {
        read_to_newline(stdin);
        print_error_message(ID_NO_FOUND);
        return;
    }
	if (is_Collection_member_present(collection_ptr, record_ptr)) {
        printf("Member %d %s deleted\n", get_Record_ID(record_ptr), 
                                            get_Record_title(record_ptr));
        remove_Collection_member(collection_ptr, record_ptr);
    } else {
        print_error_message(RECORD_NOT_MEMBER);
        read_to_newline(stdin);
    }

}
/*Clear the whole library, delete all the record inside.
Return 0 and print specific error message if there is an error.
Return 1 if success.*/
int clear_library (struct Ordered_container* title_library, 
				struct Ordered_container* id_library, struct Ordered_container* catalog)
{
    if (OC_apply_if(catalog, (OC_apply_if_fp_t)has_member)) {
        print_error_message(COLLECTION_NOT_EMPTY);
        read_to_newline(stdin);
        return 0;
    } else {
        OC_apply(title_library, (OC_apply_fp_t)destroy_Record);
        OC_clear(title_library);
        OC_clear(id_library);
        reset_Record_ID_counter();
        return 1;
    }
}
/*Clear the whole catalog, delete all the collection inside.*/
void clear_catalog(struct Ordered_container* catalog)
{
    OC_apply(catalog, (OC_apply_fp_t)destroy_Collection);
    OC_clear(catalog);
}
/*Clear the whole catalog and the library, delete all the collection and record inside.*/
void clear_all(struct Ordered_container* catalog, 
			struct Ordered_container* title_library, struct Ordered_container* id_library)
{
    clear_catalog(catalog);
    clear_library(title_library, id_library, catalog);
}
/*Save the record collection into a file with a specific format.
Error message would be printed based on the error type.*/
void save_all(struct Ordered_container* title_library, struct Ordered_container* catalog)
{
	char file_buf[FILE_BUF_SIZE];
	FILE* file;
	int number;
	read_filename(file_buf);
    file = fopen(file_buf, "w");
    if (file == NULL) {
        read_to_newline(stdin);
        print_error_message(FILE_ERROR);
        return;
    }
    number = OC_get_size(title_library);
    fprintf(file, "%d\n", number);
    OC_apply_arg(title_library, (OC_apply_arg_fp_t)save_Record, file);
    number = OC_get_size(catalog);
    fprintf(file, "%d\n", number);
    OC_apply_arg(catalog, (OC_apply_arg_fp_t)save_Collection, file);
    printf("Data saved\n");
    fclose(file);
}
/*Read the formatted data from the file with given FILE.
Save the data into library and catalog.
Return 0 if there is a wrong format. Return 1 if success.*/
int read_data_from_file (FILE* file, struct Ordered_container *title_library, struct Ordered_container *id_library, struct Ordered_container *catalog)
{
    int number;
    int i;
    struct Record* record_ptr;
    struct Collection* collection_ptr;
    if (!fscanf(file, "%d", &number)) {
        return 0;
    }
    for (i = 0; i < number; i++) {
        record_ptr = load_Record(file);
        if (!record_ptr) {
            return 0;
        }
        OC_insert(title_library, record_ptr);
        OC_insert(id_library, record_ptr);
    }
    if (!fscanf(file, "%d", &number)) {
        return 0;
    }
    for (i = 0; i < number; i++) {
        collection_ptr = load_Collection(file, title_library);
        if (!collection_ptr){
            return 0;
        }
        OC_insert(catalog, collection_ptr);
    }
    return 1;
}
/*Read formatted data and restore into the library and catalog. File name is input.
Error message would be printed based on the error type.*/
void restore_all(struct Ordered_container* title_library, 
			struct Ordered_container* id_library, struct Ordered_container* catalog)
{
	char file_buf[FILE_BUF_SIZE];
	FILE* file;
	read_filename(file_buf);
  	file = fopen(file_buf, "r");
    if (file == NULL) {
        read_to_newline(stdin);
        print_error_message(FILE_ERROR);
        return;
    }
    clear_all(catalog, title_library, id_library);
    if (!read_data_from_file(file, title_library, id_library, catalog)) {
        read_to_newline(stdin);
        clear_all(catalog, title_library, id_library);
        print_error_message(INVALID_FILE_DATA);
    } else {
        printf("Data loaded\n");
    }
    fclose(file);
}
/*Quit the program and delete all the data inside.*/
void quit(struct Ordered_container* title_library, 
			struct Ordered_container* id_library, struct Ordered_container* catalog)
{
	clear_all(catalog, title_library, id_library);
    printf("All data deleted\n");
    OC_destroy_container(catalog);
    OC_destroy_container(title_library);
    OC_destroy_container(id_library);
    printf("Done\n");
}

