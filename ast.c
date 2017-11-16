
struct ExpList;

typedef struct AstExp {
    enum {
        ast_integer, /* 'c' literals are integers */
        /*ast_float, */
        /*ast_string, */
        ast_return,
        ast_call,
        ast_block,
        ast_unary_op,
        ast_binary_op,
        ast_declaration,
        ast_assignment,
        ast_function,
    } tag;
    union {
        int integral;
        struct {
            struct AstNode *return_val;
        } return_;
        struct {
            char *function_name;
            struct ExpList *arguments;
        } call;
        struct {
            char *return_type;
            char *name;
            struct ExpList *params;I 
        }
        
    }
}
