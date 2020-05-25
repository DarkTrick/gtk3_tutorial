
#ifndef __DT_TREE_VIEW_H__
#define __DT_TREE_VIEW_H__


/**
 *  License: CC-0
 *  Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)1
 **/     


#include <gtk/gtk.h>

G_BEGIN_DECLS

#ifndef CLASS_DEF
#define CLASS_DEF(_classname) typedef struct _##_classname _classname; struct _##_classname
#endif                          

#define TYPE_DT_TREE_VIEW dt_tree_view_get_type ()

G_DECLARE_DERIVABLE_TYPE(DtTreeView, dt_tree_view, DT, TREE_VIEW, GtkTreeView)
CLASS_DEF(DtTreeViewClass)
{
  GtkTreeViewClass parent_class;
};

// --------------------------
// ---- Public functions ----
// --------------------------
GType          dt_tree_view_get_type (void);
GtkWidget*     dt_tree_view_new (void);

G_END_DECLS

#endif /* include-protector */