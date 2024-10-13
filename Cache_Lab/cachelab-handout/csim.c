#include "cachelab.h"
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <stdint.h>

#define INSTLEN 64

// Performance Parameters
int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

typedef unsigned long ulong;


typedef struct {
    int valid;
    int dirty;
    ulong tag;
    int lru_counter;
} CacheLine;

typedef CacheLine* CacheSet;
typedef CacheSet* Cache;

// typedef struct {
//     cache_block block[SET_NUM];
// } cache_set;

typedef struct {
    int s;
    int E;
    int b;
    int hit_count;
    int miss_count;
    int eviction_count;
} cache_param;

// initialize cache size according to s, E, b
Cache initCache (int setIndexBits, int assoc) {
    Cache cache;
    ulong setNum = 1 << setIndexBits;
    
    cache = calloc(setNum, sizeof(CacheSet));

    for (int i = 0; i < setNum; i++){
        cache[i] = calloc(assoc, sizeof(CacheLine));
    }

    for (int i = 0; i < setNum; i++){
        for (int j = 0; j < assoc; j++){
            // cache[i][j].valid = 0;
            // cache[i][j].lru_counter = 0;
            ((*(cache+i))+j)->valid = 0;
            ((*(cache+i))+j)->lru_counter =0;
        }
    }
    
    return cache;
}

int getLruNum (ulong index, int assoc, Cache cache) {
    int lru_num;
    for (int i=0; i<assoc; i++) {
        if (cache[index][i].lru_counter == 0) {
            lru_num = i;
        }
    }

    return lru_num; 
}

void cacheInsert (ulong tag, ulong index, int assoc, int block_num, int old_lru_count, Cache cache) {
    cache[index][block_num].valid = 1;
    cache[index][block_num].tag = tag;

    if (old_lru_count != (assoc-1)) {
        for (int i = 0; i < assoc; i++) {
            if (i != block_num && cache[index][i].lru_counter > old_lru_count) {
                cache[index][i].lru_counter--;
            }
        }
        cache[index][block_num].lru_counter = assoc-1;
    }
}



void checkCache (ulong tag, ulong index, int assoc, Cache cache, int verboseFlag){
    
    int cache_hit = 0;
    int set_full = 1;
    int hit_block_num = 0;
    int hit_block_lru_old = 0;

    int lru_num;

    for (int i = 0; i < assoc; i++) {
        if (cache[index][i].valid == 0) {
            set_full = 0;
        }

        if (cache[index][i].tag == tag && cache[index][i].valid == 1){
            cache_hit = 1;
            hit_block_num = i;
            hit_block_lru_old = cache[index][i].lru_counter;
            break;
        }
    }

    if (cache_hit == 1) { // HIT
        hit_count++;
        if (verboseFlag == 1){
            printf("hit\n");
        }
        cacheInsert(tag, index, assoc, hit_block_num, hit_block_lru_old, cache);
    } else if (cache_hit == 0 && set_full == 1){ // MISS, Cache Full, need EVICT
        miss_count++;
        eviction_count++;
        if (verboseFlag == 1){
            printf("miss\t");
            printf("evict\n");
        }
        lru_num = getLruNum(index, assoc, cache);
        cacheInsert(tag, index, assoc, lru_num, hit_block_lru_old, cache);
    } else {  // MISS
        miss_count++;
        if (verboseFlag == 1){
            printf("miss\n");
        }
        lru_num = getLruNum(index, assoc, cache);
        cacheInsert(tag, index, assoc, lru_num, hit_block_lru_old, cache);
    }
    /*
    if (cache_hit == 0) {   // miss
        increase miss count
        if (set is full)
            evict the LRU one, 
            change LRU
            increase evict count
        else 
            store the tag in empty ones
            set valid bit to 1
            change LRU
    }
   
    */
}

void usage () {
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h       Print this help message.\n");
    printf("  -v       Optional verbose flag.\n");
    printf("  -s <num> Number of set index bits.\n");
    printf("  -E <num> Number of lines per set.\n");
    printf("  -b <num> Number of block offset bits.\n");
    printf("  -t <num> Trace file.\n");
}



int main(int argc, char *argv[]) {
    int option;

    // cache_param Param;

    int setIndexBits = 0; // setIndexBits
    int assoc = 0; // associativity
    int blockBits = 0;
    char *tracefile = NULL;
    int verboseFlag = 0;

    // Process command-line options using getopt
    while ((option = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch (option) {
            case 'h':
                usage();
                break;
            case 'v':
                verboseFlag = 1;
                break;
            case 's':
                setIndexBits = atoi(optarg);
                break;
            case 'E':
                assoc = atoi(optarg); // optarg contains the value for option 'b'
                break;
            case 'b':
                blockBits = atoi(optarg);
                break;
            case 't':
                tracefile = optarg;
                break;
            case '?':
                // if (optopt == 'b') {
                //     fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                // } else {
                    fprintf(stderr, "Unknown option -%c\n", optopt);
                // }
                // break;
                return 1;
            default:
                abort();
        }
    }

    // Process non-option arguments after the options
    for (int i = optind; i < argc; i++) {
        printf("Non-option argument: %s\n", argv[i]);
    }
    

    Cache cache = initCache(setIndexBits,assoc);

    FILE* pFile;
    pFile = fopen(tracefile,"r");
    if (pFile == NULL) {
        printf("no such file");
        return 1;
    }

    char identifier;
    ulong address;
    int size;

    ulong tag;
    ulong index;

    while (fscanf(pFile," %c %lx,%d", &identifier, &address, &size) > 0) {
        int tagBits = INSTLEN-blockBits-setIndexBits;
        tag = address >> (blockBits + setIndexBits);
        index = (address << tagBits) >> (tagBits + blockBits);
        // printf("identifier:%c, address:%lx, tag:%lx, index:%lx\n", identifier, address, tag, index);
        
        if (verboseFlag == 1) {
            printf("%c %lx,%d ", identifier, address, size);
            printf("(Decimal) [index: %ld, tag: %ld] ------ ", index, tag);
        }


        switch (identifier)
        {
        case 'I':
            break;
        case 'M':
            checkCache(tag, index, assoc, cache, verboseFlag);
            checkCache(tag, index, assoc, cache, verboseFlag);
            break;
        case 'S':
            checkCache(tag, index, assoc, cache, verboseFlag);
            break;
        case 'L':
            checkCache(tag, index, assoc, cache, verboseFlag);
            break;
        default:
            break;
        }

        // printf("hit:%d, miss:%d, eviction:%d\n\n",hit_count, miss_count, eviction_count);
    }

    printSummary(hit_count, miss_count, eviction_count);

    free(cache);
    return 0;
}
