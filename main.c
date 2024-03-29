
#ifndef HEADS
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" //extern code
#include <gtk/gtk.h>
#pragma GCC diagnostic pop
#include <json-glib/json-glib.h>
#else
#include "inc/gtk.h"
#include "inc/json.h"
#include "inc/stdbool.h"
#include "inc/stdlib.h"
#include "inc/stdio.h"
#include "inc/unistd.h"
#define NULL ((void*)0)
enum{FALSE=0!=0,TRUE=1==1};
#endif

struct option{
	char*name;
	char*help;
};
#define number_of_options 4
#define number_of_options_proj 16
#define number_of_options_proj_src 2
struct stk{
	char*file;
	JsonParser*json;JsonParser*jsonp;JsonParser*jsons;
	struct option options[number_of_options];
	struct option options_proj[number_of_options_proj];
	struct option options_proj_src[number_of_options_proj_src];
	GtkWindow*main_win;GtkTreeView*tree;GtkWidget*entry_add;
};
enum{width_id,height_id,folder_id,file_id};
enum{cdir_id,comp_id,dfile_id,dex_id,man_id,res_id,pakf_id,dbg_id,pak_id,upd_id
	,sigf_id,sig_id,inst_id,times_id,edit_id,srcs_id};
enum {
  SRC_ITEM = 0,
  CLASS_ITEM = 1,
  N_COLUMNS
};
#define help_text "Launch the program with the file options.\n\
e.g. builder example.json"

