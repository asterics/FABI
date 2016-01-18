#include "main.h"


GtkListStore *logStore = NULL;
GtkTreeIter logIter;
GtkWidget *labelConnected = NULL;
GtkWidget *btnStatusConnect = NULL;
char currentCOMPort[512];
int isConnected = 0;

GtkWidget *labelParameterB1 = NULL;
GtkWidget *labelParameterB2 = NULL;
GtkWidget *labelParameterB3 = NULL;
GtkWidget *labelParameterB4 = NULL;
GtkWidget *labelParameterB5 = NULL;
GtkWidget *labelParameterB6 = NULL;

GtkWidget *spinMouseMoveB1 = NULL;
GtkWidget *spinMouseMoveB2 = NULL;
GtkWidget *spinMouseMoveB3 = NULL;
GtkWidget *spinMouseMoveB4 = NULL;
GtkWidget *spinMouseMoveB5 = NULL;
GtkWidget *spinMouseMoveB6 = NULL;

GtkWidget *textWriteTextB1 = NULL;
GtkWidget *textWriteTextB2 = NULL;
GtkWidget *textWriteTextB3 = NULL;
GtkWidget *textWriteTextB4 = NULL;
GtkWidget *textWriteTextB5 = NULL;
GtkWidget *textWriteTextB6 = NULL;

GtkWidget *textLabelKeysB1 = NULL;
GtkWidget *textLabelKeysB2 = NULL;
GtkWidget *textLabelKeysB3 = NULL;
GtkWidget *textLabelKeysB4 = NULL;
GtkWidget *textLabelKeysB5 = NULL;
GtkWidget *textLabelKeysB6 = NULL;

GtkWidget *dropKeysB1 = NULL;
GtkWidget *dropKeysB2 = NULL;
GtkWidget *dropKeysB3 = NULL;
GtkWidget *dropKeysB4 = NULL;
GtkWidget *dropKeysB5 = NULL;
GtkWidget *dropKeysB6 = NULL;

GtkWidget *dropB1 = NULL;
GtkWidget *dropB2 = NULL;
GtkWidget *dropB3 = NULL;
GtkWidget *dropB4 = NULL;
GtkWidget *dropB5 = NULL;
GtkWidget *dropB6 = NULL;


GtkWidget *comboSlotNames = NULL;

char cmd [100];  // command string for sending AT commands

char ButtonCommands [7][100] = {"AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n",""};

char actions[100][40] = {"No action","Switch to next configuration","Click left mouse button","Click right mouse button",
                         "Click middle mouse button","Double click left mouse button","Hold left mouse button","Hold right mouse button",
                         "Hold middle mouse button","Mouse wheel up","Mouse wheel down","Mouse move X","Mouse move Y","Write text","Press keys",""
                        };


