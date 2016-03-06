#include "main.h"

gint a=250;
GtkListStore *logStore = NULL;
GtkTreeIter logIter;
GtkWidget *labelConnected = NULL;
GtkWidget *labelFiller = NULL;
GtkWidget *btnStatusConnect = NULL;
GtkWidget *btnStatusDownload = NULL;
char currentCOMPort[512];
int isConnected = 0;

static GtkWidget *window = NULL;



GtkWidget *labelParameterB1 = NULL;
GtkWidget *labelParameterB2 = NULL;
GtkWidget *labelParameterB3 = NULL;
GtkWidget *labelParameterB4 = NULL;
GtkWidget *labelParameterB5 = NULL;
GtkWidget *labelParameterB6 = NULL;
GtkWidget *labelParameterB7 = NULL;
GtkWidget *labelParameterB8 = NULL;
GtkWidget *labelParameterB9 = NULL;
GtkWidget *labelParameterBS = NULL;
GtkWidget *labelParameterBP = NULL;

GtkWidget *spinMouseMoveB1 = NULL;
GtkWidget *spinMouseMoveB2 = NULL;
GtkWidget *spinMouseMoveB3 = NULL;
GtkWidget *spinMouseMoveB4 = NULL;
GtkWidget *spinMouseMoveB5 = NULL;
GtkWidget *spinMouseMoveB6 = NULL;
GtkWidget *spinMouseMoveB7 = NULL;
GtkWidget *spinMouseMoveB8 = NULL;
GtkWidget *spinMouseMoveB9 = NULL;
GtkWidget *spinMouseMoveBS = NULL;
GtkWidget *spinMouseMoveBP = NULL;


GtkWidget *thresholdadj = NULL;
GtkWidget *thresholdadj1 = NULL;

GtkWidget *widget1;
GtkWidget *widget2;
GtkLevelBar *sload;
GtkLevelBar *pload;


int A=750;
int min=0;
int max=1023;


gchar *GA;
gchar *Gmins;
gchar *Gmaxs;
gchar *Gminp;
gchar *Gmaxp;



GtkWidget *textWriteTextB1 = NULL;
GtkWidget *textWriteTextB2 = NULL;
GtkWidget *textWriteTextB3 = NULL;
GtkWidget *textWriteTextB4 = NULL;
GtkWidget *textWriteTextB5 = NULL;
GtkWidget *textWriteTextB6 = NULL;
GtkWidget *textWriteTextB7 = NULL;
GtkWidget *textWriteTextB8 = NULL;
GtkWidget *textWriteTextB9 = NULL;
GtkWidget *textWriteTextBS = NULL;
GtkWidget *textWriteTextBP = NULL;

GtkWidget *textLabelKeysB1 = NULL;
GtkWidget *textLabelKeysB2 = NULL;
GtkWidget *textLabelKeysB3 = NULL;
GtkWidget *textLabelKeysB4 = NULL;
GtkWidget *textLabelKeysB5 = NULL;
GtkWidget *textLabelKeysB6 = NULL;
GtkWidget *textLabelKeysB7 = NULL;
GtkWidget *textLabelKeysB8 = NULL;
GtkWidget *textLabelKeysB9 = NULL;
GtkWidget *textLabelKeysBS = NULL;
GtkWidget *textLabelKeysBP = NULL;

GtkWidget *dropKeysB1 = NULL;
GtkWidget *dropKeysB2 = NULL;
GtkWidget *dropKeysB3 = NULL;
GtkWidget *dropKeysB4 = NULL;
GtkWidget *dropKeysB5 = NULL;
GtkWidget *dropKeysB6 = NULL;
GtkWidget *dropKeysB7 = NULL;
GtkWidget *dropKeysB8 = NULL;
GtkWidget *dropKeysB9 = NULL;
GtkWidget *dropKeysBS = NULL;
GtkWidget *dropKeysBP = NULL;

GtkWidget *dropB1 = NULL;
GtkWidget *dropB2 = NULL;
GtkWidget *dropB3 = NULL;
GtkWidget *dropB4 = NULL;
GtkWidget *dropB5 = NULL;
GtkWidget *dropB6 = NULL;
GtkWidget *dropB7 = NULL;
GtkWidget *dropB8 = NULL;
GtkWidget *dropB9 = NULL;
GtkWidget *dropBS = NULL;
GtkWidget *dropBP = NULL;

int Z=0;
GtkWidget *comboSlotNames = NULL;

char cmd [100];  // command string for sending AT commands