static void help_popup(struct stk*st){
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Help",
			st->main_win, (GtkDialogFlags)(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL),
			"_OK",GTK_RESPONSE_NONE,NULL);
	int w;int h;
	gtk_window_get_default_size (st->main_win,&w,&h);
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
	gtk_text_buffer_insert(text_buffer,&it,"\n\nMain options (see example.json):",-1);
	for(unsigned int i=0;i<number_of_options;i++){
		gtk_text_buffer_insert(text_buffer,&it,"\n",1);
		gtk_text_buffer_insert(text_buffer,&it,st->options[i].name,-1);
		gtk_text_buffer_insert(text_buffer,&it,"=",1);
		gtk_text_buffer_insert(text_buffer,&it,st->options[i].help,-1);
	}
	gtk_text_buffer_insert(text_buffer,&it,"\n\nProject options (see example_proj.json):",-1);
	for(unsigned int i=0;i<number_of_options_proj;i++){
		gtk_text_buffer_insert(text_buffer,&it,"\n",1);
		gtk_text_buffer_insert(text_buffer,&it,st->options_proj[i].name,-1);
		gtk_text_buffer_insert(text_buffer,&it,"=",1);
		gtk_text_buffer_insert(text_buffer,&it,st->options_proj[i].help,-1);
		if(i==srcs_id){
			for(unsigned int j=0;j<number_of_options_proj_src;j++){
				gtk_text_buffer_insert(text_buffer,&it,"\n\t",2);
				gtk_text_buffer_insert(text_buffer,&it,st->options_proj_src[j].name,-1);
				gtk_text_buffer_insert(text_buffer,&it,"=",1);
				gtk_text_buffer_insert(text_buffer,&it,st->options_proj_src[j].help,-1);
			}
		}
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
	JsonNode* root = json_parser_get_root(st->jsonp);
	JsonObject *object = json_node_get_object(root);
	const gchar*srcs=json_object_get_string_member(object, st->options_proj[srcs_id].name);
	root = json_parser_get_root(st->jsons);
	JsonGenerator *j=json_generator_new ();
	json_generator_set_root (j, root);
	json_generator_to_file (j, srcs, NULL);
	g_object_unref(j);
}
static void file_stamp(const char*fname,JsonObject*gen_obj){
	GFile*f=g_file_new_for_path(fname);
	GFileInfo*fi=g_file_query_info(f,G_FILE_ATTRIBUTE_TIME_MODIFIED,
	 G_FILE_QUERY_INFO_NONE,NULL,NULL);
	g_object_unref(f);
	GDateTime*dt=g_file_info_get_modification_date_time(fi);
	gint64 t=g_date_time_to_unix(dt);
	g_object_unref(fi);
	json_object_set_int_member(gen_obj,fname,t);
}
static bool file_is_mod(const char*f,JsonObject*tobj){
	GFile*fl=g_file_new_for_path(f);
	GFileInfo*fi=g_file_query_info(fl,G_FILE_ATTRIBUTE_TIME_MODIFIED,
	 G_FILE_QUERY_INFO_NONE,NULL,NULL);
	g_object_unref(fl);
	//if fi==NULL
	GDateTime*dt=g_file_info_get_modification_date_time(fi);
	gint64 t=g_date_time_to_unix(dt);
	g_object_unref(fi);
	return t>json_object_get_int_member(tobj,f);
}
static void dir_stamp(const char*path,JsonObject*gen_obj){
	GFile*f=g_file_new_for_path(path);
	GFileEnumerator* enumerate = g_file_enumerate_children(f,
	 G_FILE_ATTRIBUTE_STANDARD_TYPE "," G_FILE_ATTRIBUTE_STANDARD_NAME,
	 G_FILE_QUERY_INFO_NONE,NULL,NULL);
	g_object_unref(f);
	for(;;){
		GFileInfo*i=g_file_enumerator_next_file(enumerate,NULL,NULL);
		if(i==NULL)break;
		char*name_path=g_build_filename(path,g_file_info_get_name(i),NULL);
		if(g_file_info_get_file_type(i)==G_FILE_TYPE_DIRECTORY)
			dir_stamp(name_path,gen_obj);
		else file_stamp(name_path,gen_obj);
		g_object_unref(i);
		g_free(name_path);
	}
	g_file_enumerator_close(enumerate,NULL,NULL);
}
static void dir_stamp_copy(const char*path,JsonObject*gen_obj,JsonObject*tobj){
	GFile*f=g_file_new_for_path(path);
	GFileEnumerator* enumerate = g_file_enumerate_children(f,
	 G_FILE_ATTRIBUTE_STANDARD_TYPE "," G_FILE_ATTRIBUTE_STANDARD_NAME,
	 G_FILE_QUERY_INFO_NONE,NULL,NULL);
	g_object_unref(f);
	for(;;){
		GFileInfo*i=g_file_enumerator_next_file(enumerate,NULL,NULL);
		if(i==NULL)break;
		char*name_path=g_build_filename(path,g_file_info_get_name(i),NULL);
		if(g_file_info_get_file_type(i)==G_FILE_TYPE_DIRECTORY)
			dir_stamp_copy(name_path,gen_obj,tobj);
		else json_object_set_int_member(gen_obj,name_path,json_object_get_int_member(tobj,name_path));
		g_object_unref(i);
		g_free(name_path);
	}
	g_file_enumerator_close(enumerate,NULL,NULL);
}
static bool dir_is_mod(const char*path,JsonObject*tobj){
	GFile*f=g_file_new_for_path(path);
	GFileEnumerator* enumerate = g_file_enumerate_children(f,
	 G_FILE_ATTRIBUTE_STANDARD_TYPE "," G_FILE_ATTRIBUTE_STANDARD_NAME,
	 G_FILE_QUERY_INFO_NONE,NULL,NULL);
	g_object_unref(f);
	bool a=FALSE;
	for(;;){
		GFileInfo*i=g_file_enumerator_next_file(enumerate,NULL,NULL);
		if(i==NULL)break;
		char*name_path=g_build_filename(path,g_file_info_get_name(i),NULL);
		if(g_file_info_get_file_type(i)==G_FILE_TYPE_DIRECTORY)
			a=dir_is_mod(name_path,tobj);
		else a=file_is_mod(name_path,tobj);
		g_object_unref(i);
		g_free(name_path);
		if(a)break;
	}
	g_file_enumerator_close(enumerate,NULL,NULL);
	return a;
}
static int systemverb(char*p){
	puts(p);
	return system(p);
}
static bool proj_compile(JsonObject*object,char**p,int*size,struct stk*st,JsonObject*gen_obj){
	const gchar*d=json_object_get_string_member(object,st->options_proj[cdir_id].name);
	const gchar*c=json_object_get_string_member(object,st->options_proj[comp_id].name);
	JsonNode*root = json_parser_get_root(st->jsons);
	JsonArray*a=json_node_get_array(root);
	guint n=json_array_get_length(a);
	for(guint i=0;i<n;i++){
		JsonObject*s=json_array_get_object_element(a,i);
		const gchar*src=json_object_get_string_member(s,st->options_proj_src[SRC_ITEM].name);
		const gchar*classpath=json_object_get_string_member(s,st->options_proj_src[CLASS_ITEM].name);
		int sz=snprintf(NULL,0,c,d,classpath,src);
		if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
		sprintf(*p,c,d,classpath,src);
		if(systemverb(*p)!=EXIT_SUCCESS)return FALSE;
		file_stamp(src,gen_obj);
	}
	return TRUE;
}
//-1 wrong/0 nothing/1 new ok
static int proj_compile_mod(JsonObject*object,char**p,int*size,struct stk*st,JsonObject*gen_obj,JsonObject*tobj){
	const gchar*d=json_object_get_string_member(object,st->options_proj[cdir_id].name);
	const gchar*c=json_object_get_string_member(object,st->options_proj[comp_id].name);
	JsonNode*root = json_parser_get_root(st->jsons);
	JsonArray*a=json_node_get_array(root);
	guint n=json_array_get_length(a);
	bool good=TRUE;bool mod=FALSE;
	for(guint i=0;i<n;i++){
		JsonObject*s=json_array_get_object_element(a,i);
		const gchar*src=json_object_get_string_member(s,st->options_proj_src[SRC_ITEM].name);
		if(file_is_mod(src,tobj)){
			const gchar*classpath=json_object_get_string_member(s,st->options_proj_src[CLASS_ITEM].name);
			int sz=snprintf(NULL,0,c,d,classpath,src);
			if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
			sprintf(*p,c,d,classpath,src);
			if(systemverb(*p)==EXIT_SUCCESS){
				file_stamp(src,gen_obj);
				mod=TRUE;
				continue;
			}
			good=FALSE;
		}
		json_object_set_int_member(gen_obj,src,json_object_get_int_member(tobj,src));
	}
	if(good&&mod)return 1;
	if(good)return 0;
	return -1;
}
static bool proj_dex(JsonObject*object,char**p,int*size,struct stk*st){
	const gchar*d=json_object_get_string_member(object,st->options_proj[cdir_id].name);
	const gchar*dx_f=json_object_get_string_member(object,st->options_proj[dfile_id].name);
	const gchar*dx=json_object_get_string_member(object,st->options_proj[dex_id].name);
	int sz=snprintf(NULL,0,dx,dx_f,d);
	if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
	sprintf(*p,dx,dx_f,d);
	return systemverb(*p)==EXIT_SUCCESS;
}
static bool proj_pak(JsonObject*object,char**p,int*size,struct stk*st,JsonObject*gen_obj){
	const gchar*mf=json_object_get_string_member(object, st->options_proj[man_id].name);
	const gchar*rd=json_object_get_string_member(object, st->options_proj[res_id].name);
	const gchar*pf=json_object_get_string_member(object,st->options_proj[pakf_id].name);
	const gchar*pk=json_object_get_string_member(object,st->options_proj[pak_id].name);
	const gchar*dg=json_object_get_string_member(object,st->options_proj[dbg_id].name);
	int sz=snprintf(NULL,0,pk,mf,rd,pf,dg);
	if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
	sprintf(*p,pk,mf,rd,pf,dg);
	if(systemverb(*p)==EXIT_SUCCESS){
		file_stamp(mf,gen_obj);
		dir_stamp(rd,gen_obj);
		return TRUE;
	}
	return FALSE;
}
static int proj_pak_mod(JsonObject*object,char**p,int*size,struct stk*st,JsonObject*gen_obj,JsonObject*tobj){
	const gchar*mf=json_object_get_string_member(object, st->options_proj[man_id].name);
	const gchar*rd=json_object_get_string_member(object, st->options_proj[res_id].name);
	if(file_is_mod(mf,tobj)||dir_is_mod(rd,tobj)){
		const gchar*pf=json_object_get_string_member(object,st->options_proj[pakf_id].name);
		const gchar*pk=json_object_get_string_member(object,st->options_proj[pak_id].name);
		const gchar*dg=json_object_get_string_member(object,st->options_proj[dbg_id].name);
		int sz=snprintf(NULL,0,pk,mf,rd,pf,dg);
		if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
		sprintf(*p,pk,mf,rd,pf,dg);
		if(systemverb(*p)==EXIT_SUCCESS){
			file_stamp(mf,gen_obj);
			dir_stamp(rd,gen_obj);
			return 1;
		}
		return -1;
	}
	json_object_set_int_member(gen_obj,mf,json_object_get_int_member(tobj,mf));
	dir_stamp_copy(rd,gen_obj,tobj);
	return 0;
}
static bool proj_upd(JsonObject*object,char**p,int*size,struct stk*st){//;exit 0
	const gchar*pf=json_object_get_string_member(object,st->options_proj[pakf_id].name);
	const gchar*dx_f=json_object_get_string_member(object,st->options_proj[dfile_id].name);
	const gchar*ad=json_object_get_string_member(object,st->options_proj[upd_id].name);
	int sz=snprintf(NULL,0,ad,pf,dx_f);
	if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
	sprintf(*p,ad,pf,dx_f);
	return systemverb(*p)==EXIT_SUCCESS;
}
static bool proj_sig(JsonObject*object,char**p,int*size,struct stk*st){
	const gchar*pf=json_object_get_string_member(object,st->options_proj[pakf_id].name);
	const gchar*sf=json_object_get_string_member(object,st->options_proj[sigf_id].name);
	const gchar*sg=json_object_get_string_member(object,st->options_proj[sig_id].name);
	int sz=snprintf(NULL,0,sg,pf,sf);
	if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
	sprintf(*p,sg,pf,sf);
	return systemverb(*p)==EXIT_SUCCESS;
}
static void proj_inst(JsonObject*object,char**p,int*size,struct stk*st){
	const gchar*sf=json_object_get_string_member(object,st->options_proj[sigf_id].name);
	const gchar*in=json_object_get_string_member(object,st->options_proj[inst_id].name);
	int sz=snprintf(NULL,0,in,sf);
	if(sz>*size){*p=(char*)g_realloc(*p,sz+1);*size=sz;}
	sprintf(*p,in,sf);
	systemverb(*p);
}
static void write_temp(JsonObject*obj,const gchar*timestamp_file){
	JsonNode*nod=json_node_alloc();
	json_node_init_object(nod,obj);
	JsonGenerator*gen=json_generator_new();
	json_generator_set_root (gen,nod);
	json_generator_to_file (gen,timestamp_file,NULL);
	//g_object_unref(obj);is already node
	json_node_free(nod);
	g_object_unref(gen);
}
static void build_proj(struct stk*st){
	JsonNode*root = json_parser_get_root(st->jsonp);
	JsonObject*object = json_node_get_object(root);
	JsonObject*obj=json_object_new();
	int size=0;char*p=NULL;
	//
	const gchar*timestamp_file=json_object_get_string_member(object, st->options_proj[times_id].name);
	JsonParser*jsont=json_parser_new();
	json_parser_load_from_file(jsont,timestamp_file,NULL);
	root = json_parser_get_root(jsont);
	JsonObject *tobj = json_node_get_object(root);
	//
	int a=proj_compile_mod(object,&p,&size,st,obj,tobj);
	int b=proj_pak_mod(object,&p,&size,st,obj,tobj);
	if(a>-1&&b>-1){
		if(a==1){if(proj_dex(object,&p,&size,st)&&proj_upd(object,&p,&size,st))
			proj_sig(object,&p,&size,st);}
		else if(b==1){
			if(proj_upd(object,&p,&size,st))proj_sig(object,&p,&size,st);}
		proj_inst(object,&p,&size,st);
	}
	g_free(p);
	g_object_unref(jsont);
	write_temp(obj,timestamp_file);
}
static void rebuild_proj(struct stk*st){
	JsonNode*root = json_parser_get_root(st->jsonp);
	JsonObject*object = json_node_get_object(root);
	JsonObject*obj=json_object_new();
	int size=0;char*p=NULL;
	if(proj_compile(object,&p,&size,st,obj)&&proj_pak(object,&p,&size,st,obj)
		&&proj_dex(object,&p,&size,st)&&proj_upd(object,&p,&size,st)
		&&proj_sig(object,&p,&size,st))proj_inst(object,&p,&size,st);
	g_free(p);
	write_temp(obj,json_object_get_string_member(object, st->options_proj[times_id].name));
}
static void edit_call_go(struct stk*st,const char*item_text){
	JsonNode* root = json_parser_get_root(st->jsonp);
	JsonObject *object = json_node_get_object(root);
	const gchar*format=json_object_get_string_member(object, st->options_proj[edit_id].name);
	int sz=snprintf(NULL,0,format,item_text);
	char*a=(char*)g_malloc(sz+1);
	sprintf(a,format,item_text);
	systemverb(a);
	g_free(a);
}
static void edit_call_man(struct stk*st){
	JsonNode* root = json_parser_get_root(st->jsonp);
	JsonObject *object = json_node_get_object(root);
	const gchar*mf=json_object_get_string_member(object, st->options_proj[man_id].name);
	edit_call_go(st,mf);
}
static void main_file(struct stk*st,GtkListStore*ls){
	st->json=json_parser_new();
	json_parser_load_from_file(st->json,st->file,NULL);
	JsonNode* root = json_parser_get_root(st->json);
	JsonObject *object = json_node_get_object(root);
	int width = json_object_get_int_member(object, st->options[width_id].name);
	int height = json_object_get_int_member(object, st->options[height_id].name);
	gtk_window_set_default_size (st->main_win, width, height);
	chdir(json_object_get_string_member(object, st->options[folder_id].name));
	st->jsonp=json_parser_new();
	const gchar*f=json_object_get_string_member(object, st->options[file_id].name);
	json_parser_load_from_file(st->jsonp,f,NULL);
	root = json_parser_get_root(st->jsonp);
	object = json_node_get_object(root);
	const gchar*srcf=json_object_get_string_member(object,st->options_proj[srcs_id].name);
	st->jsons=json_parser_new();
	json_parser_load_from_file(st->jsons,srcf,NULL);
	//
	root = json_parser_get_root(st->jsons);
	JsonArray*a=json_node_get_array(root);
	guint n=json_array_get_length(a);
	for(guint i=0;i<n;i++){
		JsonObject*s=json_array_get_object_element(a,i);
		const gchar*src=json_object_get_string_member(s,st->options_proj_src[SRC_ITEM].name);
		const gchar*cp=json_object_get_string_member(s,st->options_proj_src[CLASS_ITEM].name);
		GtkTreeIter it;
		gtk_list_store_append(ls,&it);
		gtk_list_store_set(ls, &it, SRC_ITEM, src, CLASS_ITEM, cp, -1);
	}
	//cannot click on editable columns g_signal_connect_data((GtkWidget*)st->tree,"row-activated",G_CALLBACK (edit_call),st,NULL,(GConnectFlags)0);
}
static void list_edit(struct stk*st){
	GtkTreeModel*mod;GtkTreeIter it;GtkTreeSelection*sl;
	sl=gtk_tree_view_get_selection(st->tree);
	if(gtk_tree_selection_get_selected (sl,&mod,&it)){
		char*item_text;
		gtk_tree_model_get (mod, &it, SRC_ITEM, &item_text, -1);
		edit_call_go(st,item_text);
		g_free(item_text);
	}
}
static void list_add_response(GtkDialog *dialog,gint response,struct stk*st){
	if(response==GTK_RESPONSE_OK){
		const char*text=gtk_entry_buffer_get_text(gtk_entry_get_buffer((GtkEntry*)st->entry_add));
		GtkTreeModel*mod;GtkTreeIter it;
		GtkTreeSelection *sel=gtk_tree_view_get_selection(st->tree);
		gtk_tree_selection_get_selected (sel,&mod,&it);
		GtkTreeIter i2;
		gtk_list_store_insert_after((GtkListStore*)mod,&i2,&it);
		gtk_list_store_set((GtkListStore*)mod,&i2,SRC_ITEM,text,CLASS_ITEM,"",-1);
		//
		JsonNode*root = json_parser_get_root(st->jsons);
		JsonArray*ar=json_node_get_array(root);
		JsonObject*obj=json_object_new();
		json_object_set_string_member(obj,st->options_proj_src[CLASS_ITEM].name,"");
		json_object_set_string_member(obj,st->options_proj_src[SRC_ITEM].name,text);
		json_array_add_object_element(ar,obj);
		save_json(st);
	}
	gtk_window_destroy((GtkWindow*)dialog);
}
static void list_add(struct stk*st){
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("Add",
			    st->main_win, (GtkDialogFlags)(GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL),
			    "_OK",GTK_RESPONSE_OK,NULL);
	st->entry_add = gtk_entry_new();
	g_signal_connect_data (dialog,"response",G_CALLBACK (list_add_response),
	                       st,NULL,(GConnectFlags)0);
	GtkWidget*box=gtk_dialog_get_content_area((GtkDialog*)dialog);
	gtk_box_append((GtkBox*)box, st->entry_add);
	gtk_widget_show(dialog);
}
static int get_pos_from_model(GtkTreeModel*mod,GtkTreeIter*it){
	GtkTreePath * path = gtk_tree_model_get_path ( mod , it ) ;
	int i= (gtk_tree_path_get_indices ( path ))[0] ;
	gtk_tree_path_free(path);
	return i;
}
static void list_del(struct stk*st){
	GtkTreeModel*mod;GtkTreeIter it;GtkTreeSelection*sl;
	sl=gtk_tree_view_get_selection(st->tree);
	if(gtk_tree_selection_get_selected (sl,&mod,&it)){
		int i=get_pos_from_model(mod,&it);
		JsonNode*root = json_parser_get_root(st->jsons);
		JsonArray*ar=json_node_get_array(root);
		json_array_remove_element(ar,i);
		save_json(st);
		gtk_list_store_remove ((GtkListStore*)mod,&it);
	}
}
static void list_move(struct stk*st,int x,int a){
	JsonNode*root = json_parser_get_root(st->jsons);
	JsonArray*ar=json_node_get_array(root);
	guint n=json_array_get_length(ar);
	JsonArray*arr=json_array_sized_new(n);
	GList*list=json_array_get_elements(ar);
	gpointer p=g_list_nth_data(list,x);
	list=g_list_remove(list,p);
	list=g_list_insert(list,p,x+a);
	GList*lst=list;
	for(;;){
		JsonNode*nd=json_node_copy(lst->data);
		json_array_add_element(arr,nd);
		lst=g_list_next(lst);
		if(lst==NULL)break;
	}
	g_list_free(list);
	json_node_take_array(root,arr);//without copy,here is critical unrefing old elements
	save_json(st);
}
static void list_up(struct stk*st){
	GtkTreeModel*mod;GtkTreeIter it;
	GtkTreeSelection *sel=gtk_tree_view_get_selection(st->tree);
	gtk_tree_selection_get_selected (sel,&mod,&it);
	GtkTreeIter i2=it;
	if(gtk_tree_model_iter_previous(mod,&i2)){
		int i=get_pos_from_model(mod,&it);
		list_move(st,i,-1);
		gtk_list_store_swap((GtkListStore*)mod,&it,&i2);
	}
}
static void list_down(struct stk*st){
	GtkTreeModel*mod;GtkTreeIter it;
	GtkTreeSelection *sel=gtk_tree_view_get_selection(st->tree);
	gtk_tree_selection_get_selected (sel,&mod,&it);
	GtkTreeIter i2=it;
	if(gtk_tree_model_iter_next(mod,&i2)){
		int i=get_pos_from_model(mod,&it);
		list_move(st,i,1);
		gtk_list_store_swap((GtkListStore*)mod,&it,&i2);
	}
}
static void cell_edited(struct stk*st,gchar *path,gchar *new_text, int n){
	GtkTreeModel*mod=gtk_tree_view_get_model(st->tree);
	GtkTreeIter iter;
	gtk_tree_model_get_iter_from_string(mod,&iter,path);
	gtk_list_store_set((GtkListStore*)mod, &iter, n, new_text, -1);
	//
	int i=get_pos_from_model(mod,&iter);
	JsonNode*root = json_parser_get_root(st->jsons);
	JsonArray*ar=json_node_get_array(root);
	JsonObject*s=json_array_get_object_element(ar,i);
	json_object_set_string_member(s,st->options_proj_src[n].name,new_text);
	save_json(st);
}
static void cell_edited_1(struct stk*st,gchar *path,gchar *new_text){
	cell_edited(st,path,new_text,SRC_ITEM);}
