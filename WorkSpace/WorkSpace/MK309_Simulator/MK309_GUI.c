 /******************************************************************************* 
 
   FILE: MK309_GUI.c
 
   DESCRIPTION:
   This module contains the functions Holder of ALL the MK309 GUI, widgets, 
   handle button clicks, combo box.  
   This file was created in conjunction with the SPAWAR Software Coding 
   Guidelines document.
 
   FUNCTIONS:
   TODO static void destroy(GtkWidget *widget, gpointer data)
   static void on_combo_RangeScale_changed ()
   int main(int argc, char *argv[])
 
   FILE CHANGE LIST:
   05/18/18 (ARF) ALways changing
 
*******************************************************************************/



// User Entry handlers
/*void entry_activated_CQO(GtkWidget *entry, gpointer data)
{
	const gchar *inputString =  gtk_entry_get_text(GTK_ENTRY(entry));
        gint i = atoi(inputString);
        
        g_print("TEST: %d\n", i);
        
	/*if ( (inputString >= 0) && (inputString <= 360) )
	{
		g_print("MEETS Correct Entry text: '%ld'\n", inputString);
	}
	else
	{
		g_print("\n ERROR - invalid input for CQO");
	}	
}*/


/******************************************************************************* 
   FUNCTION: on_btn_set_clicked()
 
   DESCRIPTION:                                                               
	Enables all the different user entry widgets

 
   CALL WITH:                                                                 
      widget       Is the GTK widget handle for the combo box object.
      user_data	   Data that is given by the user.
 
   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/18/18 (AF) Initial Coding
 
*******************************************************************************/
void on_btn_set_clicked()
{
    MK309_TMA *TMA_item = GetTMAitems(); 
    GUI_WIDGETS *gwidget = GetGuiWidgets();

    printf("\nSet button clicked\n");
    
    gtk_text_buffer_set_text(gwidget->textBuffer, "\nSet button clicked",-1);
    gtk_widget_set_sensitive(gwidget->entry_CQO, TRUE);
    gtk_widget_set_sensitive(gwidget->entry_DMHO, TRUE);
    gtk_widget_set_sensitive(gwidget->entry_CT, TRUE);
    gtk_widget_set_sensitive(gwidget->entry_DMHT, TRUE);
    gtk_widget_set_sensitive(gwidget->entry_Range, TRUE);
    gtk_widget_set_sensitive(gwidget->entry_Bearing, TRUE);  
        
    tma_main();
}

/******************************************************************************* 
   FUNCTION: on_btn_play_clicked()
 
   DESCRIPTION:                                                               
	Records when the range scale combo box selected item is changed. 
 
   CALL WITH:                                                                 
      widget       Is the GTK widget handle for the combo box object.
      user_data	   Data that is given by the user.
 
   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/18/18 (AF) Initial Coding
 
*******************************************************************************/
void on_btn_play_clicked()
{
    printf("\n************** Play button clicked **************\n");
    
    GUI_WIDGETS *gwidget = GetGuiWidgets();
    MK309_TMA *TMA_item = GetTMAitems(); 
    OWNSHIP *ownship = GetOwnshipItems();
	TARGET *target = GetTargetItems();
    
    gtk_text_buffer_set_text(gwidget->textBuffer, "\nPlay button clicked",-1);

    gtk_widget_set_sensitive(gwidget->entry_CQO, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_DMHO, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_CT, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_DMHT, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_Range, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_Bearing, FALSE);
    
    TMA_item->CQO_input = atoi (gtk_entry_get_text(GTK_ENTRY(gwidget->entry_CQO)));
    TMA_item->DMHO_input = atoi (gtk_entry_get_text(GTK_ENTRY(gwidget->entry_DMHO)));
    TMA_item->CT_input = atoi (gtk_entry_get_text(GTK_ENTRY(gwidget->entry_CT)));
    TMA_item->DMHT_input = atoi (gtk_entry_get_text(GTK_ENTRY(gwidget->entry_DMHT)));
    TMA_item->Range_input = atoi (gtk_entry_get_text(GTK_ENTRY(gwidget->entry_Range)));
    TMA_item->Bearing_input = atoi (gtk_entry_get_text(GTK_ENTRY(gwidget->entry_Bearing)));
    
    TMA_item->runTMA = checkTMAinputs();
    
    // Make sure that the entry items are a good input and also that 
    //  the graphics thread/window is started
    if ( TMA_item->runTMA && TMA_item->graphicsThreadStarted)
    {
    	g_print("\nUser input correct, continuing to calculate\n\n");
    	
    	ownship->CQO = TMA_item->CQO_input;
    	ownship->DMHO = TMA_item->DMHO_input;
    	target->CT = TMA_item->CT_input;
    	target->DMHT = TMA_item->DMHT_input;
        
        // TODO: RUN CALCULATIONS MUST MOVE
        Calculations();

        RedrawTargetTriangle(250, 355, false);
    }
    else
    {
    	g_print("\nERROR - Incorrect input, please try again!\n");
    }
    
    //TODO: HOOK for TMA calculations if runTMA returns true
    /*g_print("\n\nEntry text CQO: %f\n\n", TMA_item->CQO_input);
    g_print("Entry text DMHO: %f\n\n", TMA_item->DMHO_input);
    g_print("Entry text CT: %f\n\n", TMA_item->CT_input);
    g_print("Entry text DMHT: %f\n\n", TMA_item->DMHT_input);
    g_print("Entry text Range: %f\n\n", TMA_item->Range_input);
    g_print("Entry text Bearing: %f\n\n", TMA_item->Bearing_input);*/
    

}

