#include "master_block.h"

#define STATUS_FREE 0
#define STATUS_ALLOCATED 1
#define BLOCK_PTRS_PER_INODE 16
#define INODE_SIZE 128
typedef enum inode_type_t{
    FILETYPE,
    DIRTYPE,
    SYMLINKTYPE
}type_t;
typedef int64_t inode_addr_t;
typedef uint64_t timestamp_t;


typedef struct INode {
    inode_addr_t   inode_num;  // which inode this is (index in the inode map)
    uint32_t       owner_uid;  // who owns this
    uint32_t       gid;        // which group id the perms refer to
    type_t         type;       // file, directory, symlink, ...
    uint16_t       perms;      // RWX permissions for owner / group / all
    uint16_t       status;     // whether it's allocated, needs to be written, etc
    uint16_t       link_count; // how many hard links there are to this INode
    timestamp_t    cdate;      // creation_time
    timestamp_t    mdate;      // most recent modification time
    uint16_t       level;      // level
    uint64_t       num_bytes;  // the current size of the inode
    disk_addr_t    block_ptrs[BLOCK_PTRS_PER_INODE]; // data blocks, or blocks of disk_address_t[]
} INode, *inode_t;

typedef INode* INodeMap_t;

void freeINode (INodeMap_t m, inode_addr_t inode_to_free);

inode_addr_t allocateINode(INodeMap_t m, type_t type, block_device_t bd);

INodeMap_t allocateINodeMap(int num_inodes, block_device_t bd);

inode_t addrToINodeMap(INodeMap_t m, inode_addr_t index);
//inode_type_t is a pointer, so it's the same as INode*, right?
disk_addr_t readINodeMap(block_device_t bd, INodeMap_t inode_map, disk_addr_t disk_address);

disk_addr_t writeINodeMap(block_device_t bd, INodeMap_t inode_map, disk_addr_t disk_address);