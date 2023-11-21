/*************************************************************************\
*                  Copyright (C) Gabriel Fynecontry, 2023.                *
\*************************************************************************/


#ifndef __mapreduce1_h__
#define __mapreduce1_h__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <assert.h>

#define PARTITION_SIZE 5

// Datastructure definitions

typedef struct __value_t {
    char *value;
    struct __value_t *next_value;
} value_t;


// Define the node (KV pairs) structure for the linked list
typedef struct __node_t {
    char *key;
    value_t *values;
    bool processed;
    struct __node_t *next;
    pthread_mutex_t node_lock;
} node_t;

// Define the node linked list structure
typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

// Define the hashtable with chaining
typedef struct __hashtable_t {
    int num_partitions;
    list_t *lists;
} hashtable_t;

// Create hashtable
hashtable_t myHashtable;

// Linked list and Hashtable function operators
void List_Insert(list_t *L, char *key, char *value);
void List_AppendValue(list_t *L, char *key, char *value);
bool List_Lookup(list_t *L, char *key);
void Hashtable_Insert(hashtable_t *H, char *key, char *value);
void Hashtable_AppendValue(hashtable_t *H, char *key, char *value);
void Hashtable_Lookup(hashtable_t *H, char *key);
void createhashT();

// Different function pointer types used by MR
typedef char *(*Getter)(char *key, int partition_number);
typedef void *(*Mapper)(void *file_name);
typedef void (*Reducer)(char *key, Getter get_func, int partition_number);
typedef unsigned long (*Partitioner)(char *key, int num_partitions);

// External functions: these are what you must define
void MR_Emit(char *key, char *value);
void MR_Run(int argc, char *argv[],
            Mapper map, int num_mappers,
            Reducer reduce, int num_reducers,
            Partitioner partition);
void MR_View();
void MR_CleanUp();


unsigned long MR_DefaultHashPartition(char *key, int num_partitions);
void* Reduce_Wrapper(int partition);

// Initialize the linked list
void List_Init(list_t *L) {
    L->head = NULL;
    int rc =  pthread_mutex_init(&L->lock, NULL);
    assert(rc == 0);
}

// Insert a key-value pair into the linked list
void List_Insert(list_t *L, char *key, char *value) {
    pthread_mutex_lock(&L->lock);
    if (List_Lookup(L, key)){
        List_AppendValue(L, key, value);
        pthread_mutex_unlock(&L->lock);
        return;
    }
    node_t *n = (node_t *)malloc(sizeof(node_t));
    if (n == NULL) {
        perror("malloc");
        pthread_mutex_unlock(&L->lock);
        return;
    }
    int rc = pthread_mutex_init(&n->node_lock, NULL);
    assert(rc == 0);
    n->key = strdup(key);
    n->processed = false;
    n->values = (value_t *)malloc(sizeof(value_t));
    if (n->values == NULL) {
        perror("malloc");
        free(n);
        pthread_mutex_unlock(&L->lock);
        return;
    }
    n->values->value = value;
    n->values->next_value = NULL;


    n->next = L->head;
    L->head = n;
    pthread_mutex_unlock(&L->lock);
}

// Append a value to the list of values for a given key
void List_AppendValue(list_t *L, char *key, char *value) {
    node_t *curr = L->head;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            value_t *n_value = (value_t *)malloc(sizeof(value_t));
            if (n_value == NULL) {
                perror("malloc");
                return;
            }
            n_value->value = value;
            n_value->next_value = curr->values;
            curr->values = n_value;
            break;
        }
        curr = curr->next;
    }
}

// Check if parsed key exist in linked list
bool List_Lookup(list_t *L, char *key) {
    node_t *curr = L->head;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}


