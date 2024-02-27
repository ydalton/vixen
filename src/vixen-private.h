/*
 * vixen-private.h
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

#ifndef VIXEN_PRIVATE_H_
#define VIXEN_PRIVATE_H_

/* TODO: find a way to not include stdio.h in this header */
#include <wayland-client.h>
#include <stdlib.h> 
#include <stdio.h>

#include <vixen/vixen.h>

#include "xdg-shell-client-protocol.h"

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
	VX_BOOL debug;
	struct wl_display *display;
	struct wl_compositor *compositor;
	struct wl_shm *shm;
	struct wl_seat *seat;
	struct xdg_wm_base *wm_base;
};

extern struct __vixen_state_t *vixen;

struct wl_buffer *create_window_buffer(vixen_window_t *window);

#endif
