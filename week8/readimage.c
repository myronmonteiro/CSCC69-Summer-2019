#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ext2.h"

unsigned char *disk;


int main(int argc, char **argv) {

    if(argc != 2) {
        fprintf(stderr, "Usage: readimg <image file name>\n");
        exit(1);
    }
    int fd = open(argv[1], O_RDWR);

    disk = mmap(NULL, 128 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(disk == MAP_FAILED) {
	perror("mmap");
	exit(1);
    }

    struct ext2_super_block *sb = (struct ext2_super_block *)(disk + 1024);
    printf("Inodes: %d\n", sb->s_inodes_count);
    printf("Blocks: %d\n", sb->s_blocks_count);
    printf("Block group:\n");


    /**********************************************************************************/
    /************************************** TASK 1 ************************************/
    /**********************************************************************************/


    // Index to the group descriptor, cast to the required struct
    struct   ext2_group_desc *bgd = (struct ext2_group_desc *) (disk + 2048);
    // Get the attributes needed
    printf("    block bitmap: %d\n", bgd->bg_block_bitmap);
    printf("    inode bitmap: %d\n", bgd->bg_inode_bitmap);
    printf("    inode table: %d\n", bgd->bg_inode_table);
    printf("    free blocks: %d\n", bgd->bg_free_blocks_count);
    printf("    free inodes: %d\n", bgd->bg_free_inodes_count);
    printf("    used_dirs: %d\n", bgd->bg_used_dirs_count);


    
    /**********************************************************************************/
    /************************************** TASK 2 ************************************/
    /**********************************************************************************/


    
    /******************************* Block Bitmap *************************************/

    // Get block bitmap
    char *bm = (char *) (disk + (bgd->bg_block_bitmap * EXT2_BLOCK_SIZE);
    // counter for shift
    printf("block bitmap: ");
    int index = 0;
    for (int i = 0; i < sb->s_blocks_count; i++) {
        // get the corresponding byte
	unsigned c = bm[i/8];
        // Print the correcponding bit
	printf("%d", (c & (1 << index)) > 0);
        // increment shift index, if > 8 reset.
	if (++index == 8) (index = 0, printf(" "));
    }
    printf("\n");

    /******************************* Inode Bitmap *************************************/
    /************************* + store used inodes ************************************/

    // Get inode bitmap
    char *ib = (char *) (disk + (bgd->bg_inode_bitmap * EXT2_BLOCK_SIZE);					 
    // Want to keep track of the used inodes in this array
    int inum[32];
    // Root inode is Inode number 2, index 1
    // current size of the array
    // because we stored the first one
    // counter for shift
    printf("inode bitmap: ");
    int index2 = 0;
    for (int i = 0; i < sb->s_inodes_count; i++) {
        // get the corresponding byte
	unsigned b = ib[i/8];
        // Print the correcponding bit
	printf("%d", (b & (1 << index2) > 0);
        // If that bit was a 1, inode is used, store it into the array.
        // Note, this is the index number, NOT the inode number
        // inode number = index number + 1
        if ((c & (1 << index2)) > 0 && i > 10) {    // > 10 because first 11 not used
            inum[inumc++] = i;
        }
        // increment shift index, if > 8 reset.
	if (++index2 == 8) (index2 = 0, printf(" "));
    }
    printf("\n\n");

    struct ext2_inode* in = (struct ext2_inode*) (disk + bgd->bg_inode_table * EXT2_BLOCK_SIZE);



    /******************************* Print Inodes *************************************/


    /**** The following array is used to keep track of directories ****/
    int dirs[128];
    int dirsin = 0;

    printf("Inodes:\n");

    // Go through all the used inodes stored in the array above
    for (int i = 0; i < inumc; i++) {
        // Remember array stores the index
        struct ext2_inode* curr = in + inum[i];
        // Number = index + 1
        char type = (S_ISDIR(curr->i_mode)) ? 'd' : ((S_ISREG(curr->i_mode)) ? 'f' : 's');
        // Print Inode info
        printf("[%d] ", i+1);
	    printf("type: %c ", curr->i_mode);
	    printf("size:&d ", curr->i_size);
		printf("links: %d ", curr->i_links_count);
		printf("blocks: %d\n", curr->i_blocks);
        // Now to print all the blocks for the inode  
        // Get the array of blocks from inode
        // Loop through and print all value till a 0 is seen in the array
        while(1) {
            if (*arr == 0) {
                break;
            }
            // If it's a directory, add to the array.
            printf("%d ", *arr++);
        }
        printf("\n");
    }
    printf("\n");

    /**********************************************************************************/
    /************************************** TASK 3 ************************************/
    /**********************************************************************************/


    printf("Directory Blocks:\n");
    // For all the directory blocks
    for (int i = 0; i < dirsin; i++) {
        // Get the block number
        // Get the position in bytes and index to block
        unsigned long pos = (unsigned long) disk + blocknum * EXT2_BLOCK_SIZE;
        struct ext2_dir_entry_2 *dir = (struct ext2_dir_entry_2 *) pos;

        printf("    DIR BLOCK NUM: %d (for inode %d)\n", blocknum, dir->inode);

        do {
            // Get the length of the current block and type
            int cur_len = dir->rec_len;
            char typ = (dir->file_type == EXT2_FT_REG_FILE) ? 'f' : 
                        ((dir->file_type == EXT2_FT_DIR) ? 'd' : 's');
            // Print the current directory entry
            // Update position and index into it

            // Last directory entry leads to the end of block. Check if 
            // Position is multiple of block size, means we have reached the end
        } while (pos % EXT2_BLOCK_SIZE != 0);
    }

    
    return 0;
}
