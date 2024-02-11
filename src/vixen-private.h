#ifndef VIXEN_PRIVATE_H_
#define VIXEN_PRIVATE_H_

/* TODO: find a way to not include stdio.h in this header */
#include <wayland-client.h>
#include <stdlib.h> 
#include <stdio.h>

#define	VIXEN_LOG(...) 		(fprintf(stderr, "[LOG] libvixen: " __VA_ARGS__))
#define	VIXEN_ERROR(...) 	(fprintf(stderr, "[ERROR] libvixen: " __VA_ARGS__))
#define	VIXEN_FATAL(...) 	(fprintf(stderr, "[FATAL] libvixen: " __VA_ARGS__))

#define VX_MALLOC(n)	calloc(1, n)
#define VX_FREE(ptr)		\
	do {			\
		free(ptr);	\
		ptr = NULL;	\
	} while(0)	

struct __vixen_state_t {
	int flags;
	VX_BOOL debug;
	struct wl_display *display;
	struct wl_compositor *compositor;
	struct wl_shm *shm;
	struct wl_seat *seat;
};

extern struct __vixen_state_t *vixen;

#endif