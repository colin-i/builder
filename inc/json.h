
__extension__ typedef signed long long gint64;

typedef struct _JsonArray JsonArray;
typedef struct _JsonGenerator JsonGenerator;
typedef struct _JsonNode JsonNode;
typedef struct _JsonObject JsonObject;
typedef struct _JsonParser JsonParser;
typedef struct _GError GError;

void json_array_add_element (JsonArray *array,JsonNode *value);
void json_array_add_object_element (JsonArray *array,JsonObject *value);
GList * json_array_get_elements (JsonArray *array);
guint json_array_get_length (JsonArray *array);
JsonObject * json_array_get_object_element (JsonArray *array, guint index_);
void json_array_remove_element (JsonArray *array,guint index_);
JsonArray * json_array_sized_new (guint n_elements);
JsonGenerator *json_generator_new (void);
void json_generator_set_root (JsonGenerator *generator, JsonNode *node);
gboolean json_generator_to_file (JsonGenerator *generator, const gchar *filename, GError **error);
JsonNode * json_node_alloc (void);
JsonNode * json_node_copy (JsonNode *node);
void json_node_free (JsonNode *node);
JsonArray * json_node_get_array (JsonNode *node);
JsonObject * json_node_get_object (JsonNode *node);
JsonNode * json_node_init_object (JsonNode *node,JsonObject *object);
void json_node_take_array (JsonNode *node,JsonArray *array);
gint64 json_object_get_int_member (JsonObject *object, const gchar *member_name);
const gchar * json_object_get_string_member (JsonObject *object, const gchar *member_name);
JsonObject * json_object_new (void);
void json_object_set_int_member (JsonObject *object,const gchar *member_name,gint64 value);
void json_object_set_string_member (JsonObject *object,const gchar *member_name,const gchar *value);
JsonNode *json_parser_get_root (JsonParser *parser);
gboolean json_parser_load_from_file (JsonParser *parser,const gchar *filename,GError **error);
JsonParser *json_parser_new (void);
