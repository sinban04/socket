#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#define SOCKET_PATH "SM.sock"

uv_loop_t *loop;
int len;


void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;
	
	printf("Buffer allocated!\n");
}



void alloc_buffer1(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
	buf->base = malloc(4);
	buf->len = 4;

	printf("first buffer allocated\n");
}


void OnWrite(uv_write_t *req, int status) {
	if (status < 0) {
		fprintf(stderr, "Write error %s\n", uv_err_name(status));
	}
	
	printf("Wrote successfully\n");

	free(req);
}

void OnRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0) {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*) client, NULL);
		return;
	}

	char *str = buf->base;
	int len;
	char data[1024];
	char *temp;
	
	printf("received : %s\n", buf->base);



	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
//	uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
	uv_buf_t wrbuf = uv_buf_init("zzzzzz", 6);
	uv_write(req, client, &wrbuf, 1, OnWrite);
	free(buf->base);

//	uv_read_stop(client);
}


void OnRead1(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	if (nread < 0) {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*) client, NULL);
		return;
	}

	char *str = buf->base;

	char data[1024];
	char *temp;

	
	len = *((int*)buf->base);
	
	printf("%d \n", len);
	printf("received1 : %s\n", buf->base);

	printf("reading1\n");	

	/*
	uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
//	uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
	uv_buf_t wrbuf = uv_buf_init("zzzzzz", 6);
	uv_write(req, client, &wrbuf, 1, OnWrite);
	
	*/
	free(buf->base);
	uv_read_stop(client);

	uv_read_start((uv_stream_t*) client, alloc_buffer, OnRead);
}


void on_new_connection(uv_stream_t *server, int status) {
	if (status == -1) {
		// error!
		return;
	}
	printf("New connection established\n");

	uv_pipe_t *client = (uv_pipe_t*) malloc(sizeof(uv_pipe_t));
	uv_pipe_init(loop, client, 0);
	
	if (uv_accept(server, (uv_stream_t*) client) == 0) {
		printf("accept!\n");
//		uv_read_start((uv_stream_t*) client, alloc_buffer1, OnRead1);
		uv_read_start((uv_stream_t*) client, alloc_buffer, OnRead);
	}
	else {
		printf("client socket closed\n");
		uv_close((uv_handle_t*) client, NULL);
	}
}

void remove_sock(int sig) {
	uv_fs_t req;
	uv_fs_unlink(loop, &req, SOCKET_PATH, NULL);
	exit(0);
}

int main() {
	loop = uv_default_loop();

	uv_pipe_t server;
	uv_pipe_init(loop, &server, 0);

	signal(SIGINT, remove_sock);

	
	int r;
	if ((r = uv_pipe_bind(&server, SOCKET_PATH))) {
		fprintf(stderr, "Bind error %s\n", uv_err_name(r));
		return 1;
	}
	if ((r = uv_listen((uv_stream_t*) &server, 128, on_new_connection))) {
		fprintf(stderr, "Listen error %s\n", uv_err_name(r));
		return 2;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}


