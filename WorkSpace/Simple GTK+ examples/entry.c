#include <gtk/gtk.h>

static void entry_activated(GtkWidget *entry, gpointer data)
{
    g_print("Entry text: '%s'\n", gtk_entry_get_text(GTK_ENTRY(entry)));
}

void print_msg(GtkWidget *widget, gpointer window) 
{
	g_printf("Button clicked\n");
}

int main(int argc, char *argv[]) 
{
	GtkWidget *window;

	GtkWidget *table;
	GtkWidget *halign;
	GtkWidget *button;

	GtkWidget *label1;
	GtkWidget *label2;
	GtkWidget *label3;

	GtkWidget *entry1;
	GtkWidget *entry2;
	GtkWidget *entry3;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), "GtkEntry");
	gtk_container_set_border_width(GTK_CONTAINER(window), 100);

	button = gtk_button_new_with_mnemonic("_Button");
	g_signal_connect(button, "clicked",
	G_CALLBACK(print_msg), NULL);

	halign = gtk_alignment_new(0, 0, 0, 0);

	table = gtk_table_new(3, 2, FALSE);
	gtk_container_add(GTK_CONTAINER(window), table);
	label1 = gtk_label_new("Name");
	label2 = gtk_label_new("Age");
	label3 = gtk_label_new("Occupation");

	gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(table), label3, 0, 1, 2, 3,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	entry1 = gtk_entry_new();
	entry2 = gtk_entry_new();
	entry3 = gtk_entry_new();

	gtk_container_add(GTK_CONTAINER(halign), button);
	gtk_container_add(GTK_CONTAINER(window), halign);

	gtk_entry_set_text(GTK_ENTRY(entry1), "April Freier");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry1), "Placeholder Text");
    g_signal_connect(GTK_ENTRY(entry1), "activate", G_CALLBACK(entry_activated), NULL);

	gtk_entry_set_text(GTK_ENTRY(entry2), "26");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry2), "Placeholder Text");
    g_signal_connect(GTK_ENTRY(entry2), "activate", G_CALLBACK(entry_activated), NULL);

	gtk_entry_set_text(GTK_ENTRY(entry3), "Engineer");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry3), "Placeholder Text");
    g_signal_connect(GTK_ENTRY(entry3), "activate", G_CALLBACK(entry_activated), NULL);


	gtk_table_attach(GTK_TABLE(table), entry1, 1, 2, 0, 1,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	gtk_table_attach(GTK_TABLE(table), entry2, 1, 2, 1, 2,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	gtk_table_attach(GTK_TABLE(table), entry3, 1, 2, 2, 3,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	gtk_table_attach(GTK_TABLE(table), button, 1, 2, 4, 4,
	GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

	gtk_widget_show_all(window);

	g_signal_connect(window, "destroy",
	G_CALLBACK(gtk_main_quit), NULL);
	gtk_main();

	return 0;
}
