#include "dylanBitSet.h"

BitSet* allocBlockMap(int num_bits) {
    BitSet_p ret = calloc(1, sizeof(BitSet));
    ret->buffer = calloc(num_bits, sizeof(char));
    ret->count = num_bits;
    return ret;
}

// frees a BitSet
void freeBitSet(BitSet* bs) {
    free(bs->buffer);
    free(bs);
}

// sets bit i of the BitSet
void setBit(BitSet* b, int i) {
    SET_BIT(b->buffer, i);
}

// clears bit i of the BitSet
void clearBit(BitSet* b, int i) {
    CLEAR_BIT(b->buffer, i);
}

// returns the bit (boolean) value of the ith bit of the BitSet
bool getBit(BitSet* b, int i) {
    return GET_BIT(b->buffer, i);
}

// =================================================
// Now using BitSet, create an allocator interface
// =================================================
// returns the index of an item that was free, but is now allocated. If there are no more free items, returns a negative number
int allocItem(BitSet* b) {
    for (int i = 0; i < b->count; i++) {
        if (!GET_BIT(b->buffer, i)) {
            SET_BIT(b->buffer, i);
            return i;
        }
    }
    return -1;
}

// frees the item at location i, first checking that it wasn't already free. Returns 0 on success, <0 if there was an error
int freeItem(BitSet* b, int i) {
    if (!GET_BIT(b->buffer, i)) {
        printf("oops - freeing an item that is already marked free\n");
        return -1;
    }
    CLEAR_BIT(b->buffer, i);
    return 0;
}

int getFileSize(int f) {
    int ret = lseek(f, 0, SEEK_END);
    lseek(f,0,SEEK_SET);
    return ret;
}

int writeBlockMap(block_device_t bd, BitSet* block_map, disk_addr_t disk_loc){
    int num_blocks=block_map->count/(8*bd->m_blockCount); //convert from bits to bytes to blocks
    for(int i=1; i<=num_blocks; ++i){
        writeBlock(bd, i+disk_loc, &(block_map->buffer[i*bd->m_bytesPerBlock])); 
    }
    return 0; 
}
int readBlockMap(block_device_t bd, BitSet* block_map, disk_addr_t disk_loc){
    int num_blocks=block_map->count/(8*bd->m_blockCount); //convert from bits to bytes to blocks
    for(int i=1; i<=num_blocks; ++i){
        readBlock(bd, i+disk_loc, &(block_map->buffer[i*bd->m_bytesPerBlock])); 
    } 
    return 0;
}


// reads, from a file (whose name is pointed to by filename), a Bit Set into the memory pointed to by b.
void readBitSet(char *filename, BitSet* b) {
    int f = open(filename, O_RDONLY);
    assert(f >= 0);
    int num_bytes = getFileSize(f);
    int num_read = read(f, b->buffer, num_bytes);
    assert(num_read > 0);
    b->count = num_bytes * 8;
    close(f);
}

// writes Bit Set b to a file
void writeBitSet(char *filename, BitSet *b) {
    int f = open(filename, O_WRONLY | O_CREAT, 0666);
    assert(f >= 0);
    write(f, b->buffer, (b->count + 7) / 8);
    close(f);
}
