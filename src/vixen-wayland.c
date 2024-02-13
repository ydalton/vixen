#define VIXEN_EXPOSE_WAYLAND
#include "vixen-private.h"

VX_EXPORT struct wl_display *vixen_get_wl_display(void)
{
	return vixen->display;
}

VX_EXPORT struct wl_seat *vixen_get_wl_seat(void)
{
	return vixen->seat;
}
