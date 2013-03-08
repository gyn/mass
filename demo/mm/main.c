#include <stdio.h>
#include <stdlib.h>


#define CONFIG_MAX_LOOP		6
#define CONFIG_MEM_SIZE		0x100000

#define CONFIG_MEMBLK_SIZE	2048
#define CONFIG_MEMBLK_MASK	(CONFIG_MEMBLK_SIZE - 1)


extern void pbuf_init(int id, void *start, void *limit);
extern void pbuf_free(int id, void *ptr);
extern void *pbuf_malloc(int id);


int main(int argc, char **argv)
{
	int i;
	int j;

	void *ptr;
	void *top;
	void *limit;
	void *buf[CONFIG_MAX_LOOP];

	ptr = malloc(CONFIG_MEM_SIZE);
	if (ptr == NULL) {
		printf("%s %d : malloc failed\n", __FUNCTION__, __LINE__);

		return 1;
	}

	top   = (void *)(((unsigned long)ptr + CONFIG_MEMBLK_SIZE - 1) & (~CONFIG_MEMBLK_MASK));
	limit = (void *)(((unsigned long)ptr + CONFIG_MEM_SIZE) & (~CONFIG_MEMBLK_MASK));

	printf("%s %d : %p %p %p\n", __FUNCTION__, __LINE__, ptr, top, limit);

	pbuf_init(0, top, limit);

	for (j = 0; j < CONFIG_MAX_LOOP; ++j)
	{
		printf("%s %d : =============== %d ===============\n",
				__FUNCTION__, __LINE__, j);

		for (i = 0; i < j; ++i) {
			buf[i] = pbuf_malloc(0);
		}

		for (i = 0; i < j; ++i) {
			pbuf_free(0, buf[i]);
		}
	}

	free(ptr);

	return 0;
}
