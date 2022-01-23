#ifndef CHUNK
#define CHUNK

#include "block.h"

#define CHUNKS_IN_MEMORY 128
#define VIEW_DISTANCE 16

struct Vertex {
	int pos_x, pos_y, pos_z;
};

struct Mesh {
	struct Vertex* vertices;
};

typedef struct Chunk {
	struct Layer* layers;
	int layer_count;
	int height_map[16][16];
	struct Mesh surface_mesh;
} Chunk;


struct Layer {
	Block blocks[16][16];
	int layer_height;
};

void init_chunks();
void add_to_chunk_list(Chunk chunk, int pos_x, int pos_y);
void update_player(int pos_x, int pos_y, int pos_z, int view_angle,
		int camera_target_x, int camera_target_y, int camera_target_z);
void render_chunks();

#endif
