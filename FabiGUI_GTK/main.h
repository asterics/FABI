#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "serial.h"


/** Callback methods for GUI actions... **/

//About dailog...
static void about (GtkWidget *wid, GtkWidget *win);
//Button "Connect": connect/disconnect to FABI...
static void connect (GtkWidget *wid, GtkWidget *win);
//Callback for selected serial port from combobox
void combo_selected(GtkWidget *widget, gpointer window);

//ComboBox callback button 1 action
void comboAction1_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 2 action
void comboAction2_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 3 action
void comboAction3_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 4 action
void comboAction4_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 5 action
void comboAction5_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 6 action
void comboAction6_selected(GtkWidget *widget, gpointer window);


/** General methods for the main... **/
//Log facility: add a log entry
void logAdd(char * logmessage);

/** GUI builder **/
//Create the log facility
static GtkWidget *createLog (void);
//Build the status part of the main GUI
static void createGUIMenubar(GtkWidget *mainBox, GtkWidget *win);
//Build the status part of the main GUI
static void createGUIStatus(GtkWidget *mainBox, GtkWidget *win);
//Build the button action notebook
static void createGUIActions(GtkWidget *mainBox, GtkWidget *win);
//Build the load/store GUI for the slots
static void createGUILoadStore(GtkWidget *mainBox, GtkWidget *win);
//Build the log GUI
static void createGUILog(GtkWidget *mainBox, GtkWidget *win);



#endif // MAIN_H_INCLUDED
