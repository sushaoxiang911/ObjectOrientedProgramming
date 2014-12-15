#include "Ordered_container.h"
#include <stdlib.h>
#include "p1_globals.h"
#include <stdio.h>
#include "Utility.h"

struct LL_Node {
    struct LL_Node* prev;
    struct LL_Node* next;
    void* data_ptr;
};

struct Ordered_container {
    OC_comp_fp_t comp_func;
    struct LL_Node* first;
    struct LL_Node* last;
    int size;
};

struct Ordered_container* OC_create_container(OC_comp_fp_t fptr) {
    
    struct Ordered_container *new_container = allocate_memory(sizeof(struct Ordered_container));
    
    g_Container_count++;

    new_container->comp_func = fptr;
    new_container->size = 0;
    new_container->first = NULL;
    new_container->last = NULL;
    
    return new_container;
}

void OC_destroy_container(struct Ordered_container* c_ptr) {

    g_Container_count--;    

    OC_clear(c_ptr); 
    free(c_ptr);
}

void OC_clear(struct Ordered_container* c_ptr) {

    struct LL_Node *current_node = c_ptr->first, *temp;      
    
    g_Container_items_in_use -= c_ptr->size;
    g_Container_items_allocated -= c_ptr->size;

    while(current_node) {
        temp = current_node;
        current_node = current_node->next;
        free(temp);
    }
    c_ptr->first = NULL;
    c_ptr->last = NULL;
    c_ptr->size = 0;
}

int OC_get_size(const struct Ordered_container* c_ptr) {
    return c_ptr->size;
}

int OC_empty(const struct Ordered_container* c_ptr) {
    return !c_ptr->size;
}

void* OC_get_data_ptr(const void* item_ptr) {
    const struct LL_Node* node_ptr = item_ptr;
    if (!node_ptr)
        return NULL;
    return node_ptr->data_ptr;
}

void OC_delete_item(struct Ordered_container* c_ptr, void* item_ptr) {

    struct LL_Node *node = item_ptr;
    
    g_Container_items_in_use--;
    g_Container_items_allocated--;
    
            
    c_ptr->size--;
    if (node->next && node->prev) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        free(node);
        return;
    } else if (!node->next && !node->prev) {
        c_ptr->first = NULL;
        c_ptr->last = NULL;
        free(node); 
        return;
    } else if (node->next && !node->prev) {
        node->next->prev = NULL;
        c_ptr->first = node->next;
        free(node);
        return;
    } else {
        node->prev->next = NULL;
        c_ptr->last = node->prev;
        free(node);
        return;
    }

}

void OC_insert(struct Ordered_container* c_ptr, void* data_ptr) {

    struct LL_Node *current_node, *new_node;   
    
    g_Container_items_in_use++;
    g_Container_items_allocated++;   
 

    current_node = c_ptr->first;
    new_node = allocate_memory(sizeof(struct LL_Node));
    new_node->data_ptr = data_ptr;
    new_node->next = NULL;
    new_node->prev = NULL;

    c_ptr->size++;
    
    if (!current_node) {
        c_ptr->first = new_node;
        c_ptr->last = new_node;
        return;
    } else if ((c_ptr->comp_func)(data_ptr, c_ptr->first->data_ptr) <= 0) {
        c_ptr->first->prev = new_node;
        new_node->next = c_ptr->first;
        c_ptr->first = new_node;
        return;
    } else if ((c_ptr->comp_func)(data_ptr, c_ptr->last->data_ptr) > 0) {
        c_ptr->last->next = new_node;
        new_node->prev = c_ptr->last;
        c_ptr->last = new_node;
        return;
    }
    while (current_node) {
        if (((c_ptr->comp_func)(data_ptr, current_node->data_ptr) > 0) &&
                ((c_ptr->comp_func)(data_ptr, current_node->next->data_ptr) <= 0)){
            current_node->next->prev = new_node;
            new_node->next = current_node->next;
            current_node->next = new_node;
            new_node->prev = current_node;
            return;
        }
        current_node = current_node->next;
    }
    
}

void* OC_find_item(const struct Ordered_container* c_ptr, const void* data_ptr) {
    
    struct LL_Node *current_node = c_ptr->first;
    while (current_node) {
        if ((c_ptr->comp_func)(data_ptr, current_node->data_ptr) == 0) {
            return current_node;
        } 
        current_node = current_node->next;
    }
    return NULL;

}

void* OC_find_item_arg(const struct Ordered_container* c_ptr, const void* arg_ptr, 
                                                        OC_find_item_arg_fp_t fafp){
    struct LL_Node *current_node = c_ptr->first;
    while (current_node) {
        if ((fafp)(arg_ptr, current_node->data_ptr) == 0) {
            return current_node;
        }
        current_node = current_node->next;       
    }
    return NULL;
}

void OC_apply(const struct Ordered_container* c_ptr, OC_apply_fp_t afp) {
    struct LL_Node *current_node = c_ptr->first;
    while (current_node) {
        (afp)(current_node->data_ptr);
        current_node = current_node->next;
    }
}

int OC_apply_if(const struct Ordered_container* c_ptr, OC_apply_if_fp_t afp) {
    struct LL_Node *current_node = c_ptr->first;
    while (current_node) {
        int func_result = (afp)(current_node->data_ptr);
        if (func_result != 0){
            return func_result;
        }
        current_node = current_node->next;
    }
    return 0;
}

void OC_apply_arg(const struct Ordered_container* c_ptr, OC_apply_arg_fp_t afp, void* arg_ptr) {
    struct LL_Node *current_node = c_ptr->first;
    while (current_node) {
        (afp)(current_node->data_ptr, arg_ptr);
        current_node = current_node->next;
    }
}

int OC_apply_if_arg(const struct Ordered_container* c_ptr, OC_apply_if_arg_fp_t afp, void* arg_ptr) {
    struct LL_Node *current_node = c_ptr->first;
    while (current_node) {
        int func_result = (afp)(current_node->data_ptr, arg_ptr);
        if (func_result != 0){
            return func_result;
        }
        current_node = current_node->next;
    }
    return 0;
}