char keyStrings[100][20] = {"Clear!","KEY_A","KEY_B","KEY_C","KEY_D","KEY_E","KEY_F","KEY_G","KEY_H","KEY_I","KEY_J","KEY_K","KEY_L","KEY_M",
                            "KEY_N","KEY_O","KEY_P","KEY_Q","KEY_R","KEY_S","KEY_T","KEY_U","KEY_V","KEY_W","KEY_X","KEY_Y","KEY_Z",
                            "KEY_1","KEY_2","KEY_3","KEY_4","KEY_5","KEY_6","KEY_7","KEY_8","KEY_9","KEY_0","KEY_F1","KEY_F2","KEY_F3","KEY_F4",
                            "KEY_F5","KEY_F6","KEY_F7","KEY_F8","KEY_F9","KEY_F10","KEY_F11","KEY_F12","KEY_UP","KEY_DOWN","KEY_LEFT","KEY_RIGHT",
                            "KEY_SPACE","KEY_ALT","KEY_ENTER","KEY_BACKSPACE","KEY_CAPS_LOCK","KEY_CTRL","KEY_DELETE","KEY_END","KEY_ESC","KEY_GUI",
                            "KEY_HOME","KEY_INSERT","KEY_NUM_LOCK","KEY_PAGE_UP","KEY_PAGE_DOWN","KEY_PAUSE","KEY_RIGHT_ALT","KEY_RIGHT_GUI",
                            "KEY_SCROLL_LOCK","KEY_SHIFT","KEY_TAB",""
                           };



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

    /* Hide some widgets again... */
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB1));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB2));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB3));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB4));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB5));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB6));

    gtk_widget_hide(GTK_WIDGET(textLabelKeysB1));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB2));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB3));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB4));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB5));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB6));

    gtk_widget_hide(GTK_WIDGET(textWriteTextB1));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB2));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB3));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB4));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB5));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB6));

    gtk_widget_hide(GTK_WIDGET(dropKeysB1));
    gtk_widget_hide(GTK_WIDGET(dropKeysB2));
    gtk_widget_hide(GTK_WIDGET(dropKeysB3));
    gtk_widget_hide(GTK_WIDGET(dropKeysB4));
    gtk_widget_hide(GTK_WIDGET(dropKeysB5));
    gtk_widget_hide(GTK_WIDGET(dropKeysB6));

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
//    while ((token = strsep((char**)&comlist, ";")) != NULL)
    {
        if(strcmp(token,"") != 0)
        {
            gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(comboCOM), NULL, (gchar*)token);
        }
    }
    //free(dup);
    //free(token);

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

    labelParameterB1 = gtk_label_new("");
    labelParameterB2 = gtk_label_new("");
    labelParameterB3 = gtk_label_new("");
    labelParameterB4 = gtk_label_new("");
    labelParameterB5 = gtk_label_new("");
    labelParameterB6 = gtk_label_new("");

    dropB1 = gtk_combo_box_text_new();
    dropB2 = gtk_combo_box_text_new();
    dropB3 = gtk_combo_box_text_new();
    dropB4 = gtk_combo_box_text_new();
    dropB5 = gtk_combo_box_text_new();
    dropB6 = gtk_combo_box_text_new();

    spinMouseMoveB1 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB2 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB3 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB4 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB5 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB6 = gtk_spin_button_new(NULL,1.0,0);

    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB1),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB2),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB3),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB4),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB5),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB6),-50.0,50.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB1),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB2),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB3),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB4),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB5),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB6),1.0,2.0);

    textWriteTextB1 = gtk_entry_new();
    textWriteTextB2 = gtk_entry_new();
    textWriteTextB3 = gtk_entry_new();
    textWriteTextB4 = gtk_entry_new();
    textWriteTextB5 = gtk_entry_new();
    textWriteTextB6 = gtk_entry_new();

    textLabelKeysB1 = gtk_label_new("Keys:");
    textLabelKeysB2 = gtk_label_new("Keys:");
    textLabelKeysB3 = gtk_label_new("Keys:");
    textLabelKeysB4 = gtk_label_new("Keys:");
    textLabelKeysB5 = gtk_label_new("Keys:");
    textLabelKeysB6 = gtk_label_new("Keys:");

    dropKeysB1 = gtk_combo_box_text_new();
    dropKeysB2 = gtk_combo_box_text_new();
    dropKeysB3 = gtk_combo_box_text_new();
    dropKeysB4 = gtk_combo_box_text_new();
    dropKeysB5 = gtk_combo_box_text_new();
    dropKeysB6 = gtk_combo_box_text_new();

    //Build the key codes drop down
    populateKeyCodes(dropKeysB1);
    populateKeyCodes(dropKeysB2);
    populateKeyCodes(dropKeysB3);
    populateKeyCodes(dropKeysB4);
    populateKeyCodes(dropKeysB5);
    populateKeyCodes(dropKeysB6);

    //Build grid
    tableActions = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(tableActions),TRUE);

    //build each drop down menu & attach signals
    populateActions(dropB1);
    g_signal_connect(G_OBJECT(dropB1), "changed",G_CALLBACK(comboAction1_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB1), 2);
    populateActions(dropB2);
    g_signal_connect(G_OBJECT(dropB2), "changed",G_CALLBACK(comboAction2_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB2), 2);
    populateActions(dropB3);
    g_signal_connect(G_OBJECT(dropB3), "changed",G_CALLBACK(comboAction3_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB3), 2);
    populateActions(dropB4);
    g_signal_connect(G_OBJECT(dropB4), "changed",G_CALLBACK(comboAction4_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB4), 2);
    populateActions(dropB5);
    g_signal_connect(G_OBJECT(dropB5), "changed",G_CALLBACK(comboAction5_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB5), 2);
    populateActions(dropB6);
    g_signal_connect(G_OBJECT(dropB6), "changed",G_CALLBACK(comboAction6_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB6), 2);



    /** attach everything to the grid **/
    //Button 1
    gtk_grid_attach(GTK_GRID(tableActions),labelB1,0,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB1,0,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB1,0,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB1,1,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB1,0,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB1,2,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB1,1,2,3,1);


    //Button 2
    gtk_grid_attach(GTK_GRID(tableActions),labelB2,0,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB2,0,4,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB2,0,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB2,1,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB2,0,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB2,2,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB2,1,5,3,1);

    //Button 3
    gtk_grid_attach(GTK_GRID(tableActions),labelB3,0,6,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB3,0,7,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB3,0,8,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB3,1,8,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB3,0,8,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB3,2,8,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB3,1,8,3,1);

    //Button 4
    gtk_grid_attach(GTK_GRID(tableActions),labelB4,4,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB4,4,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB4,4,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB4,5,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB4,4,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB4,6,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB4,5,2,3,1);

    //Button 5
    gtk_grid_attach(GTK_GRID(tableActions),labelB5,4,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB5,4,4,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB5,4,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB5,5,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB5,4,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB5,6,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB5,5,5,3,1);

    //Button 6
    gtk_grid_attach(GTK_GRID(tableActions),labelB6,4,6,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB6,4,7,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB6,4,8,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB6,5,8,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB6,4,8,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB6,6,8,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB6,5,8,3,1);



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
    g_signal_connect(G_OBJECT(btnSaveApply), "clicked", G_CALLBACK(apply), G_OBJECT(win));

    btnSaveStoreAs = gtk_button_new_with_label("Store Slot as:");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveStoreAs);

