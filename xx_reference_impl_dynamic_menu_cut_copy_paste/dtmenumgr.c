/**
*  License: CC-0
*  Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
*
*   This is not a class!
*   This file encapsulates all functions related to menu
*    creation and handling. 
*   Reason: Don't pollute the `window` class
*
**/  

#include "dtmenumgr.h"
#include "dtclipboardactions.h"
#include "glibx.h"

// ===================================================================
// ===================================================================

// @brief: States for submenus can be TRUE (indication, that a menu was
//          opened) or FALSE (indication, that a menu was closed). 
//         These defines are to make code more readable
#define MENU_OPENED TRUE
#define MENU_CLOSED FALSE


#define ACTION_GROUP_MENU_EDIT "win_edit"

// -------------------------------------------------------------------

// @brief:     If there's a focused widget, send signal_name to it.
// @contraint: Expects the widget to support the signal.
// @return:    TRUE, if signal was sent
//             FALSE, if signal was not sent
static gboolean
dt_menu_mgr_emit_signal_to_focused_widget( GtkWindow * window, const char * signal_name )
{
  GtkWidget * focusedWidget = gtk_window_get_focus (GTK_WINDOW (window));
  
  // safety net
  if (NULL == focusedWidget ) return FALSE;

  // run action
  g_signal_emit_by_name (focusedWidget, signal_name);
  return TRUE;
}

// -------------------------------------------------------------------

static void
dt_menu_mgr_edit_copy_callback(GSimpleAction* a, GVariant * b, gpointer window){
  g_print("dt_menu_mgr_edit_copy_callback() - emit `copy-clipboard` signal on widget\n");
  dt_menu_mgr_emit_signal_to_focused_widget (GTK_WINDOW (window), "copy-clipboard");  
}

// -------------------------------------------------------------------

static void
dt_menu_mgr_edit_cut_callback(GSimpleAction* a, GVariant * b, gpointer window){
  g_print("dt_menu_mgr_cut_callback() - emit `cut-clipboard` signal on widget\n");
  dt_menu_mgr_emit_signal_to_focused_widget (GTK_WINDOW (window), "cut-clipboard");  
}

// -------------------------------------------------------------------

static void
dt_menu_mgr_edit_paste_callback(GSimpleAction* a, GVariant * b, gpointer window){
  g_print("dt_menu_mgr_paste_callback() - emit `paste-clipboard` signal on widget\n");
  dt_menu_mgr_emit_signal_to_focused_widget (GTK_WINDOW (window), "paste-clipboard");  
} 

// -------------------------------------------------------------------

// @brief:  Insert action group of currently focused widget into the window to
//          connect all menu items.
// @return: TRUE:  a group was added or the menu closed
//          FALSE: there was no action group in the focused widget
static gboolean
dt_menu_mgr_insert_action_group_of_focused_widget_into_window(GtkWindow   * window, 
                                                              const gchar * actionGroupName)
{
  // set action group to NULL to prevent `_get_action_group`
  //  picking up the window's action group
  gtk_widget_insert_action_group (GTK_WIDGET (window), actionGroupName, NULL);
  
  GtkWidget * focused = gtk_window_get_focus (GTK_WINDOW (window));
  
  if(NULL != focused) {
    GActionGroup * group = NULL;
    group = gtk_widget_get_action_group (focused, actionGroupName);

    if (NULL == group) 
      return FALSE;

    gtk_widget_insert_action_group (GTK_WIDGET (window), actionGroupName, group);    
  }
  return TRUE;
}

// -------------------------------------------------------------------

