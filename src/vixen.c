#include <string.h>

#include "vixen-private.h"

struct __vixen_state_t *vixen = NULL;

static void handle_ping(void *data, struct xdg_wm_base *wm_base, uint32_t serial)
{
	xdg_wm_base_pong(wm_base, serial);
}

static struct xdg_wm_base_listener wm_base_listener = {
	.ping = handle_ping,
};

static void handle_global(void *data, struct wl_registry *registry,
			  uint32_t name, const char *interface, uint32_t version)
{
	if(strcmp(interface, wl_compositor_interface.name) == 0) {
		vixen->compositor = wl_registry_bind(registry, name,
						&wl_compositor_interface, version);
	} else if(strcmp(interface, wl_shm_interface.name) == 0) {
		vixen->shm = wl_registry_bind(registry, name,
						&wl_shm_interface, version);
	} else if(strcmp(interface, wl_seat_interface.name) == 0) {
		vixen->seat = wl_registry_bind(registry, name,
						&wl_seat_interface, version);
	} else if(strcmp(interface, xdg_wm_base_interface.name) == 0) {
		vixen->wm_base = wl_registry_bind(registry, name,
						  &xdg_wm_base_interface,
						  version);
		xdg_wm_base_add_listener(vixen->wm_base, &wm_base_listener,
					 vixen);
	}
}

static void handle_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
}

static struct wl_registry_listener registry_listener = {
	.global = handle_global,
	.global_remove = handle_global_remove,
};

VX_EXPORT int vixen_init()
{
	struct wl_registry *registry;

	/* already initialized */
	if(vixen)
		return EXIT_SUCCESS;

	vixen = VX_MALLOC(sizeof(*vixen));
	if(!vixen)
		return EXIT_FAILURE;

	if(getenv("VIXEN_DEBUG"))
		vixen->debug = VX_TRUE;

	vixen->display = wl_display_connect(NULL);
	if(!vixen->display)
		goto err;

	registry = wl_display_get_registry(vixen->display);
	wl_registry_add_listener(registry, &registry_listener, vixen);

	wl_display_roundtrip(vixen->display);

	if(vixen->debug)
		VIXEN_LOG("Successfully initialized Vixen.\n");

	return EXIT_SUCCESS;

err:
	VX_FREE(vixen);
	return EXIT_FAILURE;
}

VX_EXPORT void vixen_exit(void)
{
	if(!vixen)
		return;
	wl_display_disconnect(vixen->display);
	VX_FREE(vixen);
}
