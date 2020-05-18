
/**
 *  License: CC-0
 *  Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)1
 **/     

#include "dttreeview.h"
// ===================================================================

CLASS_DEF(DtTreeViewPrivate)
{
  char dummy; // we need at least one elem within our class
};
G_DEFINE_TYPE_WITH_PRIVATE(DtTreeView, dt_tree_view,  GTK_TYPE_TREE_VIEW)

// ===================================================================
// ===================================================================
// ===================================================================

// -------------------------------------------------------------------

static void
dt_tree_view_onCopy (GSimpleAction* a, GVariant * b, gpointer dtTreeView){
  g_print("DtTreeView::onCopy()\n");
}

// -------------------------------------------------------------------

static void
dt_tree_view_onCut (GSimpleAction* a, GVariant * b, gpointer dtTreeView){
  g_print("DtTreeView::onCut()\n");
}

// -------------------------------------------------------------------

static void  
dt_tree_view_onSpecial (GSimpleAction* a, GVariant * b, gpointer dtTreeView){
  g_print("DtTreeView::onSpecial()\n");
}

// -------------------------------------------------------------------

static void *
setupActionsAndAccelerators (DtTreeView * self)
{

  GActionEntry actions[] = {
    {.name="copy",    .activate=dt_tree_view_onCopy   },
    {.name="cut",     .activate=dt_tree_view_onCut    },
    {.name="special", .activate=dt_tree_view_onSpecial}
  };

  GSimpleActionGroup * group = g_simple_action_group_new ();
  g_action_map_add_action_entries (G_ACTION_MAP (group), actions, G_N_ELEMENTS (actions), self);
  gtk_widget_insert_action_group (GTK_WIDGET (self), "edit", G_ACTION_GROUP (group));

  // ---- add (local) accelerators ----:
  GtkApplication * app = GTK_APPLICATION(g_application_get_default());
  {
    const gchar * const shortcuts[] = { "<Ctrl>C", NULL };
    gtk_application_set_accels_for_action (app, "edit.copy", shortcuts);    
  }
  {
    const gchar * const shortcuts[] = { "<Ctrl>X", NULL };
    gtk_application_set_accels_for_action (app, "edit.cut", shortcuts);    
  }
  {
    const gchar * const shortcuts[] = { "<Ctrl>S", NULL };
    gtk_application_set_accels_for_action (app, "edit.special", shortcuts);    
  }
  
}
// -------------------------------------------------------------------


// -------------------------------------------------------------------

// ===================================================================
// ===================================================================
// ===================================================================

// @brief: init class stuff like methods
static void 
dt_tree_view_class_init (DtTreeViewClass* klass)
{
}

// ===================================================================

static void 
dt_tree_view_init (DtTreeView* self)
{  
  // init variables  
  setupActionsAndAccelerators(DT_TREE_VIEW(self));
}

// ===================================================================

GtkWidget*
dt_tree_view_new ()
{
  GtkWidget * self = g_object_new (dt_tree_view_get_type(), NULL);   
  
  // Setup treeview to make it shown on screen. 
  //  Nothing important for the reference impl itself
  {
    // Add some data
    {
      GtkListStore * model = gtk_list_store_new(2,G_TYPE_STRING, G_TYPE_STRING);
      
      GtkTreeIter rowIter;
      gtk_list_store_append(model, &rowIter);
      gtk_list_store_set(model, &rowIter,0, "AAAA", 1, "foo1", -1);
      gtk_list_store_append(model, &rowIter);
      gtk_list_store_set(model, &rowIter,0, "CCCC", 1, "foo2", -1);
      gtk_list_store_append(model, &rowIter);
      gtk_list_store_set(model, &rowIter,0, "BBBB", 1, "foo3", -1);

      gtk_tree_view_set_model(GTK_TREE_VIEW(self),GTK_TREE_MODEL(model));
    }
    // setup view
    {
      GtkCellRenderer * renderer = (GtkCellRenderer *)gtk_cell_renderer_text_new();  
      // setup col 1    
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(self), -1, "Name",
                              renderer, "text", 0, NULL);
      // setup col 2
      gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(self), -1, "Age", 
                              renderer, "text", 1,  NULL);    
    }
  }


  return self;
}

// ===================================================================