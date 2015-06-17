#include <zmq.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://192.168.122.61:5555");
    assert (rc == 0);
    int i=0;

    while (1) {
        char buffer [10];
        zmq_recv (responder, buffer, 10, 0);
        if (strcmp(buffer,"Hello")) {
            i++;
            printf("Count: %d\n", i);
        }
        sleep (1);
        zmq_send (responder, "World", 5, 0);
    }
    return 0;
}
