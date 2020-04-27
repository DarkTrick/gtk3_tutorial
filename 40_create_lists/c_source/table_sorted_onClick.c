/*
    License: CC-0 
    Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)
*/

#include <gtk/gtk.h>
#include <stdio.h>

// ------------------------------------------------------------

enum EListCols{
  LIST_COL_NAME = 0,
  LIST_COL_AGE,
  LIST_MAX_COLS
};

// ------------------------------------------------------------

/**
 * @brief: This function will create a new data model ('data store').
 * @return: Newly created model (GtkListStore*)
 */
GtkListStore * 
createListModel (void)
{
  GtkListStore *listStore;
  listStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_UINT);
 
  GtkTreeIter rowIter;

  // Info: the below could be done in a loop with an array, but
  //      this way is easier to grasp.

  // Create data row 1 ( like a database entry )
  gtk_list_store_append(listStore, &rowIter);
  gtk_list_store_set(listStore, &rowIter,
                    LIST_COL_NAME, "AAAA", 
                    LIST_COL_AGE, 100, -1);

  // Create data row 2 ( like a database entry )
  gtk_list_store_append(listStore, &rowIter);
  gtk_list_store_set(listStore, &rowIter,
                    LIST_COL_NAME, "CCCC", 
                    LIST_COL_AGE, 80, -1);

  // Create data row 3 ( like a database entry )
  gtk_list_store_append(listStore, &rowIter);
  gtk_list_store_set(listStore, &rowIter,
                    LIST_COL_NAME, "BBBB", 
                    LIST_COL_AGE, 60, -1);  
  
  return listStore;
}

// ------------------------------------------------------------

// @brief: adds a column to a tree view and make it sortable
//          - all at once
void 
gtk_tree_view_append_column_sortable (
                                      GtkTreeView * treeview,
                                      GtkCellRenderer * renderer,
                                      const gchar * type,
                                      gint columnId,
                                      const gchar * columnTitle)
{ 
  // create a column and append it to the tree view
  GtkTreeViewColumn * col;
  col = gtk_tree_view_column_new_with_attributes(columnTitle,renderer,type, columnId,NULL);
  gtk_tree_view_append_column(treeview,col);
  
  // make column sortable on click:
  gtk_tree_view_column_set_sort_column_id(col,columnId);
}                                  

// ------------------------------------------------------------

/**
 * @brief: Populate the `window` with gtk elements
 */
void 
populateWindow (GtkWindow *window){

  // ======== create a layout to place the gui elements in ==========
  GtkBox* layout;
  {
    layout = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    gtk_container_add((GtkContainer*)window, (GtkWidget*)layout);
  }
  
  // ========= create model for the list =============
  GtkTreeModel * model;
  model = (GtkTreeModel*)createListModel();

  // ========= create sortmodel from model =============
  GtkTreeModel * sortmodel;
  {
    sortmodel = gtk_tree_model_sort_new_with_model(model);   
  }

  // ============== create first treeView ===================
  {  
    GtkTreeView * treeview = (GtkTreeView *) gtk_tree_view_new();
    gtk_box_pack_start(layout, (GtkWidget*)treeview,TRUE,TRUE,0);
    // connect model and view
    gtk_tree_view_set_model(treeview,model);       

    GtkCellRenderer * renderer = (GtkCellRenderer *)gtk_cell_renderer_text_new();  
    
    // ---- setup columns -----
    gtk_tree_view_append_column_sortable(treeview,renderer,"text",LIST_COL_NAME,"Name");
    gtk_tree_view_append_column_sortable(treeview,renderer,"text",LIST_COL_AGE,"Age");                      

  }   
 
}

// ------------------------------------------------------------
/**
 * @brief: Do basic window setup and start real population of the window.
 *         `activate` gets called, when the app is 
 *         called without any file arguments
*/
static void
activate (GApplication *app,
          gpointer      user_data)
{
  GtkWindow *window = (GtkWindow*)gtk_application_window_new (GTK_APPLICATION (app));
  gtk_window_set_default_size (window, 200, 200);
  gtk_window_set_position     (window, GTK_WIN_POS_CENTER);

  populateWindow(window);  

  gtk_widget_show_all ((GtkWidget*)window);
}
// ------------------------------------------------------------
/**
 * @brief: This function does the basic calls to setup a window.
*/
int 
main (int argc, char *argv[]) 
{
  // always use a GtkApplication. Otherwise advanced features might 
  //  "magically" not work for you.
  GtkApplication * app = gtk_application_new(NULL,G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK (activate), NULL);


  int status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}

// ------------------------------------------------------------
