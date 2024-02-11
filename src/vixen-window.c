#include "vixen/vixen.h"

#include "vixen-private.h"
#include "vixen-window.h"

static void handle_toplevel_close(void *data, struct xdg_toplevel *toplevel)
{
	vixen_window_t *window = data;

	window->running = VX_FALSE;
}

static void handle_toplevel_configure(void *data, struct xdg_toplevel *toplevel,
					int width, int height, struct wl_array *states)
{
	vixen_window_t *win = data;

	/*
	 * Ignore the first configure event, otherwise the surface will be
	 * invisible.
	 */
	if(width == 0 || height == 0)
		return;

	win->width = width;
	win->height = height;
}

static void handle_toplevel_configure_bounds(void *data, struct xdg_toplevel *toplevel,
						int width, int height)
{
}

static struct xdg_toplevel_listener toplevel_listener = {
	.configure = handle_toplevel_configure,
	.close = handle_toplevel_close,
	/*
	 * This seems to be emitted since we don't have a buffer to back our
	 * wl_surface on.
	 */
	.configure_bounds = handle_toplevel_configure_bounds,
};

static void handle_surface_configure(void *data, struct xdg_surface *surface,
								uint32_t serial)
{
	xdg_surface_ack_configure(surface, serial);
}

static struct xdg_surface_listener surface_listener = {
	.configure = handle_surface_configure,
};

VX_EXPORT vixen_window_t *vixen_window_create(int width, int height,
						const char *name,
						const char *id, int flags)
{
	vixen_window_t *win;

	if(!vixen) {
		if(vixen->debug)
			VIXEN_ERROR("Vixen has not been initialized.");
	}

	if(!vixen->compositor || !vixen->shm) {
		if(vixen->debug)
			VIXEN_FATAL("Your Wayland compositor does not support the %s and %s interfaces.",
					wl_compositor_interface.name, wl_shm_interface.name);
		return NULL;
	}

	if(vixen->flags & VX_USE_INPUT && !vixen->seat) {
		if(vixen->debug)
			VIXEN_ERROR("Input was asked but the compositor does not support the %s interface.",
					wl_seat_interface.name);
		return NULL;
	}

	win = VX_MALLOC(sizeof(*win));
	if(!win)
		return NULL;

	win->width = width;
	win->height = height;
	win->name = name;
	win->app_id = id;
	win->flags = flags;
	win->running = VX_TRUE;

	win->surface = wl_compositor_create_surface(vixen->compositor);
	win->xdg_surface = xdg_wm_base_get_xdg_surface(vixen->wm_base, win->surface);
	xdg_surface_add_listener(win->xdg_surface, &surface_listener, win);

	win->toplevel = xdg_surface_get_toplevel(win->xdg_surface);
	xdg_toplevel_set_app_id(win->toplevel, win->app_id);
	xdg_toplevel_set_title(win->toplevel, win->name);
	xdg_toplevel_add_listener(win->toplevel, &toplevel_listener, win);

	wl_surface_commit(win->surface);

	return win;
}

VX_EXPORT VX_BOOL vixen_window_is_running(vixen_window_t *window)
{
	return wl_display_dispatch(vixen->display) && window->running;
}

VX_EXPORT void vixen_window_destroy(vixen_window_t *window)
{
	xdg_toplevel_destroy(window->toplevel);
	xdg_surface_destroy(window->xdg_surface);
	wl_surface_destroy(window->surface);
	VX_FREE(window);
}

VX_EXPORT void vixen_window_set_frame_callback(vixen_window_t *window,
		void (*func)(vixen_window_t *window, void* mem))
{
	window->frame_callback = func;
}
