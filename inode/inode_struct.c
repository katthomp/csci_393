#include "inode_struct.h"


void freeINode(INodeMap_t m, inode_addr_t inode_to_free){
    if (m[inode_to_free].status==STATUS_FREE){
        return;
    }else{
        m[inode_to_free].status=STATUS_FREE;
    }
    return;
}

inode_addr_t allocateINode(INodeMap_t m, type_t type, block_device_t bd){
    for(int64_t i=0; i<bd->num_inodes; ++i){
        printf("%d \n",bd->num_inodes);
        if(m[i].status==STATUS_FREE){
            m[i].status=STATUS_ALLOCATED;
            return m[i].inode_num;
        }
    }
    return -1;
}

inode_t addrToINodeMap(INodeMap_t m, inode_addr_t index){
    if(m[index].status==STATUS_FREE){
        fprintf(stderr, "problem finding inode in map");
    }
    return &m[index];
}

INodeMap_t allocateINodeMap(int num_inodes, block_device_t bd){
    INodeMap_t inodeMap = (INodeMap_t)calloc(num_inodes, sizeof(INode));
    for(int64_t i=0; i<num_inodes; ++i){
        inodeMap[i].status=STATUS_FREE;
        inodeMap[i].gid=0xDEADBEEF;
        inodeMap[i].inode_num=i;
    }
    if(!inodeMap){
        fprintf(stderr, "problem allocating nodemap");
        exit(-1);
    }

    bd->num_inodes=num_inodes;
    return inodeMap;
}

disk_addr_t writeINodeMap(block_device_t bd, INodeMap_t inode_map, disk_addr_t disk_loc){
    int blocks_to_loop = ceilDiv(bd->num_inodes,bd->inodesPerBlock);
    //i want to find the number of inodes to write to each block
    for(int i=0; i<blocks_to_loop; ++i){
        char* tbuf= (char*) &(inode_map[i*bd->inodesPerBlock]);
        writeBlock(bd, i+disk_loc, tbuf); 
    }
    return 0;
}

disk_addr_t readINodeMap(block_device_t bd, INodeMap_t inode_map, disk_addr_t disk_loc){
    int blocks_to_loop = ceilDiv(bd->num_inodes,bd->inodesPerBlock);
    for(int i=0; i<blocks_to_loop; ++i){
        char* tbuf= (char*) &(inode_map[i*bd->inodesPerBlock]);
        readBlock(bd, i+disk_loc, tbuf); 
    } 
    return 0;
}
// int main(){
//     printf("LOL %lu\n",sizeof(INode));
// }
//what should i set the inodes_per_block
//it should be a power of 2
