    
#include <gtk/gtk.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
//#include <iostream>
#include "CommPacketTest.h"
#include "sendpacketapp.h"

// Global data to be passed around
GLOBAL_DATA     gdata;

// SERIAL comm for Linux
struct termios options;

GLOBAL_DATA *GetGlobalData()
{
   return &gdata;
}

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

static void entry_activated(GtkWidget *entry, gpointer data)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	
	gdata->inputString = gtk_entry_get_text(GTK_ENTRY(entry));

    g_print("Entry text: '%s'\n", gdata->inputString);
}

void ListeningThread()
{
    GLOBAL_DATA *gdata = GetGlobalData();

    int nbr, nbtr=1;
    HEADER *hdr;
}

void InitComm()
{
    GLOBAL_DATA *gdata = GetGlobalData();

    // TODO: check comm port and change
    gdata->commPort2 = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);

    if ( gdata->commPort2 == -1)
    {
        g_print(" ERROR cannot open port!!\n");
        perror("Open_port: unable to open ttyUSB0 - \n");
    }
    else
    {
        g_print("\nComm Port Open successful!");
        gdata->commStarted = true;
        gdata->readPort = true;

        // get commport info
        tcgetattr(gdata->commPort2, &options);

        // set the Baud rate
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);

        options.c_cflag |= (CLOCAL | CREAD);

        // set the new options
        tcsetattr(gdata->commPort2, TCSANOW, &options);

        // Mask the character size bits
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;

        // No parity
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;

        // disable hardware and software controls
        // HW
        options.c_cflag &= CRTSCTS;
        
        // SW
        options.c_iflag &= ~( IXON | IXOFF | IXANY );
    }
}

static void Send_Packet (GtkWidget *widget, gpointer data)
{   
    GLOBAL_DATA *gdata = GetGlobalData();
	DISPLAY_MESSAGE_1 msg1;

    g_print ("\nGetting ready to send packet now\n");

    if ( !(gdata->commStarted) )
        InitComm();

	// populate the packet
	strcpy(msg1.myTitle, "Change Height");
	//msg1.myTitle = "Change Height";
	msg1.hdr.message = (char) 0x7;
	msg1.hdr.size = sizeof(msg1); 

	int n = write(gdata->commPort2, (char *)&msg1, sizeof(msg1));
    
    printf("\n");
}

static void Send (GtkWidget *widget, gpointer data)
{   
    GLOBAL_DATA *gdata = GetGlobalData();

    g_print ("Getting ready to send message now\n");

    if ( !(gdata->commStarted) )
        InitComm();

    int n = write(gdata->commPort2, "April", 5);
        
    if(n < 0)
        g_print("\nUnable to write to comm port\n");
}

static void print_hello (GtkWidget *widget,gpointer data)
{
    g_print ("Hello World\n");
}

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;

    /* create a new window, and set its title */
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Main Window");
    gtk_container_set_border_width (GTK_CONTAINER (window), 200);
    
    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new ();
    
    /* Pack the container in the window */
    gtk_container_add (GTK_CONTAINER (window), grid);
    button = gtk_button_new_with_label ("Send");
    g_signal_connect (button, "clicked", G_CALLBACK (Send), NULL);
    
    /* Place the first button in the grid cell (0, 0), and make it fill
    * just 1 cell horizontally and vertically (ie no spanning)*/
    gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
    button = gtk_button_new_with_label ("Send Packet");
    g_signal_connect (button, "clicked", G_CALLBACK (Send_Packet), NULL);
    
    /* Place the second button in the grid cell (1, 0), and make it fill
    * just 1 cell horizontally and vertically (ie no spanning)
    */
    gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
    button = gtk_button_new_with_label ("Quit");
    g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
    

	/* Place the Quit button in the grid cell (0, 1), and make it
    * span 2 columns.
    */
    gtk_grid_attach (GTK_GRID (grid), button, 0, 1, 2, 1);
    
    /* Now that we are done packing our widgets, we show them all
    * in one go, by calling gtk_widget_show_all() on the window.
    * This call recursively calls gtk_widget_show() on all widgets
    * that are contained in the window, directly or indirectly.
    */
    gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;
    
    GLOBAL_DATA *gdata = GetGlobalData();

// start add for text entry    
/*	gtk_init(&argc, &argv);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(destroy), NULL);
    
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "A Text Entry Widget");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Placeholder Text");
    g_signal_connect(GTK_ENTRY(entry), "activate", G_CALLBACK(entry_activated), NULL);
    gtk_container_add(GTK_CONTAINER(window), entry);
    
    gtk_widget_show_all(window);
    
    gtk_main();*/
//end

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    
    return status;
}
