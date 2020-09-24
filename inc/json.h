
typedef int gint;
typedef gint gboolean;

typedef struct _JsonGenerator JsonGenerator;
typedef struct _JsonNode JsonNode;
typedef struct _JsonParser JsonParser;
typedef struct _GError GError;

JsonGenerator *json_generator_new (void);
void json_generator_set_root (JsonGenerator *generator, JsonNode *node);
gboolean json_generator_to_file (JsonGenerator *generator, const gchar *filename, GError **error);
JsonNode *json_parser_get_root (JsonParser *parser);
gboolean json_parser_load_from_file (JsonParser *parser,const gchar *filename,GError **error);
JsonParser *json_parser_new (void);
