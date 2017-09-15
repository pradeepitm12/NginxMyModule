#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>



static char *ngx_http_helloworld(ngx_conf_t *cf, ngx_command_t *cmd,
void *conf);
static void ngx_http_form_input_post_read(ngx_http_request_t *r);

typedef struct {
ngx_flag_t done : 1;
ngx_flag_t waiting_more_body : 1;
} ngx_http_helloworld_ctx_t;

static ngx_command_t ngx_http_helloworld_commands[] = {

{ ngx_string("helloworld"),
NGX_HTTP_MAIN_CONF | NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF |
NGX_CONF_NOARGS,
ngx_http_helloworld,
NGX_HTTP_LOC_CONF_OFFSET,
0,
NULL},

ngx_null_command
};




static u_char ngx_helloworld[] = "ABCD";


static ngx_http_module_t ngx_http_helloworld_module_ctx = {
NULL, /* preconfiguration */
NULL, /* postconfiguration */

NULL, /* create main configuration */
NULL, /* init main configuration */

NULL, /* create server configuration */
NULL, /* merge server configuration */

NULL, /* create location configuration */
NULL /* merge location configuration */
};


ngx_module_t ngx_http_helloworld_module = {
NGX_MODULE_V1,
&ngx_http_helloworld_module_ctx, /* module context */
ngx_http_helloworld_commands, /* module directives */
NGX_HTTP_MODULE, /* module type */
NULL, /* init master */
NULL, /* init module */
NULL, /* init process */
NULL, /* init thread */
NULL, /* exit thread */
NULL, /* exit process */
NULL, /* exit master */
NGX_MODULE_V1_PADDING
};


static ngx_str_t ngx_http_gif_type = ngx_string("text/html");

static ngx_int_t
ngx_http_helloworld_handler(ngx_http_request_t *r) {
ngx_http_complex_value_t cv;
char* response;



if (!(r->method & (NGX_HTTP_GET | NGX_HTTP_POST))) {
return NGX_HTTP_NOT_ALLOWED;
}

ngx_memzero(&cv, sizeof (ngx_http_complex_value_t));
if (r->method == NGX_HTTP_POST) {
ngx_int_t rc = ngx_http_read_client_request_body(r,
ngx_http_form_input_post_read);
}

//do some processing here
return ngx_http_send_response(r, NGX_HTTP_OK, &ngx_http_gif_type, &cv);
}

static char *
ngx_http_helloworld(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
ngx_http_core_loc_conf_t *clcf;

clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
clcf->handler = ngx_http_helloworld_handler;

return NGX_CONF_OK;
}

static void ngx_http_form_input_post_read(ngx_http_request_t *r) {
ngx_http_helloworld_input_ctx_t *ctx;

ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0,
"http form_input post read request body");

r->read_event_handler = ngx_http_request_empty_handler;

ctx = ngx_http_get_module_ctx(r, ngx_http_helloworld_module);
ctx->done = 1;

#if defined(nginx_version) && nginx_version >= 8011

r->main->count--;
#endif




if (ctx->waiting_more_body) {
ctx->waiting_more_body = 0;

ngx_http_core_run_phases(r);
}
}