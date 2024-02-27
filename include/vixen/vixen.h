/*
 * vixen.h
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
