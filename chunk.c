#include "chunk.h"

#include <GL/glew.h>
#include <math.h>
#include <malloc.h>

Chunk** memory_chunks;
int memory_chunks_x = 0;
int memory_chunks_y = 0;

#define NO_BLOCK 0xA 
#define CHUNK_MIN_VERTEX_COUNT 100

enum BlockCorner {
	LEFT_FRONT_BOTTOM = 0,
	RIGHT_FRONT_BOTTOM = 1,
	LEFT_FRONT_TOP = 2,
	RIGHT_FRONT_TOP = 3,
	LEFT_BACK_BOTTOM = 4,
	RIGHT_BACK_BOTTOM = 5,
	LEFT_BACK_TOP = 6,
	RIGHT_BACK_TOP = 7,
};

void init_chunks() {
	memory_chunks = malloc(2 * VIEW_DISTANCE * sizeof(Chunk*));
	memory_chunks[0] = malloc(2 * VIEW_DISTANCE * sizeof(Chunk));
	memory_chunks_x = 2 * VIEW_DISTANCE - 1;
	memory_chunks_y = 2 * VIEW_DISTANCE - 1;
}

static struct BlockVertex gen_vertex(int x, int y, int z, int type, enum BlockCorner vertex_pos) {
	struct BlockVertex ret = {.pos_x = x, .pos_y = y, .pos_z = z, .type = (int)type,
		.vertex_position = vertex_pos};
	return ret;
}

static void move_vertex_limit(Chunk *c, int vertex_count) {
	if(vertex_count >= CHUNK_MIN_VERTEX_COUNT) {
		c->mesh.vertices = realloc(c->mesh.vertices, (vertex_count + 1)	* sizeof(struct BlockVertex));
	}								
}


static void gen_mesh(Chunk *chunk) {
	chunk->mesh.vertices = malloc(CHUNK_MIN_VERTEX_COUNT * sizeof(struct BlockVertex));
	int vertex_count = 0;
	for(int z = 0; z < chunk->chunk_height; ++z) {
		for(int x = 0; x < 16; ++x) {
			for(int y = 0; y < 16; ++y) {
				if(y == 0 || chunk->layers[z].blocks[x - 1][y].is_air) {
					/* Render the left face */
					vertex_count += 4;
					move_vertex_limit(chunk, vertex_count);
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_FRONT_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_BACK_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_FRONT_TOP); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_BACK_TOP); 
				}
				if(x == 0 || chunk->layers[z].blocks[x][y - 1].is_air) {
					/* Render the back face */
					vertex_count += 4;
					move_vertex_limit(chunk, vertex_count);
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_FRONT_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, RIGHT_FRONT_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_FRONT_TOP); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, RIGHT_FRONT_TOP); 

				}
				if(z != 0 || chunk->layers[z - 1].blocks[x][y].is_air) {	
					/* Generate Bottom face */
					vertex_count += 4;
					move_vertex_limit(chunk, vertex_count);
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_FRONT_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, RIGHT_FRONT_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, LEFT_BACK_BOTTOM); 
					chunk->mesh.vertices[vertex_count - 4] = gen_vertex(x, z, y, 0, RIGHT_BACK_BOTTOM); 

				}
			}
		}
	}
}

void update_player(int pos_x, int pos_y, int pos_z, int view_angle,
		int camera_target_x, int camera_target_y, int camera_target_z) {

}

void add_to_chunk_list(Chunk chunk, int pos_x, int pos_y) {
	if(pos_y > memory_chunks_y) {
		memory_chunks = realloc(memory_chunks, 2 * pos_y * sizeof(Chunk*));
		memory_chunks_y = 2 * pos_y - 1;
	}

	if(pos_x > memory_chunks_x) {
		for(int i = 0; i < memory_chunks_y; ++i) {
			memory_chunks[i] = realloc(memory_chunks[i],
					2 * pos_x * sizeof(Chunk));
		}
		memory_chunks_x = 2 * pos_x - 1;
	}
	memory_chunks[pos_x][pos_y] = chunk;
	gen_mesh(&memory_chunks[pos_x][pos_y]);
}

void render_chunks() {

}

void free_chunks() {
	for(int i = 0; i <=  memory_chunks_y; ++i) {
		for(int j = 0; j < memory_chunks_x; ++j) {
			free(memory_chunks[memory_chunks_x][memory_chunks_y].mesh.vertices);
		}	
		free(memory_chunks[i]);
	}
	free(memory_chunks);
}
