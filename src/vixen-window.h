#ifndef VIXEN_WINDOW_H_
#define VIXEN_WINDOW_H_

#include <wayland-client.h>

struct vixen_window_t {
	struct wl_display *display;
	struct wl_surface *surface;
	int width, height;
	void *mem;
	void (*frame_callback)(void* mem);
};

#endif
