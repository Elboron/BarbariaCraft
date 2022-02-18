#include "path_generator.h"

#include <malloc.h>
#include <string.h>

char* path;
char* base_path;

void gen_base_path() {
	char* b_path = "assets/";
	base_path = malloc(strlen(b_path) * sizeof(char));
	strcpy(base_path, b_path);
}

static void extend_path(int to_size, char* str) {
	path = realloc(path, to_size * sizeof(char));
	strcat(path, str);
}

char* get_path(char* name, enum AssetType type, enum AssetVariant variant) {
	int size = strlen(base_path);
	path = malloc(size * sizeof(char));
	strcpy(path, base_path);
	char file_extension[MAX_FILE_EXTENSION_LENGTH];
	switch(type) {
	case TEXTURE:
		size += 9;
		extend_path(size, "textures/");
		strcpy(file_extension, TEXTURE_FILE_EXTENSION);
		break;
	case VERTEX_SHADER:
		size += 8;
		extend_path(size, "shaders/");
		strcpy(file_extension, VERTEX_SHADER_FILE_EXTENSION);
		break;
	case FRAGMENT_SHADER:
		size += 8;
		extend_path(size, "shaders/");
		strcpy(file_extension, FRAGMENT_SHADER_FILE_EXTENSION);
		break;
	}

	switch(variant) {
	case BLOCK:
		size += 7;
		extend_path(size, "blocks/");
		break;
	case ENTITY:
		size += 9;
		extend_path(size, "entities/");
		break;
	}

	size += strlen(name);
	extend_path(size, name);
	size += MAX_FILE_EXTENSION_LENGTH;
	extend_path(size, file_extension);
	return path;
}

void free_path() {
	free(path);
}

void free_path_generator() {
	free(base_path);
}
