
//"stdlib.h"
#define EXIT_SUCCESS 0

typedef char gchar;
typedef int gint;
typedef gint gboolean;
typedef unsigned int gsize;
typedef unsigned int guint;
typedef unsigned long gulong;
typedef void* gpointer;

typedef struct _GApplication GApplication;
typedef struct _GtkApplication GtkApplication;
typedef struct _GtkBox GtkBox;
typedef struct _GtkDialog GtkDialog;
typedef struct _GtkFrame GtkFrame;
typedef struct _GtkScrolledWindow GtkScrolledWindow;
typedef struct _GtkTextBuffer GtkTextBuffer;
typedef struct _GtkTextIter GtkTextIter;
typedef struct _GtkTextView GtkTextView;
typedef struct _GtkWidget GtkWidget;
typedef struct _GtkWindow GtkWindow;

typedef void (*GCallback)(void);
typedef struct _GClosure GClosure;
typedef void (*GClosureNotify)(gpointer data,GClosure *closure);

typedef enum{  G_APPLICATION_HANDLES_COMMAND_LINE = 1 << 3}
 GApplicationFlags;
typedef enum{G_CONNECT_SWAPPED = 1 << 1}
 GConnectFlags;
typedef enum{  GTK_ALIGN_FILL,  GTK_ALIGN_START,  GTK_ALIGN_END,  GTK_ALIGN_CENTER,  GTK_ALIGN_BASELINE}
 GtkAlign;
typedef enum{  GTK_DIALOG_MODAL = 1 << 0,  GTK_DIALOG_DESTROY_WITH_PARENT = 1 << 1}
 GtkDialogFlags;
typedef enum{GTK_ORIENTATION_HORIZONTAL,GTK_ORIENTATION_VERTICAL}
 GtkOrientation;
typedef enum{  GTK_RESPONSE_NONE = -1}
 GtkResponseType;

#define G_CALLBACK(f) ((GCallback) (f))

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

void g_application_activate (GApplication *application);
void g_application_quit (GApplication *application);
int g_application_run (GApplication *application,int argc,char **argv);
void g_free (gpointer mem);
gpointer g_malloc(gsize n_bytes);
void g_object_unref (gpointer object);
gulong g_signal_connect_data (gpointer instance,const gchar *detailed_signal,GCallback c_handler,gpointer data,GClosureNotify destroy_data,GConnectFlags con);
GtkApplication * gtk_application_new (const gchar *application_id, GApplicationFlags flags);
GtkWidget * gtk_application_window_new (GtkApplication *application);
void gtk_box_append (GtkBox *box, GtkWidget *child);
GtkWidget* gtk_box_new (GtkOrientation orientation,int spacing);
GtkWidget* gtk_button_new_with_label (const char *label);
GtkWidget * gtk_dialog_get_content_area (GtkDialog *dialog);
GtkWidget* gtk_dialog_new_with_buttons (const char *title,GtkWindow *parent,GtkDialogFlags flags,const char *first_button_text,...) __attribute__((__sentinel__));
GtkWidget *gtk_frame_new (const gchar *label);
GtkWidget* gtk_scrolled_window_new (void);
void gtk_scrolled_window_set_child (GtkScrolledWindow *scrolled_window, GtkWidget *child);
void gtk_text_buffer_insert (GtkTextBuffer *buffer,GtkTextIter *iter,const char *text,int len);
void gtk_text_buffer_get_end_iter (GtkTextBuffer *buffer,GtkTextIter *iter);
void gtk_text_buffer_set_text (GtkTextBuffer *buffer,const char *text,int len);
GtkTextBuffer *gtk_text_view_get_buffer (GtkTextView *text_view);
GtkWidget * gtk_text_view_new (void);
void gtk_text_view_set_editable (GtkTextView *text_view,gboolean setting);
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
