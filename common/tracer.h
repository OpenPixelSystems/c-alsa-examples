/**
 * @file tracer.h
 * @brief  A simple header only tracer lib
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version v0.1
 * @date 2020-04-16
 */

#ifndef _TRACER_H_
#define _TRACER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

struct time_trace_t {
	clock_t begin;
	clock_t end;
	double diff;
	bool ended;
};

static inline struct time_trace_t *tracer_setup_time_trace(void)
{
	struct time_trace_t *t = (struct time_trace_t *)malloc(sizeof(struct time_trace_t));
	if (!t) return NULL;

	t->begin = clock();
	return t;
}

static inline void tracer_time_trace_end(struct time_trace_t *t)
{
	t->end = clock();
	t->diff = (double)(t->end - t->begin);
	t->ended = true;
	printf("function took %lf\n", t->diff);
}

static inline void tracer_free_tracer(struct time_trace_t *t)
{
	if (!t->ended) {
		//WARN user that tracer has not ended
	}
	free(t);
}

#endif /* _TRACER_H_ */