//char ButtonCommands [12][100] = {"AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n","AT CL\n"};

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
    gtk_window_set_default_icon_from_file("fabi.png", NULL);
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
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB7));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB8));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveB9));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveBS));
    gtk_widget_hide(GTK_WIDGET(spinMouseMoveBP));


    gtk_widget_hide(GTK_WIDGET(textLabelKeysB1));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB2));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB3));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB4));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB5));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB6));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB7));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB8));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysB9));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysBS));
    gtk_widget_hide(GTK_WIDGET(textLabelKeysBP));

    gtk_widget_hide(GTK_WIDGET(textWriteTextB1));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB2));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB3));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB4));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB5));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB6));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB7));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB8));
    gtk_widget_hide(GTK_WIDGET(textWriteTextB9));
    gtk_widget_hide(GTK_WIDGET(textWriteTextBS));
    gtk_widget_hide(GTK_WIDGET(textWriteTextBP));

    gtk_widget_hide(GTK_WIDGET(dropKeysB1));
    gtk_widget_hide(GTK_WIDGET(dropKeysB2));
    gtk_widget_hide(GTK_WIDGET(dropKeysB3));
    gtk_widget_hide(GTK_WIDGET(dropKeysB4));
    gtk_widget_hide(GTK_WIDGET(dropKeysB5));
    gtk_widget_hide(GTK_WIDGET(dropKeysB6));
    gtk_widget_hide(GTK_WIDGET(dropKeysB7));
    gtk_widget_hide(GTK_WIDGET(dropKeysB8));
    gtk_widget_hide(GTK_WIDGET(dropKeysB9));
    gtk_widget_hide(GTK_WIDGET(dropKeysBS));
    gtk_widget_hide(GTK_WIDGET(dropKeysBP));



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
    GtkWidget *fileSubQuit;
    GtkWidget *aboutMi;
    GtkWidget *aboutMenu;
    GtkWidget *aboutSubAbout;

    /*Create menu bar */
    menubar = gtk_menu_bar_new();
    fileMenu = gtk_menu_new();
    fileMi = gtk_menu_item_new_with_label("File");
    fileSubStoreFile = gtk_menu_item_new_with_label("Store settings: FABI->File");
    fileSubLoadFile = gtk_menu_item_new_with_label("Store settings: File->FABI");
    fileSubQuit = gtk_menu_item_new_with_label("Quit");
    g_signal_connect (G_OBJECT (fileSubQuit), "activate", G_CALLBACK (quit), G_OBJECT(win));

    aboutMenu = gtk_menu_new();
    aboutMi = gtk_menu_item_new_with_label("About");
    aboutSubAbout = gtk_menu_item_new_with_label("About");
    g_signal_connect (G_OBJECT (aboutSubAbout), "activate", G_CALLBACK (about), G_OBJECT(win));

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileSubStoreFile);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileSubLoadFile);
    gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileSubQuit);

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
    GtkWidget *label;

    tableStatus = gtk_grid_new();

    gtk_window_set_title (GTK_WINDOW (window), "Links");

    label = gtk_label_new ("For major informations please visit our <a href=\"http://www.asterics-academy.net\""
                             "title=\"plain text\">AsTeRICS</a> Website or our <a href=\"https://github.com/asterics\""
                             "title=\"plain text\">Github</a> Repository");

    gtk_label_set_use_markup (GTK_LABEL (label), TRUE);
    g_signal_connect (label, "activate-link", G_CALLBACK (activate_link), NULL);



    //Label 1
    labelStatus1 = gtk_label_new("Please select the COM port of your FABI:");
    gtk_label_set_justify(GTK_LABEL(labelStatus1), GTK_JUSTIFY_LEFT);
    gtk_grid_attach(GTK_GRID(tableStatus),labelStatus1,0,0,2,1);
    gtk_grid_attach(GTK_GRID(tableStatus),label,0,2,2,1);

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

    //Filler
    labelFiller = gtk_label_new("          ");
    gtk_grid_attach(GTK_GRID(tableStatus),labelFiller,4,1,1,1);
    labelFiller = gtk_label_new("          ");
    gtk_grid_attach(GTK_GRID(tableStatus),labelFiller,5,1,1,1);
    labelFiller = gtk_label_new("          ");
    gtk_grid_attach(GTK_GRID(tableStatus),labelFiller,6,1,1,1);

    //Button "Upload/Download"
    btnStatusDownload = gtk_button_new_with_label("Download");
    gtk_grid_attach(GTK_GRID(tableStatus),btnStatusDownload,7,1,1,1);
    g_signal_connect(G_OBJECT(btnStatusDownload), "clicked", G_CALLBACK(write2fabi), NULL);

    //Port status
    labelFiller = gtk_label_new("       ");
    gtk_grid_attach(GTK_GRID(tableStatus),labelFiller,2,0,1,1);

    labelStatus2 = gtk_label_new("Port Status: ");
    gtk_label_set_justify(GTK_LABEL(labelStatus2), GTK_JUSTIFY_RIGHT);
    gtk_grid_attach(GTK_GRID(tableStatus),labelStatus2,3,0,1,1);

    labelFiller = gtk_label_new("       ");
    gtk_grid_attach(GTK_GRID(tableStatus),labelFiller,2,1,1,1);

    labelConnected = gtk_label_new("Not connected");
    gtk_label_set_justify(GTK_LABEL(labelConnected), GTK_JUSTIFY_LEFT);
    gtk_grid_attach(GTK_GRID(tableStatus),labelConnected,3,1,1,1);

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


    labelParameterB1 = gtk_label_new("");
    labelParameterB2 = gtk_label_new("");
    labelParameterB3 = gtk_label_new("");
    labelParameterB4 = gtk_label_new("");


    dropB1 = gtk_combo_box_text_new();
    dropB2 = gtk_combo_box_text_new();
    dropB3 = gtk_combo_box_text_new();
    dropB4 = gtk_combo_box_text_new();


    spinMouseMoveB1 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB2 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB3 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB4 = gtk_spin_button_new(NULL,1.0,0);


    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB1),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB2),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB3),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB4),-50.0,50.0);

    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB1),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB2),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB3),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB4),1.0,2.0);


    textWriteTextB1 = gtk_entry_new();
    textWriteTextB2 = gtk_entry_new();
    textWriteTextB3 = gtk_entry_new();
    textWriteTextB4 = gtk_entry_new();


    textLabelKeysB1 = gtk_label_new("Keys:");
    textLabelKeysB2 = gtk_label_new("Keys:");
    textLabelKeysB3 = gtk_label_new("Keys:");
    textLabelKeysB4 = gtk_label_new("Keys:");


    dropKeysB1 = gtk_combo_box_text_new();
    dropKeysB2 = gtk_combo_box_text_new();
    dropKeysB3 = gtk_combo_box_text_new();
    dropKeysB4 = gtk_combo_box_text_new();


    //Build the key codes drop down
    populateKeyCodes(dropKeysB1);
    populateKeyCodes(dropKeysB2);
    populateKeyCodes(dropKeysB3);
    populateKeyCodes(dropKeysB4);


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
    gtk_grid_attach(GTK_GRID(tableActions),labelB3,4,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB3,4,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB3,4,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB3,5,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB3,4,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB3,6,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB3,5,2,3,1);

    //Button 4
    gtk_grid_attach(GTK_GRID(tableActions),labelB4,4,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB4,4,4,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB4,4,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB4,5,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB4,4,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB4,6,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB4,5,5,3,1);



    notebook = gtk_notebook_new();

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),tableActions,gtk_label_new("Button 1-4 Actions"));



    GtkWidget *labelB5 = gtk_label_new("Button 5");
    GtkWidget *labelB6 = gtk_label_new("Button 6");
    GtkWidget *labelB7 = gtk_label_new("Button 7");
    GtkWidget *labelB8 = gtk_label_new("Button 8");
    GtkWidget *labelB9 = gtk_label_new("Button 9");



    labelParameterB5 = gtk_label_new("");
    labelParameterB6 = gtk_label_new("");
    labelParameterB7 = gtk_label_new("");
    labelParameterB8 = gtk_label_new("");
    labelParameterB9 = gtk_label_new("");

    dropB5 = gtk_combo_box_text_new();
    dropB6 = gtk_combo_box_text_new();
    dropB7 = gtk_combo_box_text_new();
    dropB8 = gtk_combo_box_text_new();
    dropB9 = gtk_combo_box_text_new();


    spinMouseMoveB5 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB6 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB7 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB8 = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveB9 = gtk_spin_button_new(NULL,1.0,0);

    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB5),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB6),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB7),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB8),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveB9),-50.0,50.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB5),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB6),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB7),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB8),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveB9),1.0,2.0);


    textWriteTextB5 = gtk_entry_new();
    textWriteTextB6 = gtk_entry_new();
    textWriteTextB7 = gtk_entry_new();
    textWriteTextB8 = gtk_entry_new();
    textWriteTextB9 = gtk_entry_new();


    textLabelKeysB5 = gtk_label_new("Keys:");
    textLabelKeysB6 = gtk_label_new("Keys:");
    textLabelKeysB7 = gtk_label_new("Keys:");
    textLabelKeysB8 = gtk_label_new("Keys:");
    textLabelKeysB9 = gtk_label_new("Keys:");

    dropKeysB5 = gtk_combo_box_text_new();
    dropKeysB6 = gtk_combo_box_text_new();
    dropKeysB7 = gtk_combo_box_text_new();
    dropKeysB8 = gtk_combo_box_text_new();
    dropKeysB9 = gtk_combo_box_text_new();


    //Build the key codes drop down
    populateKeyCodes(dropKeysB5);
    populateKeyCodes(dropKeysB6);
    populateKeyCodes(dropKeysB7);
    populateKeyCodes(dropKeysB8);
    populateKeyCodes(dropKeysB9);

    tableActions = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(tableActions),TRUE);


    //build each drop down menu & attach signals
    populateActions(dropB5);
    g_signal_connect(G_OBJECT(dropB5), "changed",G_CALLBACK(comboAction5_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB5), 2);
    populateActions(dropB6);
    g_signal_connect(G_OBJECT(dropB6), "changed",G_CALLBACK(comboAction6_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB6), 2);
    populateActions(dropB7);
    g_signal_connect(G_OBJECT(dropB7), "changed",G_CALLBACK(comboAction7_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB7), 2);
    populateActions(dropB8);
    g_signal_connect(G_OBJECT(dropB8), "changed",G_CALLBACK(comboAction8_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB8), 2);
    populateActions(dropB9);
    g_signal_connect(G_OBJECT(dropB9), "changed",G_CALLBACK(comboAction9_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB9), 2);


    /** attach everything to the grid **/
    //Button 5
    gtk_grid_attach(GTK_GRID(tableActions),labelB5,0,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB5,0,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB5,0,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB5,1,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB5,0,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB5,2,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB5,1,2,3,1);

    //Button 6
    gtk_grid_attach(GTK_GRID(tableActions),labelB6,0,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB6,0,4,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB6,0,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB6,1,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB6,0,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB6,2,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB6,1,5,3,1);

    //Button 7
    gtk_grid_attach(GTK_GRID(tableActions),labelB7,0,6,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB7,0,7,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB7,0,8,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB7,1,8,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB7,0,8,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB7,2,8,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB7,1,8,3,1);

    //Button 8
    gtk_grid_attach(GTK_GRID(tableActions),labelB8,4,0,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB8,4,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB8,4,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB8,5,2,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB8,4,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB8,6,2,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB8,5,2,3,1);

    //Button 9
    gtk_grid_attach(GTK_GRID(tableActions),labelB9,4,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropB9,4,4,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterB9,4,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveB9,5,5,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysB9,4,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysB9,6,5,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextB9,5,5,3,1);



    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),tableActions,gtk_label_new("Button 5-9 Actions"));


    tableActions = gtk_grid_new();
    gtk_grid_set_column_homogeneous(GTK_GRID(tableActions),TRUE);
