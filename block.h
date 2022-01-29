#ifndef BLOCK
#define BLOCK

typedef struct Block {
	int pos_x;
	int pos_y;
	int pos_z;
	int block_type;
	int is_air;
} Block;

#endif
