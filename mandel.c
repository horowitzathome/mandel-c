#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>

static void hello_handler(struct evhttp_request *req, void *arg)
{
    char response[256];
    int height, width, iter;

    const char *uri = evhttp_request_get_uri(req);
    if (sscanf(uri, "/hello/%d/%d/%d", &iter, &height, &width) != 3)
    {
        evhttp_send_error(req, HTTP_BADREQUEST, "Invalid request");
        return;
    }

    snprintf(response, sizeof(response), "Hello from /hello: iter=%d, height=%d, width=%d", iter, height, width);
    struct evbuffer *buf = evbuffer_new();

    if (buf == NULL)
    {
        fprintf(stderr, "Failed to create response buffer\n");
        return;
    }

    evbuffer_add_printf(buf, "%s", response);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

static void health_handler(struct evhttp_request *req, void *arg)
{
    char *response = "Hello from /health!";
    struct evbuffer *buf = evbuffer_new();

    if (buf == NULL)
    {
        fprintf(stderr, "Failed to create response buffer\n");
        return;
    }

    evbuffer_add_printf(buf, "%s", response);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

int main(int argc, char **argv)
{
    struct event_base *base;
    struct evhttp *http;
    struct evhttp_bound_socket *handle;

    base = event_base_new();
    if (!base)
    {
        fprintf(stderr, "Failed to create event base\n");
        return 1;
    }

    http = evhttp_new(base);
    if (!http)
    {
        fprintf(stderr, "Failed to create HTTP server\n");
        return 1;
    }

    evhttp_set_cb(http, "/hello", hello_handler, NULL);
    evhttp_set_cb(http, "/health", health_handler, NULL);

    handle = evhttp_bind_socket_with_handle(http, "0.0.0.0", 8080);
    if (!handle)
    {
        fprintf(stderr, "Failed to bind socket\n");
        return 1;
    }

    event_base_dispatch(base);
    return 0;
}
