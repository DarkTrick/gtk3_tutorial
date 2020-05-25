/**
*  License: CC-0
*  Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
*
**/  

#include "glibx.h"

// ----------------------------------------------------------------------------

// @param type: get the type with `G_OBJECT_TYPE( myobject )`
// @return true,  if type supports the signal
//         false, otherwise
gboolean 
gx_type_get_has_signal(      GType  type, 
                      const gchar *signal_name)
{
  if (g_signal_lookup(signal_name, type) != 0)
    return TRUE;
  else 
    return FALSE;
}

// ----------------------------------------------------------------------------

void
gx_menu_append_with_accel(GMenu          *menu,
                         const gchar    *label,
                         const gchar    *detailed_action,
                         const gchar    *shortcut,
                         GtkApplication *app)
{
  g_menu_append (menu, label , detailed_action);    

  const gchar * const shortcuts[] = { shortcut, NULL };
  gtk_application_set_accels_for_action (app, detailed_action, shortcuts);    
}                            


// ----------------------------------------------------------------------------

/// @brief: creates a submenu, that can run a callback onOpened AND onClosed
/// @param submenu_content: A GMenuModel with menu items of the submenu
///                         E.g. a GMenu
///                         Menu items can (also) be added later
/// @details: Usually a submenu cannot have a `detailed_action`. This
///           function makes a detailed action possible. However,
///           its callback will run on open AND close of the submenu.
GMenuItem *
gx_menu_item_new_submenu_with_action (const char *label, 
                                      const char *detailed_action,
                                      GMenuModel *submenu_content)
{
  GMenuItem * submenu = g_menu_item_new_submenu(label, submenu_content);
  g_menu_item_set_attribute (submenu,"submenu-action", "&s", 
                               detailed_action, NULL);      

  return submenu;
}                


// ----------------------------------------------------------------------------