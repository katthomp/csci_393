#include "inode_struct.h"
#define MAGIC_NUM 0xF00DCAFE
#define MY_MAGIC 0xFEEDF00D
INodeMap_t INodeMap_test(int num_inodes, block_device_t bd){
    type_t type=FILETYPE;
    INodeMap_t map=allocateINodeMap(num_inodes, bd);
    if(!map){
        fprintf(stderr, "problem allocating inodemap");
    }
    inode_addr_t curr_inode_i=(inode_addr_t) allocateINode(map, type, bd);
    if (map[curr_inode_i].status==0){
        fprintf(stderr, "problem with allocateInode\n");
    }
    
    return map;
}

inode_t addINode(INodeMap_t m, inode_addr_t index){
    inode_t inode=(inode_t) addrToINodeMap(m, index);
    if(!inode){
        fprintf(stderr, "addrInode went wrong");
    }
    return inode;
}


int map_read(block_device_t bd, INodeMap_t map, disk_addr_t disk_loc, int num_inodes){
    if (!bd){
        printf("No bd \n");
        exit(-1);
    }
    if (!map){
        printf("No inodemap :( \n");
        exit(-1);
    }
    map[0].gid=MY_MAGIC;
    readINodeMap(bd, map, disk_loc);
    
    if(map[0].gid!=MY_MAGIC){
        printf("readmap didn't work");
        exit(-1);
    }
    printf("map_read is correct\n");
    return 0;
}

int map_write(block_device_t bd, INodeMap_t map, disk_addr_t disk_loc, int num_inodes){
    if (!bd){
        printf("No bd \n");
        exit(-1);
    }
    if (!map){
        printf("No blockmap \n");
        exit(-1);
    }
    map[0].gid=MY_MAGIC;
    writeINodeMap(bd, map, disk_loc);
    if(map[0].gid!=MY_MAGIC){
        fprintf(stderr, "writemap didn't work");
    }
    printf("map_write is correct\n");
    return 0;
}


int main(int argc, char** argv){
    char * deviceName="TESTINODE.txt";
    block_device_t bd=createBlockDevice(deviceName, 512, 16);
    master_block_t mb=allocMasterBlock(bd->m_bytesPerBlock, bd->m_blockCount, 0);
    char* testbuff= (char*) mb;
    if (write(bd->m_deviceHandle, testbuff, sizeof(MasterBlock))!=sizeof(MasterBlock)){
        exit(-1);
    }
    if(readFirstBytes(bd, testbuff, sizeof(MasterBlock))!= sizeof(MasterBlock)){
        exit(-1);
    }
    if(mb->magic_number!=MAGIC_NUM){
        exit(-1);
    }
    printf("Init_test was a success\n");
    INodeMap_t map=(INodeMap_t)INodeMap_test(bd->num_inodes, bd);
    type_t type=FILETYPE;
    map_write(bd, map, 3, bd->num_inodes);
    map_read(bd, map, 3, bd->num_inodes);
    map_write(bd, map, 4, bd->num_inodes);
    inode_addr_t curr=allocateINode(map, type, bd);
    printf("%lld \n",map[curr].inode_num);
    if(!addINode(map, curr)){
        printf("AA");
        exit(-1);
    }
    inode_addr_t curr_i=allocateINode(map, type, bd);
    printf("%lld \n",map[curr_i].inode_num);
        if(!addINode(map, curr_i)){
        printf("AA");
        exit(-1);
    }
    inode_addr_t curr_e=allocateINode(map, type, bd);
    printf("%lld \n",map[curr_e].inode_num);
        if(!addINode(map, curr_e)){
        printf("AA");
        exit(-1);
    }
    map_read(bd,map, 4, bd->num_inodes);
    

    free(map);
    // closeDevice(bd);
    return 0;
}
