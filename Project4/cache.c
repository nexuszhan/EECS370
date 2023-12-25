/*
 * EECS 370, University of Michigan
 * Project 4: LC-2K Cache Simulator
 * Instructions are found in the project spec.
 */

#include <stdio.h>
#include <math.h>

#define MAX_CACHE_SIZE 256
#define MAX_BLOCK_SIZE 256

extern int mem_access(int addr, int write_flag, int write_data);
extern int get_num_mem_accesses();

enum actionType
{
    cacheToProcessor,
    processorToCache,
    memoryToCache,
    cacheToMemory,
    cacheToNowhere
};

typedef struct blockStruct
{
    int data[MAX_BLOCK_SIZE];
    int dirty;
    int lruLabel;
    int set;
    int tag;
} blockStruct;

typedef struct cacheStruct
{
    blockStruct blocks[MAX_CACHE_SIZE];
    int blockSize;
    int numSets;
    int blocksPerSet;
} cacheStruct;

/* Global Cache variable */
cacheStruct cache;

void printAction(int, int, enum actionType);
void printCache();

/*
 * Set up the cache with given command line parameters. This is 
 * called once in main(). You must implement this function.
 */
void cache_init(int blockSize, int numSets, int blocksPerSet){
    cache.blockSize = blockSize;
    cache.numSets = numSets;
    cache.blocksPerSet = blocksPerSet;

    for (int i=0; i<numSets*blocksPerSet; i++)
    {
        cache.blocks[i].lruLabel = 0;
        cache.blocks[i].tag = -1;
        cache.blocks[i].set = i / blocksPerSet;
        cache.blocks[i].dirty = 0;
        for (int j=0; j<blockSize ; j++)
            cache.blocks[i].data[j] = 0;
    }
    return;
}

/*
 * Access the cache. This is the main part of the project,
 * and should call printAction as is appropriate.
 * It should only call mem_access when absolutely necessary.
 * addr is a 16-bit LC2K word address.
 * write_flag is 0 for reads (fetch/lw) and 1 for writes (sw).
 * write_data is a word, and is only valid if write_flag is 1.
 * The return of mem_access is undefined if write_flag is 1.
 * Thus the return of cache_access is undefined if write_flag is 1.
 */
