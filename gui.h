#ifndef GUI
#define GUI

#include "config.h"

struct GuiElement {
	struct GuiElement* next_element;
	float start_x;
	float start_y;
	float end_x;
	float end_y;
	int sprite_id;
	_Bool is_clickable;
};

typedef struct Gui {
	float start_x;
	float start_y;
	float end_x;
	float end_y;
	int background_sprite_id;
	struct GuiElement* first_element;
	int element_count;
} Gui;

struct GuiInstance {
	Gui* gui;
	struct GuiInstance* next_instance;
	int height;
};


void init_gui();
/* GUIs with a higher high overlay those with lower ones and receive the input. */
Gui* open_gui(float start_x, float start_y, float end_x, float end_y, float height, char* background_texture_name);
/* Elements added last take precedence over those added before in terms of overlay and input */
void add_element(Gui* gui, float start_x, float start_y, float end_x, float end_y, char* texture_name, _Bool is_clickable);
void render_guis();
void process_click(float x, float y);
void close_gui(Gui* gui);
void free_gui();

#endif