//    gtk_grid_set_row_homogeneous(GTK_GRID(tableActions),TRUE);


    GtkWidget *labelBS = gtk_label_new("Sip");
    GtkWidget *labelBP = gtk_label_new("Puff");

    labelParameterBS = gtk_label_new("");
    labelParameterBP = gtk_label_new("");

    dropBS = gtk_combo_box_text_new();
    dropBP = gtk_combo_box_text_new();

    spinMouseMoveBS = gtk_spin_button_new(NULL,1.0,0);
    spinMouseMoveBP = gtk_spin_button_new(NULL,1.0,0);

    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveBS),-50.0,50.0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(spinMouseMoveBP),-50.0,50.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveBS),1.0,2.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(spinMouseMoveBP),1.0,2.0);

    textWriteTextBS = gtk_entry_new();
    textWriteTextBP = gtk_entry_new();

    textLabelKeysBS = gtk_label_new("Keys:");
    textLabelKeysBP = gtk_label_new("Keys:");

    dropKeysBS = gtk_combo_box_text_new();
    dropKeysBP = gtk_combo_box_text_new();

    populateKeyCodes(dropKeysBS);
    populateKeyCodes(dropKeysBP);

    populateActions(dropBS);
    g_signal_connect(G_OBJECT(dropBS), "changed",G_CALLBACK(comboActionS_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropBS), 2);
    populateActions(dropBP);
    g_signal_connect(G_OBJECT(dropBP), "changed",G_CALLBACK(comboActionP_selected), G_OBJECT(win));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropBP), 2);


    widget1 = gtk_level_bar_new ();
    sload = GTK_LEVEL_BAR (widget1);
    gtk_level_bar_set_inverted (sload, TRUE);

    widget2 = gtk_level_bar_new ();
    pload = GTK_LEVEL_BAR (widget2);
    gtk_level_bar_set_inverted (pload, FALSE);



    gtk_level_bar_set_min_value (sload, min);
    gtk_level_bar_set_min_value (pload, 512);
    gtk_level_bar_set_max_value (sload, 511);
    gtk_level_bar_set_max_value (pload, max);

    if(A<512 && A>0)
    {
        gtk_level_bar_set_value (sload, 512-A);
        gtk_level_bar_set_value (pload, 512);
        Z=1;

    }
    if(A>511 && A<1024)
    {
        gtk_level_bar_set_value (pload, A);
        gtk_level_bar_set_value (sload, 0);
        Z=4;
    }

    thresholdadj = gtk_spin_button_new(NULL,1.0,0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(thresholdadj),min,511.0);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(thresholdadj),1.0,2.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(thresholdadj),300.0);

    thresholdadj1 = gtk_spin_button_new(NULL,1.0,0);
    gtk_spin_button_set_range(GTK_SPIN_BUTTON(thresholdadj1),512.0,max);
    gtk_spin_button_set_increments (GTK_SPIN_BUTTON(thresholdadj1),1.0,2.0);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(thresholdadj1),700.0);




    GA=g_strdup_printf("Current Value: %d",A);
    Gmins=g_strdup_printf("%d      ",min);
    Gmaxs=g_strdup_printf("          %d",511);
    Gminp=g_strdup_printf("          %d",max);
    Gmaxp=g_strdup_printf("%d          ",512);


    GtkWidget *labelA = gtk_label_new(GA);
    GtkWidget *labelSP = gtk_label_new("Sip and Puff Level");
    GtkWidget *labelS = gtk_label_new("      Sip");
    GtkWidget *labelP = gtk_label_new("       Puff");
    GtkWidget *labelSmin = gtk_label_new(Gmins);
    GtkWidget *labelSmax = gtk_label_new(Gmaxs);
    GtkWidget *labelPmin = gtk_label_new(Gmaxp);
    GtkWidget *labelPmax = gtk_label_new(Gminp);
    GtkWidget *labelSTH = gtk_label_new("                                 Threshold");
    GtkWidget *labelPTH = gtk_label_new("                                 Threshold");


    //Sip Button
    gtk_grid_attach(GTK_GRID(tableActions),labelBS,0,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropBS,0,2,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterBS,0,3,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveBS,1,3,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysBS,0,3,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysBS,2,3,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextBS,1,3,3,1);

    //Puf Button
    gtk_grid_attach(GTK_GRID(tableActions),labelBP,0,5,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropBP,0,5,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelParameterBP,0,6,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),spinMouseMoveBP,1,6,1,1);
    gtk_grid_attach(GTK_GRID(tableActions),textLabelKeysBP,0,6,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),dropKeysBP,2,6,2,1);
    gtk_grid_attach(GTK_GRID(tableActions),textWriteTextBP,1,6,3,1);

    gtk_grid_attach(GTK_GRID(tableActions),labelA,5,Z,4,1);

    gtk_grid_attach(GTK_GRID(tableActions),labelSP,5,0,4,1);

    gtk_grid_attach(GTK_GRID(tableActions),labelS,3,1,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelSmin,3,2,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),widget1,5,2,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelSmax,7,2,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelSTH,3,3,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),thresholdadj,6,3,1,1);

    gtk_grid_attach(GTK_GRID(tableActions),labelP,3,4,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelPmin,3,5,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),widget2,5,5,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelPmax,7,5,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),labelPTH,3,6,4,1);
    gtk_grid_attach(GTK_GRID(tableActions),thresholdadj1,6,6,1,1);



    gtk_notebook_append_page(GTK_NOTEBOOK(notebook),tableActions,gtk_label_new("Sip and Puff Levels"));

    gtk_container_add(GTK_CONTAINER(mainBox), notebook);


}
//Build the load/store GUI for the slots
static void createGUILoadStore(GtkWidget *mainBox, GtkWidget *win)
{
    //Save/Store part
    GtkWidget *hboxSave = NULL;
    GtkWidget *btnSaveClearAll = NULL;
    GtkWidget *btnSaveApply = NULL;
    GtkWidget *btnSaveStore = NULL;
    GtkWidget *btnSaveLoad = NULL;

    /* Save/Load/Slot Actions */
    //alignSave = gtk_alignment_new(0,1,1,0);
    hboxSave = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_set_homogeneous(GTK_BOX(hboxSave),TRUE);
    //gtk_container_add(GTK_CONTAINER(alignSave), hboxSave);

    btnSaveApply = gtk_button_new_with_label("Apply Settings");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveApply);
    g_signal_connect(G_OBJECT(btnSaveApply), "clicked", G_CALLBACK(apply), G_OBJECT(win));

    btnSaveStore = gtk_button_new_with_label("Save Slot:");
    gtk_container_add(GTK_CONTAINER(hboxSave), btnSaveStore);
    g_signal_connect(G_OBJECT(btnSaveStore), "clicked", G_CALLBACK(save), G_OBJECT(win));

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

