/*
 * Dynamic memory allocator.
 *
 * Compile options:
 *
 *	CONFIG_TRACE	enable tracing of allocations/deallocations
 *
 *	CONFIG_ASSERT	enable checking address
 *
 */

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdlib.h>


#define CONFIG_TRACE
#define CONFIG_ASSERT

#define CONFIG_POOL_MAX		2

#define CONFIG_MEMBLK_SIZE	2048
#define CONFIG_MEMBLK_MASK	(CONFIG_MEMBLK_SIZE - 1)


struct fl {
	struct fl	*next;
} *freelist[CONFIG_POOL_MAX];

static char *top[CONFIG_POOL_MAX];
static char *heapstart[CONFIG_POOL_MAX];
static char *heaplimit[CONFIG_POOL_MAX];


static inline void setheap(int id, void *start, void *limit)
{
	top[id]       = start;
	heapstart[id] = start;
	heaplimit[id] = limit;
}

static inline void panic(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);

	exit(-1);
}

static inline void * alloc(int id, size_t size)
{
	struct fl **f = &freelist[id];
	char *help;
	int failed;

#ifdef CONFIG_TRACE
	printf("alloc(%zu)", size);
#endif

	/* any space in freelist ? */
	failed = (*f == (struct fl *)0);

	/* nothing found */
	if (failed) {
		/*
		 * allocate from heap
		 */
		help = top[id];

		top[id] += size;

		/* any space left ? */
		if (top[id] > (char *)heaplimit[id])
			panic("heap full (0x%lx+%zu)", help, size);

#ifdef CONFIG_TRACE
		printf("=%lx Heap\n", (unsigned long)help);
#endif
		return help;
	}

	/* remove from freelist */
	help = (char *)(void *)*f;
	*f = (*f)->next;
#ifdef CONFIG_TRACE
	printf("=%lx Free\n", (unsigned long)help);
#endif
	return help;
}

static inline void
dealloc(int id, void *ptr)
{
	struct fl *f = (struct fl *)(ptr);
#ifdef CONFIG_TRACE
	printf("dealloc(%lx)\n", (unsigned long)ptr);
#endif

	/* put into freelist */
	f->next = freelist[id];
	freelist[id] = f;
}

void pbuf_init(int id, void *start, void *limit)
{
#ifdef CONFIG_ASSERT
	assert((id >= 0) && (id < CONFIG_POOL_MAX));
#endif

#ifdef CONFIG_ASSERT
	assert(((unsigned long)start & CONFIG_MEMBLK_MASK) == 0);
	assert(((unsigned long)limit & CONFIG_MEMBLK_MASK) == 0);
#endif

	setheap(id, start, limit);

	freelist[id] = NULL;
}

void pbuf_free(int id, void *ptr)
{
#ifdef CONFIG_ASSERT
	assert((id >= 0) && (id < CONFIG_POOL_MAX));
#endif

#ifdef CONFIG_ASSERT
	assert(((unsigned long)ptr & CONFIG_MEMBLK_MASK) == 0);
#endif

	dealloc(id, ptr);
}

void *pbuf_malloc(int id)
{
	void *ptr;

#ifdef CONFIG_ASSERT
	assert((id >= 0) && (id < CONFIG_POOL_MAX));
#endif

	ptr = alloc(id, CONFIG_MEMBLK_SIZE);

#ifdef CONFIG_ASSERT
	assert(((unsigned long)ptr & CONFIG_MEMBLK_MASK) == 0);
#endif

	return ptr;
}
