/**
*  License: CC-0
*  Creator: DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104
**/  

#ifndef __DT_MENU_MGR_H__
#define __DT_MENU_MGR_H__

#include <gtk/gtk.h>


// --------------------------
// ---- Public functions ----
// --------------------------
void dt_menu_mgr_setup_menu(GtkWindow * window,
                            GtkApplication * app);
void dt_menu_mgr_teardown_menu(GtkWindow * window);

#endif