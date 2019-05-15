/* mtraceall: allows multiple processes to generate mtrace() output concurrently
 *
 * Copyright (C) 2019 OPTEYA SAS
 * Copyright (C) 2019 Yann Droneaud <ydroneaud@opteya.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#define _GNU_SOURCE
#include <fcntl.h>  // O_CLOEXEC
#include <limits.h> // PATH_MAX
#include <mcheck.h> // mtrace(), muntrace()
#include <stdint.h> // intmax_t
#include <stdio.h>  // snprintf()
#include <stdlib.h> // abort(), getenv(), setenv()
#include <time.h>   // time()
#include <unistd.h> // getpid()

static void mtraceall_init(void) __attribute__((constructor));
static void mtraceall_init(void)
{
	const char *prefix = getenv("MALLOC_TRACE_ALL");
	if (!prefix)
		return;

	int r;

	char filename[PATH_MAX];

	r = snprintf(filename, sizeof(filename),
		     "%s.%jd.%jd.XXXXXX",
		     prefix,
		     (intmax_t)time(NULL),
		     (intmax_t)getpid());
	if (r < 0)
		abort();

	if ((size_t)r >= sizeof(filename))
		abort();

	int f = mkostemp(filename, O_CLOEXEC);
	if (f < 0)
		abort();

	r = setenv("MALLOC_TRACE", filename, 1);
	if (r != 0)
		abort();

	mtrace();

	close(f);
}

static void mtraceall_fini(void) __attribute__((destructor));
static void mtraceall_fini(void)
{
	muntrace();
}
