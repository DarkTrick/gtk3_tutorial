#include <gtk/gtk.h>

#include "dtapp.h"
#include "dtwindow.h"

// ============================================================================

CLASS_DEF(DtApp)
{
  GtkApplication parent;
};
G_DEFINE_TYPE(DtApp, dt_app, GTK_TYPE_APPLICATION);

// ============================================================================
//----------------------------------------------------------------------------

static void
dt_app_activate(GApplication *app)
{
  DtWindow *win = DT_WINDOW (dt_window_new ( GTK_APPLICATION(app)));
  gtk_window_present (GTK_WINDOW (win));
}

//----------------------------------------------------------------------------
//============================================================================

static void
dt_app_class_init(DtAppClass *class)
{
  G_APPLICATION_CLASS (class)->activate = dt_app_activate;
}

//============================================================================

static void
dt_app_init(DtApp *app){ }

//============================================================================

DtApp *
dt_app_new(void)
{
  return g_object_new (dt_app_get_type(),"application-id", 
                       "dt.referenceimpl.appmenu", NULL);
}

//============================================================================