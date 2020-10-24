
typedef char gchar;
typedef int gint;
typedef gint gboolean;
typedef unsigned int gsize;
typedef unsigned int guint;
typedef gsize GType;
typedef unsigned long gulong;
typedef void* gpointer;
typedef const void *gconstpointer;
__extension__ typedef signed long long gint64;

typedef struct _GApplication GApplication;
typedef struct _GDateTime GDateTime;
typedef struct _GCancellable GCancellable;
typedef struct _GError GError;
typedef struct _GFile GFile;
typedef struct _GFileEnumerator GFileEnumerator;
typedef struct _GFileInfo GFileInfo;
typedef struct _GList GList;
typedef struct _GdkEvent GdkEvent;
typedef struct _GtkApplication GtkApplication;
typedef struct _GtkBox GtkBox;
typedef struct _GtkCellRenderer GtkCellRenderer;
typedef struct _GtkDialog GtkDialog;
typedef struct _GtkEntry GtkEntry;
typedef struct _GtkEntryBuffer GtkEntryBuffer;
typedef struct _GtkFrame GtkFrame;
typedef struct _GtkListStore GtkListStore;
typedef struct _GtkScrolledWindow GtkScrolledWindow;
typedef struct _GtkTextBuffer GtkTextBuffer;
typedef struct _GtkTextIter GtkTextIter;
typedef struct _GtkTextView GtkTextView;
typedef struct _GtkTreeIter GtkTreeIter;
typedef struct _GtkTreePath GtkTreePath;
typedef struct _GtkTreeSelection GtkTreeSelection;
typedef struct _GtkTreeView GtkTreeView;
typedef struct _GtkTreeViewColumn GtkTreeViewColumn;
typedef struct _GtkTreeModel GtkTreeModel;
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;

typedef void (*GCallback)(void);
typedef struct _GClosure GClosure;
typedef void (*GClosureNotify)(gpointer data,GClosure *closure);

typedef enum{  G_APPLICATION_HANDLES_COMMAND_LINE = 1 << 3}
 GApplicationFlags;
typedef enum{G_CONNECT_SWAPPED = 1 << 1}
 GConnectFlags;
typedef enum {  G_FILE_QUERY_INFO_NONE = 0}
 GFileQueryInfoFlags;
typedef enum {  G_FILE_TYPE_UNKNOWN = 0,  G_FILE_TYPE_REGULAR,  G_FILE_TYPE_DIRECTORY}
 GFileType;
typedef enum{  GTK_ALIGN_FILL,  GTK_ALIGN_START,  GTK_ALIGN_END,  GTK_ALIGN_CENTER,  GTK_ALIGN_BASELINE}
 GtkAlign;
typedef enum{  GTK_DIALOG_MODAL = 1 << 0,  GTK_DIALOG_DESTROY_WITH_PARENT = 1 << 1}
 GtkDialogFlags;
typedef enum{GTK_ORIENTATION_HORIZONTAL,GTK_ORIENTATION_VERTICAL}
 GtkOrientation;
typedef enum{GTK_RESPONSE_NONE = -1,GTK_RESPONSE_OK = -5}
 GtkResponseType;

#define G_CALLBACK(f) ((GCallback) (f))
#define G_FILE_ATTRIBUTE_TIME_MODIFIED "time::modified"
#define G_FILE_ATTRIBUTE_STANDARD_NAME "standard::name"
#define G_FILE_ATTRIBUTE_STANDARD_TYPE "standard::type"
#define G_TYPE_FUNDAMENTAL_SHIFT (2)
#define G_TYPE_MAKE_FUNDAMENTAL(x) ((GType) ((x) << G_TYPE_FUNDAMENTAL_SHIFT))
#define G_TYPE_STRING G_TYPE_MAKE_FUNDAMENTAL (16)

struct _GList
{
  gpointer data;
  GList *next;
  GList *prev;
};
struct _GtkTextIter {
  gpointer dummy1;
  gpointer dummy2;
  int dummy3;
  int dummy4;
  int dummy5;
  int dummy6;
  int dummy7;
  int dummy8;
  gpointer dummy9;
  gpointer dummy10;
  int dummy11;
  int dummy12;
  int dummy13;
  gpointer dummy14;
};
struct _GtkTreeIter
{
  int stamp;
  gpointer user_data;
  gpointer user_data2;
  gpointer user_data3;
};

