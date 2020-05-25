#include "dtwindow.h"
#include "dttreeview.h"
#include "dtentry.h"
#include "dtclipboardactions.h"
#include "dtmenumgr.h"

// ===================================================================

G_DEFINE_TYPE(DtWindow, dt_window,  GTK_TYPE_APPLICATION_WINDOW)

// ===================================================================


// -------------------------------------------------------------------

static void 
setupWindowContents(DtWindow * self, GtkApplication * app)
{
  GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_container_add(GTK_CONTAINER(self), vbox);  

  // random button
  gtk_container_add(GTK_CONTAINER(vbox), gtk_button_new_with_label("Button1")); 
  
  // insert DtEntry: 
  gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(dt_entry_new()));
  
  // insert: DtTreeView:
  gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(dt_tree_view_new()));
  
  // insert GtkTextView 
  gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(gtk_text_view_new()));
  
  // insert unwritable GtkTextView
  GtkWidget * w = gtk_text_view_new();
  gtk_text_view_set_editable ((GtkTextView*)w, FALSE);
  gtk_container_add(GTK_CONTAINER(vbox), w);


  // don't forget to show everything
  gtk_widget_show_all(GTK_WIDGET(self));
}

// -------------------------------------------------------------------

// ===================================================================

static void
dt_window_destructor (GObject *self)
{
  dt_menu_mgr_teardown_menu (GTK_WINDOW (self));

  // chain up:
  G_OBJECT_CLASS (dt_window_parent_class)->finalize (self);
}

// ===================================================================

// @brief: init class stuff like methods
static void 
dt_window_class_init(DtWindowClass* klass) 
{  
  // enable destructor
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = dt_window_destructor;
}

// ===================================================================

static void 
dt_window_init(DtWindow* self) {     
  // priorize local key events over window key events
  g_signal_connect ( self, "key-press-event", G_CALLBACK (gtk_window_propagate_key_event), NULL);
}

// ===================================================================

GtkWidget*
dt_window_new(GtkApplication *app)
{
  GtkWidget * self = g_object_new (dt_window_get_type(), "application", app, NULL); 
    
  //GtkApplication * app = GTK_APPLICATION(g_application_get_default());
  // create menu
  dt_menu_mgr_setup_menu (GTK_WINDOW (self), app);

  // create window content
  setupWindowContents (DT_WINDOW(self), app); 

  return self;
}

// ===================================================================
