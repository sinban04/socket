#include <uv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024
#define SOCKET_PATH "SM.sock"

uv_loop_t * loop;
uv_pipe_t * handle;
uv_pipe_t * handle2;




void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf){
	buf->base = malloc(suggested_size);
	buf->len = suggested_size;

	printf("Allocate buffer\n");
}


void OnRead(uv_stream_t *read, ssize_t nread, const uv_buf_t *buf){
	if(nread < 0){
		if(nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*)read, NULL);
		return ;
	}

	
	printf("Received: %s\n", buf->base);

	free(buf->base);
//	uv_read_stop(read);
}


void OnWrite(uv_write_t *req, int status){
	if (status < 0){
		fprintf(stderr, "Write error %s\n", uv_err_name(status));
	}
	printf("Write complete\n");

	free(req);
	
//	uv_read_start((uv_stream_t*) handle, alloc_buffer, OnRead);

}




void OnConnect(uv_connect_t* connect, int status){
	
	if(status < 0){
		//error!
		printf("connection error!\n");
		return ;
	}
	
	printf("Connection built\n");
	
	int len;
	char *str;
	
	str = "aaaaa";
	len = strlen(str);
	
	/*	
	uv_write_t* req = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_write_t* req2 = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t first_buf = uv_buf_init((char*)&len, 4);
	uv_buf_t second_buf = uv_buf_init((char*)&str, len);
	uv_write(req, (uv_stream_t*) handle, &first_buf, 1, OnWrite);
	uv_write(req2, (uv_stream_t*) handle, &second_buf, 1, OnWrite);
	*/

	
	int i;
	for(i=0; i<10; i++){
	uv_write_t* req = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init("5678baaaaa", 10);
	uv_write(req,(uv_stream_t*) handle, &wrbuf, 1, OnWrite);
	
	}


}

void OnConnect2(uv_connect_t* connect, int status){
	
	if(status < 0){
		//error!
		printf("connection2 error!\n");
		return ;
	}
	
	printf("Connection2 built\n");

	int i =0;

	uv_write_t* req = (uv_write_t*) malloc(sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init("1234bbaaaaa", 10);
	uv_write(req,(uv_stream_t*) handle2, &wrbuf, 1, OnWrite);

	
	


}

int main(){
	loop = uv_default_loop();
	


	uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	handle = (uv_pipe_t*)malloc(sizeof(uv_pipe_t));
	uv_pipe_init(uv_default_loop(), handle, 0);
	uv_pipe_open(handle, socket(PF_UNIX, SOCK_STREAM, 0));
	
	int r;
	uv_pipe_connect(connect, handle, SOCKET_PATH, OnConnect);

	int i;

	uv_read_start((uv_stream_t*) handle, alloc_buffer, OnRead);
	
/*	

	uv_connect_t* connect2 = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	handle2 = (uv_pipe_t*)malloc(sizeof(uv_pipe_t));
	uv_pipe_init(uv_default_loop(), handle2, 0);
	uv_pipe_open(handle2, socket(PF_UNIX, SOCK_STREAM, 0));
	uv_pipe_connect(connect2, handle2, SOCKET_PATH, OnConnect2);

	uv_read_start((uv_stream_t*) handle2, alloc_buffer, OnRead);
*/		

	uv_run(loop, UV_RUN_DEFAULT);


	return 0;

}
