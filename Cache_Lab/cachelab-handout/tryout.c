#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>


struct person {
   int age;
   float weight;
   char name[30];
};

// Assuming ulong is defined somewhere in your code
typedef unsigned long ulong;

typedef struct {
    int valid;
    int dirty;
    ulong tag;
    int lru_counter;
} CacheLine, *CacheSet, **Cache;


int main()
{
//    struct person *ptr;
//    int i, n;

//    printf("Enter the number of persons: ");
//    scanf("%d", &n);

//    // allocating memory for n numbers of struct person
//    ptr = (struct person*) malloc(n * sizeof(struct person));

//    for(i = 0; i < n; ++i)
//    {
//        printf("Enter first name and age respectively: ");

//        // To access members of 1st struct person,
//        // ptr->name and ptr->age is used

//        // To access members of 2nd struct person,
//        // (ptr+1)->name and (ptr+1)->age is used
//        scanf("%s %d", (ptr+i)->name, &(ptr+i)->age);
//    }

//    printf("Displaying Information:\n");
//    for(i = 0; i < n; ++i)
//        printf("Name: %s\tAge: %d\n", (ptr+i)->name, (ptr+i)->age);

    /*
        Example 2
    */

    CacheLine *cacheLines[4];

    // Allocate memory for each CacheLine and store the pointers in the array
    for (int i = 0; i < 4; ++i) {
        cacheLines[i] = (CacheLine *)malloc(sizeof(CacheLine));
        cacheLines[i]->valid = 1;
        cacheLines[i]->dirty = 0;
        cacheLines[i]->tag = 0xABCD1234 + i;
        cacheLines[i]->lru_counter = i;
    }

    // Access and print information from each CacheLine in the array
    for (int i = 0; i < 4; ++i) {
        printf("Example 1\n");
        printf("Cache Line %d:\n", i + 1);
        printf("Valid: %d\n", cacheLines[i]->valid);
        printf("Dirty: %d\n", cacheLines[i]->dirty);
        printf("Tag: %lu\n", cacheLines[i]->tag);
        printf("LRU Counter: %d\n", cacheLines[i]->lru_counter);
        printf("\n");
    }

    // Don't forget to free the allocated memory when done
    for (int i = 0; i < 4; ++i) {
        free(cacheLines[i]);
    }

    /*
        Example 3
    */

    // Instantiate a single CacheSet
    CacheSet myCacheSet = (CacheSet)malloc(sizeof(CacheLine) * 4);  // Assuming 4 cache lines in a set
    // pointer(address) to start of the array

    // Reference the cache lines within the set
    for (int i = 0; i < 4; ++i) {
        myCacheSet[i].valid = 1;            // Set valid bit to 1
        myCacheSet[i].dirty = 0;            // Set dirty bit to 0
        myCacheSet[i].tag = 0xABCD1234;     // Set a sample tag value
        myCacheSet[i].lru_counter = i;      // Set LRU counter (just an example)
    }

    // Access and print information from cache lines within the set
    for (int i = 0; i < 4; ++i) {
        printf("Example 2\n");
        printf("Cache Line: %d\n", i + 1);
        printf("Valid: %d\n", myCacheSet[i].valid);
        printf("Dirty: %d\n", myCacheSet[i].dirty);
        printf("Tag: %lu\n", myCacheSet[i].tag);
        printf("LRU Counter: %d\n", myCacheSet[i].lru_counter);
        printf("\n");
    }

    // Don't forget to free the allocated memory when done
    free(myCacheSet);

     /*
        Example 4
    */

    // Instantiate a single CacheSet
    CacheSet myCacheSet2 = (CacheSet)malloc(sizeof(CacheLine) * 4);  // Assuming 4 cache lines in a set

    // Reference the cache lines within the set
    for (int i = 0; i < 4; ++i) {
        (myCacheSet2+i)->valid = 1;            // Set valid bit to 1
        (myCacheSet2+i)->dirty = 0;            // Set dirty bit to 0
        (myCacheSet2+i)->tag = 0xABCD1234;     // Set a sample tag value
        (myCacheSet2+i)->lru_counter = i;      // Set LRU counter (just an example)
    }

    // Access and print information from cache lines within the set
    for (int i = 0; i < 4; ++i) {
        printf("Example 3\n");
        printf("Cache Line: %d\n", i + 1);
        printf("Valid: %d\n", (*(myCacheSet2+i)).valid);
        printf("Dirty: %d\n", (*(myCacheSet2+i)).dirty);
        printf("Tag: %lu\n", (*(myCacheSet2+i)).tag);
        printf("LRU Counter: %d\n", (*(myCacheSet2+i)).lru_counter);
        // printf("Value: %x\n", *(myCacheSet2+i));
        printf("\n");
    }

    // Don't forget to free the allocated memory when done
    free(myCacheSet2);

   return 0;
}