static void quit (GtkWidget *win)
{
    gtk_main_quit();
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
    int i = 0;

    if(isConnected != 0)
    {
        char * selectedSlot=gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));

        activeSlot=(((int) gtk_combo_box_get_active(GTK_COMBO_BOX(widget)))-1);//---------------------------------------
        printf("ActiveSlot: %d\n",activeSlot);

        if(!GUI_COM[activeSlot].Loadstate)
        {
            if (strlen(selectedSlot)>1)
            {
                sprintf(cmd,"AT LO %s\n",selectedSlot);
                processSerialCommand(cmd);
                strcpy(GUI_COM[activeSlot].slotname, selectedSlot);
                GUI_COM[activeSlot].Loadstate=1;
            }
        }

/*
printf("lebst du\n");
        //dropB1
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB1), (GUI_COM[activeSlot].Commands[0]));
    printf("%d",GUI_COM[activeSlot].Commands[1]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB2), (GUI_COM[activeSlot].Commands[1]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB3), (GUI_COM[activeSlot].Commands[2]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB4), (GUI_COM[activeSlot].Commands[3]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB5), (GUI_COM[activeSlot].Commands[4]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB6), (GUI_COM[activeSlot].Commands[5]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB7), (GUI_COM[activeSlot].Commands[6]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB8), (GUI_COM[activeSlot].Commands[7]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropB9), (GUI_COM[activeSlot].Commands[8]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropBS), (GUI_COM[activeSlot].Commands[9]));
    gtk_combo_box_set_active(GTK_COMBO_BOX(dropBP), (GUI_COM[activeSlot].Commands[10]));


    //Zahlenwerte auslesen
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB1), GUI_COM[activeSlot].par[0]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB2), GUI_COM[activeSlot].par[1]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB3), GUI_COM[activeSlot].par[2]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB4), GUI_COM[activeSlot].par[3]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB5), GUI_COM[activeSlot].par[4]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB6), GUI_COM[activeSlot].par[5]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB7), GUI_COM[activeSlot].par[6]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB8), GUI_COM[activeSlot].par[7]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveB9), GUI_COM[activeSlot].par[8]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveBS), GUI_COM[activeSlot].par[9]);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinMouseMoveBP), GUI_COM[activeSlot].par[10]);

    //dropKeysB1
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[0]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB1), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[1]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB2), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[2]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB3), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[3]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB4), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[4]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB5), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[5]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB6), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[6]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB7), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[7]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB8), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[8]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysB9), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[9]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysBS), i+1);
            break;
        }
    }
    for(i=0;i<76;i++)
    {
        if(!strcmp(keyStrings[i], GUI_COM[activeSlot].key[10]))
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropKeysBP), i+1);
            break;
        }
    }


    //textWriteTextB1
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB1), GUI_COM[activeSlot].text[0]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB2), GUI_COM[activeSlot].text[1]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB3), GUI_COM[activeSlot].text[2]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB4), GUI_COM[activeSlot].text[3]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB5), GUI_COM[activeSlot].text[4]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB6), GUI_COM[activeSlot].text[5]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB7), GUI_COM[activeSlot].text[6]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB8), GUI_COM[activeSlot].text[7]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextB9), GUI_COM[activeSlot].text[8]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextBS), GUI_COM[activeSlot].text[9]);
    gtk_entry_set_text(GTK_ENTRY(textWriteTextBP), GUI_COM[activeSlot].text[10]);
    */


    }
    else  logAdd("Please connect serial port first.");
}

