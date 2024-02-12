#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include "vixen-private.h"

static void randname(char *buf)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	long r = ts.tv_nsec;
	for (int i = 0; i < 6; ++i) {
		buf[i] = 'A'+(r&15)+(r&16)*2;
		r >>= 5;
	}
}

static int create_shm_file(void)
{
	int retries = 100;
	do {
		char name[] = "/wl_shm-XXXXXX";
		randname(name + sizeof(name) - 7);
		--retries;
		int fd = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0600);
		if (fd >= 0) {
			shm_unlink(name);
			return fd;
		}
	} while (retries > 0 && errno == EEXIST);
	return -1;
}

static int allocate_shm_file(size_t size)
{
	int fd = create_shm_file();
	if (fd < 0)
		return -1;
	int ret;
	do {
		ret = ftruncate(fd, size);
	} while (ret < 0 && errno == EINTR);
	if (ret < 0) {
		close(fd);
		return -1;
	}
	return fd;
}

static void wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
	wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
	.release = wl_buffer_release,
};

struct wl_buffer *create_window_buffer(vixen_window_t *window)
{
	struct wl_shm_pool *pool;
	struct wl_buffer *buffer;

	int fd, stride, size;
	uint32_t *mem;

	stride = window->width * 4;
	size = window->height * stride;

	fd = allocate_shm_file(size);
	if(fd == -1)
		return NULL;

	mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED,
			      fd, 0);
	if(mem == MAP_FAILED) {
		close(fd);
		return NULL;
	}
	
	pool = wl_shm_create_pool(vixen->shm, fd, size);
	buffer = wl_shm_pool_create_buffer(pool, 0, window->width, window->height,
					   stride, WL_SHM_FORMAT_XRGB8888);
	wl_shm_pool_destroy(pool);
	close(fd);

	if(window->frame_callback)
		window->frame_callback(window, mem);

	munmap(mem, size);
	wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
	return buffer;
}
