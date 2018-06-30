#include <gtk/gtk.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "CommPacketTest.h"
#include "sendpacketapp.h"

// Global data to be passed around
GLOBAL_DATA     gdata;
bool commStarted = false;

void InitComm();
// SERIAL comm for Linux
struct termios options;

GLOBAL_DATA *GetGlobalData()
{
   return &gdata;
}

void print_msg(GtkWidget *widget, gpointer window) 
{

	printf("Send button clicked\n");

	GLOBAL_DATA *gdata = GetGlobalData();

    g_print ("Getting ready to send message now\n");

    if ( !commStarted )
        InitComm();

    int n = write(gdata->commPort2, "April", 5);
        
    if(n < 0)
        g_print("\nUnable to write to comm port\n");
	else
		g_print("\n SUCCESSFUL WRITE! \n\n");
	
}

void InitComm()
{
	GLOBAL_DATA *gdata = GetGlobalData();
 	
	// TODO: check comm port and change
    gdata->commPort2 = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);

    if ( gdata->commPort2 == -1)
    {
        g_print("\n ERROR cannot open port!!\n");
        perror("\nOpen_port: unable to open ttyUSB1 - \n");
    }
    else
    {
        g_print("\nComm Port Open successful!\n");
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

int main(int argc, char *argv[]) 
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *button2;
	GtkWidget *halign;
	GtkWidget *valign;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_title(GTK_WINDOW(window), "Send Serial Data");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
	gtk_container_set_border_width(GTK_CONTAINER(window), 15);

	button2 = gtk_button_new_with_mnemonic("_Send Packet");
	g_signal_connect(button, "clicked",
	G_CALLBACK(Send_Packet), NULL);

	button = gtk_button_new_with_mnemonic("_Send");
	g_signal_connect(button, "clicked",
	G_CALLBACK(print_msg), NULL);

	halign = gtk_alignment_new(0, 0, 0, 0);
	valign = gtk_alignment_new(1, 0, 1, 1);

	gtk_container_add(GTK_CONTAINER(halign), button);
	gtk_container_add(GTK_CONTAINER(window), halign);
	gtk_container_add(GTK_CONTAINER(valign), button2);
	gtk_container_add(GTK_CONTAINER(window), valign);
	gtk_widget_show_all(window);

	g_signal_connect(G_OBJECT(window), "destroy",
	G_CALLBACK(gtk_main_quit), NULL);

	gtk_main();

	return 0;
}
