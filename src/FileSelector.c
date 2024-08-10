#include "FileSelector.h"
#include <gtk/gtk.h>
#include <cstdio>
#include <cstring>

static GtkWidget *file_label;
static GtkWidget *launch_button;

static void on_file_selected(GtkWidget *widget, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
        char *filename = gtk_file_chooser_get_filename(chooser);
        FILE *file = fopen("selected_file.txt", "w");
        if (file != nullptr) {
            fprintf(file, "%s\n", filename);
            fclose(file);
        }
        gchar *display_name = g_filename_display_basename(filename);
        gtk_label_set_text(GTK_LABEL(file_label), display_name);
        g_free(display_name);
        g_free(filename);
        gtk_widget_set_sensitive(launch_button, TRUE);
    }
    gtk_widget_destroy(widget);
    if (gtk_main_level() > 0) {
        gtk_main_quit();
    }
}

static void on_open_file_button_clicked(GtkWidget *button, gpointer user_data) {
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    GTK_WINDOW(user_data),
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    g_signal_connect(dialog, "response", G_CALLBACK(on_file_selected), NULL);
    gtk_widget_show_all(dialog);
}

static void on_remove_file_button_clicked(GtkWidget *button, gpointer user_data) {
    gtk_label_set_text(GTK_LABEL(file_label), "No file selected");
    gtk_widget_set_sensitive(launch_button, FALSE);
    remove("selected_file.txt");
}

static void on_launch_button_clicked(GtkWidget *button, gpointer user_data) {
    gtk_widget_destroy(GTK_WIDGET(user_data));
    if (gtk_main_level() > 0) {
        gtk_main_quit();
    }
}

static void on_window_destroy(GtkWidget *widget, gpointer user_data) {
    if (gtk_main_level() > 0) {
        gtk_main_quit();
    }
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *open_file_button;
    GtkWidget *remove_file_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "NMAP Project");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    gtk_container_add(GTK_CONTAINER(window), grid);

    label = gtk_label_new("Welcome to the NMAP Project!\nClick the button below to upload an NMAP file.");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 2, 1);

    open_file_button = gtk_button_new_with_label("Open NMAP File");
    g_signal_connect(open_file_button, "clicked", G_CALLBACK(on_open_file_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), open_file_button, 0, 1, 1, 1);

    remove_file_button = gtk_button_new_with_label("Remove File");
    g_signal_connect(remove_file_button, "clicked", G_CALLBACK(on_remove_file_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), remove_file_button, 1, 1, 1, 1);

    file_label = gtk_label_new("No file selected");
    gtk_grid_attach(GTK_GRID(grid), file_label, 0, 2, 2, 1);

    launch_button = gtk_button_new_with_label("Launch");
    gtk_widget_set_sensitive(launch_button, FALSE);
    g_signal_connect(launch_button, "clicked", G_CALLBACK(on_launch_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), launch_button, 0, 3, 2, 1);

    gtk_widget_show_all(window);
}

void select_nmap_file() {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.nmapproject", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, nullptr);
    g_object_unref(app);
}
