/**
 * @file queue.h
 *      Interface of a queue module.
 * @author Jiang Yu-Kuan yukuan.jiang@gmail.com
 * @date 2006/05/18
 * @version 2.0
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_


#include <stdlib.h>
#include "platform.h"


typedef char QueueItem; ///< the item type in a queue

typedef struct {
    Index first;    ///< index of the first (front) item of a queue.
    Index end;      ///< index of the end (last+1) of a queue.
    size_t count;   ///< the number of items in a queue.
    QueueItem* buf; ///< a pointer that indicates the buffer of a queue.
    size_t buf_size;///< buffer size.
} Queue;


void Q_init( Queue* q, QueueItem* buf, size_t buf_size );
void Q_clear( Queue* );

void Q_put( Queue*, QueueItem );
QueueItem Q_get( Queue* );

QueueItem Q_unget( Queue* );

QueueItem Q_first( const Queue* );
QueueItem Q_last( const Queue* );

size_t Q_size( const Queue* );
bool Q_empty( const Queue* );
bool Q_full( const Queue* );

#endif // _QUEUE_H_

/** @example Queue_test.c
 *      This is an example of how to use the Queue module.
 */