void gotSlotName(char * newName)
{
    printf("Slotname received:%s\n",newName);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSlotNames),newName);
}

static void save (GtkWidget *wid, GtkWidget *win)
{

  GtkWidget *content_area;
  GtkWidget *dialog;
  GtkEntryBuffer *display_entry;
  GtkWidget *dialog_label;
  GtkWidget *entry_widget;


  GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;

  dialog = gtk_dialog_new_with_buttons ("Save Slot",NULL,flags,("_Save"),GTK_RESPONSE_OK,("_Close"),GTK_RESPONSE_CLOSE,NULL);
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);

  display_entry = gtk_entry_buffer_new(NULL, 0);
  entry_widget = gtk_entry_new_with_buffer(display_entry);

  dialog_label = gtk_label_new ("Save Slot as:\n");


  content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

  gtk_container_add (GTK_CONTAINER (content_area), dialog_label);
  gtk_container_add (GTK_CONTAINER (content_area), entry_widget);


  gtk_widget_show_all (gtk_bin_get_child (GTK_BIN (dialog)));

  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
  {

     gtk_widget_destroy (dialog);

     GtkWidget *dialog1 = NULL;

     const gchar * text = gtk_entry_buffer_get_text (display_entry);
     gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(comboSlotNames), text);

     dialog1 = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                "Saved Slot as: %s", text);
     gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
     gtk_dialog_run (GTK_DIALOG (dialog1));
     gtk_widget_destroy (dialog1);
     gtk_widget_destroy (dialog);


  }else
  {
    gtk_widget_destroy (dialog);
  }


}


