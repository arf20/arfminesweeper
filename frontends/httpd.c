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
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>

#include <pthread.h>

#include "common.h"
#include "httpd.h"
#include "../game.h"

#define BUFF_SIZE 65535

static const int *board = NULL;
static int size = 0;


static const char *indexHeaders =
    "HTTP/1.1 200 OK\n"
    "Server: arfminesweeper httpd\n"
    "Content-Type: text/html\n";

static const char *err404Headers =
    "HTTP/1.1 404 Not Found\n"
    "Server: arfminesweeper httpd\n";

static const char *indexHeadSource =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "    <head>\n"
    "        <title>arfminesweeper</title>\n"
    "        <style>\n"
    "            table, th, td {\n"
    "                /*border: 1px solid black;*/\n"
    "                padding: 0px;\n"
    "                margin: 0px;\n"
    "            }\n"
    "            .cell {\n"
    "                display: inline-block;\n"
    "                width: 20px;\n"
    "                height: 20px;\n"
    "                /*padding: 0px;*/\n"
    "            }\n"
    "        </style>\n"
    "    </head>\n"
    "    <body>\n"
    "        <h1>arfminesweeper</h1>\n"
    "        <p>%d</p>\n"
    "        <hr>\n"
    "        <table>\n";

static const char *indexFooterSource =
    "        </table>\n"
    "    </body>\n"
    "</html>\n";


static char *htmlContent = NULL;
static char *sendBuffer = NULL;



size_t strlcat(char *restrict dst, const char *restrict src, size_t dstsize) {
    int d_len, s_len, offset, src_index;

    /* obtain initial sizes */
    d_len = strlen(dst);
    s_len = strlen(src);

    /* get the end of dst */
    offset = d_len;

    /* append src */
    src_index = 0;
    while(*(src+src_index) != '\0')
    {
        *(dst + offset) = *(src + src_index);
        offset++;
        src_index++;
        /* don't copy more than dstsize characters
           minus one */
        if(offset == dstsize - 1)
            break;
    }
    /* always cap the string! */
    *(dst + offset) = '\0';

    return d_len + s_len;
}

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
    size_t bufflen = strlen(buff) + 1;
    char *ptr = buff;

    while (*ptr) {
        char *p = strchr(ptr, '\n');
        /* walked past last occurrence of needle or run out of buffer */
        if (p == NULL || bufflen + 1 >= size || p >= buff + size) {
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
generateBoardResponse() {
    char tmpBuff[1024];
    snprintf(tmpBuff, 1024, indexHeadSource, gameGetFlagsLeft());
    snprintf(sendBuffer, BUFF_SIZE, "%s\n%s", indexHeaders, tmpBuff);
    for (int y = 0; y < size; y++) {
        strlcat(sendBuffer, "<tr>\n", BUFF_SIZE);
        for (int x = 0; x < size; x++) {
            /* If clear, count surrounding cells and print n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                int n = gameGetSurroundingMines(x, y);
                const char *color = NULL;
                if (n) {
                    switch (n) {
                        case 1: color = "blue"; break;
                        case 2: color = "green"; break;
                        case 3: color = "red"; break;
                        case 4: color = "darkblue"; break;
                        case 5: color = "darkred"; break;
                        case 6: color = "darkcyan"; break;
                        case 7: color = "black"; break;
                        case 8: color = "darkgrey"; break;
                    }
                    snprintf(tmpBuff, 1024, "<td><span style=\"color: %s;\">%d</span></td>\n", color, n);
                    strlcat(sendBuffer, tmpBuff, BUFF_SIZE);
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                snprintf(tmpBuff, 1024, "<td><img src=\"/flag.png\"></td>\n");
                strlcat(sendBuffer, tmpBuff, BUFF_SIZE);
            }
            /* Otherwise just a tile */
            else {
                snprintf(tmpBuff, 1024, "<td><button class=\"cell\"></button></td>\n");
                strlcat(sendBuffer, tmpBuff, BUFF_SIZE);
            }
        }
        strlcat(sendBuffer, "</tr>\n", BUFF_SIZE);
    }
    strlcat(sendBuffer, indexFooterSource, BUFF_SIZE);

    convertcrlf(sendBuffer, BUFF_SIZE);
}

void
generate404Response() {
    snprintf(sendBuffer, BUFF_SIZE, "%s", err404Headers);
    convertcrlf(sendBuffer, BUFF_SIZE);
}

static void*
clientThread(void *data) {
    int cfd = *(int*)data;

    char recvBuff[BUFF_SIZE];
    int r = -1;

    printf("Accepted connection\n");

    /* Recv loop loop */
    while (1) {
        r = recv(cfd, recvBuff, BUFF_SIZE, 0);
        if (r < 0) {
            printf("Error receiving \n");
            return NULL;
        } else if (r == 0) {
            printf("Client exited \n");
            return NULL;
        }

        /*fwrite(recvBuff, 1, r, stdout);*/

        if (strncmp(recvBuff, "GET", 3) == 0) {
            if (strncmp(recvBuff + 4, "/ ", 2) == 0) {
                /* Index Response */
                generateBoardResponse();
                send(cfd, sendBuffer, strlen(sendBuffer), 0);
                printf("Index Response sent\n");
            } else {
                /* 404 Response */
                generate404Response();
                send(cfd, sendBuffer, strlen(sendBuffer), 0);
                printf("Warning: 404 Fot Found\n");
            }
        } else if (strncmp(recvBuff, "POST", 4)) {

        } else {
            printf("Warning: Only GET and POST supported\n");
            return NULL;
        }

        close(cfd);
        printf("Connection closed\n");
        return NULL;
    }
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

int
httpdStart(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

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