//<<<<<<< HEAD

    comboSlotNames = gtk_combo_box_text_new();
    g_signal_connect(G_OBJECT(comboSlotNames), "changed",G_CALLBACK(comboSlotName_selected), G_OBJECT(win));
    gtk_container_add(GTK_CONTAINER(hboxSave), comboSlotNames);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSlotNames),"default");
//    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSlotNames),"dummy");

//=======
//    textSlotName = gtk_entry_new();
//    gtk_container_add(GTK_CONTAINER(hboxSave), textSlotName);

//    dropSlotName = gtk_combo_box_text_new();
//    gtk_container_add(GTK_CONTAINER(hboxSave), dropSlotName);
//>>>>>>> bdb943c1d5b68ea38c402abd05a2bb50cb64ddc6

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

void comboSlotName_selected(GtkWidget *widget, gpointer window)
{
    if(isConnected != 0)
    {
        char * selectedSlot=gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));

        if (strlen(selectedSlot)>1)
        {
            sprintf(cmd,"AT LO %s\n",selectedSlot);
            processSerialCommand(cmd);
        }
    }
    else  logAdd("Please connect serial port first.");
}

void gotSlotName(char * newName)
{
    printf("Slotname received:%s\n",newName);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSlotNames),newName);
}


static void apply (GtkWidget *wid, GtkWidget *win)
{
    int i;

    if(isConnected != 0)
    {
        printf("Now writing settings to serial Port..\n");
        for (i=0; i<6; i++)
        {
            sprintf(cmd,"AT BM %d\n",i+1);
            writeCOM(cmd,strlen(cmd));
            printf("wrote to COMPORT:%s\n",cmd);

            writeCOM(ButtonCommands[i],strlen(ButtonCommands[i]));
            printf("wrote to COMPORT:%s\n",ButtonCommands[i]);
        }
        printf("Settings applied.\n");
    }
    else   logAdd("Please connect serial port first.");
}