static void apply (GtkWidget *wid, GtkWidget *win)
{
    //dropB1
    GUI_COM[activeSlot].Commands[0]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB1)))-1);
    GUI_COM[activeSlot].Commands[1]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB2)))-1);
    GUI_COM[activeSlot].Commands[2]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB3)))-1);
    GUI_COM[activeSlot].Commands[3]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB4)))-1);
    GUI_COM[activeSlot].Commands[4]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB5)))-1);
    GUI_COM[activeSlot].Commands[5]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB6)))-1);
    GUI_COM[activeSlot].Commands[6]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB7)))-1);
    GUI_COM[activeSlot].Commands[7]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB8)))-1);
    GUI_COM[activeSlot].Commands[8]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropB9)))-1);
    GUI_COM[activeSlot].Commands[9]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropBS)))-1);
    GUI_COM[activeSlot].Commands[10]=((gtk_combo_box_get_active(GTK_COMBO_BOX(dropBP)))-1);

    //Zahlenwerte auslesen
    GUI_COM[activeSlot].par[0]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB1));
    GUI_COM[activeSlot].par[1]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB2));
    GUI_COM[activeSlot].par[2]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB3));
    GUI_COM[activeSlot].par[3]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB4));
    GUI_COM[activeSlot].par[4]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB5));
    GUI_COM[activeSlot].par[5]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB6));
    GUI_COM[activeSlot].par[6]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB7));
    GUI_COM[activeSlot].par[7]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB8));
    GUI_COM[activeSlot].par[8]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveB9));
    GUI_COM[activeSlot].par[9]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveBS));
    GUI_COM[activeSlot].par[10]= gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinMouseMoveBP));

    //dropKeysB1
    strcpy(GUI_COM[activeSlot].key[0], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB1)))-1)]);
    strcpy(GUI_COM[activeSlot].key[1], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB2)))-1)]);
    strcpy(GUI_COM[activeSlot].key[2], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB3)))-1)]);
    strcpy(GUI_COM[activeSlot].key[3], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB4)))-1)]);
    strcpy(GUI_COM[activeSlot].key[4], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB5)))-1)]);
    strcpy(GUI_COM[activeSlot].key[5], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB6)))-1)]);
    strcpy(GUI_COM[activeSlot].key[6], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB7)))-1)]);
    strcpy(GUI_COM[activeSlot].key[7], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB8)))-1)]);
    strcpy(GUI_COM[activeSlot].key[8], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysB9)))-1)]);
    strcpy(GUI_COM[activeSlot].key[9], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysBS)))-1)]);
    strcpy(GUI_COM[activeSlot].key[10], keyStrings[((gtk_combo_box_get_active(GTK_COMBO_BOX(dropKeysBP)))-1)]);

    //textWriteTextB1
    strcpy(GUI_COM[activeSlot].text[0], gtk_entry_get_text(GTK_ENTRY(textWriteTextB1)));
    strcpy(GUI_COM[activeSlot].text[1], gtk_entry_get_text(GTK_ENTRY(textWriteTextB2)));
    strcpy(GUI_COM[activeSlot].text[2], gtk_entry_get_text(GTK_ENTRY(textWriteTextB3)));
    strcpy(GUI_COM[activeSlot].text[3], gtk_entry_get_text(GTK_ENTRY(textWriteTextB4)));
    strcpy(GUI_COM[activeSlot].text[4], gtk_entry_get_text(GTK_ENTRY(textWriteTextB5)));
    strcpy(GUI_COM[activeSlot].text[5], gtk_entry_get_text(GTK_ENTRY(textWriteTextB6)));
    strcpy(GUI_COM[activeSlot].text[6], gtk_entry_get_text(GTK_ENTRY(textWriteTextB7)));
    strcpy(GUI_COM[activeSlot].text[7], gtk_entry_get_text(GTK_ENTRY(textWriteTextB8)));
    strcpy(GUI_COM[activeSlot].text[8], gtk_entry_get_text(GTK_ENTRY(textWriteTextB9)));
    strcpy(GUI_COM[activeSlot].text[9], gtk_entry_get_text(GTK_ENTRY(textWriteTextBS)));
    strcpy(GUI_COM[activeSlot].text[10], gtk_entry_get_text(GTK_ENTRY(textWriteTextBP)));

}

