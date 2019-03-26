#include "block_device.h"

typedef struct MasterBlock{
    uint64_t magic_num;
    uint64_t bytes_per_block;
    disk_addr_t blocks_per_device;
    disk_addr_t disk_addr_block_map;
} MasterBlock, *master_block_t;

int readMasterBlock (BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_loc);

int writeMasterBlock(BlockDevice *bd, MasterBlock *mb, disk_addr_t disk_loc);

master_block_t allocMasterBlock(uint64_t bytes_per_block, uint64_t number_of_blocks, disk_addr_t block_map_address);

void freeMasterBlock(master_block_t mb);
//shouldn't this have a memory component of some kind???
