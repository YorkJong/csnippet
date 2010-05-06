/**
 * @file queue.c
 *      Implementes a queue module, and that uses a circular array.
 * @author Jiang Yu-Kuan yukuan.jiang@gmail.com
 * @date 2006/05/07 (initial version)
 * @date 2006/05/18 (last revision)
 * @version 2.0
 */
#include "Queue.h"
#include <assert.h>


/** Initilizes a queue.
 * @param[in,out] q the queue to be initialized.
 * @param[in] buf the buffer to store items.
 * @param[in] buf_size the buffer size.
 */
void Q_init( Queue* q, QueueItem* buf, size_t buf_size )
{
    q->first= 0;
    q->end= 0;
    q->count= 0;
    q->buf= buf;
    q->buf_size= buf_size;
}


/** Clear the queue */
void Q_clear( Queue* q )
{
    q->first= 0;
    q->end= 0;
    q->count= 0;
}


/** Puts an item to the end of a queue.
 * @param[in,out] q the queue to add an item.
 * @param[in] i the added item.
 */
void Q_put( Queue* q, QueueItem i )
{
    assert (!Q_full(q));

    ++q->count;
    q->buf[q->end]= i;
    q->end= (q->end+1) % q->buf_size;
}


/** Gets the first item of a queue.
 * @param[in,out] q the queue to get an item.
 * @return the gotton item
 */
QueueItem Q_get( Queue* q )
{
    QueueItem i;
    assert (!Q_empty(q));

    --q->count;
    i= q->buf[q->first];
    q->first= (q->first+1) % q->buf_size;
    return i;
}


/** Rolls back a "get" operation.
 * This cannot work correctly next to 'clear' operation.
 * @return the previous character
 */
QueueItem Q_unget( Queue* q )
{
    ++q->count;
    if (q->first == 0)
        q->first= q->buf_size - 1;
    else
        --q->first;
    return q->buf[q->first];
}


/** Peeks the first item of a queue.
 * @param[in] q the queue to get an item.
 * @return the peeked item
 */
QueueItem Q_first( const Queue* q )
{
    assert (!Q_empty(q));

    return q->buf[q->first];
}


/** Peeks the last item of a queue.
 * @param[in] q the queue to get an item.
 * @return the peeked item
 */
QueueItem Q_last( const Queue* q )
{
    Index i;
    assert (!Q_empty(q));

    if (q->end == 0)
        i= q->buf_size - 1;
    else
        i= q->end - 1;
    return q->buf[i];
}


/** Gets the size of a queue at the moment. */
size_t Q_size( const Queue* q )
{
    return q->count;
}


/** Determines if a queue is empty. */
bool Q_empty( const Queue* q )
{
    return q->count == 0;
}


/** Determines if an queue is full */
bool Q_full( const Queue* q )
{
    return q->count == q->buf_size;
}