static void connect (GtkWidget *wid, GtkWidget *win)
{

    char comName[256];
    // comName = strdup("");     (!!)
    strcpy(comName,"");


#ifdef ARCH_LINUX
    strcat(comName,"/dev/serial/by-id/");
#endif // ARCH_LINUX
    //char antwort[3];

//    writeCOM("AT\n", strlen("AT\n"));
//    readCOM(antwort,3);
//
//    GtkWidget *dialog1 = NULL;
//
//    if(strcmp(antwort,"OK"))
//    {
//        isConnected=0;
//        dialog1 = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
//                "No FABI-Device detected!");
//
//        gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
//        gtk_dialog_run (GTK_DIALOG (dialog1));
//        gtk_widget_destroy (dialog1);
//    }
//        else
//    {
//        isConnected=1;
//
//        dialog1 = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
//                "FABI detected!");
//
//        gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
//        gtk_dialog_run (GTK_DIALOG (dialog1));
//        gtk_widget_destroy (dialog1);
//    }

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
                {
                    logAdd("FABI Device recognized !\n");

//                        GtkWidget *dialog = NULL;
//                        GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
//
//                        dialog = gtk_dialog_new_with_buttons ("Upload FABI to GUI ?"
//                                                                ,NULL,flags,("_OK"),GTK_RESPONSE_OK,("_Close"),GTK_RESPONSE_CLOSE,NULL);
//                        gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
//
//
//
//                         if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
//                         {
//
//                            gtk_widget_destroy (dialog);
//
//                            GtkWidget *dialog1 = NULL;
//
//                            dialog1 = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
//                                    "Allright!!");
//
//                            gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
//                            gtk_dialog_run (GTK_DIALOG (dialog1));
//                            gtk_widget_destroy (dialog1);
//
//
//                         }



                }
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

static gboolean activate_link (GtkWidget   *label, const gchar *uri, gpointer     data)
{
  if (g_strcmp0 (uri, "keynav") == 0)
    {
      GtkWidget *dialog;
      GtkWidget *parent;

      parent = gtk_widget_get_toplevel (label);
      dialog = gtk_message_dialog_new_with_markup (GTK_WINDOW (parent),
                 GTK_DIALOG_DESTROY_WITH_PARENT,
                 GTK_MESSAGE_INFO,
                 GTK_BUTTONS_OK,
                 "The term <i>keynav</i> is a shorthand for "
                 "keyboard navigation and refers to the process of using "
                 "a program (exclusively) via keyboard input.");
      gtk_window_set_modal (GTK_WINDOW (dialog), TRUE);

      gtk_window_present (GTK_WINDOW (dialog));
      g_signal_connect (dialog, "response", G_CALLBACK (response_cb), NULL);

      return TRUE;
    }

  return FALSE;
}

static void response_cb (GtkWidget *dialog, gint response_id, gpointer data)
{
  gtk_widget_destroy (dialog);
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

void comboAction7_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB7,selection);
    setParameterExtras(spinMouseMoveB7, textWriteTextB7, textLabelKeysB7, dropKeysB7, selection);
    setButtonCommand(7,spinMouseMoveB7, textWriteTextB7, textLabelKeysB7, selection);
}

void comboAction8_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB8,selection);
    setParameterExtras(spinMouseMoveB8, textWriteTextB8, textLabelKeysB8, dropKeysB8, selection);
    setButtonCommand(8,spinMouseMoveB8, textWriteTextB8, textLabelKeysB8, selection);
}

