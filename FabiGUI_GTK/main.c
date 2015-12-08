#include "main.h"


GtkListStore *logStore = NULL;
GtkTreeIter logIter;
GtkWidget *labelConnected = NULL;
GtkWidget *btnStatusConnect = NULL;
char currentCOMPort[512];
int isConnected = 0;


int main (int argc, char *argv[])
{
    GtkWidget *win = NULL;
    GtkWidget *vbox = NULL;

    /* Initialize GTK+ */
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
    gtk_init (&argc, &argv);
    g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

    /* Create the main window */
    win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_set_border_width (GTK_CONTAINER (win), 8);
    gtk_window_set_title (GTK_WINDOW (win), "FABI Settings Manager");
    gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
    gtk_widget_realize (win);
    g_signal_connect (win, "destroy", gtk_main_quit, NULL);
    gtk_container_add (GTK_CONTAINER (win), vbox);

    //Create the menubar
    createGUIMenubar(vbox,win);

    //Create the status GUI part
    createGUIStatus(vbox,win);

    //Create the action GUI part
    createGUIActions(vbox,win);

    //Create the buttons for Loading/Storing
    createGUILoadStore(vbox,win);

    //Create the log GUI part
    createGUILog(vbox,win);

    /* Enter the main loop */
    gtk_widget_show_all (win);
    gtk_main ();
    return 0;
}

//Build the menu bar
static void createGUIMenubar(GtkWidget *mainBox, GtkWidget *win)
{
    //Menubar
    GtkWidget *menubar = NULL;
    GtkWidget *fileMenu;
    GtkWidget *fileMi;
    GtkWidget *fileSubStoreFile;
    GtkWidget *fileSubLoadFile;
    GtkWidget *aboutMi;
    GtkWidget *aboutMenu;
    GtkWidget *aboutSubAbout;

    /*Create menu bar */
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
    /*gtk_menu_shell_append(GTK_MENU_SHELL(menubar), aboutMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMenu);*/
    gtk_box_pack_start(GTK_BOX(mainBox), menubar, FALSE, FALSE, 0);
}

