#ifndef PATH_GENERATOR
#define PATH_GENERATOR

#include "config.h"

enum AssetType {
	TEXTURE,
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

enum AssetVariant {
	BLOCK,
	ENTITY
};

void gen_base_path();
char* get_path(char* name, enum AssetType type, enum AssetVariant variant);
void free_path();
void free_path_generator();

#endif
