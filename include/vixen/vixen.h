#ifndef VIXEN_H_
#define VIXEN_H_

#if defined(__GNUC__) && __GNUC__ >= 4
#define VX_EXPORT 	__attribute__((visibility("default")))
#else
#define VX_EXPORT
#endif 

#define	VX_TRUE 	1
#define VX_FALSE 	0

typedef unsigned int VX_BOOL;
typedef struct vixen_window_t vixen_window_t;

enum {
	VX_USE_INPUT = 1,
	VX_USE_KEYBOARD = 2 | VX_USE_INPUT,
	VX_USE_MOUSE = 4 | VX_USE_INPUT,
};

enum { 
	VX_WINDOW_RESIZABLE = 1,
	VX_WINDOW_STATIC = 2,
};

VX_EXPORT int vixen_init(int flags);
VX_EXPORT void vixen_exit(void);

VX_EXPORT vixen_window_t *vixen_window_create(int width, int height, const char *name, const char *id, int flags);
VX_EXPORT void vixen_window_destroy(vixen_window_t *window);
VX_EXPORT void vixen_window_set_frame_callback(vixen_window_t *window, void (*func)(void *mem));
VX_EXPORT VX_BOOL vixen_window_is_running(vixen_window_t *window);

#endif
