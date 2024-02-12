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

    drmfb.c: DRI DRM framebuffer

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <xf86drm.h>
#include <xf86drmMode.h>

#include "drmfb.h"

#define DRM_DEV "/dev/dri/card0"    /* FIXME: use first DRI device */

typedef struct drm_dev_s {
	uint32_t *buf;
	uint32_t conn_id, enc_id, crtc_id, fb_id;
	uint32_t width, height;
	uint32_t pitch, size, handle;
	drmModeModeInfo mode;
	drmModeCrtc *saved_crtc;
	struct drm_dev_s *next;
} drm_dev_t;

static const int *board = NULL;
static int size = 0;

static int wWidth = 0, wHeight = 0;

/* Pixel format */
enum {
	DEPTH = 24,
	BPP = 32,
};

static drm_dev_t *
drm_find_dev(int fd) {
	int i;
	drm_dev_t *dev = NULL, *dev_head = NULL;
	drmModeRes *res;
	drmModeConnector *conn;
	drmModeEncoder *enc;

	if ((res = drmModeGetResources(fd)) == NULL) {
		printf("Error drmModeGetResources: %s\n", strerror(errno));
        return NULL;
    }

	/* find all available connectors */
	for (i = 0; i < res->count_connectors; i++) {
		conn = drmModeGetConnector(fd, res->connectors[i]);

		if (conn != NULL && conn->connection == DRM_MODE_CONNECTED && conn->count_modes > 0) {
			dev = (drm_dev_t*)malloc(sizeof(drm_dev_t));
			memset(dev, 0, sizeof(drm_dev_t));

			dev->conn_id = conn->connector_id;
			dev->enc_id = conn->encoder_id;
			dev->next = NULL;

			memcpy(&dev->mode, &conn->modes[0], sizeof(drmModeModeInfo));
			dev->width = conn->modes[0].hdisplay;
			dev->height = conn->modes[0].vdisplay;

			/* FIXME: use default encoder/crtc pair */
			if ((enc = drmModeGetEncoder(fd, dev->enc_id)) == NULL) {
				printf("Error drmModeGetEncoder: %s\n", strerror(errno));
                return NULL;
            }
			dev->crtc_id = enc->crtc_id;
			drmModeFreeEncoder(enc);

			dev->saved_crtc = NULL;

			/* create dev list */
			dev->next = dev_head;
			dev_head = dev;
		}
		drmModeFreeConnector(conn);
	}
	drmModeFreeResources(res);

	return dev_head;
}

static int
drm_setup_fb(int fd, drm_dev_t *dev) {
	struct drm_mode_create_dumb creq;
	struct drm_mode_map_dumb mreq;
	int ret = 0;

	memset(&creq, 0, sizeof(struct drm_mode_create_dumb));
	creq.width = dev->width;
	creq.height = dev->height;
	creq.bpp = BPP; /* hard conding */

	if (drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq) < 0) {
		printf("Error drmIoctl DRM_IOCTL_MODE_CREATE_DUMB: %s\n", strerror(errno));
        return -1;
    }

	dev->pitch = creq.pitch;
	dev->size = creq.size;
	dev->handle = creq.handle;

	if (drmModeAddFB(fd, dev->width, dev->height,
		DEPTH, BPP, dev->pitch, dev->handle, &dev->fb_id)) {
		printf("Error drmModeAddFB: %s\n", strerror(errno));
        return -1;
    }

	memset(&mreq, 0, sizeof(struct drm_mode_map_dumb));
	mreq.handle = dev->handle;

	if (drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq)) {
		printf("Error drmIoctl DRM_IOCTL_MODE_MAP_DUMB: %s\n", strerror(errno));
        return -1;
    }

	dev->buf = (uint32_t*)mmap(0, dev->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mreq.offset);
    if (dev->buf == MAP_FAILED) {
        printf("Error drmIoctl DRM_IOCTL_MODE_MAP_DUMB: %s\n", strerror(errno));
        return -1;
    }

	dev->saved_crtc = drmModeGetCrtc(fd, dev->crtc_id); /* must store crtc data */
	if (ret = drmModeSetCrtc(fd, dev->crtc_id, dev->fb_id, 0, 0, &dev->conn_id, 1, &dev->mode)) {
		printf("Error drmModeSetCrtc: %s\n", strerror(-ret));
		return -1;
	}

    return 0;
}

int
drmfbStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    /* Open DRM device */
    int drmfd = 0;
    if ((drmfd = open(DRM_DEV, O_RDWR)) < 0) {
		printf("Error opening %s: %s\n", DRM_DEV, strerror(errno));
        return -1;
	}

    /* Set FD_CLOEXEC flag */
    int flags = 0;
	if ((flags = fcntl(drmfd, F_GETFD)) < 0
		|| fcntl(drmfd, F_SETFD, flags | FD_CLOEXEC) < 0) {
		printf("Error fcntl FD_CLOEXEC failed: %s\n", strerror(errno));
        return -1;
    }

    /* Check capability */
    uint64_t has_dumb = 0;
	if (drmGetCap(drmfd, DRM_CAP_DUMB_BUFFER, &has_dumb) < 0 || has_dumb == 0) {
		printf("Error drmGetCap DRM_CAP_DUMB_BUFFER failed or card does not "
        "have dumb buffer: %s\n", strerror(errno));
        return -1;
    }

    drm_dev_t *dev_head = NULL;
    if ((dev_head = drm_find_dev(drmfd)) == NULL) {
		printf("No drm_dev available\n");
		return -1;
	}

    printf("Available connector(s):\n");
	for (drm_dev_t *dev = dev_head; dev != NULL; dev = dev->next) {
		printf("\tconn_id: %d\n", dev->conn_id);
		printf("\t\tenc_id: %d crtc_id: %d fb_id: %d\n", dev->enc_id, dev->crtc_id, dev->fb_id);
		printf("\t\tMode: %dx%d\n", dev->width, dev->height);
	}

    /* FIXME: use first drm_dev */
    drm_dev_t *dev = dev_head;
    if (drm_setup_fb(drmfd, dev)) {
        printf("Failed setting up framebuffer\n");
		return -1;
    }

    
}

void
drmfbDestroy() {

}
