#include "vixen/vixen.h"

#include <stdio.h>

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

int main(int argc, char **argv)
{
	vixen_window_t *window;
	int ret;

	ret = vixen_init(0);
	if(ret)
		eprintf("Vixen failed to start!\n");

	window = vixen_window_create(400, 400, "Example window", "vixen-example");
	if(!window)
		eprintf("Failed to create window!\n");

	vixen_window_destroy(window);
	vixen_exit();
	return 0;
}