// @brief: updates sensitivity of menu items "cut"/"copy"/"paste"
static void
dt_menu_mgr_update_sensitiveness_cut_copy_paste(GtkWindow * window)
{

  GActionGroup * group = gtk_widget_get_action_group (GTK_WIDGET (window),ACTION_GROUP_MENU_EDIT); 

  // TODO? Keep as references?
  GSimpleAction * action_copy  = G_SIMPLE_ACTION (g_action_map_lookup_action (G_ACTION_MAP (group), "copy" ));
  GSimpleAction * action_cut   = G_SIMPLE_ACTION (g_action_map_lookup_action (G_ACTION_MAP (group), "cut"  ));
  GSimpleAction * action_paste = G_SIMPLE_ACTION (g_action_map_lookup_action (G_ACTION_MAP (group), "paste"));

  GtkWidget * focused = gtk_window_get_focus (GTK_WINDOW (window));
  // if nothing is focused, no action shall be available
  if (NULL == focused) {
    g_simple_action_set_enabled (action_copy,  FALSE);
    g_simple_action_set_enabled (action_cut,   FALSE);
    g_simple_action_set_enabled (action_paste, FALSE);
    return;
    // TODO: maybe use a "default" widget, that is treated as "focused" if nothing is focused
  }  

  // first enable all:
  g_simple_action_set_enabled (action_copy,  TRUE);
  g_simple_action_set_enabled (action_cut,   TRUE);
  g_simple_action_set_enabled (action_paste, TRUE);

  if (DT_IS_CLIPBOARD_ACTIONS (focused))
    {
      g_simple_action_set_enabled (action_copy,  dt_clipboard_actions_is_copy_possible  (DT_CLIPBOARD_ACTIONS (focused)));
      g_simple_action_set_enabled (action_cut,   dt_clipboard_actions_is_cut_possible   (DT_CLIPBOARD_ACTIONS (focused)));
      g_simple_action_set_enabled (action_paste, dt_clipboard_actions_is_paste_possible (DT_CLIPBOARD_ACTIONS (focused)));
    }
  else if (GTK_IS_EDITABLE (focused))
    {
      // maybe disable paste / cut
      guint tmp;
      gboolean selectionAvailable = gtk_editable_get_selection_bounds (GTK_EDITABLE (focused), &tmp, &tmp);
      gboolean editable = gtk_editable_get_editable (GTK_EDITABLE (focused));

      g_simple_action_set_enabled (action_copy,  selectionAvailable);    
      g_simple_action_set_enabled (action_cut,   editable && selectionAvailable);
      g_simple_action_set_enabled (action_paste, editable);      
    } 
  else if (GTK_IS_TEXT_VIEW (focused))
    {
      // disable paste / cut
      gboolean editable = gtk_text_view_get_editable ( GTK_TEXT_VIEW (focused));
      GtkTextBuffer * buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (focused));
      gboolean selectionAvailable = (NULL != buffer && (gtk_text_buffer_get_has_selection (buffer) == TRUE));

      g_simple_action_set_enabled (action_copy,  selectionAvailable);    
      g_simple_action_set_enabled (action_cut,   editable && selectionAvailable);
      g_simple_action_set_enabled (action_paste, editable);      
    }  
  else 
    { // fallback: enable/disable according to signal support only
      
      GType type = G_OBJECT_TYPE (focused);
    
      g_simple_action_set_enabled (action_copy,  gx_type_get_has_signal (type, "copy-clipboard" ));      
      g_simple_action_set_enabled (action_cut,   gx_type_get_has_signal (type, "cut-clipboard"  ));
      g_simple_action_set_enabled (action_paste, gx_type_get_has_signal (type, "paste-clipboard"));
    }

}

// -------------------------------------------------------------------

static void 
dt_menu_mgr_menu_edit_on_open(GtkWindow * window)
{
  //g_print ("<menu opened>\n");    

  // 1. generally enable actions of the focused widget
  dt_menu_mgr_insert_action_group_of_focused_widget_into_window (window, "edit");  
  
  // 2. special sensitivity handling for cut/copy/paste
  dt_menu_mgr_update_sensitiveness_cut_copy_paste (window);  
}

// -------------------------------------------------------------------