int cache_access(int addr, int write_flag, int write_data) {
    int offset = addr & (cache.blockSize-1);
    int offset_bits = log2(cache.blockSize);
    int set = (addr & (cache.blockSize * (cache.numSets-1))) >> offset_bits;
    int tag = addr & ((2<<16) - cache.numSets * cache.blockSize);

    //printf("%d  ",addr);
    //printf("%d %d %d %d \n",tag,set,offset,write_flag);
    //printCache();

    int block_index;
    int found = 0;
    for (block_index=set*cache.blocksPerSet; block_index<(set+1)*cache.blocksPerSet; block_index++)
    {
        if (cache.blocks[block_index].tag == tag)
        {
            found = 1;
            break;
        }
    }

    if (write_flag) // sw
    {
        if (found)
        {
            cache.blocks[block_index].dirty = 1;
            int tmp = cache.blocks[block_index].lruLabel;
            cache.blocks[block_index].lruLabel = -1;
            cache.blocks[block_index].data[offset] = write_data;
            printAction(addr, 1, processorToCache);
            // update valid blocks' lru
            for (int i=set*cache.blocksPerSet; i<(set+1)*cache.blocksPerSet; i++)
            {
                if (cache.blocks[i].tag != -1 && cache.blocks[i].lruLabel < tmp)
                {
                    cache.blocks[i].lruLabel++;
                }
            }
            return write_data;
        }
        else
        {
            // find the block to write
            int block_to_write = 0; //set*cache.blocksPerSet;
            int max_lru = -1;
            for (int i=set*cache.blocksPerSet; i<(set+1)*cache.blocksPerSet; i++)
            {
                if (cache.blocks[i].tag == -1)
                {
                    block_to_write = i;
                    break;
                }
                else
                {
                    if (cache.blocks[i].lruLabel > max_lru)
                    {
                        block_to_write = i;
                        max_lru = cache.blocks[i].lruLabel;
                    }
                }
            }

            // check if the block is occipied
            if (cache.blocks[block_to_write].tag != -1)
            {
                int block_addr = cache.blocks[block_to_write].tag + (cache.blocks[block_to_write].set << offset_bits);
                if (cache.blocks[block_to_write].dirty) // check if the block is dirty
                {
                    for (int i=0; i<cache.blockSize; i++)
                        mem_access(block_addr+i, 1, cache.blocks[block_to_write].data[i]);
                    printAction(block_addr, cache.blockSize, cacheToMemory);

                    //cache.blocks[block_to_write].dirty = 1;
                }
                else
                {
                    printAction(block_addr, cache.blockSize, cacheToNowhere);
                }
            }
            
            // write to cache
            cache.blocks[block_to_write].tag = tag;
            cache.blocks[block_to_write].lruLabel = -1;
            for (int i=0; i<cache.blockSize;i++)
            {
                //if (i != offset)
                cache.blocks[block_to_write].data[i] = mem_access(addr-offset+i, 0, 0);
            }
            printAction(addr-offset, cache.blockSize, memoryToCache);
            
            cache.blocks[block_to_write].data[offset] = write_data;
            cache.blocks[block_to_write].dirty = 1;
            // update valid blocks' lru
            for (int i=set*cache.blocksPerSet; i<(set+1)*cache.blocksPerSet; i++)
            {
                if (cache.blocks[i].tag != -1)
                {
                    cache.blocks[i].lruLabel++;
                }
            }

            printAction(addr, 1, processorToCache);
            return write_data;
        }
    }
    else // lw
    {
        if (found) 
        {
            int tmp = cache.blocks[block_index].lruLabel;
            cache.blocks[block_index].lruLabel = -1;
            // update valid blocks' lru
            for (int i=set*cache.blocksPerSet; i<(set+1)*cache.blocksPerSet; i++)
            {
                if (cache.blocks[i].tag != -1 && cache.blocks[i].lruLabel < tmp)
                {
                    cache.blocks[i].lruLabel++;
                }
            }
            printAction(addr, 1, cacheToProcessor);
            return cache.blocks[block_index].data[offset];
        }
        else
        {
            // find the block to write
            int block_to_write = 0; //set*cache.blocksPerSet;
            int max_lru = -1;
            for (int i=set*cache.blocksPerSet; i<(set+1)*cache.blocksPerSet; i++)
            {
                if (cache.blocks[i].tag == -1)
                {
                    block_to_write = i;
                    break;
                }
                else
                {
                    if (cache.blocks[i].lruLabel > max_lru)
                    {
                        block_to_write = i;
                        max_lru = cache.blocks[i].lruLabel;
                    }
                }
            }

            // check if the block is occipied
            if (cache.blocks[block_to_write].tag != -1)
            {
                int block_addr = cache.blocks[block_to_write].tag + (cache.blocks[block_to_write].set << offset_bits);
                if (cache.blocks[block_to_write].dirty) // check if the block is dirty
                {
                    //int block_addr = cache.blocks[block_to_write].tag + cache.blocks[block_to_write].set;
                    for (int i=0; i<cache.blockSize; i++)
                        mem_access(block_addr+i, 1, cache.blocks[block_to_write].data[i]);
                    printAction(block_addr, cache.blockSize, cacheToMemory);

                    cache.blocks[block_to_write].dirty = 0;
                }
                else
                {
                    printAction(block_addr, cache.blockSize, cacheToNowhere);
                }
            }

            // write to cache
            cache.blocks[block_to_write].tag = tag;
            cache.blocks[block_to_write].lruLabel = -1;
            for (int i=0; i<cache.blockSize; i++)
                cache.blocks[block_to_write].data[i] = mem_access(addr-offset+i, 0, 0);
            printAction(addr-offset, cache.blockSize, memoryToCache);
            // update valid blocks' lru
            for (int i=set*cache.blocksPerSet; i<(set+1)*cache.blocksPerSet; i++)
            {
                if (cache.blocks[i].tag != -1)
                {
                    cache.blocks[i].lruLabel++;
                }
            }

            printAction(addr, 1, cacheToProcessor);
            return cache.blocks[block_to_write].data[offset];
        }
    }
    return mem_access(addr, write_flag, write_data); // not supposed to happen
}


/*
 * print end of run statistics like in the spec. This is not required,
 * but is very helpful in debugging.
 * This should be called once a halt is reached.
 * DO NOT delete this function, or else it won't compile.
 * DO NOT print $$$ in this function
 */
void printStats(){

    return;
}

/*
 * Log the specifics of each cache action.
 *
 * address is the starting word address of the range of data being transferred.
 * size is the size of the range of data being transferred.
 * type specifies the source and destination of the data being transferred.
 *  -    cacheToProcessor: reading data from the cache to the processor
 *  -    processorToCache: writing data from the processor to the cache
 *  -    memoryToCache: reading data from the memory to the cache
 *  -    cacheToMemory: evicting cache data and writing it to the memory
 *  -    cacheToNowhere: evicting cache data and throwing it away
 */
void printAction(int address, int size, enum actionType type)
{
    printf("$$$ transferring word [%d-%d] ", address, address + size - 1);

    if (type == cacheToProcessor) {
        printf("from the cache to the processor\n");
    }
    else if (type == processorToCache) {
        printf("from the processor to the cache\n");
    }
    else if (type == memoryToCache) {
        printf("from the memory to the cache\n");
    }
    else if (type == cacheToMemory) {
        printf("from the cache to the memory\n");
    }
    else if (type == cacheToNowhere) {
        printf("from the cache to nowhere\n");
    }
}

/*
 * Prints the cache based on the configurations of the struct
 * This is for debugging only and is not graded, so you may
 * modify it, but that is not recommended.
 */
void printCache()
{
    printf("\ncache:\n");
    for (int set = 0; set < cache.numSets; ++set) {
        printf("\tset %i:\n", set);
        for (int block = 0; block < cache.blocksPerSet; ++block) {
            printf("\t %i", cache.blocks[set * cache.blocksPerSet + block].dirty); // !!!
            printf("\t\t[ %i ]: {", block);
            for (int index = 0; index < cache.blockSize; ++index) {
                printf(" %i", cache.blocks[set * cache.blocksPerSet + block].data[index]);
            }
            printf(" }\n");
        }
    }
    printf("end cache\n");
}