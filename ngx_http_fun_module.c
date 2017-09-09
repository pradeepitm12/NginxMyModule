#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <cairo.h>
#define M_PI 3.14159265
/*Declare a handler*/
static char *ngx_http_fun(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
/* The HTTP BOdY   The hello world string. */
static u_char ngx_fun_string[] = "This is Fun hello world";
/*handler for the request */
static ngx_int_t ngx_http_fun_handler(ngx_http_request_t *r);


static ngx_command_t ngx_http_fun_commands[] = {

    { //entry point
      ngx_string("fun_http"), /* directive */
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS, /* location context and takes
                                            no arguments*/
      //A pointer to our handler-function
      ngx_http_fun, /* configuration setup function */
      0, /* No offset. Only one context is supported. */
      0, /* No offset when storing the module configuration on struct. */
      NULL},

    ngx_null_command /* command termination */
};

/* The module context. */
static ngx_http_module_t ngx_http_fun_module_ctx = {
    NULL, /* preconfiguration */
    NULL, /* postconfiguration */

    NULL, /* create main configuration */
    NULL, /* init main configuration */

    NULL, /* create server configuration */
    NULL, /* merge server configuration */

    NULL, /* create location configuration */
    NULL /* merge location configuration */
};

/* Module definition. */
ngx_module_t ngx_http_fun_module = {
    NGX_MODULE_V1,
    &ngx_http_fun_module_ctx, /* module context */
    ngx_http_fun_commands, /* module directives */
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

/**
 * Content handler.
 *
 * @param r
 *   Pointer to the request structure. See http_request.h.
 * @return
 *   The status of the response generation.
 */

static ngx_int_t ngx_http_fun_handler(ngx_http_request_t *r){
	ngx_int_t     rc;
	ngx_buf_t    * b;
	ngx_chain_t  out;
	/**FOR FUN make it commmented**/
	//We response GET and Head  request only
	if(!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))){
		return NGX_HTTP_NOT_ALLOWED;
	}
	//discard request body since we dont need it here 
	rc= ngx_http_discard_request_body(r);
	if(rc !=NGX_OK){
		return rc;
	}
	//set the content type header 
	r->headers_out.content_type_len=sizeof("text/html")-1;
	r->headers_out.content_type.len=sizeof("text/html")-1;
	r->headers_out.content_type.data=(u_char *) "text/html";
	
	//send the header only, if request type is 	http 'head'
	if(r->method==NGX_HTTP_HEAD){
		r->headers_out.status=NGX_HTTP_OK;
		r->headers_out.content_length_n=sizeof(ngx_fun_string)-1;
		return ngx_http_send_header(r);
	}
	//allocate a buffer for your response body
	b =ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
	if(b==NULL){
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}
	// attach this buffer to the buffer chain
	out.buf=b;
	out.next=NULL;
	//adjust the pointer of the buffer 
	b->pos=ngx_fun_string;
	b->last=ngx_fun_string + sizeof(ngx_fun_string) - 1;
	b->memory=1; //This buffer is read only memory
					// This means that filter should copy it, and not try to rewrite in place 
	b->last_buf=1;
	// set the status line
	r->headers_out.status= NGX_HTTP_OK;
	r->headers_out.content_length_n=sizeof(ngx_fun_string)-1;

	// send the headers of your response

	if(rc==NGX_ERROR || rc > NGX_OK || r->header_only){
		return rc;

	}
//send the buffer chain of your response 
	return ngx_http_output_filter(r,&out);
}

/*Attach the handler 
in this handler-function we can validate on the incoming ngx_command_t and attach different handlers*/

static char *ngx_http_fun(ngx_conf_t * cf, ngx_command_t *cmd, void * conf){
	ngx_http_core_loc_conf_t * clcf;
	clcf=ngx_http_conf_get_module_loc_conf(cf,ngx_http_core_module);
	clcf->handler=ngx_http_fun_handler;//handler to process the fun directice
	return NGX_CONF_OK;

}

/****Till here every thing workds as earlier****/