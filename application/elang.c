#include "../include/math.h"
#include "../include/string.h"
#include "../include/memory.h"
#include "../include/print_string.h"
#include "../include/fs_global.h"
#include "../include/format_string.h"
#include "../include/NULL.h"
#include "../include/sscanf.h"

// Custom isspace for freestanding environment
static int my_isspace(unsigned char c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

#define MAX_LINE_LENGTH 1024
#define MAX_VARIABLES 100
#define MAX_TOKEN_LENGTH 64
#define MAX_ARRAY_SIZE 100
#define MAX_FUNCTIONS 50
#define MAX_STACK_SIZE 100
#define MAX_ARGS 10
#define MAX_STRING_LENGTH 1024

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_ARRAY,
    TYPE_STRING
} ValueType;

typedef struct {
    union {
        int i;
        float f;
        struct {
            ValueType type;
            int size;
            union {
                int* i;
                float* f;
            } data;
        } array;
        struct {
            char* data;
            int length;
        } string;
    } value;
    ValueType type;
    char name[MAX_TOKEN_LENGTH];
} Variable;

#define MAX_LINES 1000

typedef struct {
    char name[MAX_TOKEN_LENGTH];
    int start_line;
    char args[MAX_ARGS][MAX_TOKEN_LENGTH];
    int arg_count;
} Function;

typedef struct {
    char lines[MAX_LINES][MAX_LINE_LENGTH];
    int line_count;
    int current_line;
} Program;

Program program;

void process_line(char* line);

// Helpers to adapt freestanding environment
static void println_str(const char* s) {
    if(s) {
        print_string(s);
    }
    print_char('\n');
}

static void println_int(int v) {
    print_int(v);
    print_char('\n');
}

static void println_double(double d) {
    print_double(d);
    print_char('\n');
}

static int to_int(const char* s) {
    return (int)string_to_double(s);
}

// Simple realloc for strings using my_malloc/my_free
static char* my_realloc_str(char* ptr, int new_size) {
    char* newp = my_malloc(new_size);
    if(!newp) return NULL;
    if(ptr) {
        int old = strlen(ptr);
        int to_copy = old < (new_size - 1) ? old : (new_size - 1);
        memcpy(newp, ptr, to_copy);
        newp[to_copy] = '\0';
        my_free(ptr);
    } else {
        newp[0] = '\0';
    }
    return newp;
}

// Tokenize by whitespace. Returns count. tokens must be tokens[count][MAX_TOKEN_LENGTH]
static int split_tokens(char* str, char tokens[][MAX_TOKEN_LENGTH], int max_tokens) {
    int n = 0;
    char* p = str;
    while(n < max_tokens) {
        while(*p && my_isspace((unsigned char)*p)) p++;
        if(!*p || *p == '\n') break;
        int i = 0;
        while(*p && !my_isspace((unsigned char)*p) && *p != '\n') {
            if(i < MAX_TOKEN_LENGTH - 1) tokens[n][i++] = *p;
            p++;
        }
        tokens[n][i] = '\0';
        n++;
    }
    return n;
}

typedef struct {
    int return_line;
    int var_count_at_call;
} CallFrame;

// Stato globale
Variable variables[MAX_VARIABLES];
int var_count = 0;
Function functions[MAX_FUNCTIONS];
int func_count = 0;
CallFrame call_stack[MAX_STACK_SIZE];
int stack_pointer = 0;
// file handling will use FileSystem APIs

// Funzioni di utilità per le stringhe
char* string_duplicate(const char* str) {
    if (!str) return NULL;
    size_t len = strlen(str);
    char* new_str = my_malloc(len + 1);
    if (new_str) {
        strcpy(new_str, str);
    }
    return new_str;
}

void string_free(Variable* var) {
    if (var && var->type == TYPE_STRING && var->value.string.data) {
        my_free(var->value.string.data);
        var->value.string.data = NULL;
        var->value.string.length = 0;
    }
}

