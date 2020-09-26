
typedef int gint;
typedef gint gboolean;
__extension__ typedef signed long long gint64;

typedef struct _JsonGenerator JsonGenerator;
typedef struct _JsonNode JsonNode;
typedef struct _JsonObject JsonObject;
typedef struct _JsonParser JsonParser;
typedef struct _GError GError;

JsonGenerator *json_generator_new (void);
void json_generator_set_root (JsonGenerator *generator, JsonNode *node);
gboolean json_generator_to_file (JsonGenerator *generator, const gchar *filename, GError **error);
JsonObject * json_node_get_object (JsonNode *node);
gint64 json_object_get_int_member (JsonObject *object, const gchar *member_name);
const gchar * json_object_get_string_member (JsonObject *object, const gchar *member_name);
JsonNode *json_parser_get_root (JsonParser *parser);
gboolean json_parser_load_from_file (JsonParser *parser,const gchar *filename,GError **error);
JsonParser *json_parser_new (void);
