#include "gui.h"

#include <malloc.h>

/* This list shall be sorted according to the Gui heights */
struct GuiInstance* gui_list;
int gui_count = 0;

void init_gui() {
	gui_list = malloc(sizeof(struct GuiInstance*));
	gui_list->gui = (void*)0;
	gui_list->next_instance = (void*)0;
	gui_list->height = 0.0f;
}

static int load_sprite_get_id(char* path) {
	return 0;
}

static void insert_gui(Gui* gui, float height) {
	/* Determine the position to enter the GUI into */
	float closest_min_height = 0.0f;
	struct GuiInstance* current = gui_list;
	struct GuiInstance* closest = gui_list;
	for(int i = 0; i < gui_count; ++i) {
		if(current->height > closest_min_height && current->height < height) {
			closest_min_height = current->height;
			closest = current;
		}
		current = current->next_instance;
	}
	
	/* Enter the GUI as a link */
	struct GuiInstance* new_instance = gui_list;
	if(current != gui_list) {
		new_instance = malloc(sizeof(struct GuiInstance));
	}
	if(closest->next_instance != (void*)0) {
		/* If the previous link linked to a link, let the inserted link link to this link*/
		new_instance->next_instance = closest->next_instance;	
	} else {
		new_instance->next_instance = (void*)0;
	}
	new_instance->gui = gui;
	new_instance->height = height;
	/* Let the previous link link to this link */
	current->next_instance = new_instance;
	++gui_count;
	
}

Gui* open_gui(float start_x, float start_y, float end_x, float end_y, float height, char* background_texture_name) {
	int sprite_id = load_sprite_get_id(background_texture_name);
	Gui *g = malloc(sizeof(Gui));
	*g = (Gui){
		.start_x = start_x,
		.start_y = start_y,
		.end_x = end_x,
		.end_y = end_y,
		.background_sprite_id = sprite_id,
		.element_count = 0,
		.first_element = malloc(sizeof(struct GuiElement*))
	};
	insert_gui(g, height);
	return g;
}

void add_element(Gui* gui, float start_x, float start_y, float end_x, float end_y, char* texture_name, _Bool is_clickable) {
	struct GuiElement* current_element = gui->first_element;
	for(int i = 0; i <= gui->element_count; ++i) {
		current_element = current_element->next_element;
	}
	current_element->start_x = start_x;
	current_element->start_y = start_y;
	current_element->end_x = end_x;
	current_element->end_y = end_y;
	current_element->sprite_id = load_sprite_get_id(texture_name);
	current_element->is_clickable = is_clickable;
	current_element->next_element = malloc(sizeof(struct GuiElement*));
	gui->element_count++;
}

void render_guis() {

}

void process_click(float x, float y) {

}

void close_gui(Gui* gui) {
	struct GuiInstance* current = gui_list;
	int old_pointer_to_change = 0;
	struct GuiInstance* next_instance = gui_list;
	for(int i = 0; i < gui_count; ++i) {
		/* Free the Gui memory */
		if(current->gui == gui) {
			old_pointer_to_change = i - 1;
			next_instance = current->next_instance;
			free(current->gui);
			free(current);
		}
		current = current->next_instance;	
	}
	/* Let the previous link point to the next link */
	current = gui_list;
	for(int i = 0; i == old_pointer_to_change; ++i) {
		current = current->next_instance;
	}
	current->next_instance = next_instance;
}

void free_gui() {
	free(gui_list);
}

