#ifndef __DTAPP_H
#define __DTAPP_H

#include <gtk/gtk.h>

#ifndef CLASS_DEF
#define CLASS_DEF(_classname) typedef struct _##_classname _classname; \
                          struct _##_classname
#endif                          

G_DECLARE_FINAL_TYPE (DtApp, dt_app, DT, APP, GtkApplication)

// --------------------------
// ---- Public functions ----
// --------------------------
DtApp     *dt_app_new         (void);



#endif /* __DTAPP_H */
