/*	Copyright 2015-2022 Rivoreo
	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:
	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
// CONFIGURE IN HERE
int port = 23333;

// Some configure in here
int main() {
	// start a socket
	int server_socket = socket(PF_INET, SOCK_STREAM, 0);
	// A -1 is returned if an error occurs, otherwise the return value is a descriptor referencing the socket.
	if (server_socket == -1) {
		printf("Socket ERROR\n");
		exit(1);
	} else {
		static const int reuseaddr = 1;
		if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof reuseaddr) < 0) {
			printf("re-using socket\n");
		}

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr  = htonl(INADDR_ANY);
	server_address.sin_port = htons(port);

	int bind_result = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
	if (bind_result == -1) {
		printf("Bind ERROR\n");
		exit(1);
	}

	int listen_result = listen(server_socket, 5);
	if (listen_result == -1) {
		printf("Listen ERROR\n");
		exit(1);
	}

	struct sockaddr_in client_address;
	socklen_t client_address_size;

	while (1) {
		client_address_size = sizeof(client_address);
		int client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_size);
		if (client_socket == -1) {
			printf("Accept ERROR\n");
		}

		char buffer[1024];
		read(client_socket, buffer, sizeof(buffer) - 1);
	
		char status[] = "HTTP/1.0 200 OK\r\n";
		char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";
		char body[] = "<html><head><title>Server: A Simple Web Server  TEST OK</title></head><body><img src=\"https://http.cat/200\" alt=\"https://http.cat/200\"></body></html>";

		write(client_socket, status, sizeof(status));
		write(client_socket, header, sizeof(header));
		write(client_socket, body, sizeof(body));
	
		close(client_socket);
		}
	}
}