/******************************************************************************* 
   FUNCTION: on_btn_stop_clicked()
 
   DESCRIPTION:                                                               
	Records when the range scale combo box selected item is changed. 
 
   CALL WITH:                                                                 
      widget       Is the GTK widget handle for the combo box object.
      user_data	   Data that is given by the user.
 
   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/18/18 (AF) Initial Coding
 
*******************************************************************************/
void on_btn_stop_clicked()
{
    GUI_WIDGETS *gwidget = GetGuiWidgets();

    gtk_text_buffer_set_text(gwidget->textBuffer, "\nStop button clicked",-1);
    printf("\nStop button clicked\n");
    RedrawTargetTriangle(400, 300, false);
}

bool checkTMAinputs()
{
    MK309_TMA *TMA_item = GetTMAitems();
    
    if ( ((TMA_item->CQO_input >=0) && (TMA_item->CQO_input<=360)) 
            && ((TMA_item->DMHO_input >=0) && (TMA_item->DMHO_input<=25))
            && ((TMA_item->CT_input >=0) && (TMA_item->CT_input<=360))
            && ((TMA_item->DMHT_input >=0) && (TMA_item->DMHT_input<=25))
            && ((TMA_item->Range_input >=0) && (TMA_item->Range_input<=20))
            && ((TMA_item->Bearing_input >=0) && (TMA_item->Bearing_input<=360)) )
    {
       return TRUE; 
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************* 
   FUNCTION: on_combo_RangeScale_changed
 
   DESCRIPTION:                                                               
	Records when the range scale combo box selected item is changed. 
 
   CALL WITH:                                                                 
      widget       Is the GTK widget handle for the combo box object.
      user_data	   Data that is given by the user.
 
   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/18/18 (AF) Initial Coding
 
*******************************************************************************/
void on_combo_RangeScale_changed (GtkComboBox *widget, gpointer user_data)
 {
    MK309_TMA *TMA_item = GetTMAitems();
    GtkComboBox *combo_box = widget;

    if (gtk_combo_box_get_active (combo_box) != 6) 
    {
        TMA_item->RangeScale = atoi (gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT(combo_box)));
        g_print ("Range Scale = %d \n", TMA_item->RangeScale);
        
        if ( TMA_item->runTMA )
        {
            RedrawTargetTriangle(400, 300, false);
        }
    }
}

/******************************************************************************* 
   FUNCTION: destroy(GtkWidget *widget, gpointer data)
 
   DESCRIPTION:                                                               
	Correctly destroys/kills the main window 
 
   CALL WITH:                                                                 
      widget       The Gtk widget that is going to get destroyed.
      data         Any needed data that is associated with the widget above.
 
   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/15/18 (AF) Initial Coding
 
*******************************************************************************/
static void destroy(GtkWidget *widget, gpointer data)
{
    MK309_TMA *TMA_item = GetTMAitems();

    TMA_item->closeWindow = true;

    gtk_main_quit();
}

/******************************************************************************* 
   FUNCTION: InitWidgets()
 
   DESCRIPTION:                                                               
	Initializes all widgets that are needed for the MK309 GUI 
 
   CALL WITH:                                                                 
      nothing

   RETURN:                                                                    
      Nothing
 
   CHANGE LIST:
   05/15/18 (AF) Initial Coding
 
*******************************************************************************/
void InitWidgets()
{
    GUI_WIDGETS *gwidget = GetGuiWidgets();
    MK309_TMA *TMA_item = GetTMAitems();

    TMA_item->closeWindow = false;
    TMA_item->graphicsThreadStarted = false;

    gwidget->builder = gtk_builder_new();
    gtk_builder_add_from_file( gwidget->builder, "MK309_Simulator2.glade", NULL);

    gwidget->window = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "MainWindow"));
    gtk_builder_connect_signals(gwidget->builder, NULL);

    gwidget->btn_set = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "btn_set"));
    gwidget->rangeScaleCombo = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "combo_RangeScale"));

    // init all entry widgets
    gwidget->entry_CQO = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "entry_CQO"));
    gwidget->entry_DMHO = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "entry_DMHO"));
    gwidget->entry_CT = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "entry_CT"));
    gwidget->entry_DMHT = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "entry_DMHT"));
    gwidget->entry_Range = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "entry_RANGE"));
    gwidget->entry_Bearing = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "entry_BEARING"));

    // Initialize text view widget
    gwidget->textBuffer = gtk_text_buffer_new(NULL);

    gwidget->text_view = GTK_WIDGET(gtk_builder_get_object(gwidget->builder, "txt_Logging"));
    gtk_text_view_set_buffer(GTK_TEXT_VIEW(gwidget->text_view), gwidget->textBuffer );
    gtk_text_buffer_set_text(gwidget->textBuffer, "\nMK309 Simulator",-1);
    
    // Disables widgets until SET button is pressed
    gtk_widget_set_sensitive(gwidget->entry_CQO, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_DMHO, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_CT, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_DMHT, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_Range, FALSE);
    gtk_widget_set_sensitive(gwidget->entry_Bearing, FALSE);
   
    g_signal_connect (gwidget->rangeScaleCombo,"changed", G_CALLBACK (on_combo_RangeScale_changed), NULL);

    gtk_combo_box_set_active (GTK_COMBO_BOX (gwidget->rangeScaleCombo), 0);

    g_object_unref(gwidget->builder);

    // correctly closes window, terminates thread
    g_signal_connect(G_OBJECT(gwidget->window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show(gwidget->window);
    
}





