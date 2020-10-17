
__extension__ typedef signed long long gint64;

typedef struct _JsonArray JsonArray;
typedef struct _JsonGenerator JsonGenerator;
typedef struct _JsonNode JsonNode;
typedef struct _JsonObject JsonObject;
typedef struct _JsonParser JsonParser;
typedef struct _GError GError;

guint json_array_get_length (JsonArray *array);
JsonObject * json_array_get_object_element (JsonArray *array, guint index_);
JsonGenerator *json_generator_new (void);
void json_generator_set_root (JsonGenerator *generator, JsonNode *node);
gboolean json_generator_to_file (JsonGenerator *generator, const gchar *filename, GError **error);
JsonNode * json_node_alloc (void);
void json_node_free (JsonNode *node);
JsonObject * json_node_get_object (JsonNode *node);
JsonNode * json_node_init_object (JsonNode *node,JsonObject *object);
JsonArray * json_object_get_array_member (JsonObject *object, const gchar *member_name);
gint64 json_object_get_int_member (JsonObject *object, const gchar *member_name);
const gchar * json_object_get_string_member (JsonObject *object, const gchar *member_name);
JsonObject * json_object_new (void);
void json_object_set_string_member (JsonObject *object,const gchar *member_name,const gchar *value);
JsonNode *json_parser_get_root (JsonParser *parser);
gboolean json_parser_load_from_file (JsonParser *parser,const gchar *filename,GError **error);
JsonParser *json_parser_new (void);
