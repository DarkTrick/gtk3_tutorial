/**
*  License: CC-0
*  Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
*
*  This file provides convenience functions, that the standard glib 
*   would not provide
*
**/  

#include <gtk/gtk.h>

gboolean gx_type_get_has_signal (      GType  type, 
                                 const gchar *signal_name);


void gx_menu_append_with_accel (      GMenu    *menu,
                                const gchar    *label,
                                const gchar    *detailed_action,
                                const gchar    *shortcut,
                                GtkApplication *app);

GMenuItem * gx_menu_item_new_submenu_with_action (const char *label, 
                                          const char *detailed_action,
                                          GMenuModel *submenu_content);