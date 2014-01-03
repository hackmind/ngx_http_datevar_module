/**
 * jenkinslee (warriorlee2009@gmail.com)
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static ngx_int_t ngx_http_datevar_year(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_year2(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_month(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_day(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_hour(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_hour12(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_minute(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_second(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data);
static ngx_int_t ngx_http_datevar_time_fmt(ngx_http_request_t *r, ngx_http_variable_value_t *v, ngx_int_t t, ngx_int_t len);

static ngx_int_t ngx_http_datevar_add_variables(ngx_conf_t *cf);

static ngx_http_variable_t  ngx_http_datevar_vars[] = {

    { ngx_string("year"), NULL, ngx_http_datevar_year, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("year2"), NULL, ngx_http_datevar_year2, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("month"), NULL, ngx_http_datevar_month, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("day"), NULL, ngx_http_datevar_day, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("hour"), NULL, ngx_http_datevar_hour, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("hour12"), NULL, ngx_http_datevar_hour12, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("minute"), NULL, ngx_http_datevar_minute, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_string("second"), NULL, ngx_http_datevar_second, 0,
        NGX_HTTP_VAR_NOCACHEABLE, 0 },

    { ngx_null_string, NULL, NULL, 0, 0, 0 }
};


static ngx_http_module_t  ngx_http_datevar_module_ctx = {
    ngx_http_datevar_add_variables,        /* preconfiguration */
    NULL,                                  /* postconfiguration */

    NULL,                                  /* create main configuration */
    NULL,                                  /* init main configuration */

    NULL,                                  /* create server configuration */
    NULL,                                  /* merge server configuration */

    NULL,                                  /* create location configration */
    NULL                                   /* merge location configration */
};


ngx_module_t  ngx_http_datevar_module = {
    NGX_MODULE_V1,
    &ngx_http_datevar_module_ctx,          /* module context */
    NULL,                                  /* module directives */
    NGX_HTTP_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};


static ngx_int_t
ngx_http_datevar_add_variables(ngx_conf_t *cf)
{
    ngx_http_variable_t  *var, *v;

    for (v = ngx_http_datevar_vars; v->name.len; v++) {
        var = ngx_http_add_variable(cf, &v->name, v->flags);
        if (var == NULL) {
            return NGX_ERROR;
        }

        var->get_handler = v->get_handler;
        var->data = v->data;
    }

    return NGX_OK;
}

static ngx_int_t
ngx_http_datevar_year(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_year, NGX_INT32_LEN);
}


static ngx_int_t
ngx_http_datevar_year2(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_year % 100, 2);
}


static ngx_int_t
ngx_http_datevar_month(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_mon, 2);
}


static ngx_int_t
ngx_http_datevar_day(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_mday, 2);
}


static ngx_int_t
ngx_http_datevar_hour(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_hour, 2);
}


static ngx_int_t
ngx_http_datevar_hour12(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);

    ngx_int_t hour;
    hour = tm.ngx_tm_hour;

    if (hour == 0 || hour == 12) {
        return ngx_http_datevar_time_fmt(r, v, 12, 2);
    } else {
        return ngx_http_datevar_time_fmt(r, v, hour % 12, 2);
    }
}


static ngx_int_t
ngx_http_datevar_minute(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_min, 2);
}


static ngx_int_t
ngx_http_datevar_second(ngx_http_request_t *r, ngx_http_variable_value_t *v, uintptr_t data)
{
    ngx_tm_t tm;
    ngx_localtime(ngx_time(), &tm);
    return ngx_http_datevar_time_fmt(r, v, tm.ngx_tm_sec, 2);
}

static ngx_int_t
ngx_http_datevar_time_fmt(ngx_http_request_t *r, ngx_http_variable_value_t *v, ngx_int_t t, ngx_int_t len)
{
    u_char *p;

    p = ngx_pnalloc(r->pool, len);
    if (p == NULL) {
        return NGX_ERROR;
    }

    v->len = ngx_sprintf(p, "%02i", t) - p;
    v->valid = 1;
    v->no_cacheable = 0;
    v->not_found = 0;
    v->data = p;

    return NGX_OK;
}
