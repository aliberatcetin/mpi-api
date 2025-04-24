#include <stdio.h>
#include <ulfius.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_PORT 9000

char* SERVER_HOST;


void send_to_mpi(const char *data) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, SERVER_HOST, &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    write(sock, data, strlen(data));
    close(sock);
}

int callback_hello_world (const struct _u_request * request, struct _u_response * response, void * user_data) {
    ulfius_set_string_body_response(response, 200, "Hello World!");
    return U_CALLBACK_CONTINUE;
}

int callback_post (const struct _u_request * request, struct _u_response * response, void * user_data) {
    json_t * json_body = ulfius_get_json_body_request(request, NULL);
    char *json_string = NULL;

    if (json_body) {
        json_string = json_dumps(json_body, JSON_INDENT(2));
        if (json_string) {
            printf("Received JSON body:\n%s\n", json_string);
            send_to_mpi(json_string);
            ulfius_set_string_body_response(response, 200, json_string);
            free(json_string);
        }
        json_decref(json_body);
    } else {
        printf("No JSON body or invalid JSON\n");
    }

    return U_CALLBACK_CONTINUE;
}

int main(void) {
    struct _u_instance instance;
    SERVER_HOST = getenv("GRAPH_MASTER_IP");
    if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return(1);
    }

    ulfius_add_endpoint_by_val(&instance, "GET", "/helloworld", NULL, 0, &callback_hello_world, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/api", NULL, 0, &callback_post, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/api/terminate", NULL, 0, &callback_post, NULL);


    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d target mpi ip : %s\n", instance.port,SERVER_HOST);
        while(true){
            
        }
    } else {
        fprintf(stderr, "Error starting framework\n");
    }
    printf("End framework\n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
}
