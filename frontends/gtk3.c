/*

  Copyright (C) 2023 Ángel Ruiz Fernandez

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, version 3.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program.  If not, see
  <http://www.gnu.org/licenses/>

  x11.c: X11 Xlib frontend

*/

#include <gtk/gtk.h>

#include "common.h"
#include "../game.h"

static const int *board = NULL;
static int size = 0;

static GtkWidget *flaglabel = NULL, *flagimage = NULL;
static GtkWidget **buttons = NULL;

static void
updateButtons() {
    static char buff[256];

    /* Show flags left */
    snprintf(buff, 256, "%d", gameGetFlagsLeft());
    gtk_label_set_label(GTK_LABEL(flaglabel), buff);

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            /* Variables stuff */
            int btni = (x * size) + y;

            /* If clear, hide the button, count surrounding cells and print
                n of mines */
            if (CHECK_CLEAR(BOARDXY(x, y))) {
                gtk_widget_hide(buttons[btni]);
                int n = gameGetSurroundingMines(x, y);

                if (n) {
                    snprintf(buff, 256, "%d", n);

                    /* Set text */
                    //fl_set_object_label(numbers[btni], buff);

                    /* Set color */
                    /*switch (n) {
                        case 1: fl_set_object_lcolor(numbers[btni],
                            FL_BLUE); break;
                        case 2: fl_set_object_lcolor(numbers[btni],
                            FL_GREEN);break;
                        case 3: fl_set_object_lcolor(numbers[btni],
                            FL_RED); break;
                        case 4: fl_set_object_lcolor(numbers[btni],
                            FL_DARKCYAN); break;
                        case 5: fl_set_object_lcolor(numbers[btni],
                            FL_INDIANRED); break;
                        case 6: fl_set_object_lcolor(numbers[btni],
                            FL_CYAN); break;
                        case 7: fl_set_object_lcolor(numbers[btni],
                            FL_BLACK); break;
                        case 8: fl_set_object_lcolor(numbers[btni],
                            FL_INACTIVE); break;
                    }*/

                    /* Show text */
                    //fl_show_object(numbers[btni]);
                }
            }
            /* If not clear, check flag and draw it */
            else if (CHECK_FLAG(BOARDXY(x, y))) {
                //gtk_button_set_label(GTK_BUTTON(buttons[btni]), NULL);
            }
            /* Otherwise just a tile */
            else {
                /* Clear flag if applicable */
                //gtk_button_set_image(GTK_BUTTON(buttons[btni]), NULL);
                //gtk_button_set_label(GTK_BUTTON(buttons[btni]), " ");
            }
        }
    }
}

static gboolean
buttonCallback(GtkWidget *btn, GdkEventButton *event, gpointer data) {
    int btni = *(int*)data;

    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) {
        /* 3 is right mouse btn */
        gameFlagCell(btni / size, btni % size);
    }
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 1) {
        /* 1 is left mouse btn */
        gameClearCell(btni / size, btni % size);
    }

    updateButtons();

    return 0;
}

static void
activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window, *grid, *titlelabel, *buttongrid;

    buttons = malloc(sizeof(GtkWidget*) * size * size);
    int *btnids = malloc(sizeof(int) * size * size); /* Stupid */

    /* Window*/
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), TXT_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    /* Grid layout */
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    /* Title label */
    titlelabel = gtk_label_new(TXT_TITLE);
    gtk_grid_attach(GTK_GRID(grid), titlelabel, 0, 0, 2, 1);

    /* Flag label */
    flaglabel = gtk_label_new("test");
    gtk_grid_attach(GTK_GRID(grid), flaglabel, 1, 1, 1, 1);

    /* Button grid */
    buttongrid = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grid), buttongrid, 0, 2, 2, 1);

    /* Flag image */
    flagimage = gtk_image_new_from_file("../flag.png");

    /* Add buttons to grid */
    int btni = 0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            /* Variables stuff */
            btni = (size * y) + x;

            /* Create button, attach it to the grind,
                and connect click signal to callback*/
            btnids[btni] = btni;
            buttons[btni] = gtk_button_new();
            gtk_grid_attach(GTK_GRID(buttongrid), buttons[btni], x, y, 1, 1);
            g_signal_connect(G_OBJECT(buttons[btni]), "button-press-event",
                G_CALLBACK(buttonCallback), btnids + btni);
            gtk_button_set_image(GTK_BUTTON(buttons[btni]), flagimage);
        }
    }

    updateButtons();

    gtk_widget_show_all(window);
}

int
gtk3Start(const int *lboard, int lsize) {
    board = lboard;
    size = lsize;

    GtkApplication *app;
    int status;

    char *argv[2] = { TXT_TITLE, NULL };
    int argc = 1;

    app = gtk_application_new ("org.gtk." TXT_TITLE,
        G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
}

void
gtk3Destroy() {

}

