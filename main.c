
#ifndef HEADS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" //extern code
#include <gtk/gtk.h>
#include <json-glib/json-glib.h>
#pragma GCC diagnostic pop
#else
#include "inc/gtk.h"
#include "inc/json.h"
#define NULL ((void*)0)
#endif

struct stk{
	char*file;
	JsonParser*json;
};

static void parse_json(struct stk*st){
	st->json=json_parser_new();
	//if
	json_parser_load_from_file(st->json,st->file,NULL);
}
static void save_json(struct stk*st){
	JsonGenerator *j=json_generator_new ();
	json_generator_set_root (j, json_parser_get_root(st->json));
	json_generator_to_file (j, st->file, NULL);
	g_object_unref(j);
}
static void activate(GtkApplication* app,struct stk*st){
	GtkWidget *window = gtk_application_window_new (app);
	GtkWidget*box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget*b;
	b=gtk_button_new_with_label("Open");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (parse_json),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Save");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (save_json),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Exit");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (g_application_quit),app,NULL,G_CONNECT_SWAPPED);
	gtk_window_set_child((GtkWindow*)window, box);
	gtk_widget_show(window);
}
static int
command_line (GApplication *a)//,GApplicationCommandLine *cmdline
{
	g_application_activate(a);
	return EXIT_SUCCESS;
}
int main(int argc,char**argv){
	struct stk st;
	st.file=argv[1];
	GtkApplication *app;
	app = gtk_application_new (NULL, G_APPLICATION_HANDLES_COMMAND_LINE);//G_APPLICATION_FLAGS_NONE
	g_signal_connect_data (app, "activate", G_CALLBACK (activate), &st, NULL, (GConnectFlags) 0);
	g_signal_connect_data (app, "command-line", G_CALLBACK (command_line), NULL, NULL, (GConnectFlags) 0);
	int status=g_application_run ((GApplication*)app, argc, argv);
	g_object_unref (app);
	g_object_unref(st.json);
	return status;
}
