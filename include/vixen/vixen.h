#ifndef VIXEN_H_
#define VIXEN_H_

#if defined(__GNUC__) && __GNUC__ >= 4
#define VX_EXPORT 	__attribute__((visibility("default")))
#else
#define VX_EXPORT
#endif 

#define	VX_TRUE 	1
#define VX_FALSE 	0

#include <stdint.h>

typedef unsigned int VX_BOOL;

/* dumb */
struct vixen_window_t;
typedef struct vixen_window_t vixen_window_t;

struct vixen_window_t {
	void *internal;
	int width, height;
	int flags;
	VX_BOOL running;
	const char *name;
	const char *app_id;
	void (*frame_callback)(vixen_window_t *window, uint32_t* mem);
};

enum {
	VX_USE_INPUT = 1,
	VX_USE_KEYBOARD = 2 | VX_USE_INPUT,
	VX_USE_MOUSE = 4 | VX_USE_INPUT,
};

enum { 
	VX_WINDOW_RESIZEABLE = 1,
	VX_WINDOW_STATIC = 2,
};

VX_EXPORT int vixen_init(void);
VX_EXPORT void vixen_exit(void);

VX_EXPORT vixen_window_t *vixen_window_create(int width, int height, const char *name, const char *id, int flags);
VX_EXPORT void vixen_window_destroy(vixen_window_t *window);
VX_EXPORT void vixen_window_set_frame_callback(vixen_window_t *window, void (*func)(vixen_window_t *window, uint32_t *mem));
VX_EXPORT VX_BOOL vixen_window_is_running(vixen_window_t *window);

#ifdef VIXEN_EXPOSE_WAYLAND
#include <vixen/vixen-wayland.h>
#endif

#endif
