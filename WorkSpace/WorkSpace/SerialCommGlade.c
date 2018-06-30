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

// SERIAL comm for Linux
struct termios options;

GLOBAL_DATA *GetGlobalData()
{
   return &gdata;
}

static void entry_activated(GtkWidget *entry, gpointer data)
{
	GLOBAL_DATA *gdata = GetGlobalData();
	
	gdata->inputString = gtk_entry_get_text(GTK_ENTRY(entry));

    g_print("Entry text: '%s'\n", gdata->inputString);
}

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

void on_btn_Start_clicked()
{
	printf("Start button clicked\n");

	InitComm();
}

void InitComm()
{
    GLOBAL_DATA *gdata = GetGlobalData();

	stepProgressBar();

    // TODO: check comm port and change
    gdata->commPort = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    if ( gdata->commPort == -1)
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
        tcgetattr(gdata->commPort, &options);

        // set the Baud rate
        cfsetispeed(&options, B9600);
        cfsetospeed(&options, B9600);

        options.c_cflag |= (CLOCAL | CREAD);

        // set the new options
        tcsetattr(gdata->commPort, TCSANOW, &options);

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

		stepProgressBar();
    }
}

void on_btn_send_clicked()
{
	printf("Send button clicked\n");

	GLOBAL_DATA *gdata = GetGlobalData();

    g_print ("Getting ready to send message now\n");

    if ( !(gdata->commStarted) )
        InitComm();

    int n = write(gdata->commPort, "A", 5);
        
    if(n < 0)
        g_print("\nUnable to write to comm port\n");
}

void on_btn_recieve_packet_clicked()
{
	GLOBAL_DATA *gdata = GetGlobalData();
	HEADER *hdr;
	gdata->readPort = true;
	int readCount = 0;

	printf("Recieve Packet button clicked\n");

    g_print ("\nGetting ready to recieve message now\n");

    if ( !(gdata->commStarted) )
        InitComm();

    char buffer[10];
    memset(buffer, 10, 0);
    
    while ( gdata->readPort )
    {
        usleep(2000000);
        // read message from serial port1
        int n = read(gdata->commPort, &buffer , 20);

        if(n < 0)
		{
            g_print("\nUnable to read to comm port\n");
			readCount++;	
			g_print("\nread count = %d", readCount);	
		}        
		else
		{
			DISPLAY_MESSAGE_1 *msg1;
			hdr = (HEADER *)buffer;

            gdata->readPort = false;
			g_print("\nReading Packet Header\n");
			g_print("\nThe header of the packet is: %d", hdr->message);
			
		}
		
		// break out of while loop after 20 loops to avoid lock
		if (readCount == 20)
			break;
    }

    printf("\n\nContents of buffer[]: %s", buffer);
    printf("\n");
	//gtk_close_window(gdata->window);
}

void on_btn_recieve_clicked()
{
	GLOBAL_DATA *gdata = GetGlobalData();

	int readCount = 0;

	gdata->readPort = true;

	printf("Recieve button clicked\n");

    g_print ("\nGetting ready to recieve message now\n");

    if ( !(gdata->commStarted) )
        InitComm();

    char buffer[10];
    memset(buffer, 10, 0);
    
    while ( gdata->readPort )
    {
        usleep(2000000);
        // read message from serial port1
        int n = read(gdata->commPort, &buffer , 1);

        if(n < 0)
		{
            g_print("\nUnable to read to comm port\n");
			readCount++;	
			g_print("\nread count = %d", readCount);	
		}        
		else
		{
            gdata->readPort = false;
		}
		
		// break out of while loop after 20 loops to avoid lock
		if (readCount == 20)
			break;
    }

    printf("\n\nContents of buffer[]: %s", buffer);
    printf("\n");
}

int main(int argc, char *argv[])
{
	GLOBAL_DATA *gdata = GetGlobalData();

	gtk_init(&argc, &argv);

	gdata->builder = gtk_builder_new();
	gtk_builder_add_from_file( gdata->builder, "SerialCommTest.glade", NULL);

	gdata->window = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "MainWindow"));
	gtk_builder_connect_signals(gdata->builder, NULL);

	initWidgets();

	g_object_unref(gdata->builder);

	// correctly closes window, terminates thread
	g_signal_connect(G_OBJECT(gdata->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show(gdata->window);
	gtk_main();

	return 0;
}

void initWidgets()
{
	GLOBAL_DATA *gdata = GetGlobalData();

	// get pointer to the buttons/ labels
	gdata->btn_Send = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "btn_send"));
	gdata->btn_Recieve = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "btn_recieve"));
	gdata->btn_Start = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "btn_Start"));
	gdata->btn_Quit = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "btn_recieve_packet"));

	// text entry
	gdata->entry_inputWidth = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "entry_inputWidth"));
	gdata->entry_inputHeight = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "entry_inputHeight"));
	gdata->entry_input3 = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "entry_input3"));
	gdata->entry_input4 = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "entry_input4"));
	gdata->entry_input5 = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "entry_input5"));
	gdata->entry_input6 = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "entry_input6"));

	// text view
// segmentation fault
/*	gdata->textView = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "textDisplay"));
	GtkTextBuffer *buffer;
    GtkTextIter start, end;
    GtkTextIter iter;

	gtk_text_buffer_create_tag(buffer, "gap","pixels_above_lines", 30, NULL);
	gtk_text_buffer_create_tag(buffer, "lmarg","left_margin", 5, NULL);

	gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
	gtk_text_buffer_insert(buffer, &iter, "Plain text\n", 0);

	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gdata->textView));*/
// end

	// initialize entry
	g_signal_connect(GTK_ENTRY(gdata->entry_inputWidth), "activate", G_CALLBACK(entry_activated), NULL);
	g_signal_connect(GTK_ENTRY(gdata->entry_inputHeight), "activate", G_CALLBACK(entry_activated), NULL);
	g_signal_connect(GTK_ENTRY(gdata->entry_input3), "activate", G_CALLBACK(entry_activated), NULL);
	g_signal_connect(GTK_ENTRY(gdata->entry_input4), "activate", G_CALLBACK(entry_activated), NULL);

	// Progress bar
	gdata->progressBar = GTK_WIDGET(gtk_builder_get_object(gdata->builder, "testProgressBar"));
}

void stepProgressBar()
{
	GLOBAL_DATA *gdata = GetGlobalData();

	/*Get the current progress*/
	gdouble fraction;
	fraction = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (gdata->progressBar));

	/*Increase the bar by 10% each time this function is called*/
	fraction += 0.1;

	/*Fill in the bar with the new fraction*/
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gdata->progressBar), fraction);

	/*Fill in the given fraction of the bar. Has to be between 0.0-1.0 inclusive*/
	gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gdata->progressBar), fraction);
}










