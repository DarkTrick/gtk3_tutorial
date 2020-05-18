#ifndef __DT_ENTRY_H__
#define __DT_ENTRY_H__

/** 
 *  License: CC-0 
 *  Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)*
 */

#include <gtk/gtk.h>

G_BEGIN_DECLS

#ifndef CLASS_DEF
#define CLASS_DEF(_classname) typedef struct _##_classname _classname; struct _##_classname
#endif                          

#define TYPE_DT_ENTRY dt_entry_get_type ()

// use with only class definition
G_DECLARE_DERIVABLE_TYPE(DtEntry, dt_entry, DT, ENTRY, GtkEntry)

CLASS_DEF(DtEntryClass)
{
  GtkEntryClass parent_class;
};

// --------------------------
// ---- Public functions ----
// --------------------------
GType          dt_entry_get_type (void);
GtkWidget*     dt_entry_new      (void);



G_END_DECLS

#endif /* include-protector */