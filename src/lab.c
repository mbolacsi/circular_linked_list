#include "lab.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Creates a new list with the provided data management callbacks.
 *
 * Allocates memory for a new list and initializes the function pointers
 * for data destruction and comparison. Also, initializes the list size
 * to 0 and sets up a sentinel node.
 *
 * @param destroy_data Function to destroy data elements when removing from the list (Already implemented in test-lab.c)
 * @param compare_to Function to compare data elements (Already implemented in test-lab.c)
 * @return A pointer to the newly allocated list, or NULL if memory allocation fails
 */
list_t *list_init(void (*destroy_data)(void *),
                  int (*compare_to)(const void *, const void *)) {

    // Allocate memory... Hopefully we have enough... it'd really suck if we... ran out
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (!list) {
        return NULL;  // fuck we ran out
    }

    // Callbacks go brrr.
    list->destroy_data = destroy_data;  // WE MUST DESTROY
    list->compare_to = compare_to;  // A dereferenced null pointer is equal to a dereferenced null pointer right????
    list->size = 0;  

    // We should make a pact with the devil to ensure we have enough RAM for this madness.
    node_t *sentinel = (node_t *)malloc(sizeof(node_t));
    if (!sentinel) {
        free(list);  // OH NO, WE'RE OUT OF MEMORY, HIDE THE EVIDENCE
        return NULL;
    }

     // Sentinel node  just points to itself... like a psycopath.
    sentinel->data = NULL;  
    sentinel->next = sentinel;  
    sentinel->prev = sentinel;  

    // We will make the pyscopath the head... Just like in government... YAY!
    list->head = sentinel;

    return list;
}

/**
 * @brief Destroys the list and frees all associated memory.
 *
 * Iterates over all nodes in the list, calling the destroy_data function
 * on each node's data and freeing the node memory. Finally, frees the
 * memory allocated for the list structure itself.
 *
 * @param list A pointer to the list to destroy
 */
void list_destroy(list_t **list) {
    if (list == NULL || *list == NULL || (*list)->head == NULL) {
        return; // sad error noises
    }

    // We’re about to commit total annihilation, but first, where the fuck are we? I guess we'll just point and hope the computer understands.
    node_t *current = (*list)->head->next;
    node_t *next_node;

    // WE WILL EXTERMINATE EVERYTHING, NOTHING SHALL SURVIVE.
    while (current != (*list)->head) { 
        next_node = current->next;

        // WTF, What if I wanted to dereference a null pointer... now I can't, my day is ruined.
        if ((*list)->destroy_data != NULL) {
            (*list)->destroy_data(current->data);
        }

        free(current);
        current = next_node;
    }

    // Rest in pieces, sentinel.
    free((*list)->head);
    free(*list);
    *list = NULL;
}

/**
 * @brief Adds data to the front of the list.
 *
 * Creates a new node, sets its data pointer, and inserts it at the front
 * of the list. Updates the head pointer and the size of the list accordingly.
 *
 * @param list A pointer to an existing list
 * @param data The data to add to the list
 * @return A pointer to the updated list
 */
list_t *list_add(list_t *list, void *data) {
    // We must make space to bring a new node into this world.
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {
        return NULL;  // Overpopulation moment
    }

    // Teach this node to exist in the harsh world of lists.
    new_node->data = data;

    // There is a war going on!! put him on the front lines!!!
    node_t *sentinel = list->head;  // Sentinel node is the head
    new_node->next = sentinel->next;  // The new node's next points to the current first node
    new_node->prev = sentinel;  // The new node's prev points to the sentinel

    sentinel->next->prev = new_node;  // Update the old front’s prev to the new guy.
    sentinel->next = new_node;  // Make the sentinel point to the new front.

    // To create value for our shareholders.
    list->size++;

    return list;
}

/**
 * @brief Removes the data at the specified index from the list.
 *
 * Validates the index and removes the node at that position, updating
 * the list pointers (prev/next of adjacent nodes). Calls the destroy_data
 * function on the data of the removed node.
 *
 * @param list A pointer to the list
 * @param index The index of the element to remove
 * @return The data of the removed node, or NULL if the index is invalid
 */
void *list_remove_index(list_t *list, size_t index) {
    if (list == NULL || list->size == 0 || index >= list->size) {
        return NULL; // Your index is invalid. Take your L and move on.
    }

    // We must know where we are to find our victim.
    node_t *current = list->head->next; 

    // Search for the node we have been assigned to kill.
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    // Update the pointers of its neighbors... no witnesses.
    current->prev->next = current->next;
    current->next->prev = current->prev;

    // Find out everything this guy knows before you kill him ... copy it down.
    void *removed_data = current->data;

    // MURDER!!!
    free(current);

    // We lost him... node homie is dead, so sad, the shareholders won't be happy about this loss :( 
    list->size--;

    return removed_data;
}

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
int list_indexof(list_t *list, void *data) {
    if (list == NULL || list->head == NULL || list->size == 0 || data == NULL) {
        return -1; // Sad error noises
    }

   // Pointing at yourself  is fun!!! and the best part is no one thinks you are crazy.
    node_t *current = list->head->next; 
    size_t index = 0;

    // Are we there yet????
    while (current != list->head) { 
        if (current->data != NULL && list->compare_to(current->data, data) == 0) {
            return index; // We made it!!! yipii yipee yip yip yippeee
        }
        current = current->next; // No, we have to keep going :(
        index++;
    }

    return -1; // You can't go somewhere that doesn't exist loser.
}
