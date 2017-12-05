/*
 *   Copyright © 2017 Keith Packard. All rights reserved.
 *   Author: Keith Packard <keithp@keithp.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ao_scheme.h"
#include <stdio.h>
#include <system/readline.h>

static FILE *ao_scheme_file;

static char save_file[] = "scheme.image";

int
ao_scheme_os_save(void)
{
	FILE	*save = fopen(save_file, "w");

	if (!save) {
		perror(save_file);
		return 0;
	}
	fwrite(ao_scheme_pool, 1, AO_SCHEME_POOL_TOTAL, save);
	fclose(save);
	return 1;
}

int
ao_scheme_os_restore_save(struct ao_scheme_os_save *save, int offset)
{
	FILE	*restore = fopen(save_file, "r");
	size_t	ret;

	if (!restore) {
		perror(save_file);
		return 0;
	}
	fseek(restore, offset, SEEK_SET);
	ret = fread(save, sizeof (struct ao_scheme_os_save), 1, restore);
	fclose(restore);
	if (ret != 1)
		return 0;
	return 1;
}

int
ao_scheme_os_restore(void)
{
	FILE	*restore = fopen(save_file, "r");
	size_t	ret;

	if (!restore) {
		perror(save_file);
		return 0;
	}
	ret = fread(ao_scheme_pool, 1, AO_SCHEME_POOL_TOTAL, restore);
	fclose(restore);
	if (ret != AO_SCHEME_POOL_TOTAL)
		return 0;
	return 1;
}

#define MAX_LINE	128

static char	line[MAX_LINE];
static int	cur;

int
_ao_scheme_getc(void)
{
	if (ao_scheme_file)
		return getc(ao_scheme_file);

	if (line[cur] == '\0') {
		char *prompt;
		if (ao_scheme_read_list)
			prompt = "+ ";
		else
			prompt = "> ";
		printf(prompt); fflush(stdout);
		readline_prompt(prompt);
		if (std_readline(line, sizeof(line)) == EOF)
			return EOF;
		cur = 0;
	}
	return (line[cur++] & 0xff);
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int scheme_main(int argc, char *argv[])
#endif
{
	while (*++argv) {
		ao_scheme_file = fopen(*argv, "r");
		if (!ao_scheme_file) {
			perror(*argv);
			exit(1);
		}
		ao_scheme_read_eval_print(ao_scheme_file, NULL, false);
		fclose(ao_scheme_file);
		ao_scheme_file = NULL;
	}
	ao_scheme_read_eval_print(stdin, stdout, false);
	return 0;
}
