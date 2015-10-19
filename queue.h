#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "SWP_frame.h"
#define QUEUESIZE  5

typedef struct {
        FRAME *data;		                /* body of queue */
        int head;                      /* position of first element */
        int tail;                       /* position of last element */
        int count;                      /* number of queue elements */
} QFRAME;

#endif
