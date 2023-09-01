#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
#include "hashtable.h"

struct hashtable
{
    linkedlist_t **hashtable;
    int num_buckets;
    int num_mappings;
};

/**
 * Hash function to hash a key into the range [0, max_range)
 */
static int hash(int key, int max_range)
{
    key = (key > 0) ? key : -key;
    return key % max_range;
}

hashtable_t *ht_init(int num_buckets)
{

    hashtable_t *table = malloc(sizeof(hashtable_t));
    linkedlist_t **buckets = malloc(sizeof(linkedlist_t *) * num_buckets);
    table->hashtable = buckets;

    for (int i = 0; i < num_buckets; i++)
    {
        linkedlist_t *bucket = ll_init();
        buckets[i] = bucket;
    }
    table->num_buckets = num_buckets;
    table->num_mappings = 0;
    return table;
}

void ht_free(hashtable_t *table)
{
    for (int i = 0; i < table->num_buckets; i++)
    {
        ll_free(table->hashtable[i]);
    }
    free(table->hashtable);
    free(table);
}

void ht_add(hashtable_t *table, int key, int value)
{
    int bucketNum = hash(key, table->num_buckets);
    linkedlist_t *bucket = table->hashtable[bucketNum];
    table->num_mappings = table->num_mappings - ll_size(bucket);
    ll_add(bucket, key, value);
    table->num_mappings = table->num_mappings + ll_size(bucket);
}

int ht_get(hashtable_t *table, int key)
{

    int bucketNum = hash(key, table->num_buckets);
    linkedlist_t *bucket = table->hashtable[bucketNum];
    return ll_get(bucket, key);
}

int ht_size(hashtable_t *table)
{
    return table->num_mappings;
}
