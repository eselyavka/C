#include <zmq.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    printf ("Connecting to hello world server…\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    char pkeystr[]="rq:rM>}U?@Lns47E1%kR.o@n%FcmmsL/@{H8]yf7";
    char cpkeystr[]="Yne@$w-vo<fVvi]a<NY6T1ed:M$fCG*[IaLV{hID";
    char cskeystr[]="D:)Q[IlAW!ahhC2ac:9*A}h:p?([4%wOTJ%JR%cs";
    uint8_t public_key[32]; 
    uint8_t client_pub_key[32]; 
    uint8_t client_server_key[32]; 

    zmq_z85_decode(public_key, pkeystr);
    zmq_z85_decode(client_server_key, cskeystr);
    zmq_z85_decode(client_pub_key, cpkeystr);

    zmq_setsockopt(requester, ZMQ_CURVE_SERVERKEY, &public_key, sizeof(public_key));
    zmq_setsockopt(requester, ZMQ_CURVE_PUBLICKEY, &client_pub_key, sizeof(client_pub_key));
    zmq_setsockopt(requester, ZMQ_CURVE_SECRETKEY, &client_server_key, sizeof(client_server_key));
    zmq_connect (requester, "tcp://localhost:55555");

    int request_nbr;
    for (request_nbr = 0; request_nbr != 10; request_nbr++) {
        char buffer [10];
        printf ("Sending Hello %d…\n", request_nbr);
        zmq_send (requester, "Hello", 5, 0);
        zmq_recv (requester, buffer, 10, 0);
        printf ("Received World %d\n", request_nbr);
    }
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;
}
