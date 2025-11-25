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

*/

#ifndef _WL_UTIL_H
#define _WL_UTIL_H

#include <stdint.h>
#include <stdio.h>

#include <wayland-client.h>
#include <libdecor.h>
#include "wayland-xdg-shell-client-protocol.h"


typedef struct {
   uint32_t event_mask;
   wl_fixed_t surface_x, surface_y;
   uint32_t button, state;
   uint32_t time;
   uint32_t serial;
   struct {
       int valid;
       wl_fixed_t value;
       int32_t discrete;
   } axes[2];
   uint32_t axis_source;
} pointer_event_t;

typedef struct {
    int run;

    /* globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_compositor *wl_compositor;
    struct wl_shm *wl_shm;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_seat *wl_seat;
    /* objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_pointer *wl_pointer;

    int closed;
    pointer_event_t pointer_event;

    /* libdecor */
    enum libdecor_window_state window_state;
    struct libdecor_frame *frame;
} state_t;

enum pointer_event_mask {
   POINTER_EVENT_ENTER = 1 << 0,
   POINTER_EVENT_LEAVE = 1 << 1,
   POINTER_EVENT_MOTION = 1 << 2,
   POINTER_EVENT_BUTTON = 1 << 3,
   POINTER_EVENT_AXIS = 1 << 4,
   POINTER_EVENT_AXIS_SOURCE = 1 << 5,
   POINTER_EVENT_AXIS_STOP = 1 << 6,
   POINTER_EVENT_AXIS_DISCRETE = 1 << 7,
};




void global_registry_remover(void *data, struct wl_registry *registry,
    uint32_t id);
void randname(char *buf);
int create_shm_file(void);
int allocate_shm_file(size_t size);
void wl_buffer_release(void *data, struct wl_buffer *wl_buffer);
void xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base,
    uint32_t serial);
//void global_registry_handler(void *data, struct wl_registry *registry,
//    uint32_t id, const char *interface, uint32_t version);


void wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name);
void wl_pointer_enter(void *data, struct wl_pointer *wl_pointer,
    uint32_t serial, struct wl_surface *surface, wl_fixed_t surface_x,
    wl_fixed_t surface_y);
void wl_pointer_leave(void *data, struct wl_pointer *wl_pointer,
    uint32_t serial, struct wl_surface *surface);
void wl_pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time,
    wl_fixed_t surface_x, wl_fixed_t surface_y);
void wl_pointer_button(void *data, struct wl_pointer *wl_pointer,
    uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
void wl_pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time,
    uint32_t axis, wl_fixed_t value);
void wl_pointer_axis_source(void *data, struct wl_pointer *wl_pointer,
    uint32_t axis_source);
void wl_pointer_axis_stop(void *data, struct wl_pointer *wl_pointer,
    uint32_t time, uint32_t axis);
void wl_pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer,
    uint32_t axis, int32_t discrete);

#endif /* _WL_UTIL_H */

