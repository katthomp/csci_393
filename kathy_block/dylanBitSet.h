#ifndef DYLANBITSET

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>

#include "master_block.h"
#include "block_device.h"
#define DYLANBITSET

typedef struct BitSet {
    int count;
    char *buffer;
} BitSet, *BitSet_p;

#define SET_BIT(cs, i)   ((cs[i/8]) |= (1 << (i % 8)))
#define CLEAR_BIT(cs, i) ((cs[i/8]) &= ~(1 << (i % 8)))
#define GET_BIT(cs, i) (((cs[i/8]) & (1 << (i % 8))) != 0)


BitSet* allocBlockMap(int num_bits);

// frees a BitSet
void freeBitSet(BitSet* bs);

// sets bit i of the BitSet
void setBit(BitSet* b, int i);

// clears bit i of the BitSet
void clearBit(BitSet* b, int i);

// returns the bit (boolean) value of the ith bit of the BitSet
bool getBit(BitSet* b, int i);

// =================================================
// Now using BitSet, create an allocator interface
// =================================================
// returns the index of an item that was free, but is now allocated. If there are no more free items, returns a negative number
int allocItem(BitSet* b);

// frees the item at location i, first checking that it wasn't already free. Returns 0 on success, <0 if there was an error
int freeItem(BitSet* b, int i);

int getFileSize(int f);

// reads, from a file (whose name is pointed to by filename), a Bit Set into the memory pointed to by b.
void readBitSet(char *filename, BitSet* b);

// writes Bit Set b to a file
void writeBitSet(char *filename, BitSet *b);

int readBlockMap(block_device_t bd, BitSet* block_map, disk_addr_t disk_loc);

int writeBlockMap(block_device_t bd, BitSet* block_map, disk_addr_t disk_loc);



#endif