// Initialize the hashtable
void Hashtable_Init(hashtable_t *H, int num_partitions) {
    H->num_partitions = num_partitions;

    // Allocate memory for the array of lists
    H->lists = (list_t *)malloc(num_partitions * sizeof(list_t));
    if (H->lists == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Initialize each list in the array
    for (int i = 0; i < num_partitions; ++i) {
        List_Init(&H->lists[i]);
    }
}

// Insert a key-value pair into the hashtable
void Hashtable_Insert(hashtable_t *H, char *key, char *value) {
    // Use the partitioning function to determine the partition index
    int partition = MR_DefaultHashPartition(key, H->num_partitions);
    //printf("inserting %s into partition %d\n", key, partition);

    // Insert the key-value pair into the corresponding list
    List_Insert(&H->lists[partition], key, value);
}

// Append a value to the list of values for a given key in the hashtable
void Hashtable_AppendValue(hashtable_t *H, char *key, char *value) {
    // Use the partitioning function to determine the partition index
    int partition = MR_DefaultHashPartition(key, H->num_partitions);

    // Append the value to the list of values for the given key in the corresponding list
    List_AppendValue(&H->lists[partition], key, value);
}

// Lookup values for a key in the hashtable
void Hashtable_Lookup(hashtable_t *H, char *key) {
    // Use the partitioning function to determine the partition index
    int partition = MR_DefaultHashPartition(key, H->num_partitions);

    // Lookup the values for the key in the corresponding list
    List_Lookup(&H->lists[partition], key);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

char* get_value(char *key, int partition_number){
    node_t *curr = myHashtable.lists[partition_number].head;
    while (curr)
    {
        if (strcmp(curr->key, key) == 0){
            if (curr->values){
                char *value = curr->values->value;
                curr->values = curr->values->next_value;
                return value;
            }
        }
        curr = curr->next;
    }
    return NULL;
}


void* Reduce_Wrapper(int partition)
{

    node_t *curr = myHashtable.lists[partition].head;
    while (curr)
    {
        if (curr->processed) {continue;}
        if (pthread_mutex_trylock(&curr->node_lock) != 0) {
            perror("pthread_mutex_trylock");
            exit(EXIT_FAILURE);
        }
        Reduce(curr->key,get_value,partition);
        curr->processed = true;
        pthread_mutex_unlock(&curr->node_lock);
        curr = curr->next;
    }
}


void MR_Run(int argc, char *argv[], Mapper map, int num_mappers,
            Reducer reduce, int num_reducers,
            Partitioner partition)
{
    
    Hashtable_Init(&myHashtable, PARTITION_SIZE);


    printf("\n\n====Starting mapping operations====\n\n");
    // Create threads
    pthread_t mappers[num_mappers];
    pthread_t reducers[num_reducers];

    for (int i = 0; i < num_mappers; i++){
        int rc = pthread_create(&mappers[i], NULL, map, argv[i + 1]);
        if (rc != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for mappers to finish
    for (int i = 0; i < num_mappers; i++){
        int rc = pthread_join(mappers[i], NULL);
        if (rc != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }
    printf("====Mappers are done====\n\n====Starting reduce operations====\n\n");

    for (int i = 0; i < num_reducers; i++){
        int rc = pthread_create(&reducers[i], NULL, Reduce_Wrapper, i);
        if (rc != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for reducers to finish
    for (int i = 0; i < num_reducers; i++){
        int rc = pthread_join(reducers[i], NULL);
        if (rc != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Memory clean up operations!
    MR_CleanUp();
}

// Produces intemediate key K and set of value for key
void MR_Emit(char *key, char *value)
{
    Hashtable_Insert(&myHashtable, key, value);
}

// View all keys and values in hashtable
void MR_View()
{
    for (int i = 0; i < myHashtable.num_partitions; i++)
    {
        node_t *curr = myHashtable.lists[i].head;
        while (curr)
        {
            printf("Key: %s ", curr->key);
            value_t *curr_values = curr->values;
            while (curr_values)
            {
                printf("%s ", curr_values->value);
                curr_values = curr_values->next_value;
            }
            printf("\n");
            curr = curr->next;
        }
    }
}

void MR_CleanUp()
{
    for (int i = 0; i < myHashtable.num_partitions; i++)
    {
        // Free the linked lists in each partition
        node_t *curr = myHashtable.lists[i].head;
        while (curr)
        {
            node_t *temp = curr;
            curr = curr->next;
            while(temp->values){
                node_t *temp_value_ptr = temp->values->value;
                temp->values->value = temp->values->next_value;
                free(temp_value_ptr);
            }
            free(temp->values);
            free(temp->key);
            pthread_mutex_destroy(&temp->node_lock);
            free(temp);
        }
    }
    free(myHashtable.lists);
}
#endif // __mapreduce_h__