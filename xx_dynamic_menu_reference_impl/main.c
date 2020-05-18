/*
    License: CC-0 
    Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)
*/
#include <gtk/gtk.h>
#include <stdio.h>
#include "dtwindow.h"

static void 
startup (GApplication *app,
         gpointer      user_data)
{

}         

// ---------------------------------------------------------------------

static void
activate (GApplication *app,
          gpointer      user_data)
{
  GtkWidget *window = dt_window_new (GTK_APPLICATION (app));
  // populate your window here.
  gtk_window_present (GTK_WINDOW (window));
  //gtk_widget_show_all (widget);
}

// ---------------------------------------------------------------------

int main (int argc, char *argv[]) {

  GtkApplication * app = gtk_application_new(NULL,G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);
  g_signal_connect(app, "startup",  G_CALLBACK (startup), NULL);
  

  int status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

}

// ---------------------------------------------------------------------