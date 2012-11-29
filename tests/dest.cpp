#include <unistd.h>
#include <cassert>
#include <iostream>

#include "yampl.h"
#include "yampl/pipe/PipeSocketFactory.h"
#include "yampl/shm/SHMSocketFactory.h"
#include "yampl/ZMQ/ZMQSocketFactory.h"

using namespace yampl;
using namespace std;


void client(ISocketFactory *factory, const Channel &channel){
  ISocket *socket = factory->createClientSocket(channel, "client");
  char buffer[100];

  socket->send("Hello World!");
  socket->recv(buffer);
  assert(strcmp(buffer, "pong") == 0);

  delete socket;
}

void server(ISocketFactory *factory, const Channel &channel){
  ISocket *socket = factory->createServerSocket(channel);
  char buffer[100];
  std::string dest;
  socket->recv(buffer, dest);

  assert(dest == "client");

  try{
    socket->sendTo("foobar", "pong");
  }catch(UnroutableException){}

  socket->sendTo("client", "pong");

  delete socket;
}

int main(int argc, char *argv[]){
  if(fork() == 0){
    ISocketFactory *zmqFactory = new ZMQ::SocketFactory();
    client(zmqFactory, Channel("zmq"));
    ISocketFactory *pipeFactory = new pipe::SocketFactory();
    client(pipeFactory, Channel("pipe"));
    ISocketFactory *shmFactory = new shm::SocketFactory();
    client(shmFactory, Channel("shm"));
  }else{
    ISocketFactory *zmqFactory = new ZMQ::SocketFactory();
    server(zmqFactory, Channel("zmq"));
    ISocketFactory *pipeFactory = new pipe::SocketFactory();
    server(pipeFactory, Channel("pipe"));
    ISocketFactory *shmFactory = new shm::SocketFactory();
    server(shmFactory, Channel("shm"));

    wait();
    cout << "Success" << endl;
  }
}