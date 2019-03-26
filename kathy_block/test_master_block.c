#include "dylanBitSet.h"

#define MAGIC_NUM 0xFEEDF00D

// Opening a device that has had a master block written to it takes
// two steps: first open the device, which this function does.
block_device_t openBlockDevice(char * deviceName) {
    block_device_t ret = calloc(1,sizeof(BlockDevice));
    ret->m_deviceHandle = open(deviceName, O_RDWR, 0644);
    if (ret->m_deviceHandle < 0) {
        char *desc = strerror(errno);
        fprintf(stderr, "error creating device file %s due to %s\n", deviceName, desc);
        free(ret);
        return NULL;
    }
    return ret;
}

// the second half of opening a block device is for the Master Block
// functions to set the block count and block size. After this
// function is called, the block device is ready to go.
void setupBlockDevice(block_device_t bd, uint64_t blockCount, uint64_t bytesPerBlock) {
    bd->m_blockCount = blockCount;
    bd->m_bytesPerBlock = bytesPerBlock;
}

uint64_t readFirstBytes(block_device_t bd, char *buffer, int numBytes) {
    lseek(bd->m_deviceHandle, 0, SEEK_SET);
    uint64_t ret = read(bd->m_deviceHandle, buffer, numBytes);
    return ret;
}

int read_test(master_block_t mb, block_device_t bd){
    if (!bd){
        printf("No bd \n");
        exit(-1);
    }
    if(!mb){
        printf("No mb \n");
        exit(-1);
    }
    char* testbuff=(char*) mb;
    write(bd->m_deviceHandle, testbuff, sizeof(MasterBlock));
    if(readMasterBlock(bd, mb, sizeof(MasterBlock))!=0){ //need to find the magic_num
        printf("readmb \n");
        exit(-1);
    }
    if(readFirstBytes(bd, testbuff, sizeof(MasterBlock))!= sizeof(MasterBlock)){
        printf("readfirst \n");
        exit(-1);
    }
    if(mb->magic_num!=MAGIC_NUM){
        exit(-1);
    }
    printf("Read_test was a success\n");
    return 0;
}

int write_test(master_block_t mb, block_device_t bd){
    if (!bd){
        exit(-1);
    }
    if(!mb){
        exit(-1);
    }
    char* testbuff=(char*) mb;
    write(bd->m_deviceHandle, testbuff, sizeof(MasterBlock));
    if(writeMasterBlock(bd, mb, sizeof(MasterBlock))!=0){ //need to find the magic_num
        exit(-1);
    }
    if(readFirstBytes(bd, testbuff, sizeof(MasterBlock))!= sizeof(MasterBlock)){
        exit(-1);
    }
    if(mb->magic_num!=MAGIC_NUM){
        exit(-1);
    }
    printf("Write_test was a success\n");
    return 0;
}

int map_read(block_device_t bd, BitSet* blockmap, disk_addr_t disk_loc){
    if (!bd){
        printf("No bd \n");
        exit(-1);
    }
    if (!blockmap){
        printf("No blockmap \n");
        exit(-1);
    }
    char* testbuff= (char*) blockmap;
    if(write(bd->m_deviceHandle, testbuff, bd->m_bytesPerBlock)!=bd->m_bytesPerBlock){
        printf("Didn't write correctly\n");
        exit(-1);
    }
    if(readBlockMap(bd, blockmap, disk_loc)!=0){
        printf("readmap didn't work");
        exit(-1);
    }
    printf("map_read is correct\n");
    return 0;
}

int map_write(block_device_t bd, BitSet* blockmap, disk_addr_t disk_loc){
    if (!bd){
        printf("No bd \n");
        exit(-1);
    }
    if (!blockmap){
        printf("No blockmap \n");
        exit(-1);
    }
    char* testbuff= (char*) blockmap;
    if(writeBlockMap(bd, blockmap,disk_loc )!=0){
        printf("writemap didn't work");
        exit(-1);
    }
    if(read(bd->m_deviceHandle, testbuff, bd->m_bytesPerBlock)!=bd->m_bytesPerBlock){
        printf("Didn't read correctly\n");
        exit(-1);
    }
    printf("map_write is correct\n");
    return 0;
}



// as long as the magic number matches, retrieves the block size from
// and number of blocks from the device the device - for bootstrapping
// the mount process. (You need the block size to read from the block
// device) returns positive block size on success, <0 on failure  
// (usually magic didn't match)                   
    

int main(int argc, char** argv){
    char * deviceName="TESTDEVICE.txt";
 
    block_device_t bd=createBlockDevice(deviceName, sizeof(uint32_t), 3);
    bd=openBlockDevice(deviceName);
    setupBlockDevice(bd, 3, 0);
    master_block_t mb=allocMasterBlock(bd->m_bytesPerBlock, bd->m_blockCount, 0);
    char* testbuff= (char*) mb;
    if (write(bd->m_deviceHandle, testbuff, sizeof(MasterBlock))!=sizeof(MasterBlock)){
        exit(-1);
    }
    if(readFirstBytes(bd, testbuff, sizeof(MasterBlock))!= sizeof(MasterBlock)){
        exit(-1);
    }
    if(mb->magic_num!=MAGIC_NUM){
        exit(-1);
    }
    printf("Init_test was a success\n");
    read_test(mb, bd);
    write_test(mb, bd);
    BitSet* blockmap=allocBlockMap(bd->m_blockCount*8*bd->m_bytesPerBlock);

    map_read(bd, blockmap, 2);
    map_write(bd, blockmap, 2);
    freeMasterBlock(mb);
    freeBitSet(blockmap);
    closeDevice(bd);
    
    
    return 0;
}