static void connect (GtkWidget *wid, GtkWidget *win)
{

    char comName[256];
    // comName = strdup("");     (!!)
    strcpy(comName,"");


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
            }
            else
            {
                logAdd("Connected!");

                isConnected = 1;
                gtk_label_set_text(GTK_LABEL(labelConnected),"Connected");
                gtk_button_set_label(GTK_BUTTON(wid),"Disconnect");

                //             gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(comboSlotNames));
                if (processSerialCommand("AT LI\n") > 1)
                    logAdd("FABI Device recognized !\n");
                else  logAdd("Cound not connect to FABI device !\n");

            }
        }
    }
    else
    {
        closeCOM();
        logAdd("Disconnected!");
        isConnected = 0;
        gtk_label_set_text(GTK_LABEL(labelConnected),"Not connected");
        gtk_button_set_label(GTK_BUTTON(wid),"Connect");
    }

}


int getTime(char * timeBuffer)
{
    GTimeVal  time;
    GDate    *date_heap;
    gchar     tmp_buffer[256];

    /* Get current time (measured as offset from Epoch) */
    g_get_current_time( &time );
    printf( "Offset from Epoch: %ld seconds, %ld microseconds\n",
            time.tv_sec, time.tv_usec );

    /* Convert offset to real date */
    date_heap = g_date_new();
    g_date_set_time_val( date_heap, &time );
    g_date_strftime( tmp_buffer, 256, "%x", date_heap );
    printf( "Current date (heap):  %s\n", tmp_buffer );
    strcpy(timeBuffer,tmp_buffer);
    g_date_free( date_heap );

    return( 0 );
}

void logAdd(char * logmessage)
{
#define LOG_LENGTH 10
    static char message[LOG_LENGTH][256];
    static char time[LOG_LENGTH][50];
    static int isFilled = 0;
    int i = 0;

    if(isFilled == 0)
    {
        for(i = 0; i < LOG_LENGTH; i++)
        {
            strcpy(message[i],"");
            strcpy(time[i],"");
        }
        isFilled = 1;
    }

    for(i = LOG_LENGTH-2; i >= 0 ; i--)
    {
        strcpy(message[i+1],message[i]);
        strcpy(time[i+1],time[i]);
    }
    if (strlen(logmessage)<256)
        strcpy(message[0],logmessage);

//    strcpy(time[0],"time");
    getTime(time[0]);

    gtk_list_store_clear(logStore);

    for(i = 0; i < LOG_LENGTH; i++)
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
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB1,selection);
    setParameterExtras(spinMouseMoveB1, textWriteTextB1, textLabelKeysB1, dropKeysB1, selection);
    setButtonCommand(1,spinMouseMoveB1, textWriteTextB1, textLabelKeysB1, selection);
}
void comboAction2_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB2,selection);
    setParameterExtras(spinMouseMoveB2, textWriteTextB2, textLabelKeysB2, dropKeysB2, selection);
    setButtonCommand(2,spinMouseMoveB2, textWriteTextB2, textLabelKeysB2, selection);
}
void comboAction3_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB3,selection);
    setParameterExtras(spinMouseMoveB3, textWriteTextB3, textLabelKeysB3, dropKeysB3, selection);
    setButtonCommand(3,spinMouseMoveB3, textWriteTextB3, textLabelKeysB3, selection);
}
void comboAction4_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB4,selection);
    setParameterExtras(spinMouseMoveB4, textWriteTextB4, textLabelKeysB4, dropKeysB4, selection);
    setButtonCommand(4,spinMouseMoveB4, textWriteTextB4, textLabelKeysB4, selection);
}
void comboAction5_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB5,selection);
    setParameterExtras(spinMouseMoveB5, textWriteTextB5, textLabelKeysB5, dropKeysB5, selection);
    setButtonCommand(5,spinMouseMoveB5, textWriteTextB5, textLabelKeysB5, selection);
}
void comboAction6_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB6,selection);
    setParameterExtras(spinMouseMoveB6, textWriteTextB6, textLabelKeysB6, dropKeysB6, selection);
    setButtonCommand(6,spinMouseMoveB6, textWriteTextB6, textLabelKeysB6, selection);
}

