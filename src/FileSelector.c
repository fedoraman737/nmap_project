#include "FileSelector.h"
#include <gtk/gtk.h>
#include <cstdio>

static void on_file_selected(GtkWidget *widget, gint response_id, gpointer user_data) {
    if (response_id == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
        char *filename = gtk_file_chooser_get_filename(chooser);
        FILE *file = fopen("selected_file.txt", "w");
        if (file != nullptr) {
            fprintf(file, "%s\n", filename);
            fclose(file);
        }
        g_free(filename);
    }
    gtk_widget_destroy(widget);
    gtk_main_quit();
}

void select_nmap_file() {
    gtk_init(0, nullptr);

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
                                                    nullptr,
                                                    GTK_FILE_CHOOSER_ACTION_OPEN,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    "_Open", GTK_RESPONSE_ACCEPT,
                                                    NULL);

    g_signal_connect(dialog, "response", G_CALLBACK(on_file_selected), NULL);
    gtk_widget_show_all(dialog);

    gtk_main();
}
