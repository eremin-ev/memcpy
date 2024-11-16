/*
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
 */

#include <stdio.h>

int my_add(int a, int b, int c);

int main()
{
	int a = 0x0, b = 0xf, c = 0xf;
	int sum = my_add(a, b, c);
	printf("my_add %i + %i + %i = %i\n", a, b, c, sum);

	return 0;
}
