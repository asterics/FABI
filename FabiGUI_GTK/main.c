#include <stdlib.h>
#include <gtk/gtk.h>

GtkWidget *comboCOM = NULL;

static void about (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;

  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
    "Welcome to the FABI GUI! For further information, please visit either:\nwww.asterics-academy.net/tools\nor\nwww.github.com/asterics/FABI");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

static void connect (GtkWidget *wid, GtkWidget *win)
{
}

int main (int argc, char *argv[])
{
  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL;

  //Statusinfos
  GtkWidget *frameStatus = NULL;
  GtkWidget *fbox = NULL;
  GtkWidget *labelStatus1 = NULL;
  GtkWidget *tableStatus = NULL;
  GtkWidget *labelStatus2 = NULL;
  GtkWidget *labelStatus3 = NULL;
  GtkWidget *labelConnected = NULL;
  GtkWidget *btnStatusConnect = NULL;
  GtkWidget *sw = NULL;
  GtkWidget *list = NULL;

  //Button Actions
  GtkWidget *notebook = NULL;
  GtkWidget *notebookTab1 = NULL;
  GtkWidget *alignAction = NULL;
  GtkWidget *tableActions = NULL;

  //Save/Store part
  GtkWidget *alignSave = NULL;
  GtkWidget *hboxSave = NULL;
  GtkWidget *btnSaveClearAll = NULL;
  GtkWidget *btnSaveApply = NULL;
  GtkWidget *btnSaveStoreAs = NULL;
  GtkWidget *btnSaveLoad = NULL;


  //Menubar
  GtkWidget *menubar = NULL;
  GtkWidget *fileMenu;
  GtkWidget *fileMi;
  GtkWidget *fileSubStoreFile;
  GtkWidget *fileSubLoadFile;
  GtkWidget *aboutMi;
  GtkWidget *aboutMenu;
  GtkWidget *aboutSubAbout;


  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "FABI Settings Manager");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  /* Create menu bar */
  menubar = gtk_menu_bar_new();
  fileMenu = gtk_menu_new();
  fileMi = gtk_menu_item_new_with_label("File");
  fileSubStoreFile = gtk_menu_item_new_with_label("Store settings: FABI->File");
  fileSubLoadFile = gtk_menu_item_new_with_label("Store settings: File->FABI");

    aboutMenu = gtk_menu_new();
    aboutMi = gtk_menu_item_new_with_label("About");
    aboutSubAbout = gtk_menu_item_new_with_label("About");
    g_signal_connect (G_OBJECT (aboutSubAbout), "activate", G_CALLBACK (about), G_OBJECT(win));

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileSubStoreFile);
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileSubLoadFile);

  gtk_menu_item_set_submenu(GTK_MENU_ITEM(aboutMi), aboutMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(aboutMenu), aboutSubAbout);

  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), aboutMi);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), aboutMenu);
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMenu);
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);



  /* Create a horizontal box with buttons/status field */
    //hboxStatus = gtk_hbox_new(TRUE,1);
    //gtk_container_add (GTK_CONTAINER (vbox), hboxStatus);

    /* Status part (Frame -> Table) */
    fbox = gtk_fixed_new();
    frameStatus = gtk_frame_new("Status");
    gtk_fixed_put(GTK_FIXED(fbox),frameStatus,0,0);
    gtk_widget_set_size_request(frameStatus,600,170);
    gtk_container_add(GTK_CONTAINER(vbox), fbox);

    tableStatus = gtk_table_new(4, 7, FALSE);
    gtk_table_set_row_spacings(GTK_TABLE(tableStatus), 1);
    gtk_table_set_col_spacings(GTK_TABLE(tableStatus), 1);
    gtk_container_add(GTK_CONTAINER(frameStatus), tableStatus);

    //Label 1
    labelStatus1 = gtk_label_new("Please select the COM port of your FABI:");
    gtk_label_set_justify(GTK_LABEL(labelStatus1), GTK_JUSTIFY_LEFT);
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),labelStatus1,0,2,0,1);

    //Drop Down "COM Port"
    comboCOM = gtk_combo_box_text_new();
    //TODO: richtige COM ports lesen...
    gtk_combo_box_text_append(GTK_COMBO_BOX(comboCOM), NULL, "COM1");
    gtk_combo_box_text_append(GTK_COMBO_BOX(comboCOM), NULL, "COM2");
    gtk_combo_box_text_append(GTK_COMBO_BOX(comboCOM), NULL, "COM3");
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),comboCOM,0,1,1,2);

    //Button "Connect/Disconnect"
    btnStatusConnect = gtk_button_new_with_label("Connect");
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),btnStatusConnect,1,2,1,2);
    //TODO: richtige Mehtode einbauen...
    g_signal_connect(G_OBJECT(btnStatusConnect), "clicked", G_CALLBACK(connect), G_OBJECT(win));

    //Port status
    labelStatus2 = gtk_label_new("Port Status: ");
    gtk_label_set_justify(GTK_LABEL(labelStatus2), GTK_JUSTIFY_RIGHT);
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),labelStatus2,2,3,0,1);

    labelConnected = gtk_label_new("Not connected");
    //TODO: Farbe..
    gtk_label_set_justify(GTK_LABEL(labelConnected), GTK_JUSTIFY_LEFT);
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),labelConnected,2,3,1,2);

    labelStatus3 = gtk_label_new("Log:");
    gtk_label_set_justify(GTK_LABEL(labelStatus3), GTK_JUSTIFY_LEFT);
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),labelStatus3,3,6,0,1);

    //Log List
    sw = gtk_scrolled_window_new(NULL, NULL);
    list = gtk_tree_view_new();
    gtk_container_add(GTK_CONTAINER(sw), list);
    gtk_table_attach_defaults(GTK_TABLE(tableStatus),sw,3,6,1,3);



    /* Button Actions (Align->Notebook->Table) */
    alignAction = gtk_alignment_new(0, 0, 1, 1);
    tableActions = gtk_table_new(8, 9, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(tableActions), 2);
    gtk_table_set_col_spacings(GTK_TABLE(tableActions), 2);
    notebook = gtk_notebook_new();
    gtk_notebook_append_page(notebook,tableActions,gtk_label_new("Select button press functions"));
    gtk_container_add(GTK_CONTAINER(alignAction), notebook);
    gtk_container_add(GTK_CONTAINER(vbox), alignAction);

    /* Save/Load/Slot Actions */
    alignSave = gtk_alignment_new(0,1,1,0);
    hboxSave = gtk_hbox_new(TRUE,0);
    gtk_container_add(GTK_CONTAINER(alignSave), hboxSave);

    btnSaveApply = gtk_button_new_with_label("Apply Settings");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveApply);

    btnSaveStoreAs = gtk_button_new_with_label("Store Slot as:");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveStoreAs);

    btnSaveLoad = gtk_button_new_with_label("Load Slot");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveLoad);

    btnSaveClearAll = gtk_button_new_with_label("Clear all Slots");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveClearAll);

    gtk_container_add(GTK_CONTAINER(vbox), alignSave);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();
  return 0;
}
