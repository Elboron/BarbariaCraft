#ifndef CHUNK
#define CHUNK

#include "block.h"

#define CHUNKS_IN_MEMORY 128
#define VIEW_DISTANCE 16
#define VERTEX_MIN_ALLOC_NUM 100

struct BlockVertex {
	int pos_x, pos_y, pos_z;
	/* The type which corresponds to the texture to be used */
	int type;
	int vertex_position;
};

struct Mesh {
	struct BlockVertex* vertices;
};

typedef struct Chunk {
	struct Layer* layers;
	int chunk_height;
	struct Mesh mesh;
} Chunk;


struct Layer {
	Block blocks[16][16];
};

void init_chunks();
void add_to_chunk_list(Chunk chunk, int pos_x, int pos_y);
void update_player(int pos_x, int pos_y, int pos_z, int view_angle,
		int camera_target_x, int camera_target_y, int camera_target_z);
void render_chunks();
void free_chunks();

#endif
