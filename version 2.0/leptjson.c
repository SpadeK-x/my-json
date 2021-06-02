#include<stdlib.h>
#include<assert.h>
#include"leptjson.h"

typedef struct{
    const char* json;
}lept_context;

#define EXPECT(c,ch) do{ assert(*c->json==(ch));  c->json++; }  while(0)//assert(1)则继续执行，否则崩溃

/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void lept_parse_whitespace(lept_context* c){//判断是否为 空格，回车，tab，并采取措施（跳过
    const char* p=c->json;
    while(*p==' ' || *p=='\t' || *p=='\n' || *p=='\r')
        p++;
    c->json=p;
}

/* null  = "null" */
static int lept_parse_null(lept_context* c,lept_value* v){
    EXPECT(c,'n');//判断是否为null，是则继续执行（跳过null，并将type置为null
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json+=3;
    v->type=LEPT_NULL;
    return LEPT_PARSE_OK;
}

static int lept_parse_true(lept_context* c,lept_value* v){
    EXPECT(c,'t');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json+=3;
    v->type=LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context* c,lept_value* v){
    EXPECT(c,'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LEPT_PARSE_INVALID_VALUE;
    c->json+=4;
    v->type=LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_number(lept_context* c,lept_value* v){
    char* end;
    /* \TODO validate number */
    v->n = strtod(c->json,&end);
    if(c->json==end)//开头就不是数字，返回invalid
        return LEPT_PARSE_INVALID_VALUE;
    c->json=end;//跳过数字
    v->type=LEPT_NUMBER;
    return LEPT_PARSE_OK;
}

/* value = null / false / true */
static int lept_parse_value(lept_context* c,lept_value* v){
    switch(*c->json){
        case 'n'  : return lept_parse_null(c,v);
        case 't'  : return lept_parse_true(c,v);
        case 'f'  : return lept_parse_false(c,v);
        default   : return lept_parse_number(c,v);
        case '\0' : return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value* v,const char* json){
    lept_context c;
    int ret;
    assert(v!=NULL);    
    c.json=json;
    v->type=LEPT_NULL;
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0')
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
    }
    return ret;
}

lept_type lept_get_type(const lept_value* v){
    assert(v!=NULL);
    return v->type;
}

double lept_get_number(const lept_value* v){
    assert(v!=NULL && v->type==LEPT_NUMBER);
    return v->n;
}