static void cell_edited_2(struct stk*st,gchar *path,gchar *new_text){
	cell_edited(st,path,new_text,CLASS_ITEM);}
static void activate(GtkApplication* app,struct stk*st){
	GtkWidget *window = gtk_application_window_new (app);
	st->main_win=(GtkWindow*)window;
	gtk_window_set_title((GtkWindow*)window,"Builder");
	GtkWidget*box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget*b;
	b=gtk_button_new_with_label("Build");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (build_proj),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Rebuild");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (rebuild_proj),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Manifest");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (edit_call_man),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Help");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (help_popup),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("Exit");
	gtk_box_append((GtkBox*)box,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (g_application_quit),app,NULL,G_CONNECT_SWAPPED);
	//
	st->tree=(GtkTreeView*)gtk_tree_view_new();
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer, "text", SRC_ITEM, NULL);
	gtk_tree_view_append_column(st->tree, column);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect_data (renderer, "edited",G_CALLBACK (cell_edited_1),st,NULL,G_CONNECT_SWAPPED);
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes("", renderer, "text", CLASS_ITEM, NULL);
	gtk_tree_view_append_column(st->tree, column);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect_data (renderer, "edited",G_CALLBACK (cell_edited_2),st,NULL,G_CONNECT_SWAPPED);
	gtk_tree_view_set_headers_visible(st->tree,FALSE);
	GtkListStore*ls= gtk_list_store_new(N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(st->tree, (GtkTreeModel*)ls);
	g_object_unref(ls);
	GtkWidget*scroll = gtk_scrolled_window_new ();
	gtk_scrolled_window_set_child ((GtkScrolledWindow*)scroll,(GtkWidget*)st->tree);
	//
	main_file(st,ls);
	//
	GtkWidget*list_b=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	b=gtk_button_new_with_label("\u270E");
	gtk_box_append((GtkBox*)list_b,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (list_edit),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("+");
	gtk_box_append((GtkBox*)list_b,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (list_add),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("-");
	gtk_box_append((GtkBox*)list_b,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (list_del),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("\u2191");
	gtk_box_append((GtkBox*)list_b,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (list_up),st,NULL,G_CONNECT_SWAPPED);
	b=gtk_button_new_with_label("\u2193");
	gtk_box_append((GtkBox*)list_b,b);
	g_signal_connect_data (b, "clicked",G_CALLBACK (list_down),st,NULL,G_CONNECT_SWAPPED);
	GtkWidget*list_box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	gtk_box_append((GtkBox*)list_box,scroll);
	gtk_box_append((GtkBox*)list_box,list_b);
	gtk_widget_set_hexpand(scroll,TRUE);
	//
	GtkWidget*bx=gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_box_append((GtkBox*)bx,list_box);
	gtk_box_append((GtkBox*)bx,box);
	gtk_window_set_child((GtkWindow*)window, bx);
	gtk_widget_set_hexpand(list_box,TRUE);
	gtk_widget_set_vexpand(list_box,TRUE);
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
	if(argc!=2){
		printf("Error: one argument, the file argument, example.json for test\n");
		return EXIT_FAILURE;
	}
	st.file=argv[1];
	st.options[width_id].name="width";st.options[width_id].help="Window width in pixels";
	st.options[height_id].name="height";st.options[height_id].help="Window height in pixels";
	st.options[folder_id].name="folder";st.options[folder_id].help="Project folder";
	st.options[file_id].name="file";st.options[file_id].help="Parse a program file";
	st.options_proj[cdir_id].name="class_dir";st.options_proj[cdir_id].help="Produced classes directory";
	st.options_proj[comp_id].name="compiler";st.options_proj[comp_id].help="Compiler call format";
	st.options_proj[dfile_id].name="dex_file";st.options_proj[dfile_id].help="Dex file name";
	st.options_proj[dex_id].name="dex";st.options_proj[dex_id].help="Dex call format";
	st.options_proj[man_id].name="man_file";st.options_proj[man_id].help="Manifest file";
	st.options_proj[res_id].name="res_dir";st.options_proj[res_id].help="Resources directory";
	st.options_proj[pakf_id].name="pak_file";st.options_proj[pakf_id].help="Package file";
	st.options_proj[dbg_id].name="debug";st.options_proj[dbg_id].help="Debug code";
	st.options_proj[pak_id].name="pak";st.options_proj[pak_id].help="Package call format";
	st.options_proj[upd_id].name="update";st.options_proj[upd_id].help="Update call format";
	st.options_proj[sigf_id].name="sign_file";st.options_proj[sigf_id].help="Sign package file";
	st.options_proj[sig_id].name="sign";st.options_proj[sig_id].help="Sign package call format";
	st.options_proj[inst_id].name="install";st.options_proj[inst_id].help="Install call format";
	st.options_proj[times_id].name="timestamp_file";st.options_proj[times_id].help="Timestamps file";
	st.options_proj[edit_id].name="edit";st.options_proj[edit_id].help="Edit call format";
	st.options_proj[srcs_id].name="sources";st.options_proj[srcs_id].help="Sources for the compiler";
	st.options_proj_src[SRC_ITEM].name="source";st.options_proj_src[SRC_ITEM].help="Source at compiler";
	st.options_proj_src[CLASS_ITEM].name="classpath";st.options_proj_src[CLASS_ITEM].help="Classpath at compiler";
	GtkApplication *app;
	app = gtk_application_new (NULL, G_APPLICATION_HANDLES_COMMAND_LINE);//G_APPLICATION_FLAGS_NONE
	g_signal_connect_data (app, "activate", G_CALLBACK (activate), &st, NULL, (GConnectFlags) 0);
	g_signal_connect_data (app, "command-line", G_CALLBACK (command_line), NULL, NULL, (GConnectFlags) 0);
	int status=g_application_run ((GApplication*)app, argc, argv);
	g_object_unref(st.jsons);
	g_object_unref(st.jsonp);
	g_object_unref(st.json);
	g_object_unref (app);
	return status;
}
