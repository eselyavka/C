#include <zmq.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_THREADS 10

struct thread_data {
    int thread_id;
    void *ctx;
};

struct thread_data thread_data_arr[NUMBER_OF_THREADS];

void *worker(void *thread_args) {
    struct thread_data *th_data;
    th_data = (struct thread_data *) thread_args;

    printf("Sending <<Hello>> to server from thread number: %d\n", th_data->thread_id);

    void *requester = zmq_socket (th_data->ctx, ZMQ_REQ);
    zmq_connect (requester, "tcp://192.168.122.61:5555");

    char buffer [10];
    int request_nbr;
    for (request_nbr = 0; request_nbr < 100; request_nbr++) {
        zmq_send (requester, "Hello", 5, 0);
    }
    zmq_close (requester);
    pthread_exit(NULL);
}

int main (void) {
    pthread_t threads[NUMBER_OF_THREADS];
    int status, i;
    void *context = zmq_ctx_new ();
    
    if (context == NULL) {
        printf("Error while creating zmq contex");
        exit(-1);
    }

    for (i=0; i<NUMBER_OF_THREADS; i++) {
        thread_data_arr[i].thread_id=i;
        thread_data_arr[i].ctx=context;

        printf("Create thread number %d\n", i);
        status=pthread_create(&threads[i], NULL, worker, (void *) &thread_data_arr[i]);

        if (status != 0) {
            printf("Error while creating thread number: %d\n, status:%d", i, status);
            exit(-1);
        }

        pthread_join(threads[i], NULL); 
    }

    zmq_ctx_destroy (context);
    return 0;
}
