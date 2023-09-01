#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

struct linkedlist
{
    struct linkedlist_node *first;
    int length;
};

struct linkedlist_node
{
    int key;
    int val;
    struct linkedlist_node *next;
};
typedef struct linkedlist_node linkedlist_node_t;

linkedlist_t *ll_init()
{

    // We have done this TODO for you as an example of how to use malloc().
    // You might want to read more about malloc() from Linux Manual page.
    // Usually free() should be used together with malloc(). For example,
    // the linkedlist you are currently implementing usually have free() in the
    // ll_free() function.

    // First, you use C's sizeof function to determine
    // the size of the linkedlist_t data type in bytes.
    // Then, you use malloc to set aside that amount of space in memory.
    // malloc returns a void pointer to the memory you just allocated, but
    // we can assign this pointer to the specific type we know we created
    linkedlist_t *list = malloc(sizeof(linkedlist_t));

    list->first = NULL;
    list->length = 0;
    return list;
}

void ll_free(linkedlist_t *list)
{

    linkedlist_node_t *current = list->first;

    for (int i = 0; i < list->length; i++)
    {
        free(current);
        current = current->next;
    }
    free(list);
}

void ll_add(linkedlist_t *list, int key, int value)
{

    linkedlist_node_t *current = list->first;
    for (int i = 0; i < list->length; i++)
    {
        if (current->key == key)
        {
            current->val = value;
            break;
        }
        current = current->next;
    }
    if (current == NULL || current->key != key)
    {
        linkedlist_node_t *newFirst = malloc(sizeof(linkedlist_node_t));
        newFirst->key = key;
        newFirst->val = value;
        linkedlist_node_t *temp = list->first;
        list->first = newFirst;
        newFirst->next = temp;
        list->length++;
    }
}

int ll_get(linkedlist_t *list, int key)
{

    linkedlist_node_t *current = list->first;

    for (int i = 0; i < list->length; i++)
    {
        if (key == current->key)
        {
            return current->val;
        }
        current = current->next;
    }
    return 0;
}

int ll_size(linkedlist_t *list)
{
    return list->length;
}
