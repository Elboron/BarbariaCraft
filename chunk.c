#include "chunk.h"

#include <GL/glew.h>
#include <math.h>
#include <malloc.h>

Chunk** memory_chunks;
int memory_chunks_x = 0;
int memory_chunks_y = 0;

int render_bounds_p1_x;
int render_bounds_p1_y;
int render_bounds_p2_x;
int render_bounds_p2_y;
int render_begin_x;
int render_begin_y;
int view_point_x;
int view_point_y;

void init_chunks() {
	memory_chunks = malloc(2 * VIEW_DISTANCE * sizeof(Chunk*));
	memory_chunks[0] = malloc(2 * VIEW_DISTANCE * sizeof(Chunk));
	memory_chunks_x = 2 * VIEW_DISTANCE - 1;
	memory_chunks_y = 2 * VIEW_DISTANCE - 1;
}

void add_to_chunk_list(Chunk chunk, int pos_x, int pos_y) {
	if(pos_y > memory_chunks_y) {
		memory_chunks = realloc(memory_chunks, 2 * pos_y * sizeof(Chunk*));
	}

	if(pos_x > memory_chunks_x) {
		for(int i = 0; i < memory_chunks_y; ++i) {
			memory_chunks[i] = realloc(memory_chunks[i],
					2 * pos_x * sizeof(Chunk));
		}
	}
	memory_chunks[pos_x][pos_y] = chunk;
}


static int check_if_in_bounds(int x, int y) {
	/* Checks if a block is in render bounds */
	if(x >= render_bounds_p1_x && x <= view_point_x) return 1;
	if(x <= render_bounds_p1_x && x >= view_point_x) return 1;
	if(y <= render_bounds_p2_y && x >= view_point_y) return 1;
	if(y >= render_bounds_p2_y && x <= view_point_y) return 1;
	return 0;
}

static void regenerate_chunk_mesh(int x, int y) {
	Chunk chunk = memory_chunks[x][y];
	int min_height = 100;
	int max_height = 0;
	for(int i = 0; i < 16; ++i) {
		for(int j = 0; j < 16; ++j) {
			int height = chunk.height_map[i][j];
			if(height < min_height) min_height = height;
			if(height > max_height) max_height = height;
		}
	}
	chunk.surface_mesh.vertices = realloc(chunk.surface_mesh.vertices,
			sizeof(struct Vertex) * 6 * (max_height - min_height) + 1);
}

void update_player(int pos_x, int pos_y, int pos_z, int view_angle,
		int camera_target_x, int camera_target_y, int camera_target_z) {
	/* This function checks which chunks are visible*/
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

	/* The slope of the two functions */
	float pos1f_m = (render_bounds_p1_y - player_chunk_pos_y) /
		(render_bounds_p1_x - player_chunk_pos_x);
	float pos2f_m = (render_bounds_p2_y - player_chunk_pos_y) /
		(render_bounds_p2_x - player_chunk_pos_x);

	/* Iterate over each chunk of the P1 function, get all the chunks on the line
	 * to the P2 function, generate the chunk mesh for each */
	int x = player_chunk_pos_x;
	int y = player_chunk_pos_y;
	int i = 0;
	do {
		y = player_chunk_pos_x + i * pos1f_m;
		int y_bound = player_chunk_pos_y + i * pos2f_m;
		if(y_bound > y) {
			for(int j = y; j <= y_bound; ++j) {
				regenerate_chunk_mesh(i, j);
			}
		} else {
			for(int j = y; j >= y_bound; --j) {
				regenerate_chunk_mesh(i, j);
			}
		}
		++x;
	} while(check_if_in_bounds(x, y));
}

void render_chunks() {

}

