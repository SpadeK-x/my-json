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

static int lept_parse_literal(lept_context* c, lept_value* v, const char* literal, lept_type type){
    size_t i;
    EXPECT(c,literal[0]);
    for(i=0;literal[i+1];i++){
        if(c->json[i]!=literal[i+1])
            return LEPT_PARSE_INVALID_VALUE;
    }
    c->json+=i;
    v->type=type;
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
        case 'n'  : return lept_parse_literal(c, v, "null", LEPT_NULL);
        case 't'  : return lept_parse_literal(c, v, "true", LEPT_TRUE);
        case 'f'  : return lept_parse_literal(c, v, "false", LEPT_FALSE);
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



