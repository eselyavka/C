#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (void)
{
    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int i=1;
    char skeystr[] = "JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6";
    uint8_t server_key [32];
    zmq_z85_decode (server_key, skeystr);
    zmq_setsockopt(responder, ZMQ_CURVE_SERVER, &i, sizeof(i));
    zmq_setsockopt(responder, ZMQ_CURVE_SECRETKEY, &server_key, sizeof(server_key));
    int rc = zmq_bind (responder, "tcp://*:55555");
    assert (rc == 0);

    while (1) {
        char buffer [10];
        zmq_recv (responder, buffer, 10, 0);
        printf ("Received Hello\n");
        sleep (1);
        zmq_send (responder, "World", 5, 0);
    }
    return 0;
}
