/**
 * @file queue_test.c
 *      tests the queue.
 * @author Jiang Yu-Kuan, yukuan.jiang@gmail.com
 * @date 2006/05/18
 * @version 2.0
 */
#include "Queue.h"
#include "ToyUnit.h"

enum {
    BUF_SIZE= 3
};

char buf[BUF_SIZE];

int main()
{
    Queue q;

    Q_init( &q, buf, BUF_SIZE );
    TU_ASSERT("03", Q_empty(&q));
    TU_ASSERT("04", !Q_full(&q));
    TU_ASSERT("05", Q_size(&q) == 0);

    Q_put(&q, 'a');
    TU_ASSERT("12", Q_first(&q)=='a');
    TU_ASSERT("13", !Q_empty(&q));
    TU_ASSERT("14", !Q_full(&q));
    TU_ASSERT("15", Q_size(&q) == 1);
    TU_ASSERT("16", Q_last(&q)=='a');

    Q_put(&q, 'b');
    TU_ASSERT("22", Q_first(&q)=='a');
    TU_ASSERT("23", !Q_empty(&q));
    TU_ASSERT("24", !Q_full(&q));
    TU_ASSERT("25", Q_size(&q) == 2);
    TU_ASSERT("26", Q_last(&q)=='b');

    Q_put(&q, 'c');
    TU_ASSERT("32", Q_first(&q)=='a');
    TU_ASSERT("33", !Q_empty(&q));
    TU_ASSERT("34", Q_full(&q));
    TU_ASSERT("35", Q_size(&q) == 3);
    TU_ASSERT("36", Q_last(&q)=='c');

    TU_ASSERT("41", Q_get(&q)=='a');
    TU_ASSERT("42", Q_first(&q)=='b');
    TU_ASSERT("43", !Q_empty(&q));
    TU_ASSERT("44", !Q_full(&q));
    TU_ASSERT("45", Q_size(&q) == 2);
    TU_ASSERT("46", Q_last(&q)=='c');

    TU_ASSERT("51", Q_get(&q)=='b');
    TU_ASSERT("52", Q_first(&q)=='c');
    TU_ASSERT("53", !Q_empty(&q));
    TU_ASSERT("54", !Q_full(&q));
    TU_ASSERT("55", Q_size(&q) == 1);
    TU_ASSERT("56", Q_last(&q)=='c');

    TU_ASSERT("61", Q_get(&q)=='c');
    TU_ASSERT("63", Q_empty(&q));
    TU_ASSERT("64", !Q_full(&q));
    TU_ASSERT("65", Q_size(&q) == 0);

    Q_put(&q, 'd');
    TU_ASSERT("72", Q_first(&q)=='d');
    TU_ASSERT("73", !Q_empty(&q));
    TU_ASSERT("74", !Q_full(&q));
    TU_ASSERT("75", Q_size(&q) == 1);
    TU_ASSERT("76", Q_last(&q)=='d');

    TU_ASSERT("81", Q_get(&q)=='d');
    TU_ASSERT("83", Q_empty(&q));
    TU_ASSERT("84", !Q_full(&q));
    TU_ASSERT("85", Q_size(&q) == 0);

    TU_ASSERT("91", Q_unget(&q)=='d');
    TU_ASSERT("92", Q_first(&q)=='d');
    TU_ASSERT("93", !Q_empty(&q));
    TU_ASSERT("94", !Q_full(&q));
    TU_ASSERT("95", Q_size(&q) == 1);
    TU_ASSERT("96", Q_last(&q)=='d');

    Q_clear(&q);
    TU_ASSERT("103", Q_empty(&q));
    TU_ASSERT("104", !Q_full(&q));
    TU_ASSERT("105", Q_size(&q) == 0);

    TU_RESULT();

    return 0;
}
