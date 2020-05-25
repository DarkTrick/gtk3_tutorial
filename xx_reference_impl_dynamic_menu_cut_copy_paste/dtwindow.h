#ifndef __DT_WINDOW_H__
#define __DT_WINDOW_H__

#include <gtk/gtk.h>

G_BEGIN_DECLS

#ifndef CLASS_DEF
#define CLASS_DEF(_classname) typedef struct _##_classname _classname; \
                          struct _##_classname
#endif                          

G_DECLARE_DERIVABLE_TYPE (DtWindow, dt_window, DT, WINDOW, GtkApplicationWindow)

// ------------------------------------
// ---- Define class (meta class) -----
// ----- (e.g. functions/signals) -----
// ------------------------------------

CLASS_DEF(DtWindowClass)
{
  GtkApplicationWindowClass parent;  

};

// --------------------------
// ---- Public functions ----
// --------------------------
GType          dt_window_get_type (void);
GtkWidget*     dt_window_new      (GtkApplication *app);



G_END_DECLS

#endif /* include-protector */