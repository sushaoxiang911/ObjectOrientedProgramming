#include "Ordered_container.h"
#include "p1_globals.h"
#include <stdlib.h>
#include <stdio.h>
#include "Utility.h"

struct Ordered_container {
    OC_comp_fp_t comp_fun;
    void** array;
    int allocation;
    int size;
};

int b_search(const void* data_ptr, void** array, int size, int(*)(const void*, const void*));


struct Ordered_container* OC_create_container(OC_comp_fp_t f_ptr) {

    struct Ordered_container* new_container = allocate_memory(sizeof(struct Ordered_container));

    g_Container_count++;
    g_Container_items_allocated += 3;

    new_container->size = 0;
    new_container->allocation = 3;
    new_container->array = allocate_memory(3 * sizeof(void *));
    new_container->comp_fun = f_ptr;
    return new_container;
}

void OC_destroy_container(struct Ordered_container* c_ptr) {
    
    g_Container_count--;
    g_Container_items_in_use -= c_ptr->size;
    g_Container_items_allocated -= c_ptr->allocation;    

    free(c_ptr->array);
    free(c_ptr);

    
}

void OC_clear(struct Ordered_container* c_ptr) {
    
    g_Container_items_in_use -= c_ptr->size;
    g_Container_items_allocated = g_Container_items_allocated - c_ptr->allocation + 3;


    free(c_ptr->array);
    c_ptr->array = allocate_memory(3 * sizeof(void *));
    c_ptr->size = 0;
    c_ptr->allocation = 3;
}

int OC_get_size(const struct Ordered_container* c_ptr) {
    return c_ptr->size;
}

int OC_empty(const struct Ordered_container* c_ptr) {
    return !c_ptr->size;
}

void* OC_get_data_ptr(const void* item_ptr) {
    void* data_ptr;
    if (!item_ptr)
        return NULL;
    data_ptr = *((void **)item_ptr);
    return data_ptr;
}

/***************************
check item existed????
*************************/


void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr) {
    void** i_ptr = item_ptr;
    void** end_ptr = c_ptr->array + c_ptr->size - 1;
    
    g_Container_items_in_use--;
    
    while (i_ptr != end_ptr) {
        *i_ptr = *(i_ptr+1);
        i_ptr++;
    }
    c_ptr->size--;
}

void OC_insert(struct Ordered_container* c_ptr, void* data_ptr) {
    int b_search_result;

    g_Container_items_in_use++;
    g_Container_items_allocated -= c_ptr->allocation;



    /* No item */
    if (OC_empty(c_ptr)) {
        c_ptr->array[0] = data_ptr;
    } else {
    
        /* No free space, reallocation */
        if (c_ptr->size == c_ptr->allocation) {
            void **new_array;
            int i;
            c_ptr->allocation = 2 * (c_ptr->size + 1);
            new_array = allocate_memory(c_ptr->allocation * sizeof(void* ));
            for (i = 0; i < c_ptr->size; i++) {
                new_array[i] = c_ptr->array[i];
            }
            free(c_ptr->array);
            c_ptr->array = new_array;
        }
        b_search_result = b_search(data_ptr, c_ptr->array, c_ptr->size, c_ptr->comp_fun);
        if ((c_ptr->comp_fun)(data_ptr, c_ptr->array[b_search_result]) <= 0) {
            int i;
            for (i = c_ptr->size; i > b_search_result; i--) {
                c_ptr->array[i] = c_ptr->array[i-1];
            }
            c_ptr->array[b_search_result] = data_ptr;
        } else {
            int i;
            for (i = c_ptr->size; i > b_search_result+1; i--) {
                c_ptr->array[i] = c_ptr->array[i-1];
            }
            c_ptr->array[b_search_result+1] = data_ptr;
        }
    }
    c_ptr->size++;
    g_Container_items_allocated += c_ptr->allocation;


} 

void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr) 
{
  
    return OC_find_item_arg(c_ptr, data_ptr, (OC_find_item_arg_fp_t)c_ptr->comp_fun);
/*  int b_search_result;
    if (c_ptr->size == 0)
        return NULL;    

    b_search_result = b_search(data_ptr, c_ptr->array, c_ptr->size, c_ptr->comp_fun);
    if ((c_ptr->comp_fun)(c_ptr->array[b_search_result], data_ptr) == 0)
        return c_ptr->array + b_search_result;
    else 
        return NULL;
*/
}


void * OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, OC_find_item_arg_fp_t fafp)
{
    int b_search_result;
    if (OC_empty(c_ptr)) {
        return NULL;
    }
    b_search_result = b_search(arg_ptr, c_ptr->array, c_ptr->size, fafp);
    
    

    if ((fafp)(arg_ptr, c_ptr->array[b_search_result]) == 0)
        return c_ptr->array + b_search_result;
    else 
        return NULL;
}

void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp) 
{
    int i, size;
    void** array;
    size = c_ptr->size;
    array = c_ptr->array;
    for (i = 0; i < size; i++)
        afp(array[i]);
}

int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp)
{
    int i, size, result;
    void** array;
    size = c_ptr->size;
    array = c_ptr->array;
    for (i = 0; i < size; i++) {
        result = afp(array[i]);
        if (result != 0)
            return result;
    }
    return 0;
}

void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr)
{
    int i, size;
    void** array;
    size = c_ptr->size;
    array = c_ptr->array;
    for (i = 0; i < size; i++) {
        afp(array[i], arg_ptr);
    }
}

int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr)
{
    int i, size, result;
    void** array;
    size = c_ptr->size;
    array = c_ptr->array;
    for (i = 0; i < size; i++) {
        result = afp(array[i], arg_ptr);
        if (result != 0)
            return result;
    }
    return 0;
}

int b_search(const void* arg_ptr, void** array, int size, OC_find_item_arg_fp_t fafp) {
    int low, high, mid, comp_result;
    low = 0;
    high = size - 1;
    while (low <= high) {
        mid = (high + low) / 2;
        comp_result = fafp(arg_ptr, array[mid]);
        if (comp_result < 0)
            high = mid - 1;
        else if (comp_result > 0)
            low = mid + 1;
        else
            return mid;  
    }
    return mid;
}
