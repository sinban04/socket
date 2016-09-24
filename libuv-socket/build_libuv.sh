#bin/bash

gcc -o libuv_server libuv_server.c /usr/local/lib/libuv.a -lpthread
gcc -o libuv_client libuv_client.c /usr/local/lib/libuv.a -lpthread

