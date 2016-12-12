// --------------------------------------------------------------
// Project : FabiGUI_GTK
// --------------------------------------------------------------
// File : main.h
// Author : Benjamin Aigner, Peter Gl��l
// Company : FH Technikum Wien
// Last update : 12.12.2016
// Platform :
// --------------------------------------------------------------
// Description : FABI application
// --------------------------------------------------------------

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdlib.h>
#include <gtk/gtk.h>
#include <string.h>
#include "serial.h"
#include "commands.h"
//#include <delay.h>


/** Callback methods for GUI actions... **/

//About dailog...
static void about (GtkWidget *wid, GtkWidget *win);
//Quit Program
static void quit (GtkWidget *win);
//Button "Connect": connect/disconnect to FABI...
static void connecting (GtkWidget *wid, GtkWidget *win);
//Callback for selected serial port from combobox
void combo_selected(GtkWidget *widget, gpointer window);
//fill comboCOM with list of comports
void fillComboCOM (void);

//Callback for selected slot name
void comboSlotName_selected(GtkWidget *widget, gpointer window);


//Button "Apply": send current configraution to FABI...
static void apply (GtkWidget *wid, GtkWidget *win);
static void save (GtkWidget *wid, GtkWidget *win);
void write2fabi(); //write from struct to fabi

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
//ComboBox callback button 7 action
void comboAction7_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 8 action
void comboAction8_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 9 action
void comboAction9_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 8 action
void comboActionS_selected(GtkWidget *widget, gpointer window);
//ComboBox callback button 9 action
void comboActionP_selected(GtkWidget *widget, gpointer window);
//Link
static gboolean activate_link (GtkWidget *label, const gchar *uri, gpointer data);
static void response_cb (GtkWidget *dialog, gint response_id, gpointer data);



/** General methods for the main... **/
//Log facility: add a log entry
void logAdd(char * logmessage);
//Set the label for each button to the corresponding parameter type
int getSelectionIndex(char * selection);
void setParameterLabel(GtkWidget *label, char* selection);
void setParameterExtras(GtkWidget *spinMouseMove, GtkWidget *textWriteText, GtkWidget *textLabelKeys, GtkWidget *dropKeys, char* selection);
void setButtonCommand(int buttonNumber, GtkWidget *spinMouseMove, GtkWidget *textWriteText, GtkWidget *textLabelKeys, char* selection);
void populateKeyCodes(GtkWidget *dropDown);
void populateActions(GtkWidget *dropDown);

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

//Timebase
int TimerCallback (gpointer data);

#ifdef ARCH_WIN
char* strsep(char** stringp, const char* delim);
#endif // ARCH_WIN


#endif // MAIN_H_INCLUDED
