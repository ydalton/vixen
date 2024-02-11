#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>

#include "vixen/vixen.h"
#include "vixen-private.h"

struct __vixen_state_t *vixen = NULL;

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
		if(vixen->flags & VX_USE_INPUT)
			vixen->seat = wl_registry_bind(registry, name,
						&wl_seat_interface, version);
	}
}

static void handle_global_remove(void *data, struct wl_registry *registry, uint32_t name)
{
}

static struct wl_registry_listener registry_listener = {
	.global = handle_global,
	.global_remove = handle_global_remove,
};

VX_EXPORT int vixen_init(int flags)
{
	struct wl_registry *registry;

	/* already initialized */
	if(vixen)
		return EXIT_SUCCESS;

	vixen = VX_MALLOC(sizeof(*vixen));
	if(!vixen)
		return EXIT_FAILURE;

	vixen->flags = flags;
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
