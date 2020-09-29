
#ifndef HEADS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" //extern code
#include <gtk/gtk.h>
#pragma GCC diagnostic pop
#include <json-glib/json-glib.h>
#else
#include "inc/gtk.h"
#include "inc/json.h"
#define NULL ((void*)0)
enum{FALSE=0!=0,TRUE=1==1};
#endif

struct option{
	char*name;
	char*help;
};
#define number_of_options 3
struct stk{
	char*file;
	JsonParser*json;
	struct option options[number_of_options];
	GtkWindow*main_win;
};
enum{width_id,height_id,file_id};
#define help_text "Launch the program with the file options.\n\
e.g. builder example.json"

static void help_popup(struct stk*st){
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Help",
			    st->main_win, (GtkDialogFlags)(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL),
			    "_OK",GTK_RESPONSE_NONE,NULL);
	int w;int h;
	gtk_window_get_size (st->main_win,&w,&h);
	gtk_window_set_default_size((GtkWindow*)dialog,w,h);
	g_signal_connect_data (dialog,"response",G_CALLBACK (gtk_window_destroy),
	                       NULL,NULL,(GConnectFlags)0);
	//
	GtkWidget*text = gtk_text_view_new ();
	gtk_text_view_set_editable((GtkTextView*)text, FALSE);
	GtkTextBuffer *text_buffer = gtk_text_view_get_buffer ((GtkTextView*)text);
	gtk_text_buffer_set_text (text_buffer,help_text,sizeof(help_text)-1);
	//
	GtkTextIter it;
	gtk_text_buffer_get_end_iter(text_buffer,&it);
	gtk_text_buffer_insert(text_buffer,&it,"\n\nOptions:",-1);
	for(unsigned int i=0;i<number_of_options;i++){
		gtk_text_buffer_insert(text_buffer,&it,"\n",1);
		gtk_text_buffer_insert(text_buffer,&it,st->options[i].name,-1);
		gtk_text_buffer_insert(text_buffer,&it,"=",1);
		gtk_text_buffer_insert(text_buffer,&it,st->options[i].help,-1);
	}
	//
	GtkWidget*box=gtk_dialog_get_content_area((GtkDialog*)dialog);
	GtkWidget *scrolled_window = gtk_scrolled_window_new ();
	gtk_scrolled_window_set_child ((GtkScrolledWindow*)scrolled_window,text);
	gtk_box_append((GtkBox*)box, scrolled_window);
	gtk_widget_set_hexpand(scrolled_window,TRUE);
	gtk_widget_set_vexpand(scrolled_window,TRUE);
	gtk_widget_show (dialog);
}
static void save_json(struct stk*st){
	JsonGenerator *j=json_generator_new ();
	json_generator_set_root (j, json_parser_get_root(st->json));
	json_generator_to_file (j, st->file, NULL);
	g_object_unref(j);
}
static void main_file(struct stk*st,GtkWidget*window){
	st->json=json_parser_new();
	//if
	json_parser_load_from_file(st->json,st->file,NULL);
	JsonNode* root = json_parser_get_root(st->json);
	JsonObject *object = json_node_get_object(root);
	int width = json_object_get_int_member(object, st->options[width_id].name);
	int height = json_object_get_int_member(object, st->options[height_id].name);
	gtk_window_set_default_size ((GtkWindow*) window, width, height);
	st->file=g_strdup(json_object_get_string_member(object, st->options[file_id].name));
	json_parser_load_from_file(st->json,st->file,NULL);
}
static void activate(GtkApplication* app,struct stk*st){
	GtkWidget *window = gtk_application_window_new (app);
	st->main_win=(GtkWindow*)window;
	main_file(st,window);
	gtk_window_set_title((GtkWindow*)window,"Builder");
	GtkWidget*box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget*b;
	b=gtk_button_new_with_label("Build");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",NULL,NULL,NULL,(GConnectFlags)0);
	b=gtk_button_new_with_label("Save");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (save_json),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Help");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (help_popup),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Exit");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (g_application_quit),app,NULL,G_CONNECT_SWAPPED);
	GtkWidget*f=gtk_frame_new(NULL);
	GtkWidget*bx=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_append((GtkBox*)bx,f);
	gtk_box_append((GtkBox*)bx,box);
	gtk_window_set_child((GtkWindow*)window, bx);
	gtk_widget_set_hexpand(f,TRUE);
	gtk_widget_set_vexpand(f,TRUE);
	gtk_widget_set_halign(box,GTK_ALIGN_CENTER);
	gtk_widget_set_valign(box,GTK_ALIGN_BASELINE);
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
	st.options[width_id].name="width";st.options[width_id].help="Window width in pixels";
	st.options[height_id].name="height";st.options[height_id].help="Window height in pixels";
	st.options[file_id].name="file";st.options[file_id].help="Parse a program file";
	GtkApplication *app;
	app = gtk_application_new (NULL, G_APPLICATION_HANDLES_COMMAND_LINE);//G_APPLICATION_FLAGS_NONE
	g_signal_connect_data (app, "activate", G_CALLBACK (activate), &st, NULL, (GConnectFlags) 0);
	g_signal_connect_data (app, "command-line", G_CALLBACK (command_line), NULL, NULL, (GConnectFlags) 0);
	int status=g_application_run ((GApplication*)app, argc, argv);
	g_object_unref (app);
	g_free(st.file);
	g_object_unref(st.json);
	return status;
}