static void
dt_menu_mgr_edit_on_opened_or_closed(GSimpleAction *action,
                                     GVariant     *state,
                                     gpointer      window)
{  
  // leave function, if menu was closed
  if (MENU_OPENED == g_variant_get_boolean (state))
    dt_menu_mgr_menu_edit_on_open (window);
  
}

// -------------------------------------------------------------------

/// @brief: Exclusively setup copy/cut/paste, as they are very special
///         items with special treatment
static void
dt_menu_mgr_setup_menu_edit_copy_cut_paste(GtkWindow      *window, 
                                           GtkApplication *app,
                                           GMenu          *edit_content)
{
  // create items:
  gx_menu_append_with_accel (edit_content, "_Copy" ,   ACTION_GROUP_MENU_EDIT ".copy" , "<Ctrl>C", app);    
  gx_menu_append_with_accel (edit_content, "C_ut"  ,   ACTION_GROUP_MENU_EDIT ".cut"  , "<Ctrl>X", app);    
  gx_menu_append_with_accel (edit_content, "_Paste",   ACTION_GROUP_MENU_EDIT ".paste", "<Ctrl>V", app);    

  // create callbacks:
  GActionEntry actions[] = {
      {.name="copy",  .activate=dt_menu_mgr_edit_copy_callback  },
      {.name="cut",   .activate=dt_menu_mgr_edit_cut_callback   },
      {.name="paste", .activate=dt_menu_mgr_edit_paste_callback }
    };
        
  GActionGroup * group = gtk_widget_get_action_group ( GTK_WIDGET (window), ACTION_GROUP_MENU_EDIT);
  g_action_map_add_action_entries (G_ACTION_MAP (group), actions, 
                                   G_N_ELEMENTS (actions), window);
  
}                                 

// -------------------------------------------------------------------

static void
dt_menu_mgr_setup_menu_edit(GtkWindow * window, 
                            GMenu * menubar,
                            GtkApplication * app)
{
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

  // create action group for window
  GSimpleActionGroup * group = g_simple_action_group_new();
  gtk_widget_insert_action_group (GTK_WIDGET (window), ACTION_GROUP_MENU_EDIT, 
                                  G_ACTION_GROUP (group));

  // edit_button opens edit_content on click
  // (necessary to provide a callback for a submenu)
  GMenu * edit_content = g_menu_new();      
  GMenuItem * edit_button;
  {
    edit_button = gx_menu_item_new_submenu_with_action ("_Edit", ACTION_GROUP_MENU_EDIT ".edit", 
                                                        G_MENU_MODEL (edit_content));
    
    // submenus work with `state change` TRUE/FALSE (OPENED / CLOSED) instead of `activate`
    GActionEntry actions[] = {
        {.name="edit", .state="false", 
        .change_state=dt_menu_mgr_edit_on_opened_or_closed}
      };
         
    g_action_map_add_action_entries (G_ACTION_MAP (group), actions,
                                     G_N_ELEMENTS (actions),window);
  }

  dt_menu_mgr_setup_menu_edit_copy_cut_paste (window, app, edit_content); 
  
  // setup other items
  g_menu_append (edit_content, "_Special", "edit.special");  
     
  // add menu to menubar 
  g_menu_append_item (menubar, edit_button);  
}

// -------------------------------------------------------------------

void
dt_menu_mgr_setup_menu(GtkWindow      * window, 
                       GtkApplication * app)
{  
  GMenu * menubar = g_menu_new();                             
  gtk_application_set_menubar (app, G_MENU_MODEL (menubar));  
 
  // build every menu 
  dt_menu_mgr_setup_menu_edit (window, menubar, app);
  
  // why unref? because the documentation also did it
  g_object_unref (menubar);
}
// -------------------------------------------------------------------

void 
dt_menu_mgr_teardown_menu(GtkWindow * window)
{ 
  gtk_widget_insert_action_group (GTK_WIDGET (window), "edit", NULL);
}

// -------------------------------------------------------------------