//Build the status part of the main GUI
static void createGUIStatus(GtkWidget *mainBox, GtkWidget *win)
{
        //Statusinfos
    GtkWidget *frameStatus = NULL;
    GtkWidget *labelStatus1 = NULL;
    GtkWidget *tableStatus = NULL;
    GtkWidget *labelStatus2 = NULL;
    GtkWidget *comboCOM = NULL;

    tableStatus = gtk_grid_new();

    //Label 1
    labelStatus1 = gtk_label_new("Please select the COM port of your FABI:");
    gtk_label_set_justify(GTK_LABEL(labelStatus1), GTK_JUSTIFY_LEFT);
    gtk_grid_attach(GTK_GRID(tableStatus),labelStatus1,0,0,2,1);

    //Drop Down "COM Port"
    comboCOM = gtk_combo_box_text_new();
    char comlist[512] = "";
    listComPorts(comlist);

    char *token; //token of different COM port names
    char *dup = strdup(comlist); //duplicate, to own the memory (SEGFAULT otherwise...)
    while ((token = strsep(&dup, ";")) != NULL)
    {
        if(strcmp(token,"") != 0)
        {
            gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboCOM), NULL, (gchar*)token);
        }
    }
    free(dup);
    free(token);

    g_signal_connect(G_OBJECT(comboCOM), "changed",G_CALLBACK(combo_selected), G_OBJECT(win));

    gtk_grid_attach(GTK_GRID(tableStatus),comboCOM,0,1,1,1);

    //Button "Connect/Disconnect"
    btnStatusConnect = gtk_button_new_with_label("Connect");
    gtk_grid_attach(GTK_GRID(tableStatus),btnStatusConnect,1,1,1,1);
    g_signal_connect(G_OBJECT(btnStatusConnect), "clicked", G_CALLBACK(connect), G_OBJECT(win));

    //Port status
    labelStatus2 = gtk_label_new("Port Status: ");
    gtk_label_set_justify(GTK_LABEL(labelStatus2), GTK_JUSTIFY_RIGHT);
    gtk_grid_attach(GTK_GRID(tableStatus),labelStatus2,2,0,1,1);

    labelConnected = gtk_label_new("Not connected");
    gtk_label_set_justify(GTK_LABEL(labelConnected), GTK_JUSTIFY_LEFT);
    gtk_grid_attach(GTK_GRID(tableStatus),labelConnected,2,1,1,1);

    frameStatus = gtk_frame_new("Status");
    gtk_container_add(GTK_CONTAINER(frameStatus), tableStatus);
    gtk_container_add(GTK_CONTAINER(mainBox), frameStatus);
}
//Build the button action notebook
static void createGUIActions(GtkWidget *mainBox, GtkWidget *win)
{
    //Button Actions
    GtkWidget *notebook = NULL;
    GtkWidget *tableActions = NULL;

    GtkWidget *labelB1 = gtk_label_new("Button 1");
    GtkWidget *labelB2 = gtk_label_new("Button 2");
    GtkWidget *labelB3 = gtk_label_new("Button 3");
    GtkWidget *labelB4 = gtk_label_new("Button 4");
    GtkWidget *labelB5 = gtk_label_new("Button 5");
    GtkWidget *labelB6 = gtk_label_new("Button 6");

    GtkWidget *dropB1 = gtk_combo_box_text_new();
    GtkWidget *dropB2 = gtk_combo_box_text_new();
    GtkWidget *dropB3 = gtk_combo_box_text_new();
    GtkWidget *dropB4 = gtk_combo_box_text_new();
    GtkWidget *dropB5 = gtk_combo_box_text_new();
    GtkWidget *dropB6 = gtk_combo_box_text_new();

    //Build grid
    tableActions = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(tableActions),TRUE);

    //build each drop down menu
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"No action");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Switch to next configuration");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Click right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Click middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Double click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Hold left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Hold right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Hold middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Mouse wheel up");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Mouse wheel down");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Mouse move X");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Mouse move Y");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Write text");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB1),"Press keys");
    g_signal_connect(G_OBJECT(dropB1), "changed",G_CALLBACK(comboAction1_selected), G_OBJECT(win));

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"No action");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Switch to next configuration");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Click right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Click middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Double click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Hold left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Hold right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Hold middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Mouse wheel up");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Mouse wheel down");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Mouse move X");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Mouse move Y");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Write text");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB2),"Press keys");
    g_signal_connect(G_OBJECT(dropB2), "changed",G_CALLBACK(comboAction2_selected), G_OBJECT(win));

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"No action");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Switch to next configuration");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Click right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Click middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Double click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Hold left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Hold right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Hold middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Mouse wheel up");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Mouse wheel down");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Mouse move X");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Mouse move Y");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Write text");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB3),"Press keys");
    g_signal_connect(G_OBJECT(dropB3), "changed",G_CALLBACK(comboAction3_selected), G_OBJECT(win));

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"No action");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Switch to next configuration");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Click right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Click middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Double click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Hold left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Hold right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Hold middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Mouse wheel up");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Mouse wheel down");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Mouse move X");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Mouse move Y");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Write text");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB4),"Press keys");
    g_signal_connect(G_OBJECT(dropB4), "changed",G_CALLBACK(comboAction4_selected), G_OBJECT(win));

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"No action");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Switch to next configuration");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Click right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Click middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Double click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Hold left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Hold right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Hold middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Mouse wheel up");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Mouse wheel down");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Mouse move X");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Mouse move Y");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Write text");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB5),"Press keys");
    g_signal_connect(G_OBJECT(dropB5), "changed",G_CALLBACK(comboAction5_selected), G_OBJECT(win));

    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"No action");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Switch to next configuration");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Click right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Click middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Double click left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Hold left mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Hold right mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Hold middle mouse button");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Mouse wheel up");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Mouse wheel down");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Mouse move X");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Mouse move Y");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Write text");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropB6),"Press keys");
    g_signal_connect(G_OBJECT(dropB6), "changed",G_CALLBACK(comboAction6_selected), G_OBJECT(win));



    /** attach everything to the grid **/
    /** TODO: parameter fields... */
    //Button 1
    gtk_grid_attach(GTK_GRID(tableActions),labelB1,0,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB1,0,1,4,1);

    //Button 2
    gtk_grid_attach(GTK_GRID(tableActions),labelB2,0,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB2,0,4,4,1);

    //Button 3
    gtk_grid_attach(GTK_GRID(tableActions),labelB3,0,6,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB3,0,7,4,1);

    //Button 4
    gtk_grid_attach(GTK_GRID(tableActions),labelB4,4,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB4,4,1,4,1);

    //Button 5
    gtk_grid_attach(GTK_GRID(tableActions),labelB5,4,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB5,4,4,4,1);

    //Button 6
    gtk_grid_attach(GTK_GRID(tableActions),labelB6,4,6,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB6,4,7,4,1);



    notebook = gtk_notebook_new();
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),tableActions,gtk_label_new("Select button press functions"));
    gtk_container_add(GTK_CONTAINER(mainBox), notebook);
}
//Build the load/store GUI for the slots
static void createGUILoadStore(GtkWidget *mainBox, GtkWidget *win)
{
    //Save/Store part
    GtkWidget *hboxSave = NULL;
    GtkWidget *btnSaveClearAll = NULL;
    GtkWidget *btnSaveApply = NULL;
    GtkWidget *btnSaveStoreAs = NULL;
    GtkWidget *btnSaveLoad = NULL;

    /* Save/Load/Slot Actions */
    //alignSave = gtk_alignment_new(0,1,1,0);
    hboxSave = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_set_homogeneous(GTK_BOX(hboxSave),TRUE);
    //gtk_container_add(GTK_CONTAINER(alignSave), hboxSave);

    btnSaveApply = gtk_button_new_with_label("Apply Settings");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveApply);

    btnSaveStoreAs = gtk_button_new_with_label("Store Slot as:");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveStoreAs);

    btnSaveLoad = gtk_button_new_with_label("Load Slot");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveLoad);

    btnSaveClearAll = gtk_button_new_with_label("Clear all Slots");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveClearAll);

    gtk_container_add(GTK_CONTAINER(mainBox), hboxSave);
}
//Build the log GUI
static void createGUILog(GtkWidget *mainBox, GtkWidget *win)
{
     //Log..
    GtkWidget *list = NULL;
    GtkWidget *labelLog = NULL;

    /* Log View on the bottom */
    labelLog = gtk_label_new("Log:");
    gtk_label_set_justify(GTK_LABEL(labelLog), GTK_JUSTIFY_LEFT);
    gtk_container_add(GTK_CONTAINER(mainBox),labelLog);

    //Log List
    logStore = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    list = createLog();
    gtk_container_add(GTK_CONTAINER(mainBox), list);

    logAdd("FABI GUI started...");
}

