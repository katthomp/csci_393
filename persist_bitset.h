#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct BitSet{
    int count;
    int* buffer;
    } BitSet, *bit_set_t;

bit_set_t allocBitSet(int num_bits);
    //allocates and returns a pointer to a BitSet
void setBit(bit_set_t b, int i);

void clearBit(bit_set_t b , int i);

bool getBit(bit_set_t b, int i);

int allocItem(bit_set_t b);

void freeItem(bit_set_t b, int i);

void readBitSet(char* filename,bit_set_t b);

void writeBitSet(char* filename,bit_set_t b);
