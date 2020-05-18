/** 
 *  License: CC-0 
 *  Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)
 **/

#include "dtentry.h"

// ===================================================================

G_DEFINE_TYPE(DtEntry, dt_entry,  GTK_TYPE_ENTRY)

// ===================================================================
// ===================================================================
// ===================================================================

// -------------------------------------------------------------------

static void
dt_entry_onCopy (GSimpleAction* a, GVariant * b, gpointer dtTreeView){
  g_print("DtEntry::onCopy()\n");
}

// -------------------------------------------------------------------

static void
dt_entry_onPaste (GSimpleAction* a, GVariant * b, gpointer dtTreeView){
  g_print("DtEntry::onPaste()\n");
}

// -------------------------------------------------------------------

static void  
dt_entry_onSpecial (GSimpleAction* a, GVariant * b, gpointer dtTreeView){
  g_print("DtEntry::onSpecial()\n");
}

// -------------------------------------------------------------------

static void *
setupActionsAndAccelerators (DtEntry * self)
{

  // -------- add actions ------
  GActionEntry actions[] = {
    {.name="copy",    .activate=dt_entry_onCopy    },
    {.name="paste",   .activate=dt_entry_onPaste   },
    {.name="special", .activate=dt_entry_onSpecial }
  };

  GSimpleActionGroup * group = g_simple_action_group_new ();
  g_action_map_add_action_entries (G_ACTION_MAP (group), actions, G_N_ELEMENTS (actions), self);
  gtk_widget_insert_action_group (GTK_WIDGET (self), "edit", G_ACTION_GROUP (group));

  // ------ add accelerators ------
  GtkApplication * app = GTK_APPLICATION(g_application_get_default());

  {
    const gchar * const shortcuts[] = { "<Ctrl>C", NULL };
    gtk_application_set_accels_for_action (app, "edit.copy", shortcuts);        
  }
  {
    const gchar * const shortcuts[] = { "<Ctrl>V", NULL };
    gtk_application_set_accels_for_action (app, "edit.paste", shortcuts);    
  }
  {
    const gchar * const shortcuts[] = { "<Ctrl>S", NULL };
    gtk_application_set_accels_for_action (app, "edit.special", shortcuts);    
  }

}

// ===================================================================
// ===================================================================
// ===================================================================

static void 
dt_entry_class_init(DtEntryClass* klass){ }

// ===================================================================

static void 
dt_entry_init(DtEntry* self){  }

// ===================================================================

GtkWidget*
dt_entry_new()
{
  GtkWidget * self = g_object_new (dt_entry_get_type(), NULL);   
  setupActionsAndAccelerators(DT_ENTRY(self));
  return self;
}

// ===================================================================
// ===================================================================
// ===================================================================