void g_application_activate (GApplication *application);
void g_application_quit (GApplication *application);
int g_application_run (GApplication *application,int argc,char **argv);
gchar *g_build_filename (const gchar *first_element, ...) __attribute__((__malloc__)) __attribute__((__sentinel__));
gint64 g_date_time_to_unix (GDateTime *datetime);
GFileEnumerator * g_file_enumerate_children (GFile *file, const char *attributes, GFileQueryInfoFlags flags, GCancellable *cancellable, GError **error);
gboolean g_file_enumerator_close (GFileEnumerator *enumerator,GCancellable *cancellable,GError **error);
GFileInfo *g_file_enumerator_next_file (GFileEnumerator *enumerator, GCancellable *cancellable, GError **error);
GFileType g_file_info_get_file_type (GFileInfo *info);
GDateTime * g_file_info_get_modification_date_time (GFileInfo *info);
const char * g_file_info_get_name (GFileInfo *info);
GFile * g_file_new_for_path (const char *path);
GFileInfo * g_file_query_info (GFile *file,const char *attributes,GFileQueryInfoFlags flags,GCancellable *cancellable,GError **error);
void g_free (gpointer mem);
void g_list_free (GList *list);
GList* g_list_insert (GList *list, gpointer data, gint position) __attribute__((warn_unused_result));
#define g_list_next(list) ((list) ? (((GList *)(list))->next) : NULL)
gpointer g_list_nth_data (GList *list,guint n);
GList* g_list_remove (GList *list, gconstpointer data) __attribute__((warn_unused_result));
gpointer g_malloc (gsize n_bytes) __attribute__((__malloc__)) __attribute__((__alloc_size__(1)));
void g_object_set (gpointer object,const gchar *first_property_name,...) __attribute__((__sentinel__));
void g_object_unref (gpointer object);
gpointer g_realloc(gpointer mem,gsize n_bytes);
gulong g_signal_connect_data (gpointer instance,const gchar *detailed_signal,GCallback c_handler,gpointer data,GClosureNotify destroy_data,GConnectFlags con);
GtkApplication * gtk_application_new (const gchar *application_id, GApplicationFlags flags);
GtkWidget * gtk_application_window_new (GtkApplication *application);
void gtk_box_append (GtkBox *box, GtkWidget *child);
GtkWidget* gtk_box_new (GtkOrientation orientation,int spacing);
GtkWidget* gtk_button_new_with_label (const char *label);
GtkCellRenderer *gtk_cell_renderer_text_new (void);
GtkWidget * gtk_dialog_get_content_area (GtkDialog *dialog);
GtkWidget* gtk_dialog_new_with_buttons (const char *title,GtkWindow *parent,GtkDialogFlags flags,const char *first_button_text,...) __attribute__((__sentinel__));
const char * gtk_entry_buffer_get_text (GtkEntryBuffer *buffer);
GtkEntryBuffer* gtk_entry_get_buffer (GtkEntry *entry);
GtkWidget* gtk_entry_new (void);
void gtk_list_store_append (GtkListStore *list_store, GtkTreeIter *iter);
void gtk_list_store_insert_after (GtkListStore *list_store, GtkTreeIter *iter, GtkTreeIter *sibling);
GtkListStore *gtk_list_store_new (int n_columns, ...);
gboolean gtk_list_store_remove (GtkListStore *list_store, GtkTreeIter *iter);
void gtk_list_store_set (GtkListStore *list_store,GtkTreeIter *iter,...);
void gtk_list_store_swap (GtkListStore *store,GtkTreeIter *a,GtkTreeIter *b);
GtkWidget* gtk_scrolled_window_new (void);
void gtk_scrolled_window_set_child (GtkScrolledWindow *scrolled_window, GtkWidget *child);
void gtk_text_buffer_insert (GtkTextBuffer *buffer,GtkTextIter *iter,const char *text,int len);
void gtk_text_buffer_get_end_iter (GtkTextBuffer *buffer,GtkTextIter *iter);
void gtk_text_buffer_set_text (GtkTextBuffer *buffer,const char *text,int len);
GtkTextBuffer *gtk_text_view_get_buffer (GtkTextView *text_view);
GtkWidget * gtk_text_view_new (void);
void gtk_text_view_set_editable (GtkTextView *text_view,gboolean setting);
void gtk_tree_model_get (GtkTreeModel *tree_model, GtkTreeIter *iter, ...);
gboolean gtk_tree_model_get_iter_from_string (GtkTreeModel *tree_model,GtkTreeIter *iter,const gchar *path_string);
GtkTreePath * gtk_tree_model_get_path (GtkTreeModel *tree_model,GtkTreeIter *iter);
gboolean gtk_tree_model_iter_previous (GtkTreeModel *tree_model,GtkTreeIter *iter);
gboolean gtk_tree_model_iter_next (GtkTreeModel *tree_model,GtkTreeIter *iter);
void gtk_tree_path_free (GtkTreePath *path);
int *gtk_tree_path_get_indices (GtkTreePath *path);
gboolean gtk_tree_selection_get_selected (GtkTreeSelection *selection,GtkTreeModel **model,GtkTreeIter *iter);
gint gtk_tree_view_append_column (GtkTreeView *tree_view, GtkTreeViewColumn *column);
GtkTreeViewColumn *gtk_tree_view_column_new_with_attributes (const gchar *title, GtkCellRenderer *cell, ...) __attribute__((__sentinel__));
GtkTreeModel *gtk_tree_view_get_model (GtkTreeView *tree_view);
GtkTreeSelection *gtk_tree_view_get_selection (GtkTreeView *tree_view);
GtkWidget *gtk_tree_view_new (void);
void gtk_tree_view_set_headers_visible (GtkTreeView *tree_view, gboolean headers_visible);
void gtk_tree_view_set_model (GtkTreeView *tree_view, GtkTreeModel *model);
void gtk_window_set_default_size (GtkWindow *window, int width, int height);
void gtk_widget_set_halign (GtkWidget *widget,GtkAlign align);
void gtk_widget_set_hexpand (GtkWidget *widget,gboolean expand);
void gtk_widget_set_valign (GtkWidget *widget,GtkAlign align);
void gtk_widget_set_vexpand (GtkWidget *widget,gboolean expand);
void gtk_widget_show (GtkWidget *widget);
void gtk_window_destroy (GtkWindow *window);
void gtk_window_get_size (GtkWindow *window,int *width,int *height);
void gtk_window_set_child (GtkWindow *window,GtkWidget *child);
void gtk_window_set_title (GtkWindow *window, const char *title);