static void about (GtkWidget *wid, GtkWidget *win)
{
    GtkWidget *dialog = NULL;

    dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,
        "Welcome to the FABI GUI! For further information, please visit either:\nwww.asterics-academy.net/tools\nor\nwww.github.com/asterics/FABI");
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}

void combo_selected(GtkWidget *widget, gpointer window)
{
  strcpy(currentCOMPort,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));
}

static void connect (GtkWidget *wid, GtkWidget *win)
{
    char *comName;
    comName = strdup("");

    #ifdef ARCH_LINUX
        strcat(comName,"/dev/serial/by-id/");
    #endif // ARCH_LINUX

    if(isConnected == 0)
    {
        if(currentCOMPort != NULL && strlen(currentCOMPort) != 0)
        {
            strcat(comName,currentCOMPort);
            int ret = openCOM(comName);
            if(ret < 0)
            {
                logAdd("Error connecting to the serial port.");
            } else {
                logAdd("Connected!");
                isConnected = 1;
                gtk_label_set_text(GTK_LABEL(labelConnected),"Connected");
                gtk_button_set_label(GTK_BUTTON(wid),"Disconnect");
            }
        }
    } else {
        closeCOM();
        logAdd("Disconnected!");
        isConnected = 0;
        gtk_label_set_text(GTK_LABEL(labelConnected),"Not connected");
        gtk_button_set_label(GTK_BUTTON(wid),"Connect");
    }

}


void logAdd(char * logmessage)
{
    #define LOG_LENGTH 10
    static char* message[LOG_LENGTH];
    static char* time[LOG_LENGTH];
    static int isFilled = 0;

    if(isFilled == 0)
    {
        for(int i = 0; i < LOG_LENGTH; i++)
        {
            message[i] = strdup("");
            time[i] = strdup("");
        }
        isFilled = 1;
    }

    for(int i = LOG_LENGTH-2; i >= 0 ; i--)
    {
        strcpy(message[i+1],message[i]);
        strcpy(time[i+1],time[i]);
    }
    strcpy(message[0],logmessage);
    time[0] = strdup("time");

    gtk_list_store_clear(logStore);

    for(int i = 0; i < LOG_LENGTH; i++)
    {
        gtk_list_store_prepend(logStore,&logIter);
        gtk_list_store_set(logStore,&logIter,0,time[i],1,message[i],-1);
    }
}


static GtkWidget *createLog (void)
{
    GtkCellRenderer     *renderer;
    GtkTreeModel        *model;
    GtkWidget           *view;

    view = gtk_tree_view_new ();

    /* --- Column #1 --- */

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Time",
                                               renderer,
                                               "text", 0,
                                               NULL);

    /* --- Column #2 --- */

    renderer = gtk_cell_renderer_text_new ();
    gtk_tree_view_insert_column_with_attributes (GTK_TREE_VIEW (view),
                                               -1,
                                               "Message",
                                               renderer,
                                               "text", 1,
                                               NULL);

    model = GTK_TREE_MODEL(logStore);

    gtk_tree_view_set_model (GTK_TREE_VIEW (view), model);

    /* The tree view has acquired its own reference to the
    *  model, so we can drop ours. That way the model will
    *  be freed automatically when the tree view is destroyed */

    g_object_unref (model);

    return view;
}


void comboAction1_selected(GtkWidget *widget, gpointer window)
{
    printf("TBD: process action selection for button1\n");
}
void comboAction2_selected(GtkWidget *widget, gpointer window)
{
    printf("TBD: process action selection for button2\n");
}
void comboAction3_selected(GtkWidget *widget, gpointer window)
{
    printf("TBD: process action selection for button3\n");
}
void comboAction4_selected(GtkWidget *widget, gpointer window)
{
    printf("TBD: process action selection for button4\n");
}
void comboAction5_selected(GtkWidget *widget, gpointer window)
{
    printf("TBD: process action selection for button5\n");
}
void comboAction6_selected(GtkWidget *widget, gpointer window)
{
    printf("TBD: process action selection for button6\n");
}




