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

static void
activate(GtkApplication* app, gpointer user_data) {
    GtkWidget *window, *grid, *titlelabel, *flaglabel;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), TXT_TITLE);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    titlelabel = gtk_label_new(TXT_TITLE);
    gtk_grid_attach(GTK_GRID(grid), titlelabel, 0, 0, 2, 1);

    flaglabel = gtk_label_new("test");
    gtk_grid_attach(GTK_GRID(grid), flaglabel, 1, 1, 1, 1);

    gtk_widget_show_all(window);
}

int
gtk3Start(const int *lboard, int size) {
    GtkApplication *app;
    int status;

    char *argv[2] = { TXT_TITLE, NULL };
    int argc = 1;

    app = gtk_application_new ("org.gtk." TXT_TITLE,
        G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
}

void
gtk3Destroy() {

}

