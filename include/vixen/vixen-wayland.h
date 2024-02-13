#ifndef VIXEN_H_
#error "Never include <vixen/vixen-wayland.h> directly; define VIXEN_EXPOSE_WAYLAND and use <vixen/vixen.h> instead."
#endif

#include <wayland-client.h>

VX_EXPORT struct wl_display *vixen_get_wl_display(void);
VX_EXPORT struct wl_seat *vixen_get_wl_seat(void);

