#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

void *heartBeat(void *arg){
  typedef struct BEACON
  {
    int ID; // randomly generated during startup
    int StartUpTime; // the time when the client starts
    char IP[4]; // the IP address of this client
    int CmdPort; // the client listens to this port for manager cmd
  } beacon;

  srand((unsigned) time(NULL));
  beacon b;
  b.ID = rand() % 100;
  b.StartUpTime = (unsigned long)time(NULL);
  b.IP[0] = '0';
  b.IP[1] = '0';
  b.IP[2] = '0';
  b.IP[3] = '0';
  b.CmdPort = 9002;

  int network_socket;
  network_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET; //sets type of address
  server_address.sin_port = htons(9002); //sets port
  server_address.sin_addr.s_addr = INADDR_ANY; //connects to localhost


  //bid the socket to our specified ip and sin_port
  bind(network_socket, (struct sockaddr*)  &server_address, sizeof(server_address));

  //listen(network_socket, 5);

  while (1) {
    //holds client socket
    struct sockaddr client_addr;
    unsigned int client_len;

    int client_socket;
    client_socket = accept(network_socket, NULL, NULL);
    bind(client_socket, (struct sockaddr*)  &server_address, sizeof(server_address));
    char server_response[256] = "Hope to god this sends";
    //char test[4] = "tes";
    //usleep(60000000);
    usleep(60);
    printf("sending heartBeat to manager...\n");
    //send
    send(client_socket, server_response, sizeof(server_response), 0);

  }

  close(network_socket);
  return NULL;
}

int main(int argc, char const *argv[], char * envp[]) {
  pthread_t beaconHeart;
  pthread_create(&beaconHeart,NULL,heartBeat,NULL);
  while (1) {
    usleep(1);
    printf("listening to tcp\n");
  }
  pthread_join(beaconHeart,NULL);
  return 0;
}
