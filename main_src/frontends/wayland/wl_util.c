/*

    arfminesweeper: Cross-plataform multi-frontend game
    Copyright (C) 2023 arf20 (Ángel Ruiz Fernandez)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    wl_util.c: Wayland utils

*/

#include "wl_util.h"

#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>


void
global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
    printf("Got a registry losing event for %d\n", id);
}

void
randname(char *buf)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    long r = ts.tv_nsec;
    for (int i = 0; i < 6; ++i) {
        buf[i] = 'A'+(r&15)+(r&16)*2;
        r >>= 5;
    }
}

int
create_shm_file(void)
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

int
allocate_shm_file(size_t size)
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

void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}








/* pointer stuff */


void
wl_pointer_enter(void *data, struct wl_pointer *wl_pointer,
    uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x,
    wl_fixed_t surface_y)
{
    state_t *client_state = data;
    client_state->pointer_event.event_mask |= POINTER_EVENT_ENTER;
    client_state->pointer_event.serial = serial;
    client_state->pointer_event.surface_x = surface_x;
    client_state->pointer_event.surface_y = surface_y;
}

void
wl_pointer_leave(void *data, struct wl_pointer *wl_pointer, uint32_t serial,
    struct wl_surface *surface)
{
    state_t *client_state = data;
    client_state->pointer_event.serial = serial;
    client_state->pointer_event.event_mask |= POINTER_EVENT_LEAVE;
}

void
wl_pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time,
    wl_fixed_t surface_x, wl_fixed_t surface_y)
{
    state_t *client_state = data;
    client_state->pointer_event.event_mask |= POINTER_EVENT_MOTION;
    client_state->pointer_event.time = time;
    client_state->pointer_event.surface_x = surface_x;
    client_state->pointer_event.surface_y = surface_y;
}

void
wl_pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial,
    uint32_t time, uint32_t button, uint32_t state)
{
    state_t *client_state = data;
    client_state->pointer_event.event_mask |= POINTER_EVENT_BUTTON;
    client_state->pointer_event.time = time;
    client_state->pointer_event.serial = serial;
    client_state->pointer_event.button = button;
    client_state->pointer_event.state = state;
}

void
wl_pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time,
    uint32_t axis, wl_fixed_t value)
{
    state_t *client_state = data;
    client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS;
    client_state->pointer_event.time = time;
    client_state->pointer_event.axes[axis].valid = 1;
    client_state->pointer_event.axes[axis].value = value;
}

void
wl_pointer_axis_source(void *data, struct wl_pointer *wl_pointer,
    uint32_t axis_source)
{
    state_t *client_state = data;
    client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS_SOURCE;
    client_state->pointer_event.axis_source = axis_source;
}

void
wl_pointer_axis_stop(void *data, struct wl_pointer *wl_pointer, uint32_t time,
    uint32_t axis)
{
    state_t *client_state = data;
    client_state->pointer_event.time = time;
    client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS_STOP;
    client_state->pointer_event.axes[axis].valid = 1;
}

void
wl_pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer,
    uint32_t axis, int32_t discrete)
{
    state_t *client_state = data;
    client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS_DISCRETE;
    client_state->pointer_event.axes[axis].valid = 1;
    client_state->pointer_event.axes[axis].discrete = discrete;
}





void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
       fprintf(stderr, "seat name: %s\n", name);
}



