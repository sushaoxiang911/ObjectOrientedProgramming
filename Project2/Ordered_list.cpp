#include "Ordered_list.h"
#include "p2_globals.h"

template<typename T, typename OF>
Ordered_list<T, OF>::Ordered_list()
{
    list_size = 0;
    begin = end();

    ++g_Ordered_list_count;
}

template<typename T, typename OF>
Ordered_list<T, OF>::Ordered_list(const Ordered_list& original)
                :list_size(original.list_size), ordering_f(original.ordering_f)
{
    if (!original.empty()) {
        Iterator it = orginal.begin();
        Node* temp_ptr = new Node(*it, nullptr, nullptr);
        begin.node_ptr = temp_ptr;
        Node* end_ptr = begin.node_ptr;
        ++it;
        while (it != orginal.end()) {
            temp_ptr = new Node(*it, end_ptr, nullptr);
            end_ptr->next = temp_ptr;
            end_ptr = temp_ptr;
            ++it;
        }
    }
    
    g_Ordered_list_Node_count += list_size;
    ++g_Ordered_list_count;
}

template<typename T, typename OF>
Ordered_list<T, OF>::Ordered_list(Ordered_list&& original) noexcept
                :list_size(original.list_size), ordering_f(original.ordering_f)
{
    begin.node_ptr = original.begin.node_ptr;
    original.list_size = 0;
    original.begin = end();
}

template<typename T, typename OF>
Ordered_list& Ordered_list<T, OF>::operator= (const Ordered_list& rhs)
{
    Ordered_list temp(rhs);
    swap(temp);
    return *this;
}

template<typename T, typename OF>
Ordered_list& Ordered_list<T, OF>::operator= (Ordered_list&& rhs) noexcept
{
    swap(rhs);
}

template<typename T, typename OF>
Ordered_list<T, OF>::~Ordered_list()
{
    clear();
    --g_Ordered_list_count;
}

template<typename T, typename OF>
void Ordered_list<T, OF>::clear() noexcept
{
    Iterator it = begin;
    while (it != end()) {
        Iterator temp = it;
        ++it;
        delete it.node_ptr;
    }

    g_Ordered_list_Node_count -= list_size;    

    list_size = 0;
    begin = end();
}

template<typename T, typename OF>
void Ordered_list<T, OF>::insert(const T& new_datum)
{
    list_size++;
    if(empty())
        begin.node_ptr = new Node(new_datum, nullptr, nullptr);
    else {
        Iterator it = begin;
        Node* end_ptr;
        while (it != end()) {
            if (!ordering_f(*it, new_datum)) {
                Node* temp_node_ptr = new Node(new_datum, it.node_ptr->prev, it.node_ptr);
                if (it.node_ptr->prev)
                    it.node_ptr->prev->next = temp_node_ptr;
                else
                    begin.node_ptr = temp_node_ptr;
                it.node_ptr->prev = temp_node_ptr;
                break;
            }
            end_ptr = it.node_ptr;
            ++it;
        }
        if (it == end())
            end_ptr->next = new Node(new_datum, end_ptr, nullptr);
    }
    
    ++g_Ordered_list_Node_count;
}

template<typename T, typename OF>
void Ordered_list<T, OF>::erase(Iterator it) noexcept
{
    list_size--;
    if (it.node_ptr->prev)
        it.node_ptr->prev->next = it.node_ptr->next;
    else
        begin.node_ptr = it.node_ptr->next;
    if (it.node_ptr->next)
        it.node_ptr->next->prev = it.node_ptr->prev;
    delete it.node_ptr;

    --g_Ordered_list_Node_count;
}

template<typename T, typename OF>
Iterator Ordered_list<T, OF>::find(const T& probe_datum) const noexcept
{
    Iterator it = begin;
    while (it != end()) {
        if (!ordering_f(*it, probe_datum) && !ordering_f(probe_datum, *it))
            break;
        ++it;
    }
    return it;
}

