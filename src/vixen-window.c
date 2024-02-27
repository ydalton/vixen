/*
 * vixen-window.c
 *
 * Copyright 2024 ydalton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */

#include <wayland-client.h>
#include <assert.h>

#include "xdg-shell-client-protocol.h"

#include "vixen-private.h"

struct __vixen_window_internal {
	struct wl_display *display;
	struct wl_surface *surface;
	struct xdg_surface *xdg_surface;
	struct xdg_toplevel *toplevel;
};

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

	if(win->flags & VX_WINDOW_RESIZEABLE) {
		win->width = width;
		win->height = height;
	}
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
	struct __vixen_window_internal *internal;
	struct wl_buffer *buffer;
	vixen_window_t *window;

	window = data;
	internal = window->internal;

	xdg_surface_ack_configure(surface, serial);

	buffer = create_window_buffer(window);
	assert(buffer != NULL);
	wl_surface_attach(internal->surface, buffer, 0, 0);
	wl_surface_commit(internal->surface);
}

static struct xdg_surface_listener surface_listener = {
	.configure = handle_surface_configure,
};

VX_EXPORT vixen_window_t *vixen_window_create(int width, int height,
						const char *name,
						const char *id, int flags)
{
	vixen_window_t *window;
	struct __vixen_window_internal *internal;

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

	if(!vixen->seat) {
		if(vixen->debug)
			VIXEN_ERROR("Your Wayland compositor does not support the %s interface.",
					wl_seat_interface.name);
		return NULL;
	}

	window = VX_MALLOC(sizeof(*window));
	if(!window)
		return NULL;

	window->width = width;
	window->height = height;
	window->name = name;
	window->app_id = id;
	window->flags = flags;
	window->running = VX_TRUE;

	internal = VX_MALLOC(sizeof(struct __vixen_window_internal));
	if(!internal)
		goto err;

	internal->surface = wl_compositor_create_surface(vixen->compositor);
	internal->xdg_surface = xdg_wm_base_get_xdg_surface(vixen->wm_base, internal->surface);
	xdg_surface_add_listener(internal->xdg_surface, &surface_listener, window);

	internal->toplevel = xdg_surface_get_toplevel(internal->xdg_surface);
	xdg_toplevel_set_app_id(internal->toplevel, window->app_id);
	xdg_toplevel_set_title(internal->toplevel, window->name);
	xdg_toplevel_add_listener(internal->toplevel, &toplevel_listener, window);

	if(!(window->flags & VX_WINDOW_RESIZEABLE)) {
		xdg_toplevel_set_min_size(internal->toplevel, window->width,
					  window->height);
		xdg_toplevel_set_max_size(internal->toplevel, window->width,
					  window->height);
	}

	wl_surface_commit(internal->surface);

	window->internal = internal;

	return window;
err:
	VX_FREE(window);
	return NULL;
}

VX_EXPORT VX_BOOL vixen_window_is_running(vixen_window_t *window)
{
	return wl_display_dispatch(vixen->display) && window->running;
}

VX_EXPORT void vixen_window_destroy(vixen_window_t *window)
{
	struct __vixen_window_internal *internal = window->internal;
	
	xdg_toplevel_destroy(internal->toplevel);
	xdg_surface_destroy(internal->xdg_surface);
	wl_surface_destroy(internal->surface);
	VX_FREE(window->internal);
	VX_FREE(window);
}

VX_EXPORT void vixen_window_set_frame_callback(vixen_window_t *window,
		void (*func)(vixen_window_t *window, uint32_t* mem))
{
	window->frame_callback = func;
}