void comboAction9_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterB9,selection);
    setParameterExtras(spinMouseMoveB9, textWriteTextB9, textLabelKeysB9, dropKeysB9, selection);
    setButtonCommand(9,spinMouseMoveB9, textWriteTextB9, textLabelKeysB9, selection);
}

void comboActionS_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterBS,selection);
    setParameterExtras(spinMouseMoveBS, textWriteTextBS, textLabelKeysBS, dropKeysBS, selection);
    setButtonCommand(10,spinMouseMoveBS, textWriteTextBS, textLabelKeysBS, selection);
}

void comboActionP_selected(GtkWidget *widget, gpointer window)
{
    char selection[256];
    strcpy(selection,gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget)));

    setParameterLabel(labelParameterBP,selection);
    setParameterExtras(spinMouseMoveBP, textWriteTextBP, textLabelKeysBP, dropKeysBP, selection);
    setButtonCommand(11,spinMouseMoveBP, textWriteTextBP, textLabelKeysBP, selection);
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
            break;
        case 21:
            printf("Button7DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB7),atoi(actToken));
            break;
        case 22:
            printf("Button7ParameterValue=%s\n",actToken);
            break;
        case 23:
            printf("Button7ParameterText=%s\n",actToken);
            break;
        case 24:
            printf("Button8DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB8),atoi(actToken));
            break;
        case 25:
            printf("Button8ParameterValue=%s\n",actToken);
            break;
        case 26:
            printf("Button8ParameterText=%s\n",actToken);
            break;
        case 27:
            printf("Button9DropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropB9),atoi(actToken));
            break;
        case 28:
            printf("Button9ParameterValue=%s\n",actToken);
            break;
        case 29:
            printf("Button9ParameterText=%s\n",actToken);
            break;
        case 30:
            printf("ButtonSDropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropBS),atoi(actToken));
            break;
        case 31:
            printf("ButtonSParameterValue=%s\n",actToken);
            break;
        case 32:
            printf("ButtonSParameterText=%s\n",actToken);
            break;
        case 33:
            printf("ButtonPDropBoxSelection=%s\n",actToken);
            gtk_combo_box_set_active(GTK_COMBO_BOX(dropBP),atoi(actToken));
            break;
        case 34:
            printf("ButtonPParameterValue=%s\n",actToken);
            break;
        case 35:
            printf("ButtonPParameterText=%s\n",actToken);
            break;
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


void write2fabi() //static void apply (GtkWidget *wid, GtkWidget *win)
{
    int i;
    int k = 0;
    char cmd[200];

    if(isConnected != 0)
    {
        printf("Now writing settings to serial Port..\n");
        for (i=0; i<11; i++)
        {
            sprintf(cmd,"AT BM %d\n",i+1);
            writeCOM(cmd,strlen(cmd));
            printf("wrote to COMPORT:%s\n",cmd);

//            sleep(0.1);
//char ButtonCommands[15][6] = {"AT NC", "AT NE", "AT CL", "AT CR", "AT CM", "AT CD", "AT PL", "AT PR", "AT PM", "AT WU", "AT WD", "AT MX", "AT MY", "AT KW", "AT KP"};
//mx 12
            if(GUI_COM[activeSlot].Commands[i]<12) //no parameter
            {
            printf("1\n");
                sprintf(cmd, "%s\n", ButtonCommands[GUI_COM[activeSlot].Commands[i]]);

            printf("2\n");
            k=1;
            }
            printf("2,5\n");

            if(k=0)
            {
            if(GUI_COM[activeSlot].Commands[i]==12) //interger parameter
            {
            printf("2,75\n");
                sprintf(cmd, "%s %d\n", ButtonCommands[GUI_COM[activeSlot].Commands[i]], GUI_COM[activeSlot].par[i]);
            }
            if(GUI_COM[activeSlot].Commands[i]==13)
            {
                sprintf(cmd, "%s %d\n", ButtonCommands[GUI_COM[activeSlot].Commands[i]], GUI_COM[activeSlot].par[i]);
            }
            if(GUI_COM[activeSlot].Commands[i]==14) //text parameter
            {
                sprintf(cmd, "%s %s\n", ButtonCommands[GUI_COM[activeSlot].Commands[i]], GUI_COM[activeSlot].text[i]);
            }
            if(GUI_COM[activeSlot].Commands[i]==15) //key parameter
            {
                sprintf(cmd, "%s %s\n", ButtonCommands[GUI_COM[activeSlot].Commands[i]], GUI_COM[activeSlot].key[i]);
            }
            }

            printf("3\n");
            writeCOM(cmd,strlen(cmd));
            printf("4\n");
            printf("wrote to COMPORT:%s\n",cmd);
            printf("5\n");

//            sleep(0.1);
            //writeCOM(ButtonCommands[GUI_COM[activeSlot].Commands[i]],strlen(ButtonCommands[GUI_COM[activeSlot].Commands[i]]));
            //printf("wrote to COMPORT:%s\n",ButtonCommands[i]);
        }
		sprintf(cmd, "AT SA %s\n", GUI_COM[activeSlot].slotname);
		writeCOM(cmd,strlen(cmd));
        printf("Settings applied.\n");
    }
    else
    {logAdd("Please connect serial port first.");}
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
