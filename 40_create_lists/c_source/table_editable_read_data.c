/*
    License: CC-0 
    Creator: (DarkTrick - 69f925915ed0193a3b841aeec09451df2326f104)

    This code example shows how to have an editable table, 
    where it's content is printed to the console.
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

// @brief: This function prints out data of the table
void evaluateModel(GtkButton *button,
                  gpointer   treeModel)
{
    GtkTreeModel * model = (GtkTreeModel*)treeModel;

    GtkTreeIter iter;
    if(FALSE == gtk_tree_model_iter_children(model, &iter, NULL)){
        printf("ERROR: getting iter failed\n");
        return;
    }

    gchar *name;
    gint   age;

    printf("-----------\n");
    do{
      // Make sure you terminate calls to gtk_tree_model_get() with a “-1” value
      gtk_tree_model_get (model, &iter,
                          LIST_COL_NAME, &name,
                          LIST_COL_AGE, &age,
                          -1);
      printf("row: %s - %d\n", name, age);
      g_free (name);
      name = NULL;

    } while(TRUE == gtk_tree_model_iter_next(model, &iter));
    printf("-----------\n");
    
}

// ------------------------------------------------------------
/**
* @Structure of onEdited-Calls:
* 
*   onEdited_age()   onEditedName()
*          |            |
*          ↓            ↓
*        onEdited(...,column)
*/

// We need to `define` the function in order to have a dynamic type
//  for parameter `new_value`. (Hey, *you* choose to use C.)
#define \
onEdited(liststore, str_row, int_column, new_value)                 \
{                                                                   \
  /* we need to use GtkListStore, because GtkTreeModel does not */  \
  /*  provide an interface for cell changing. */                    \
  GtkListStore * model = GTK_LIST_STORE(liststore);                 \
                                                                    \
  /* changes can only be made via iterators */                      \
  GtkTreeIter record;                                               \
                                                                    \
  /* retrieve the iterator for the cell, that should be changed */  \
  gtk_tree_model_get_iter_from_string((GtkTreeModel*)model,         \
                                      &record,str_row);             \
                                                                    \
  gtk_list_store_set(model,&record, int_column,new_value,-1);       \
} 

// ------------------------------------------------------------

void 
onEdited_age (GtkCellRendererText * cell,
              gchar * row,
              gchar * new_value,
              gpointer liststore)
{      
  onEdited(liststore,row,LIST_COL_AGE,atoi(new_value));
}

// ------------------------------------------------------------

void 
onEdited_name ( GtkCellRendererText * cell,
                gchar * row,
                gchar * new_value,
                gpointer liststore)
{
  onEdited(liststore,row,LIST_COL_NAME,new_value);
}

// ------------------------------------------------------------



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
                    LIST_COL_AGE, 60, -1);

  // Create data row 3 ( like a database entry )
  gtk_list_store_append(listStore, &rowIter);
  gtk_list_store_set(listStore, &rowIter,
                    LIST_COL_NAME, "BBBB", 
                    LIST_COL_AGE, 80, -1);  
  
  return listStore;
}

// ------------------------------------------------------------

/**
  * @brief: Populate the `window` with gtk elements
  */
void 
populateWindow (GtkWindow *window)
{      
  // ======== create a layout to place the gui elements in ==========
  GtkBox* layout;
  {
    layout = (GtkBox*)gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
    gtk_container_add((GtkContainer*)window, (GtkWidget*)layout);
  }
  
  // ========= create model for the lists =============
  GtkTreeModel * model;        
  model = (GtkTreeModel*)createListModel();        

  // ============== create first treeView ===================
  {  
    GtkTreeView * treeview = (GtkTreeView *) gtk_tree_view_new();
    gtk_box_pack_start(layout, (GtkWidget*)treeview,TRUE,TRUE,0);
  
    // connect model and view
    gtk_tree_view_set_model(treeview,model);       
    
    // Per editable column we need one renderer.

    // setup col 1 - name
    {
      GtkCellRenderer * renderer = (GtkCellRenderer *)gtk_cell_renderer_text_new();
      gtk_tree_view_insert_column_with_attributes(treeview, -1, "Name",
                                renderer, "text", LIST_COL_NAME, NULL);
    
      // make content editable:
      g_object_set(renderer, "editable", TRUE, NULL);    
      g_signal_connect(renderer,"edited",
                      G_CALLBACK(onEdited_name), model);           
    }
    // setup col 2 - age          
    {
      GtkCellRenderer * renderer = (GtkCellRenderer *)gtk_cell_renderer_text_new();  
      gtk_tree_view_insert_column_with_attributes(treeview, -1, "Age", 
                              renderer, "text", LIST_COL_AGE,  NULL);    

      // make content editable:
      g_object_set(renderer, "editable", TRUE, NULL);    
      g_signal_connect(renderer,"edited",
                      G_CALLBACK(onEdited_age), model);           
    }     
  }          

  // ============== insert evaluating button ================
  {
    GtkWidget * button = gtk_button_new_with_label("evaluate model");
    g_signal_connect(button, "clicked", G_CALLBACK(evaluateModel), model);
    gtk_box_pack_start(layout, button,TRUE,TRUE,0);
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
