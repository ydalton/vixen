#include "vixen/vixen.h"

#include <stdio.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

void draw_func(vixen_window_t *window, uint32_t *mem)
{
	uint32_t color;

	/* Draw color bars */
	for (int y = 0; y < window->height; ++y) {
		color = 0xFFFFFFFF;
		for (int x = 0; x < window->width; ++x) {
			if(x >= (7*window->width/8))
				color = 0xFF000000;
			else if(x >= (6*window->width/8))
				color = 0xFF0000FF;
			else if(x >= (5*window->width/8))
				color = 0xFFFF0000;
			else if(x >= (4*window->width/8))
				color = 0xFFFF00FF;
			else if(x >= (3*window->width/8))
				color = 0xFF00FF00;
			else if(x >= (2*window->width/8))
				color = 0xFF00FFFF;
			else if(x >= (1*window->width/8))
				color = 0xFFFFFF00;
			mem[y * window->width + x] = color;
		}
	}
}

int main(int argc, char **argv)
{
	vixen_window_t *window;
	int ret;

	ret = vixen_init(0);
	if(ret)
		eprintf("Vixen failed to start!\n");

	window = vixen_window_create(400, 400, "Example window", "vixen-example", 0);
	if(!window)
		eprintf("Failed to create window!\n");

	vixen_window_set_frame_callback(window, draw_func);

	while(vixen_window_is_running(window));

	vixen_window_destroy(window);
	vixen_exit();
	return 0;
}
