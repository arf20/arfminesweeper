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

    wayland.c: Wayland frontend

*/

#define _POSIX_C_SOURCE 200112L
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

#include <common/frontconf.h>

#include "wl_util.h"

#include "wayland.h"

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;




static state_t state = { 0 };

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};






static struct wl_buffer *
draw_frame(state_t *state)
{
    int stride = wWidth * 4;
    int size = stride * wHeight;

    int fd = allocate_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = mmap(NULL, size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            wWidth, wHeight, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw checkerboxed background */
    for (int y = 0; y < wHeight; ++y) {
        for (int x = 0; x < wWidth; ++x) {
            if ((x + y / 8 * 8) % 16 < 8)
                data[y * wWidth + x] = 0xFF666666;
            else
                data[y * wWidth + x] = 0xFFEEEEEE;
        }
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}

static void
xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    state_t *state = data;
    xdg_surface_ack_configure(xdg_surface, serial);

    struct wl_buffer *buffer = draw_frame(state);
    wl_surface_attach(state->surface, buffer, 0, 0);
    wl_surface_commit(state->surface);
}



static void
wl_pointer_frame(void *data, struct wl_pointer *wl_pointer)
{
    state_t *state = data;
    pointer_event_t *event = &state->pointer_event;

    static int pointer_x = 0, pointer_y = 0;

    if (event->event_mask & POINTER_EVENT_MOTION) {
        pointer_x = wl_fixed_to_double(event->surface_x);
        pointer_y = wl_fixed_to_double(event->surface_y);
    }

    if (event->event_mask & POINTER_EVENT_BUTTON) {
        char *state = event->state == WL_POINTER_BUTTON_STATE_RELEASED ?
            "released" : "pressed";
        fprintf(stderr, "button %d %s at %d,%d\n", event->button, state,
            pointer_x, pointer_y);
    }
    
    memset(event, 0, sizeof(*event));
}



static const struct wl_pointer_listener wl_pointer_listener = {
       .enter = wl_pointer_enter,
       .leave = wl_pointer_leave,
       .motion = wl_pointer_motion,
       .button = wl_pointer_button,
       .axis = wl_pointer_axis,
       .frame = wl_pointer_frame,
       .axis_source = wl_pointer_axis_source,
       .axis_stop = wl_pointer_axis_stop,
       .axis_discrete = wl_pointer_axis_discrete,
};

static void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
    state_t *state = data;
    int have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

    if (have_pointer && state->wl_pointer == NULL) {
        state->wl_pointer = wl_seat_get_pointer(state->wl_seat);
        wl_pointer_add_listener(state->wl_pointer, &wl_pointer_listener, state);
    } else if (!have_pointer && state->wl_pointer != NULL) {
        wl_pointer_release(state->wl_pointer);
        state->wl_pointer = NULL;
    }
}

static const struct wl_seat_listener wl_seat_listener = {
    .capabilities = wl_seat_capabilities,
    .name = wl_seat_name,
};

static const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

static void
global_registry_handler(void *data, struct wl_registry *registry, uint32_t id,
    const char *interface, uint32_t version)
{
    printf("Got a registry event for %s id %d\n", interface, id);
    state_t *state = (state_t*)data;

    if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor = wl_registry_bind(registry, id,
            &wl_compositor_interface, 1);
    } else if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = wl_registry_bind(registry, id,
            &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base, &xdg_wm_base_listener,
            state);
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
        state->wl_seat = wl_registry_bind(registry, id,
            &wl_seat_interface, 7);
        wl_seat_add_listener(state->wl_seat, &wl_seat_listener, state);
    }
}



int
wayland_start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    wWidth = (2 * W_MARGIN) + (size * CELL_SIZE) + ((size - 1) * CELL_MARGIN);
    wHeight = HEADER_HEIGHT + W_MARGIN + (size * CELL_SIZE) +
        ((size - 1) * CELL_MARGIN);

    state.wl_display = wl_display_connect(NULL);
    if (state.wl_display == NULL) {
        fprintf(stderr, "Error: Cannot connect to display\n");
        return -1;
    }

    state.wl_registry = wl_display_get_registry(state.wl_display);

    const struct wl_registry_listener registry_listener = {
        global_registry_handler,
        global_registry_remover
    };
    wl_registry_add_listener(state.wl_registry, &registry_listener, &state);

    wl_display_dispatch(state.wl_display);
    wl_display_roundtrip(state.wl_display);

    if (state.wl_compositor == NULL) {
        fprintf(stderr, "Error: Compositor not found\n");
        return -1;
    } else printf("Found compositor\n");

    if (state.wl_shm == NULL) {
        fprintf(stderr, "Error: Cannot get Wayland shm\n");
        return -1;
    } else printf("Found shm\n");

    /* create surface and xdg surface */
    state.surface = wl_compositor_create_surface(state.wl_compositor);
    if (!state.surface) {
        fprintf(stderr, "Error: Cannot get surface\n");
        return -1;
    } printf("Got surface\n");

    state.xdg_surface = xdg_wm_base_get_xdg_surface(state.xdg_wm_base,
        state.surface);

    static const struct xdg_surface_listener xdg_surface_listener = {
        .configure = xdg_surface_configure,
    };
    xdg_surface_add_listener(state.xdg_surface, &xdg_surface_listener, &state);

    state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface);
    xdg_toplevel_set_title(state.xdg_toplevel, TXT_TITLE);

    wl_surface_commit(state.surface);

    while (wl_display_dispatch(state.wl_display)) {

    }

    return 0;
    
}

void
wayland_destroy() {
    if (state.wl_display)
        wl_display_disconnect(state.wl_display);
}

const char *
wayland_name() {
    return "wayland";
}

