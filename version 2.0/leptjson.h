#ifndef LEPTJSON_H_
#define LEPTJSON_H_
typedef enum { 
    LEPT_NULL, 
    LEPT_FALSE, 
    LEPT_TRUE, 
    LEPT_NUMBER, 
    LEPT_STRING, 
    LEPT_ARRAY, 
    LEPT_OBJECT 
} lept_type;

typedef struct {
    double n;
    lept_type type;
}lept_value;
    
int lept_parse(lept_value* v,const char* json);

lept_type lept_get_type(const lept_value* v);

double lept_get_number(const lept_value* v);

//返回类型
enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_NUMBER_TOO_BIG
};

#endif /* LEPTJSON_H_*/