void populateKeyCodes(GtkWidget *dropDown)
{
    int i=0;
    while (strlen(keyStrings[i])>0)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropDown),keyStrings[i++]);
}


void populateActions(GtkWidget *dropDown)
{
    int i=0;
    while (strlen(actions[i])>0)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(dropDown),actions[i++]);
}


int getSelectionIndex(char * selection)
{
    int i=0;
    while (strlen(actions[i])>0)
    {
        if (!strcmp(selection,actions[i])) return(i);
        i++ ;
    }
    return(-1);
}

void setParameterLabel(GtkWidget *label, char* selection)
{
    int i=getSelectionIndex(selection);

    if ((i >=0) && (i<=10))
    {
        gtk_label_set_text(GTK_LABEL(label),"");
    }
    else if ((i ==11)  || (i==12))
    {
        gtk_label_set_text(GTK_LABEL(label),"Speed:");
    }
    else if (i ==13)
    {
        gtk_label_set_text(GTK_LABEL(label),"Text:");
    }
    else if (i ==14)
    {
        gtk_label_set_text(GTK_LABEL(label),"");
    }
}


void setParameterExtras(GtkWidget *spinMouseMove, GtkWidget *textWriteText, GtkWidget *textLabelKeys, GtkWidget *dropKeys, char* selection)
{
    int i = getSelectionIndex(selection);

    if ((i >=0)  && (i<=10))
    {
        gtk_widget_hide(GTK_WIDGET(spinMouseMove));
        gtk_widget_hide(GTK_WIDGET(textWriteText));
        gtk_widget_hide(GTK_WIDGET(textLabelKeys));
        gtk_widget_hide(GTK_WIDGET(dropKeys));
    }
    else if ((i ==11)  || (i==12))
    {
        gtk_widget_show(GTK_WIDGET(spinMouseMove));
        gtk_widget_hide(GTK_WIDGET(textWriteText));
        gtk_widget_hide(GTK_WIDGET(textLabelKeys));
        gtk_widget_hide(GTK_WIDGET(dropKeys));
    }
    else if (i ==13)
    {
        gtk_widget_hide(GTK_WIDGET(spinMouseMove));
        gtk_widget_show(GTK_WIDGET(textWriteText));
        gtk_widget_hide(GTK_WIDGET(textLabelKeys));
        gtk_widget_hide(GTK_WIDGET(dropKeys));
    }
    else if (i ==14)
    {
        gtk_widget_hide(GTK_WIDGET(spinMouseMove));
        gtk_widget_hide(GTK_WIDGET(textWriteText));
        gtk_widget_show(GTK_WIDGET(textLabelKeys));
        gtk_widget_show(GTK_WIDGET(dropKeys));
    }
}

