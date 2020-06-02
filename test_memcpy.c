/*
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//#define SMALL
//#define NEGATIVE

void *my_memcpy(void *dest, const void *src, size_t n);
void *my_memmove(void *dest, const void *src, size_t n);
int my_add(int a, int b, int c);

struct linebuffer {
	int bufsz;
	int nl_pos;
	int len;
	char buf[2048];
};

void do_memcpy(int len)
{
	struct linebuffer lb;
	memset(&lb, 0, sizeof(struct linebuffer));
	memset(lb.buf +    0, 'A', 1024);
	memset(lb.buf + 1024, 'B', 1024);
	lb.len = len;

	//memcpy(lb.buf, lb.buf + 1024, lb.len);
	my_memcpy(lb.buf, lb.buf + 1024, lb.len);

	int i, nr = 0, continuous_nr = 0;
	int continuous = lb.buf[0] == 'B';
	for (i = 0; i < 1024; i++) {
		if (lb.buf[i] == 'B') {
			if (continuous)
				continuous_nr++;

			nr++;
		} else {
			continuous = 0;
		}
	}

	printf("%s len %x %u %i\tnr %i, continuous_nr %i\n", __func__,
		len, len, len, nr, continuous_nr);

	//int fd = open("/tmp/o", O_CREAT | O_TRUNC | O_WRONLY);
	//write(fd, lb.buf, sizeof(lb.buf));
	//close(fd);
}

void test_fixed_length_memcpy(void)
{
	const char c_val = 0xba;
	const char c_init = 0x0;
	size_t buf_len = 4096;
	size_t len = -1;
	char *src = malloc(buf_len);
	char *dst_orig = malloc(2*buf_len);
	/*
	 * +------+------+
	 * | page | page |
	 * +------+------+
	 *       ^
	 *        `--- dst
	 *
	 */
	char *dst = (char*)dst_orig + 4096 + 1;
#if BE_CAREFUL
	if (buf_len < len) {
		printf("buf_len %zi < len %zi\n", buf_len, len);
		return;
	}
#endif
	memset(src, c_init, buf_len);
	memset(dst, c_init, buf_len);
	memset(src, c_val, buf_len);
	my_memcpy(dst, src, len);
	int error_flag = 0;
	size_t i;
	for (i = 0; i < len; i++) {
		if (dst[i] != c_val) {
			printf("Error: expected %x != %x (found)\n", c_val, dst[i]);
			error_flag = 1;
			break;
		}
	}
	for (; i < buf_len; i++) {
		if (dst[i] != c_init) {
			printf("Error: expected %x != %x (found)\n", c_init, dst[i]);
			error_flag = 1;
			break;
		}
	}
	printf("%s\n", error_flag ? "Failed" : "OK");
	free(dst_orig);
	free(src);
}

void test_fixed_length_memmove(void)
{
	size_t i;
	const char c_init = 0x0;
	size_t buf_len = 4096;
	size_t len = -1;
	size_t offset = 1;
	//char src[buf_len];
	char *src = malloc(buf_len);
	char *dst = (char *)src + offset;
#if BE_CAREFUL
	if (buf_len < len) {
		printf("buf_len %zi < len %zi\n", buf_len, len);
		return;
	}
#endif
	memset(src, c_init, buf_len);
	//memset(src, c_val, len);
	for (i = 0; i < buf_len; i++) {
		src[i] = i;
	}
	// src < dst ?
	my_memcpy(dst, src, len);
	//my_memmove(dst, src, len);
	int error_flag = 0;
#if BE_CAREFUL
	for (i = 0; i < len; i++) {
		if (dst[i] != i) {
			printf("Error: expected %x != %x (found)\n", i, dst[i]);
			error_flag = 1;
			break;
		}
		//printf("%x ", A[i]);
	}
#endif
	printf("%s\n", error_flag ? "Failed" : "OK");
	free(src);
}

int my_add1(int a, int b)
{
	return a + b;
}

int main()
{
#if defined(SMALL)
	int i, len;

	for (i = 0; i < 100; i++) {
		len = 0 - i;
		do_memcpy(len);
	}
#elif defined(NEGATIVE)
	do_memcpy(0xfffffd37);
	do_memcpy(0xffffff37);
	do_memcpy(0xfffffff7);
#else
	test_fixed_length_memcpy();
	//test_fixed_length_memmove();
#endif

	//int a = 0x0, b = 0xf, c = 0xf;
	//printf("my_add %i + %i + %i = %i\n", a, b, c, my_add(a, b, c));

	return 0;
}
