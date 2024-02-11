#include "vixen/vixen.h"

#include "vixen-private.h"
#include "vixen-window.h"

VX_EXPORT vixen_window_t *vixen_window_create(int width, int height,
						const char *title_name,
						const char *app_id)
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

	win->surface = wl_compositor_create_surface(vixen->compositor);

	return win;
}

VX_EXPORT void vixen_window_destroy(vixen_window_t *window)
{
	wl_surface_destroy(window->surface);
	VX_FREE(window);
}

VX_EXPORT void vixen_window_set_frame_callback(vixen_window_t *window, void (*func)(void))
{
	window->frame_callback = func;
}
