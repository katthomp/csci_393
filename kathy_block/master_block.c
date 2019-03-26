#include "master_block.h"

//we assume the master block is at least the size of a block
int readMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_loc){
    //from blockdevice, read it
    //disk_loc is the offset from the block
    char* mbuff=(char*) mb;
    if(!bd){
        exit(-1);
    }
    if(!mb){
        exit(-1);
    } 
    if(read(bd->m_deviceHandle, mbuff, sizeof(MasterBlock)) != sizeof(MasterBlock)){
        exit(-1);
    }
    if(mb->magic_num!=0xFEEDF00D){
        exit(-1);
    }
    return 0; 
} 
int writeMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_loc){
    char* mbuff=(char*) mb;
    if(!bd){
        exit(-1);
    }
    if(!mb){
        exit(-1);
    } 
    if(write(bd->m_deviceHandle, mbuff, sizeof(MasterBlock) != sizeof(MasterBlock))){
        exit(-1);
    }
    if(read(bd->m_deviceHandle, mbuff, sizeof(MasterBlock) != sizeof(MasterBlock))){
        exit(-1);
    }
    if(mb->magic_num!=0xFEEDF00D){
        exit(-1);
    }
    return 0;
}
master_block_t allocMasterBlock(uint64_t bytes_per_block, uint64_t number_of_blocks, disk_addr_t block_map_address){
    master_block_t mb=malloc(sizeof(bytes_per_block));
    if (!mb){ //checks to see if it's malloc-ed correctly
        exit(-1);
    }
    mb->bytes_per_block=bytes_per_block;
    mb->blocks_per_device=number_of_blocks;
    mb->disk_addr_block_map=block_map_address;
    mb->magic_num=0xFEEDF00D;
    return mb;
}
void freeMasterBlock(master_block_t mb){
    free(mb);
    return;
}


//unsure how the disk_addr_t works. 
//disk addr just indexes into the array of the disk
