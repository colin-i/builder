
//"stdlib.h"
#define EXIT_SUCCESS 0

typedef char gchar;
typedef int gint;
typedef gint gboolean;
typedef unsigned long gulong;
typedef void* gpointer;

typedef struct _GApplication GApplication;
typedef struct _GtkApplication GtkApplication;
typedef struct _GtkBox GtkBox;
typedef struct _GtkFrame GtkFrame;
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
typedef enum{GTK_ORIENTATION_HORIZONTAL,GTK_ORIENTATION_VERTICAL}
 GtkOrientation;

#define G_CALLBACK(f) ((GCallback) (f))

void g_application_activate (GApplication *application);
void g_application_quit (GApplication *application);
int g_application_run (GApplication *application,int argc,char **argv);
void g_free (gpointer mem);
void g_object_unref (gpointer object);
gulong g_signal_connect_data (gpointer instance,const gchar *detailed_signal,GCallback c_handler,gpointer data,GClosureNotify destroy_data,GConnectFlags con);
gchar * g_strdup ( const gchar *str );
GtkApplication * gtk_application_new (const gchar *application_id, GApplicationFlags flags);
GtkWidget * gtk_application_window_new (GtkApplication *application);
void gtk_box_append (GtkBox *box, GtkWidget *child);
GtkWidget* gtk_box_new (GtkOrientation orientation,int spacing);
GtkWidget* gtk_button_new_with_label (const char *label);
GtkWidget *gtk_frame_new (const gchar *label);
void gtk_window_set_default_size (GtkWindow *window, int width, int height);
void gtk_widget_set_halign (GtkWidget *widget,GtkAlign align);
void gtk_widget_set_hexpand (GtkWidget *widget,gboolean expand);
void gtk_widget_set_valign (GtkWidget *widget,GtkAlign align);
void gtk_widget_set_vexpand (GtkWidget *widget,gboolean expand);
void gtk_widget_show (GtkWidget *widget);
void gtk_window_set_child (GtkWindow *window,GtkWidget *child);
void gtk_window_set_title (GtkWindow *window, const char *title);
