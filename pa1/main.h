struct stringS{char data[]; struct stringS* next; pthread_mutex_t mutex;};

void createThreads(int N);


