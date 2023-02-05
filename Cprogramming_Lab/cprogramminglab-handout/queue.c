/**
 * @file queue.c
 * @brief Implementation of a queue that supports FIFO and LIFO operations.
 *
 * This queue implementation uses a singly-linked list to represent the
 * queue elements. Each queue element stores a string value.
 *
 * Assignment for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Extended to store strings, 2018
 *
 * TODO: fill in your name and Andrew ID
 * @author XXX <XXX@andrew.cmu.edu>
 */

/*
    6,7,12 still has problems
*/

#include "queue.h"
#include "harness.h"

#include <stdlib.h>
#include <string.h>

/**
 * @brief Allocates a new queue
 * @return The new queue, or NULL if memory allocation failed
 */
queue_t *queue_new(void) {
    queue_t *q = (queue_t *)malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/**
 * @brief Frees all memory used by a queue
 * @param[in] q The queue to free
 */
void queue_free(queue_t *q) {
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q && q->head) {

        list_ele_t *free_element;
        for (size_t i = 0; i < (q->size) - 1; i++) {
            free_element = q->head;
            q->head = free_element->next;
            free(free_element->value);
            free(free_element);
        }
        free(q->head->value);
        free(q->head);
        free(q);
    } else {
        free(q);
    }
}

/**
 * @brief Attempts to insert an element at head of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_head(queue_t *q, const char *s) {
    if (!q) {
        return false;
    }
    list_ele_t *newh = (list_ele_t *)malloc(sizeof(list_ele_t));
    /* What should you do if the q is NULL? */

    if (!newh) {
        return false;
    }
    char *s_copy = (char *)malloc(strlen(s) + 1);

    if (!s_copy) {
        free(newh);
        return false;
    }
    strcpy(s_copy, s);
    newh->value = s_copy;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    if (q->head) {
        newh->next = q->head;
    } else {
        newh->next = NULL;
        q->tail = newh;
        // q->size = 0;
    }
    q->size += 1;
    q->head = newh;

    return true;
}

/**
 * @brief Attempts to insert an element at tail of a queue
 *
 * This function explicitly allocates space to create a copy of `s`.
 * The inserted element points to a copy of `s`, instead of `s` itself.
 *
 * @param[in] q The queue to insert into
 * @param[in] s String to be copied and inserted into the queue
 *
 * @return true if insertion was successful
 * @return false if q is NULL, or memory allocation failed
 */
bool queue_insert_tail(queue_t *q, const char *s) {
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q) {
        return false;
    }

    list_ele_t *newt = (list_ele_t *)malloc(sizeof(list_ele_t));

    if (!newt) {
        return false;
    }

    char *s_copy = (char *)malloc(strlen(s) + 1);

    if (!s_copy) {
        free(newt);
        return false;
    }
    strcpy(s_copy, s);

    newt->value = s_copy;
    newt->next = NULL;
    if (q->tail) {
        list_ele_t *former_tail = q->tail;
        former_tail->next = newt;
    } else {
        q->head = newt;
    }
    q->size += 1;
    q->tail = newt;
    return true;
}

/**
 * @brief Attempts to remove an element from head of a queue
 *
 * If removal succeeds, this function frees all memory used by the
 * removed list element and its string value before returning.
 *
 * If removal succeeds and `buf` is non-NULL, this function copies up to
 * `bufsize - 1` characters from the removed string into `buf`, and writes
 * a null terminator '\0' after the copied string.
 *
 * @param[in]  q       The queue to remove from
 * @param[out] buf     Output buffer to write a string value into
 * @param[in]  bufsize Size of the buffer `buf` points to
 *
 * @return true if removal succeeded
 * @return false if q is NULL or empty
 */
bool queue_remove_head(queue_t *q, char *buf, size_t bufsize) {
    /* You need to fix up this code. */

    if ((!q) || (!q->head) || (!buf)) {
        return false;
    }

    list_ele_t *current_head = q->head;
    char *current_value = current_head->value;

    q->head = current_head->next;
    current_head->next = NULL;

    if (strlen(current_value) < bufsize) {
        strncpy(buf, current_value, bufsize);
        *(buf + strlen(current_value)) = '\0';
    } else {
        strncpy(buf, current_value, bufsize - 1);
        *(buf + bufsize - 1) = '\0';
    }

    free(current_head->value);
    free(current_head);
    q->size = q->size - 1;

    return true;
}

/**
 * @brief Returns the number of elements in a queue
 *
 * This function runs in O(1) time.
 *
 * @param[in] q The queue to examine
 *
 * @return the number of elements in the queue, or
 *         0 if q is NULL or empty
 */
size_t queue_size(queue_t *q) {
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    if (!q) {
        return false;
    }
    size_t size = q->size;
    return size;
}

/**
 * @brief Reverse the elements in a queue
 *
 * This function does not allocate or free any list elements, i.e. it does
 * not call malloc or free, including inside helper functions. Instead, it
 * rearranges the existing elements of the queue.
 *
 * @param[in] q The queue to reverse
 */
void queue_reverse(queue_t *q) {
    /* You need to write the code for this function */
    if ((!q) || (q->size == 0)) {
        return;
    }

    list_ele_t *old_head = q->head;
    list_ele_t *old_tail = q->tail;

    for (size_t i = 0; i < q->size - 1; i++) {
        list_ele_t *tmp_head = q->head;
        q->head = tmp_head->next;
        tmp_head->next = (q->tail)->next;
        (q->tail)->next = tmp_head;
    }
    q->tail = old_head;
    q->head = old_tail;
}
