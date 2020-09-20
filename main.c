
#ifndef HEADS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" //extern code
#include <gtk/gtk.h>
#pragma GCC diagnostic pop
#else
#include "inc/gtk.h"
#define NULL 0
#endif

static void
activate (GtkApplication* app)
{
	GtkWidget *window = gtk_application_window_new (app);
	GtkWidget*box=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_append((GtkBox*)box,gtk_label_new("Objects"));
	gtk_box_append((GtkBox*)box,gtk_button_new());
	gtk_window_set_child((GtkWindow*)window, box);
	gtk_widget_show(window);
}
int main(){
//	gdk_disable_multidevice();//cannot vnc without this
	GtkApplication *app;
	app = gtk_application_new (NULL, G_APPLICATION_FLAGS_NONE);
	g_signal_connect_data (app, "activate", G_CALLBACK (activate), NULL, NULL, (GConnectFlags) 0);
	int status=g_application_run ((GApplication*)app, 0, NULL);
	g_object_unref (app);
	return status;
}
