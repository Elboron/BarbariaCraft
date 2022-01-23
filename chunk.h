#ifndef CHUNK
#define CHUNK

#include "block.h"

#define CHUNKS_IN_MEMORY 128
#define VIEW_DISTANCE 16

void init_chunks();
void add_to_chunk_list(Block* chunk_blocks, int pos_x, int pos_y);
void update_player(int pos_x, int pos_y, int pos_z, int view_angle,
		int camera_target_x, int camera_target_y, int camera_target_z);
void render_chunks();

#endif
