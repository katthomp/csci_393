#include "persist_bitset.h"

BitSet* allocBitSet(int num_bits){
    //allocates and returns a pointer to a BitSet
    bit_set_t bitset=calloc(1, sizeof(BitSet));
    bitset->count=num_bits;
    bitset->buffer = calloc(num_bits, sizeof(char));
    return bitset;
}

void setBit(bit_set_t b, int i){
    //sets bit i of the BitSet
    //need to do bit arithmetic because i'm accessing each byte
    //because we can only really access the byte, determine the 
    int byte_to_access=i/(sizeof(char));
    int bit_in_byte=i%(sizeof(char));
    b->buffer[byte_to_access] |= (1 << bit_in_byte);
    return;
}

void clearBit(BitSet* b , int i){
    //clears bit i of the BitSet
    int byte_to_access=i/(sizeof(char));
    int bit_in_byte=i%(sizeof(char));
    b->buffer[byte_to_access] |= ~(1 << bit_in_byte);
    return;
}
bool getBit(BitSet* b, int i){
    //returns the bit (boolean) value of the ith bit of the BitSet
    int byte_to_access=i/sizeof(char);
    int bit_in_byte= i %sizeof(char);
    bool test= b->buffer[byte_to_access] & (1 << bit_in_byte);
    if (test){
        return test;
    }
    else{
        return ~test;
    }
}
int allocItem(BitSet* b){
    //returns the index of an item that was free, but is now allocated. 
    //If there are no more free items, returns a negative number
    for (int i=0; i < sizeof(b); ++i){
        if (!(b->buffer[i])){
            setBit(b, i);
            return i;
        }
    }
    return -1;
}
void freeItem(bit_set_t b, int i){
    //frees the item at location i, first checking that it wasn't already free.
    if((b->buffer[i])==0){
        return;
    }
    else{
        b->buffer[i]=0;
    }
}
void readBitSet(char* filename, bit_set_t b){
    //reads, from file f, a Bit Set into the memory pointed to by b.
    char * line = NULL;
    size_t len = 0;
    FILE* file=fopen(filename, "r");
    if (!file){
        exit(-1);
    }
    else{
        fseek(file, 0, SEEK_END);
        int size=ftell(file);
        rewind(file);
        fread(line, sizeof(line), 1, file);
        } 

    }
 //want to make sure I'm understanding everything that's happening here.
void writeBitSet(char* filename, bit_set_t b){
    //writes Bit Set b to file f
    char * line = NULL;
    size_t len = 0;
    FILE* file=fopen(filename, "r");
    if (!file){
        exit(-1);
    }
    else{
        fseek(file, 0, SEEK_END);
        int size=ftell(file);
        rewind(file);
        fwrite(&b, sizeof(b), 1, file);
    }
}

