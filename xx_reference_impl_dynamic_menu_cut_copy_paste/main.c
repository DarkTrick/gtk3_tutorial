/*
    License: CC-0 
    Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)
*/
#include <gtk/gtk.h>
#include <stdio.h>
#include "dtwindow.h"

// ---------------------------------------------------------------------

static void
activate (GApplication *app,
          gpointer      user_data)
{
  GtkWidget *window = dt_window_new (GTK_APPLICATION (app));
  gtk_window_present (GTK_WINDOW (window));
}

// ---------------------------------------------------------------------

int main (int argc, char *argv[]) 
{  
  g_log_set_always_fatal (G_LOG_LEVEL_WARNING);  // easier debugging
  g_log_set_always_fatal (G_LOG_LEVEL_CRITICAL); // easier debugging

  GtkApplication * app = gtk_application_new(NULL,G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);  

  int status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

}

// ---------------------------------------------------------------------