void strip(char *str) {
    if (!str) return;
    char *start = str;
    while(*start && my_isspace(*start)) start++;
    if(start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    char *end = str + strlen(str) - 1;
    while(end > str && my_isspace(*end)) {
        *end = '\0';
        end--;
    }
}

Variable* find_variable(const char* name) {
    if (!name) return NULL;
    for(int i = 0; i < var_count; i++) {
        if(strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

Function* find_function(const char* name) {
    if (!name) return NULL;
    for(int i = 0; i < func_count; i++) {
        if(strcmp(functions[i].name, name) == 0) {
            return &functions[i];
        }
    }
    return NULL;
}

Variable* create_variable(const char* name, ValueType type) {
    if (!name || var_count >= MAX_VARIABLES) {
        println_str("Errore: impossibile creare la variabile");
        return NULL;
    }
    
    Variable* var = &variables[var_count++];
    strncpy(var->name, name, MAX_TOKEN_LENGTH - 1);
    var->name[MAX_TOKEN_LENGTH - 1] = '\0';
    var->type = type;
    
    if(type == TYPE_ARRAY) {
        var->value.array.data.i = NULL;
        var->value.array.data.f = NULL;
        var->value.array.size = 0;
    } else if(type == TYPE_STRING) {
        var->value.string.data = NULL;
        var->value.string.length = 0;
    }
    
    return var;
}

float get_number_value(const char* str) {
    if (!str) return 0.0f;
    Variable* var = find_variable(str);
    if(var) {
        switch(var->type) {
            case TYPE_INT: return (float)var->value.i;
            case TYPE_FLOAT: return var->value.f;
            case TYPE_STRING: return string_to_double(var->value.string.data);
            default: return 0.0f;
        }
    }
    return string_to_double(str);
}

// Funzioni per manipolazione stringhe
void string_concat(Variable* dest, const char* str) {
    if (!dest || dest->type != TYPE_STRING || !str) return;

    int add_len = strlen(str);
    int new_len = dest->value.string.length + add_len;
    char* new_data = my_realloc_str(dest->value.string.data, new_len + 1);

    if (new_data) {
        int old_len = dest->value.string.length;
        memcpy(new_data + old_len, str, add_len);
        new_data[old_len + add_len] = '\0';
        dest->value.string.data = new_data;
        dest->value.string.length = new_len;
    }
}

void string_substring(Variable* dest, Variable* src, int start, int length) {
    if (!dest || !src || src->type != TYPE_STRING || start < 0) return;
    
    int src_len = src->value.string.length;
    if (start >= src_len) return;
    
    if (length < 0 || start + length > src_len) {
        length = src_len - start;
    }
    
    char* new_str = my_malloc(length + 1);
    if (new_str) {
        strncpy(new_str, src->value.string.data + start, length);
        new_str[length] = '\0';

        string_free(dest);
        dest->type = TYPE_STRING;
        dest->value.string.data = new_str;
        dest->value.string.length = length;
    }
}

// Valutazione condizioni
int evaluate_condition(const char* cond) {
    if (!cond) return 0;
    
    char condition[MAX_LINE_LENGTH];
    strncpy(condition, cond, MAX_LINE_LENGTH - 1);
    condition[MAX_LINE_LENGTH - 1] = '\0';
    
    char var1[MAX_TOKEN_LENGTH] = {0};
    char op[3] = {0};
    char var2[MAX_TOKEN_LENGTH] = {0};
    char tokens_cond[3][MAX_TOKEN_LENGTH];
    int tn_cond = split_tokens(condition, tokens_cond, 3);
    if(tn_cond > 0) strncpy(var1, tokens_cond[0], MAX_TOKEN_LENGTH - 1);
    if(tn_cond > 1) strncpy(op, tokens_cond[1], 2);
    if(tn_cond > 2) strncpy(var2, tokens_cond[2], MAX_TOKEN_LENGTH - 1);
    
    Variable* v1 = find_variable(var1);
    Variable* v2 = find_variable(var2);
    
    // Se entrambi sono stringhe, fai confronto di stringhe
    if (v1 && v2 && v1->type == TYPE_STRING && v2->type == TYPE_STRING) {
        int cmp = strcmp(v1->value.string.data, v2->value.string.data);
        if(strcmp(op, "==") == 0) return cmp == 0;
        if(strcmp(op, "!=") == 0) return cmp != 0;
        if(strcmp(op, "<") == 0) return cmp < 0;
        if(strcmp(op, ">") == 0) return cmp > 0;
        if(strcmp(op, "<=") == 0) return cmp <= 0;
        if(strcmp(op, ">=") == 0) return cmp >= 0;
        return 0;
    }
    
    // Altrimenti usa confronto numerico
    float val1 = get_number_value(var1);
    float val2 = get_number_value(var2);
    
    if(strcmp(op, "==") == 0) return fabs(val1 - val2) < 0.0001f;
    if(strcmp(op, "!=") == 0) return fabs(val1 - val2) >= 0.0001f;
    if(strcmp(op, "<") == 0) return val1 < val2;
    if(strcmp(op, ">") == 0) return val1 > val2;
    if(strcmp(op, "<=") == 0) return val1 <= val2;
    if(strcmp(op, ">=") == 0) return val1 >= val2;
    
    return 0;
}

void process_line(char* line) {
    if (!line || strlen(line) == 0) return;
    
    strip(line);
    if(line[0] == '#') return;
    
    // Nuovo modo di gestire i token che preserva gli spazi
    char first_token[MAX_TOKEN_LENGTH] = {0};
    char rest_of_line[MAX_LINE_LENGTH] = {0};
    
    // Estrai il primo token (comando)
    sscanf(line, "%s", first_token);
    
    // Ottieni il resto della linea preservando gli spazi
    char* rest = strchr(line, ' ');
    if(rest) {
        rest++; // Salta lo spazio
        strncpy(rest_of_line, rest, MAX_LINE_LENGTH - 1);
    }
    
    // Mark position for call return (after processing this line)
    // This will be set properly when we know we're inside a function call
    
    // Comandi base
    if(strcmp(first_token, "print") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest, "%s", var_name);
        
        // Debug: print all variables
        // for(int i = 0; i < var_count; i++) {
        //     (debug) printf not available in freestanding build
        // }
        
        Variable* var = find_variable(var_name);
        if(var) {
            switch(var->type) {
                case TYPE_STRING:
                    println_str(var->value.string.data);
                    break;
                case TYPE_INT:
                    println_int(var->value.i);
                    break;
                case TYPE_FLOAT:
                    println_double(var->value.f);
                    break;
                case TYPE_ARRAY:
                    print_string("[");
                    for(int i = 0; i < var->value.array.size; i++) {
                        if(var->value.array.type == TYPE_INT) {
                            print_int(var->value.array.data.i[i]);
                        } else {
                            print_double(var->value.array.data.f[i]);
                        }
                        if(i < var->value.array.size - 1) print_string(", ");
                    }
                    println_str("]");
                    break;
            }
        } else {
            // Se non è una variabile, stampa il testo così com'è
            println_str(var_name);
        }
    }
    
    // Comando per creare stringhe
    else if(strcmp(first_token, "string") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char string_content[MAX_STRING_LENGTH] = {0};
        
        // Estrai il nome della variabile
        sscanf(rest, "%s", var_name);
        
        // Ottieni il contenuto della stringa
        char* content_start = strchr(rest, '"');
        if(content_start) {
            content_start++; // Salta la prima virgoletta
            char* content_end = strrchr(content_start, '"');
            if(content_end) {
                size_t content_length = content_end - content_start;
                strncpy(string_content, content_start, content_length);
                string_content[content_length] = '\0';
            }
        } else {
            // Se non ci sono virgolette, prendi tutto dopo il nome della variabile
            char* content_start = strchr(rest, ' ');
            if(content_start) {
                content_start++; // Salta lo spazio
                strncpy(string_content, content_start, MAX_STRING_LENGTH - 1);
            }
        }
        
        Variable* var = create_variable(var_name, TYPE_STRING);
        if(var) {
            var->value.string.data = string_duplicate(string_content);
            if(var->value.string.data) {
                var->value.string.length = strlen(string_content);
            }
        }
    }
    
    // Comando per concatenare stringhe
    else if(strcmp(first_token, "concat") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char second_arg[MAX_LINE_LENGTH] = {0};
        
        // Estrai il nome della variabile di destinazione e il secondo argomento
        sscanf(rest, "%s %[^\n]", var_name, second_arg);
        
        Variable* dest = find_variable(var_name);
        if(dest && dest->type == TYPE_STRING) {
            Variable* src = find_variable(second_arg);
            if(src && src->type == TYPE_STRING) {
                string_concat(dest, src->value.string.data);
            } else {
                string_concat(dest, second_arg);
            }
        }
    }
    
    // Comando per estrarre sottostringhe
    
    else if(strcmp(first_token, "substring") == 0 && rest) {
        char dest_name[MAX_TOKEN_LENGTH] = {0};
        char src_name[MAX_TOKEN_LENGTH] = {0};
        char start_str[MAX_TOKEN_LENGTH] = {0};
        char length_str[MAX_TOKEN_LENGTH] = {0};
        
        sscanf(rest_of_line, "%s %s %s %s", dest_name, src_name, start_str, length_str);
        
        Variable* dest = find_variable(dest_name);
        Variable* src = find_variable(src_name);
        if(dest && src) {
            int start = to_int(start_str);
            int length = to_int(length_str);
            string_substring(dest, src, start, length);
        }
    }
    
    // Comando strlen
    else if(strcmp(first_token, "strlen") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* var = find_variable(var_name);
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_INT);
        
        if(var && result_var) {
            if(var->type == TYPE_STRING) {
                result_var->type = TYPE_INT;
                result_var->value.i = var->value.string.length;
            } else {
                result_var->type = TYPE_INT;
                result_var->value.i = 0;
            }
        }
    }
    
    // Comando let aggiornato
    else if(strcmp(first_token, "let") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char value_str[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, value_str);
        Variable* var = find_variable(var_name);

        // Check if value_str is a string literal starting with '"'
        if(value_str[0] == '"') {
            // remove surrounding quotes if present
            char content[MAX_STRING_LENGTH] = {0};
            size_t len = strlen(value_str);
            if(len >= 2 && value_str[len-1] == '"') {
                strncpy(content, value_str + 1, len - 2);
                content[len-2] = '\0';
            } else {
                strncpy(content, value_str + 1, MAX_STRING_LENGTH - 1);
            }

            if(!var) var = create_variable(var_name, TYPE_STRING);
            if(var) {
                string_free(var);
                var->type = TYPE_STRING;
                var->value.string.data = string_duplicate(content);
                if(var->value.string.data) var->value.string.length = strlen(var->value.string.data);
            }
        } else {
            // If value_str is another variable, copy its type and value
            Variable* src = find_variable(value_str);
            if(src) {
                if(!var) var = create_variable(var_name, src->type);
                if(var) {
                    // Free old string if present
                    if(var->type == TYPE_STRING && src->type != TYPE_STRING) {
                        string_free(var);
                    }

                    var->type = src->type;
                    switch(src->type) {
                        case TYPE_INT:
                            var->value.i = src->value.i;
                            break;
                        case TYPE_FLOAT:
                            var->value.f = src->value.f;
                            break;
                        case TYPE_STRING:
                            string_free(var);
                            var->value.string.data = string_duplicate(src->value.string.data);
                            var->value.string.length = src->value.string.length;
                            break;
                        default:
                            break;
                    }
                }
            } else {
                // Literal numeric value
                if(!var) {
                    if(strchr(value_str, '.'))
                        var = create_variable(var_name, TYPE_FLOAT);
                    else
                        var = create_variable(var_name, TYPE_INT);
                }
                if(var) {
                    if(var->type == TYPE_INT)
                        var->value.i = to_int(value_str);
                    else
                        var->value.f = string_to_double(value_str);
                }
            }
        }
    }
    
    // Operazioni matematiche aggiornate
    else if((strcmp(first_token, "add") == 0 || strcmp(first_token, "sub") == 0 ||
             strcmp(first_token, "mul") == 0 || strcmp(first_token, "div") == 0) && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char value_str[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, value_str);
        
        Variable* var = find_variable(var_name);
        if(!var) {
            return;
        }
        
        float value = get_number_value(value_str);
        float current = var->type == TYPE_INT ? (float)var->value.i : var->value.f;
        float result;
        
        if(strcmp(first_token, "add") == 0) result = current + value;
        else if(strcmp(first_token, "sub") == 0) result = current - value;
        else if(strcmp(first_token, "mul") == 0) result = current * value;
        else {
            if(fabs(value) < 0.0001f) {
                println_str("Errore: divisione per zero");
                return;
            }
            result = current / value;
        }
        
        if(var->type == TYPE_INT) var->value.i = (int)result;
        else var->value.f = result;
    }
    
    // Operazioni matematiche avanzate
    else if(strcmp(first_token, "sqrt") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* var = find_variable(var_name);
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(var && result_var) {
            float value = get_number_value(var_name);
            if(value < 0) {
                println_str("Errore: sqrt di numero negativo");
                return;
            }
            result_var->type = TYPE_FLOAT;
            result_var->value.f = sqrt(value);
        }
    }
    
    else if(strcmp(first_token, "pow") == 0 && rest) {
        char base_str[MAX_TOKEN_LENGTH] = {0};
        char exp_str[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s %s", base_str, exp_str, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float base = get_number_value(base_str);
            float exp = get_number_value(exp_str);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = pow(base, exp);
        }
    }
    
    else if(strcmp(first_token, "abs") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* var = find_variable(var_name);
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(var && result_var) {
            float value = get_number_value(var_name);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = fabs(value);
        }
    }
    
    else if(strcmp(first_token, "min") == 0 && rest) {
        char val1_str[MAX_TOKEN_LENGTH] = {0};
        char val2_str[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s %s", val1_str, val2_str, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val1 = get_number_value(val1_str);
            float val2 = get_number_value(val2_str);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = val1 < val2 ? val1 : val2;
        }
    }
    
    else if(strcmp(first_token, "max") == 0 && rest) {
        char val1_str[MAX_TOKEN_LENGTH] = {0};
        char val2_str[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s %s", val1_str, val2_str, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val1 = get_number_value(val1_str);
            float val2 = get_number_value(val2_str);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = val1 > val2 ? val1 : val2;
        }
    }
    
    // Comando mod (resto della divisione)
    else if(strcmp(first_token, "mod") == 0 && rest) {
        char var1_name[MAX_TOKEN_LENGTH] = {0};
        char var2_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s %s", var1_name, var2_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_INT);
        
        if(result_var) {
            int val1 = (int)get_number_value(var1_name);
            int val2 = (int)get_number_value(var2_name);
            if(val2 != 0) {
                result_var->type = TYPE_INT;
                result_var->value.i = val1 % val2;
            }
        }
    }
    
    // Comando floor
    else if(strcmp(first_token, "floor") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_INT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_INT;
            result_var->value.i = (int)floor(val);
        }
    }
    
    // Comando ceil
    else if(strcmp(first_token, "ceil") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_INT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_INT;
            result_var->value.i = (int)ceil(val);
        }
    }
    
    // Comando round
    else if(strcmp(first_token, "round") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_INT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_INT;
            result_var->value.i = (int)round(val);
        }
    }
    
    // Comando sin
    else if(strcmp(first_token, "sin") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = sin(val);
        }
    }
    
    // Comando cos
    else if(strcmp(first_token, "cos") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = cos(val);
        }
    }
    
    // Comando tan
    else if(strcmp(first_token, "tan") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = tan(val);
        }
    }
    
    // Comando log
    else if(strcmp(first_token, "log") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            if(val > 0) {
                result_var->type = TYPE_FLOAT;
                result_var->value.f = log(val);
            }
        }
    }
    
    // Comando exp
    else if(strcmp(first_token, "exp") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char result_name[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, result_name);
        
        Variable* result_var = find_variable(result_name);
        if(!result_var) result_var = create_variable(result_name, TYPE_FLOAT);
        
        if(result_var) {
            float val = get_number_value(var_name);
            result_var->type = TYPE_FLOAT;
            result_var->value.f = exp(val);
        }
    }
    
    // Array aggiornato
    else if(strcmp(first_token, "array") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char size_str[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s", var_name, size_str);
        
        Variable* var = create_variable(var_name, TYPE_ARRAY);
        if(var) {
            int size = to_int(size_str);
            if(size > 0 && size <= MAX_ARRAY_SIZE) {
                var->value.array.type = TYPE_INT;
                var->value.array.size = size;
                var->value.array.data.i = my_malloc(size * sizeof(int));
                if(var->value.array.data.i) memset(var->value.array.data.i, 0, size * sizeof(int));
                if(!var->value.array.data.i) {
                    println_str("Errore: impossibile allocare memoria per l'array");
                    var_count--;
                }
            }
        }
    }

    // Set array aggiornato
    else if(strcmp(first_token, "set") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char index_str[MAX_TOKEN_LENGTH] = {0};
        char value_str[MAX_TOKEN_LENGTH] = {0};
        sscanf(rest_of_line, "%s %s %s", var_name, index_str, value_str);
        
        Variable* var = find_variable(var_name);
        if(var && var->type == TYPE_ARRAY) {
            int index = to_int(index_str);
            if(index >= 0 && index < var->value.array.size) {
                if(var->value.array.type == TYPE_INT)
                    var->value.array.data.i[index] = to_int(value_str);
                else
                    var->value.array.data.f[index] = string_to_double(value_str);
            }
        }
    }
    
    
    
    // Funzioni
    else if(strcmp(first_token, "function") == 0 && rest) {
        if(func_count >= MAX_FUNCTIONS) {
            println_str("Errore: troppe funzioni");
            return;
        }
        
        char func_name[MAX_TOKEN_LENGTH] = {0};
        char* paren = strchr(rest, '(');
        if(paren) {
            size_t name_len = paren - rest;
            strncpy(func_name, rest, name_len);
            func_name[name_len] = '\0';
            strip(func_name);
        } else {
            strncpy(func_name, rest, MAX_TOKEN_LENGTH - 1);
            strip(func_name);
        }
        
        Function* func = &functions[func_count++];
        strncpy(func->name, func_name, MAX_TOKEN_LENGTH - 1);
        func->name[MAX_TOKEN_LENGTH - 1] = '\0';
        func->start_line = program.current_line;
        func->arg_count = 0;
        
        // Parse arguments
        if(paren) {
            char* args_end = strchr(paren, ')');
            if(args_end) {
                *args_end = '\0';
                paren++; // Skip (
                
                // Split arguments
                char* arg = strtok(paren, ",");
                while(arg && func->arg_count < MAX_ARGS) {
                    strip(arg);
                    strncpy(func->args[func->arg_count], arg, MAX_TOKEN_LENGTH - 1);
                    func->args[func->arg_count][MAX_TOKEN_LENGTH - 1] = '\0';
                    func->arg_count++;
                    arg = strtok(NULL, ",");
                }
            }
        }
        
        // Skip function body by scanning through program lines
        int nesting = 1;
        int line_index = program.current_line + 1;
        while(line_index < program.line_count && nesting > 0) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[line_index], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strcmp(buffer, "end") == 0) {
                nesting--;
                if(nesting == 0) break;
            }
            // Handle nested functions
            else if(strncmp(buffer, "function", 8) == 0) {
                nesting++;
            }
            line_index++;
        }
        // Update program.current_line to skip past the function definition
        program.current_line = line_index;
    }
    
    else if(strcmp(first_token, "call") == 0 && rest) {
        char func_name[MAX_TOKEN_LENGTH] = {0};
        char* paren = strchr(rest_of_line, '(');
        if(paren) {
            size_t name_len = paren - rest_of_line;
            strncpy(func_name, rest_of_line, name_len);
            func_name[name_len] = '\0';
            strip(func_name);
        } else {
            strncpy(func_name, rest_of_line, MAX_TOKEN_LENGTH - 1);
            strip(func_name);
        }
        
        Function* func = find_function(func_name);
        if(!func) {
            char tmp[128];
            format_string(tmp, "Errore: funzione non trovata: %s", func_name);
            println_str(tmp);
            return;
        }
        
        if(stack_pointer >= MAX_STACK_SIZE) {
            println_str("Errore: stack overflow");
            return;
        }
        
        // Save current line and variable count - this is where we should return to after function execution
        call_stack[stack_pointer].return_line = program.current_line + 1;
        call_stack[stack_pointer].var_count_at_call = var_count;
        stack_pointer++;
        
        if(paren) {
            char* args_end = strchr(paren, ')');
            if(args_end) {
                *args_end = '\0';
                paren++;
                
                char* arg = strtok(paren, ",");
                int arg_index = 0;
                while(arg && arg_index < func->arg_count) {
                    strip(arg);
                    Variable* param = create_variable(func->args[arg_index], TYPE_FLOAT);
                    if(param) {
                        Variable* arg_var = find_variable(arg);
                        if(arg_var && arg_var->type == TYPE_STRING) {
                            param->type = TYPE_STRING;
                            param->value.string.data = string_duplicate(arg_var->value.string.data);
                            param->value.string.length = arg_var->value.string.length;
                        } else {
                            param->value.f = get_number_value(arg);
                        }
                    }
                    arg_index++;
                    arg = strtok(NULL, ",");
                }
            }
        }
        
        // Jump to function start position (start of first instruction, not the "function" line)
        program.current_line = func->start_line + 1;
    }
    
    // Return aggiornato
    else if(strcmp(first_token, "return") == 0) {
        if(stack_pointer > 0) {
            
            // Clean up variables created during this function call
            for(int i = var_count - 1; i >= call_stack[stack_pointer - 1].var_count_at_call; i--) {
                if(variables[i].type == TYPE_ARRAY) {
                    my_free(variables[i].value.array.data.i);
                }else if(variables[i].type == TYPE_STRING) {
                    string_free(&variables[i]);
                }
            }
            var_count = call_stack[stack_pointer - 1].var_count_at_call;
            
            stack_pointer--;
            // Jump to return position (line after the call)
            program.current_line = call_stack[stack_pointer].return_line;
        }
    }
    
    // If aggiornato
    else if(strcmp(first_token, "if") == 0 && rest) {
        int condition_met = evaluate_condition(rest_of_line);
        
        // Find else and endif positions
        int else_line = -1;
        int endif_line = -1;
        int nesting = 1;
        for(int i = program.current_line + 1; i < program.line_count && nesting > 0; i++) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[i], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strcmp(buffer, "if") == 0) nesting++;
            else if(strcmp(buffer, "endif") == 0) {
                nesting--;
                if(nesting == 0) endif_line = i;
            } else if(strcmp(buffer, "else") == 0 && nesting == 1) {
                else_line = i;
            }
        }
        
        if(condition_met) {
            // Execute if block (from current_line+1 to else or endif)
            program.current_line = program.current_line + 1;
        } else if(else_line >= 0) {
            // Skip to else block
            program.current_line = else_line + 1;
        } else {
            // Skip to endif
            program.current_line = endif_line;
        }
    }
    
    // Else (skip to endif)
    else if(strcmp(first_token, "else") == 0) {
        // Find endif
        int endif_line = -1;
        int nesting = 1;
        for(int i = program.current_line + 1; i < program.line_count && nesting > 0; i++) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[i], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strcmp(buffer, "if") == 0) nesting++;
            else if(strcmp(buffer, "endif") == 0) {
                nesting--;
                if(nesting == 0) endif_line = i;
            }
        }
        program.current_line = endif_line;
    }
    
    // Endif marker (do nothing)
    else if(strcmp(first_token, "endif") == 0) {
        // Already handled by if/else jumps
    }

    else if(strcmp(first_token, "for") == 0 && rest) {
        char var_name[MAX_TOKEN_LENGTH] = {0};
        char start_str[MAX_TOKEN_LENGTH] = {0};
        char end_str[MAX_TOKEN_LENGTH] = {0};
        char step_str[MAX_TOKEN_LENGTH] = {0};
        
        sscanf(rest_of_line, "%s from %s to %s step %s", var_name, start_str, end_str, step_str);
        
        // Find or create loop variable
        Variable* var = find_variable(var_name);
        int is_first_entry = (var == NULL);
        if(!var) var = create_variable(var_name, TYPE_INT);
        
        int start = to_int(start_str);
        int end = to_int(end_str);
        // int step = strlen(step_str) > 0 ? to_int(step_str) : 1;  // Not currently used in for-loop
        
        // On first entry, initialize loop variable
        if(is_first_entry) {
            var->value.i = start;
        }
        
        // Find endfor position
        int endfor_line = -1;
        int nesting = 1;
        for(int i = program.current_line + 1; i < program.line_count && nesting > 0; i++) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[i], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strcmp(buffer, "for") == 0) nesting++;
            else if(strcmp(buffer, "endfor") == 0) {
                nesting--;
                if(nesting == 0) endfor_line = i;
            }
        }
        
        if(var->value.i <= end) {
            // Execute for body
            program.current_line = program.current_line + 1;
        } else {
            // Skip past endfor (to the line after endfor)
            program.current_line = endfor_line + 1;
        }
    }
    
    // Endfor: increment and go back to for
    else if(strcmp(first_token, "endfor") == 0) {
        // Find corresponding for
        int for_line = -1;
        int nesting = 1;
        for(int i = program.current_line - 1; i >= 0 && nesting > 0; i--) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[i], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strcmp(buffer, "endfor") == 0) nesting++;
            else if(strncmp(buffer, "for", 3) == 0) {
                nesting--;
                if(nesting == 0) for_line = i;
            }
        }
        
        if(for_line >= 0) {
            // Extract loop variable name from for statement
            char for_stmt[MAX_LINE_LENGTH];
            strncpy(for_stmt, program.lines[for_line], MAX_LINE_LENGTH - 1);
            strip(for_stmt);
            
            char var_name[MAX_TOKEN_LENGTH] = {0};
            sscanf(for_stmt, "for %s", var_name);
            
            Variable* var = find_variable(var_name);
            if(var && var->type == TYPE_INT) {
                // Parse step from for statement
                char step_str[MAX_TOKEN_LENGTH] = {0};
                sscanf(for_stmt, "for %s from %*s to %*s step %s", var_name, step_str);
                int step = strlen(step_str) > 0 ? to_int(step_str) : 1;
                
                var->value.i += step;
            }
        }
        
        // Go back to for to re-evaluate condition
        program.current_line = for_line;  // Don't +1, main loop won't increment since we modified current_line
    }
    
    // While aggiornato
    else if(strcmp(first_token, "while") == 0 && rest) {
        // Find endwhile position
        int endwhile_line = -1;
        int nesting = 1;
        for(int i = program.current_line + 1; i < program.line_count && nesting > 0; i++) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[i], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strncmp(buffer, "while", 5) == 0) nesting++;
            else if(strcmp(buffer, "endwhile") == 0) {
                nesting--;
                if(nesting == 0) endwhile_line = i;
            }
        }
        
        if(evaluate_condition(rest_of_line)) {
            // Execute while body
            program.current_line = program.current_line + 1;
        } else {
            // Skip past endwhile (to the line after endwhile)
            program.current_line = endwhile_line + 1;
        }
    }
    
    // Endwhile: go back to while condition
    else if(strcmp(first_token, "endwhile") == 0) {
        // Find corresponding while
        int while_line = -1;
        int nesting = 1;
        for(int i = program.current_line - 1; i >= 0 && nesting > 0; i--) {
            char buffer[MAX_LINE_LENGTH];
            strncpy(buffer, program.lines[i], MAX_LINE_LENGTH - 1);
            buffer[MAX_LINE_LENGTH - 1] = '\0';
            strip(buffer);
            
            if(strcmp(buffer, "endwhile") == 0) nesting++;
            else if(strncmp(buffer, "while", 5) == 0) {
                nesting--;
                if(nesting == 0) while_line = i;
            }
        }
        // Go back to while to re-evaluate condition
        program.current_line = while_line;  // Don't -1, main loop won't increment since we modified current_line
    }
}