void setButtonCommand(int buttonNumber, GtkWidget *spinMouseMove, GtkWidget *textWriteText, GtkWidget *textLabelKeys, char* selection)
{
    int i=getSelectionIndex(selection);
    buttonNumber--;

    // TBD: add parameters to AT command (from spinMouseMove or textWriteText)

    switch (i)
    {
    case 0:
        strcpy (ButtonCommands[buttonNumber],"AT ID\n");
        break;
    case 1:
        strcpy (ButtonCommands[buttonNumber],"AT NE\n");
        break;
    case 2:
        strcpy (ButtonCommands[buttonNumber],"AT CL\n");
        break;
    case 3:
        strcpy (ButtonCommands[buttonNumber],"AT CR\n");
        break;
    case 4:
        strcpy (ButtonCommands[buttonNumber],"AT CM\n");
        break;
    case 5:
        strcpy (ButtonCommands[buttonNumber],"AT CD\n");
        break;
    case 6:
        strcpy (ButtonCommands[buttonNumber],"AT PL\n");
        break;
    case 7:
        strcpy (ButtonCommands[buttonNumber],"AT PR\n");
        break;
    case 8:
        strcpy (ButtonCommands[buttonNumber],"AT PM\n");
        break;
    case 9:
        strcpy (ButtonCommands[buttonNumber],"AT WU\n");
        break;
    case 10:
        strcpy (ButtonCommands[buttonNumber],"AT WD\n");
        break;
    case 11:
        strcpy (ButtonCommands[buttonNumber],"AT MX 10\n");   // TBD: get correct parameters from numericUpDown
        break;
    case 12:
        strcpy (ButtonCommands[buttonNumber],"AT MY 10\n");   // TBD: get correct parameters from numericUpDown
        break;
    case 13:
        strcpy (ButtonCommands[buttonNumber],"AT KW hallo\n");   // TBD: get correct parameters from textfield
        break;
    case 14:
        strcpy (ButtonCommands[buttonNumber],"AT KP KEY_A\n");   // TBD: get correct parameters from textfield
        break;
    }
}


void gotLoadValues(char * newValues)    // TBD: this is obsolete !!! replace by function which updates the GUI according to the received AT BM commands !!
{
    char * actToken = newValues;
    char * nextToken = strstr(actToken,"-,-");
    int i=0;
    int done = 0;

    // TBD: get parameters from AT command (to spinMouseMove or textWriteText)


    while ((nextToken!=0) && (!done))
    {
        *nextToken=0;
        printf("Found Token %d:%s\n",i,actToken);
        switch (i)
        {
        case 0:
            printf("SlotName=%s\n",actToken);
            break;  // slotname
        case 1:
            break;  // mouse wheel stepsize, currently not used
        case 2:
            break;  // time threshold for longpress, currently not used
        case 3:
            printf("Button1DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB1),atoi(actToken));
            break;
        case 4:
            printf("Button1ParameterValue=%s\n",actToken);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB1),atoi(actToken));
            break;
        case 5:
            printf("Button1ParameterText=%s\n",actToken);
            gtk_entry_set_text(GTK_ENTRY(textWriteTextB1),actToken);
            break;
        case 6:
            printf("Button2DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB2),atoi(actToken));
            break;
        case 7:
            printf("Button2ParameterValue=%s\n",actToken);
            break;
        case 8:
            printf("Button2ParameterText=%s\n",actToken);
            break;
        case 9:
            printf("Button3DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB3),atoi(actToken));
            break;
        case 10:
            printf("Button3ParameterValue=%s\n",actToken);
            break;
        case 11:
            printf("Button3ParameterText=%s\n",actToken);
            break;
        case 12:
            printf("Button4DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB4),atoi(actToken));
            break;
        case 13:
            printf("Button4ParameterValue=%s\n",actToken);
            break;
        case 14:
            printf("Button4ParameterText=%s\n",actToken);
            break;
        case 15:
            printf("Button5DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB5),atoi(actToken));
            break;
        case 16:
            printf("Button5ParameterValue=%s\n",actToken);
            break;
        case 17:
            printf("Button5ParameterText=%s\n",actToken);
            break;
        case 18:
            printf("Button6DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB6),atoi(actToken));
            break;
        case 19:
            printf("Button6ParameterValue=%s\n",actToken);
            break;
        case 20:
            printf("Button6ParameterText=%s\n",actToken);
        default:
            done = 1;
            break;
        }

        if (!done)
        {
            actToken=nextToken+3;
            nextToken=strstr(actToken,"-,-");
            i++;
        }
    }

}



#ifdef ARCH_WIN
char* strsep(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}
#endif // ARCH_WIN


