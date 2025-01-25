#ifndef LAB_H
#define LAB_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A node in the list
 *
 */
typedef struct node {
    void *data;
    struct node *next;
    struct node *prev;
} node_t;

/**
 * @brief Struct to represent a list. The list maintains 2 function pointers to
 * help with the management of the data it is storing. These functions must be
 * provided by the user of this library.
 */
typedef struct list {
    void (*destroy_data)(void *); /* Free's any memory that data allocated */
    int (*compare_to)(const void *, const void *); /* Returns 0 if data are the same */
    size_t size; /* How many elements are in the list */
    struct node *head; /* Sentinel node */
} list_t;

/**
 * @brief Create a new list with callbacks that know how to deal with the data that
 * the list is storing. The caller must pass the list to list_destroy when finished
 * to free any memory that was allocated.
 *
 * @param destroy_data Function that will free the memory for user-supplied data
 * @param compare_to Function that will compare two user data elements
 * @return struct list* Pointer to the newly allocated list
 */
list_t *list_init(void (*destroy_data)(void *),
                  int (*compare_to)(const void *, const void *));

/**
 * @brief Destroy the list and all associated data. This function will
 * call destroy_data on each node's data element.
 *
 * @param list A pointer to the list that needs to be destroyed
 */
void list_destroy(list_t **list);

/**
 * @brief Adds data to the front of the list
 *
 * @param list A pointer to an existing list
 * @param data The data to add
 * @return A pointer to the list
 */
list_t *list_add(list_t *list, void *data);

/**
 * @brief Removes the data at the specified index. If the index is invalid,
 * this function does nothing and returns NULL.
 *
 * @param list The list to remove the element from
 * @param index The index
 * @return void* The data that was removed or NULL if nothing was removed
 */
void *list_remove_index(list_t *list, size_t index);

/**
 * @brief Search for any occurrence of data from the list.
 * Internally, this function will call compare_to on each item in the list
 * until a match is found or the end of the list is reached. If there are
 * multiple copies of the same data in the list, the first one will be returned.
 *
 * @param list The list to search for data
 * @param data The data to look for
 * @return The index of the item if found or -1 if not
 */
int list_indexof(list_t *list, void *data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
