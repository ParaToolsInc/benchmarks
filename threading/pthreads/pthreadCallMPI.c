/*
 * Each processor will have 2 threads. Each processor's one thread will be sending data to other processors and the other thread will be receiving data from other processors. When I am implementing it, it is giving segmentation fault error with some messages like "current bytes -40, total bytes 0, remote id 5"
 */


int rank, size, msg_num;

// thread function for sending messages
void *Send_Func_For_Thread(void *arg)
{
    int send, procnum, x;
    send = rank;

    for(x=0; x < msg_num; x++)
    {
        procnum = rand()%size;
        if(procnum != rank)
            MPI_Send(&send, 1, MPI_INT, procnum, 0, MPI_COMM_WORLD);
    }

// sending special message to other processors with tag = 128 to signal the finishing of sending message

    for (x = 0; x < size; x++)
    {
        if(x != rank)
            MPI_Send(&send, 1, MPI_INT, x, 128, MPI_COMM_WORLD);
    }

    pthread_exit((void *)NULL);
}


// thread function for receiving messages

void *Recv_Func_For_Thread(void *arg)
{
    MPI_Status status;
    int recv, counter = 0;

    while(counter != size - 1)
    {
        MPI_Recv(&recv, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        if(status.MPI_TAG == 128)
            counter++;
    }

    pthread_exit((void *)NULL);
}


int main(int argc, char **argv)
{
    void *stat;
    pthread_attr_t attr;
    pthread_t thread[2];

    //MPI_Init(&argc, &argv);
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // rank -> rank of this processor
    MPI_Comm_size(MPI_COMM_WORLD, &size);   // size -> total number of processors

    srand((unsigned)time(NULL));

    msg_num = atoi(argv[1]);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // thread 0 will be sending   messages
    pthread_create(&thread[0], &attr, Send_Func_For_Thread, (void *)0);

    // thread 1 will be receiving messages
    pthread_create(&thread[1], &attr, Recv_Func_For_Thread, (void *)1);

    pthread_attr_destroy(&attr);

    pthread_join(thread[0], &stat);
    pthread_join(thread[1], &stat);

    cout << "Finished : Proc " << rank << "\n";

    MPI_Finalize();
    pthread_exit((void *)NULL);
    return 0;
}
