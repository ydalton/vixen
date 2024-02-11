#ifndef VIXEN_WINDOW_H_
#define VIXEN_WINDOW_H_

#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

struct vixen_window_t {
	struct wl_display *display;
	struct wl_surface *surface;
	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *toplevel;

	int width, height;
	int flags;
	VX_BOOL running;
	const char *name;
	const char *app_id;
	void (*frame_callback)(vixen_window_t *window, void* mem);
};

#endif
