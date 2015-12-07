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
