#include "chunk.h"

#include <GL/glew.h>
#include <math.h>

typedef struct Chunk {
	Block* block_list;
} Chunk;

Chunk* memory_chunks;

int render_bounds_p1_x;
int render_bounds_p1_y;
int render_bounds_p2_x;
int render_bounds_p2_y;
int render_begin_x;
int render_begin_y;
int view_point_x;
int view_point_y;

void init_chunks() {
}

void add_to_chunk_list(Block* chunk_blocks, int pos_x, int pos_y) {

}

static void regenerate_render_mesh(int pos_z, int camera_target_z) {
	/* This function regenerates the meshs to be rendered with all now 
	 * visible blocks */
}

static int check_if_in_bounds(int x, int y) {
	/* Checks if a block is in render bounds */
	if(x >= render_bounds_p1_x && x <= view_point_x) return 1;
	if(x <= render_bounds_p1_x && x >= view_point_x) return 1;
	if(y <= render_bounds_p2_y && x >= view_point_y) return 1;
	if(y >= render_bounds_p2_y && x <= view_point_y) return 1;
	return 0;
}

void update_player(int pos_x, int pos_y, int pos_z, int view_angle,
		int camera_target_x, int camera_target_y, int camera_target_z) {
	int player_chunk_pos_x = pos_x / 16;
	int player_chunk_pos_y = pos_y / 16;
	
	/* Camera target is a direction vector normalized to a length of one.
	 * We normalize it to the view distance. */
	float normalized_x = 0;
	float normalized_y = 0;
	normalized_x = player_chunk_pos_x + VIEW_DISTANCE * camera_target_x;
	normalized_y = player_chunk_pos_y + VIEW_DISTANCE * camera_target_y;
	
	float normalized_length = sqrtf(powf(normalized_x, 2) + powf(normalized_y, 2));
	float offset = sqrtf(powf(cosf(view_angle / 2.0f) * normalized_length, 2) - 
			powf(normalized_x, 2));

	render_bounds_p1_x = normalized_x;
	render_bounds_p2_y = normalized_y;
	if(normalized_y < 0.0f) {
		render_bounds_p1_y = normalized_y + offset;
	} else {
		render_bounds_p1_y = normalized_y - offset;
	}
	if(normalized_x < 0.0f) {
		render_bounds_p2_x = normalized_x + offset;
	} else {
		render_bounds_p2_y = normalized_x - offset;
	}
	render_begin_x = player_chunk_pos_x;
	render_begin_y = player_chunk_pos_y;

	view_point_x = normalized_x;
	view_point_y = normalized_y;

	regenerate_render_mesh(pos_z, camera_target_z);
}

void render_chunks() {

}

