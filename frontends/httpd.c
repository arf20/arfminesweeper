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

    httpd.c: HTTP webapp frontend

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <pthread.h>

#include "common.h"
#include "httpd.h"
#include "../game.h"

#define SEND_BUFF_SIZE 65535

static const char *indexHeaders =
    "HTTP/1.1 200 OK\n"
    "Server: arftracksat httpd\n"
    "Content-Type: text/html\n";

static char *htmlContent = NULL;

static char *sendBuffer = NULL;

int
strlencrlf(const char *str) {
    const char *ptr = str;
    int size = 0;
    while (*ptr != '\0') {
        if (*ptr == '\n') size++;
        size++;
        ptr++;
    }
    return size;
}

static void
convertcrlf(char *buff, size_t size) {
    size_t bufflen = strlen(buff);
    char *ptr = buff;

    while (1) {
        char *p = strchr(ptr, '\n');
        /* walked past last occurrence of needle or run out of buffer */
        if (p == NULL || bufflen + 1 >= size) {
            break;
        }

        /* move part after lf */
        memmove(p + 1, p, bufflen - (p - buff));

        /* insert cr */
        *p = '\r';

        bufflen++;
        ptr = p + 2;
    }
}

void
generateIndexResponse() {
    snprintf(sendBuffer, SEND_BUFF_SIZE, "%sContent-Length: %d\n\n%s\n", indexHeaders, strlencrlf(htmlContent), htmlContent);
    convertcrlf(sendBuffer, SEND_BUFF_SIZE);
}

static int
httpdListen(unsigned short port) {
    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = 0;

    int fd = -1;
    /* Create socket */
    if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        return -1;
    }

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) {
        return -1;
    }

    /* Bind socket */
    if (bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
        printf("Error binding socket\n");
        return -1;
    }

    /* Listen on socket */
    if (listen(fd, SOMAXCONN) < 0) {
        return -1;
    }

    return fd;
}

static void*
clientThread(void *data) {
    int cfd = *(int*)data;

    size_t bufflen = 4096;
    char buff[4096];
    int r = -1;

    printf("Accepted client\n");

    /* Recv loop loop */
    while (1) {
        r = recv(cfd, buff, bufflen, 0);
        if (r < 0) {
            printf("Error receiving \n");
            return NULL;
        } else if (r == 0) {
            printf("Client exited \n");
            return NULL;
        }

        fwrite(buff, 1, r, stdout);

        generateIndexResponse();
        send(cfd, sendBuffer, strlen(sendBuffer), 0);
    }
}

int
httpdStart(const int *lboard, int lsize) {
    /* Read HTML file */
    FILE *htmlFile = fopen("../assets/msboard.html", "r");
    if (htmlFile == NULL) {
        printf("Error opening file: %s\n", strerror(errno));
    }
    fseek(htmlFile, 0L, SEEK_END);
    size_t htmlContentSize = ftell(htmlFile);
    fseek(htmlFile, 0L, SEEK_SET);
    htmlContent = malloc(htmlContentSize);
    fread(htmlContent, 1, htmlContentSize, htmlFile);

    sendBuffer = malloc(65536);

    int lfd = httpdListen(8080);
    if (lfd < 0)
        return -1;

    printf("Listening on 0.0.0.0:8080\n");

    pthread_t cthread;
    int cfd = -1;

    /* Accept loop */
    while (1) {
        cfd = accept(lfd, NULL, NULL);
        if (cfd < 0) {
            printf("Error accepting client\n");
            continue;
        }

        pthread_create(&cthread, NULL, clientThread, &cfd);
        pthread_detach(cthread);
    }

    return 0;
}

void
httpdDestroy() {

}
