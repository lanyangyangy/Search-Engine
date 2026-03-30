#include "EchoServer.h"

int main(int argc, char **argv)
{
    EchoServer server(4, 10, "127.0.0.1", 1234);
    server.start();

    return 0;
}

