#include "queue.h"


init_queue(QFRAME *q)
{
        data = new FRAME[QUEUESIZE];
        q->head = 0;
        q->tail = 0;
        q->count = 0;
}

enqueue(QFRAME *q, FRAME *x)
{
        if (q->count >= QUEUESIZE)
		      printf("Warning: QFRAME overflow enqueue x=%d\n",x);
        else {
                q->data[ q->tail ] = *x;
                q->tail = (q->tail+1) % QUEUESIZE;
                q->count = q->count + 1;
        }
}

FRAME dequeue(QFRAME *q)
{
        FRAME x;

        if (q->count <= 0) printf("Warning: empty QFRAME dequeue.\n");
        else {
                x = q->data[ q->first ];
                q->head = (q->first+1) % QUEUESIZE;
                q->count = q->count - 1;
        }

        return(x);
}

int empty(QFRAME *q)
{
        return (q->count <= 0);
}
