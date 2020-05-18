#include "dtwindow.h"
#include "dttreeview.h"
#include "dtentry.h"

// ===================================================================

CLASS_DEF(DtWindowPrivate)
{
  // "Menu-click" callback fires for every open and close
  //  use this to only process every second call
  gboolean menu_opened; 
};
G_DEFINE_TYPE_WITH_PRIVATE(DtWindow, dt_window,  GTK_TYPE_APPLICATION_WINDOW)

// ===================================================================

// -------------------------------------------------------------------

static void
dt_window_build_menu_on_click(DtWindow * self, const gchar * actionGroupName)
{
  // leave function, if menu was closed  
  {
    DtWindowPrivate * priv = dt_window_get_instance_private (self);
    priv->menu_opened = !(priv->menu_opened);
    if (FALSE == priv->menu_opened)
        return;
  }

  g_print ("menu opened\n");    

  // set action group to NULL to prevent _get_action_group
  //  picking up the windows action group
  gtk_widget_insert_action_group (GTK_WIDGET (self), actionGroupName, NULL);

  // map local copy/cut/.. events to menu
  GtkWidget * focused = gtk_window_get_focus (GTK_WINDOW (self));
  
  if(NULL != focused) {
    GActionGroup * actionGroup = NULL;
    actionGroup = gtk_widget_get_action_group (focused, actionGroupName);

    gtk_widget_insert_action_group (GTK_WIDGET (self), actionGroupName, actionGroup);
  }
  
}

// -------------------------------------------------------------------

static void
dt_window_menu_edit_on_click(GSimpleAction *action,
                             GVariant     *state,
                             gpointer      self)
{  
 dt_window_build_menu_on_click(DT_WINDOW(self), "edit");  
}

// -------------------------------------------------------------------

static void
dt_window_create_menu(GtkWindow * self, GtkApplication * app){
  /**
   *
   *   Layout              Object structure
   *   ------               ----------------                
   *    ______               
   *   | Edit |            GMenu (menubar)  
   *   |______|__            |     
   *   | Copy    |           |-- GMenuItem ("Edit" (`edit_item`))
   *   |_________|                |     
   *   | Cut     |                |-- GMenu (`edit_content`)     
   *   |_________|                     |-- GMenuItem ("Copy") 
   *   | Paste   |                     |
   *   |_________|                     |-- GMenuItem ("Cut")  
   *   | Special |                     |
   *   |_________|                     |-- GMenuItem ("Paste")       
   *                                   |
   *                                   |-- GMenuItem ("Special")     
   * 
   */
  // Remark: I use GMenu instead of GtkMenu, because GtkMenu is not going
  //         to be there in GTK4

  // Create content for the "Edit" menu
  GMenu * edit_content = g_menu_new();
  {
    g_menu_append (edit_content, "_Copy" ,   "edit.copy"   );    
    g_menu_append (edit_content, "C_ut"  ,   "edit.cut"    );    
    g_menu_append (edit_content, "_Paste",   "edit.paste"  );    
    g_menu_append (edit_content, "_Special", "edit.special");    
  }
  
  // create the "Edit" menu button and add "on-click" event
  GMenuItem * edit_button;
  {
    edit_button = g_menu_item_new ("_Edit", "asdf"); // (action will be discarded later)
    // set callback
    g_menu_item_set_attribute (edit_button,"submenu-action", "&s", 
                               "app.menu_edit_clicked", NULL);      
    // connect button and content:
    g_menu_item_set_submenu (edit_button, G_MENU_MODEL (edit_content));            

    // setup on-click
    GActionEntry actions[] = {
      {.name="menu_edit_clicked", .state="false", 
       .parameter_type="s", .change_state=dt_window_menu_edit_on_click}};
    g_action_map_add_action_entries (G_ACTION_MAP (app), actions,
                                     G_N_ELEMENTS (actions),self);
  }

  
  GMenu * menubar = g_menu_new();                             // create menu bar   
  g_menu_append_item (menubar, edit_button);                  // add "Edit" menu 
  gtk_application_set_menubar (app, G_MENU_MODEL (menubar));  // set it in the application

  g_object_unref (menubar);
}

// -------------------------------------------------------------------

static void 
setupWindowContents(DtWindow * self, GtkApplication * app)
{
  GtkWidget * vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
  gtk_container_add(GTK_CONTAINER(self), vbox);  

  // menu
  dt_window_create_menu(GTK_WINDOW(self), app);

  // random button
  gtk_container_add(GTK_CONTAINER(vbox), gtk_button_new_with_label("Button1")); 
  
  // insert DtEntry: supports "edit.copy", "edit.paste", "edit.special"
  gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(dt_entry_new()));
  
  // insert: DtTreeView: supports "edit.copy, "edit.cut", "edit.special"
  gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(dt_tree_view_new()));
  
  // insert GtkTextView 
  gtk_container_add(GTK_CONTAINER(vbox), GTK_WIDGET(gtk_text_view_new()));
  //    Remark: GtkTextView supports copy/cut/paste. But not as GActionGroup elements.
  //            therefore it's like it doesn't support the actions.
  //            Also making GtkTextView available on click in the menu is
  //            handled in another example.

  // don't forget to show everything
  gtk_widget_show_all(GTK_WIDGET(self));
}

// -------------------------------------------------------------------


// ===================================================================

// @brief: init class stuff like methods
static void 
dt_window_class_init(DtWindowClass* klass)
{

}

// ===================================================================

static void 
dt_window_init(DtWindow* self)
{  
  DtWindowPrivate * priv = dt_window_get_instance_private(self);
  priv->menu_opened = FALSE;
  // init variables
}

// ===================================================================

GtkWidget*
dt_window_new(GtkApplication *app)
{
  GtkWidget * self = g_object_new (dt_window_get_type(), "application", app, NULL); 
  
  setupWindowContents (DT_WINDOW(self), app);

  return self;
}

// ===================================================================