int elang(int argc, char* argv[]) {
    if(argc != 2) {
        print_string("Elang 0.0.1 Copyright Vincenzo M. Franchino (c) 2026  \n");
println_str("Uso: elang <filename.e>");
println_str("Esegue un programma scritto nel linguaggio di scripting ELang.");
        return 1;
    }
    
    char filebuf[MAX_FILE_SIZE + 1];
    int size = fs_read_file(&fs, argv[1], filebuf, MAX_FILE_SIZE);
    if(size <= 0) {
        char tmp[256];
        format_string(tmp, "Errore: impossibile aprire il file %s", argv[1]);
        println_str(tmp);
        return 1;
    }
    filebuf[size] = '\0';
    // Reset interpreter state from any previous run: free variables and reset counters
    for (int i = 0; i < var_count; i++) {
        if (variables[i].type == TYPE_ARRAY) {
            my_free(variables[i].value.array.data.i);
        } else if (variables[i].type == TYPE_STRING) {
            string_free(&variables[i]);
        }
    }
    var_count = 0;
    func_count = 0;
    stack_pointer = 0;

    // Read all lines into memory (split on '\n')
    program.line_count = 0;
    char* tok = strtok(filebuf, "\n");
    while(tok && program.line_count < MAX_LINES) {
        // remove carriage return if present
        int len = strlen(tok);
        if(len > 0 && tok[len-1] == '\r') tok[len-1] = '\0';
        strncpy(program.lines[program.line_count], tok, MAX_LINE_LENGTH - 1);
        program.lines[program.line_count][MAX_LINE_LENGTH - 1] = '\0';
        program.line_count++;
        tok = strtok(NULL, "\n");
    }
    
    // Execute program line by line
    program.current_line = 0;
    while(program.current_line < program.line_count) {
        int line_before = program.current_line;
        process_line(program.lines[program.current_line]);
        // Only increment if process_line didn't change the line (e.g., no jump/call/return)
        if(program.current_line == line_before) {
            program.current_line++;
        }
    }
    
    // Pulizia memoria
    for(int i = 0; i < var_count; i++) {
        if(variables[i].type == TYPE_ARRAY) {
            my_free(variables[i].value.array.data.i);
        } else if(variables[i].type == TYPE_STRING) {
            string_free(&variables[i]);
        }
    }
    
    return 0;
}