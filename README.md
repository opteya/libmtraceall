# libmtraceall - enable `mtrace()` into programs

libmtraceall is a wrapper around [mtrace()] that can be `LD_PRELOAD`'ed into the target process.

[mtrace()][debug] is a [GNU C Library][glibc] feature that enables `malloc()` hooks to trace allocations and deallocation.

Environment variable [MALLOC_TRACE] must be defined and the [mtrace()] function must be called by the program for the hooks to be enabled.

It usually cumbersome to modify a target program to call [mtrace()], so using a shared object, `LD_PRELOAD`'ed into the executable program, could be useful then.

But using the same filename in [MALLOC_TRACE] environment variable will lead to multiple program writing to the same file, overwriting each other.

libmtraceall takes care of generating an unique [MALLOC_TRACE] file name for each program it's preloaded into, so that [mtrace()][debug] output is not overwritten.

Example:

```
$ make
$ LD_PRELOAD=./libmtraceall.so MALLOC_TRACE_ALL=mtraceall.out nm mtraceall.o
$ ls mtrace.out.*
mtraceall.out.1557930662.23814.eApkxw
```

The generated filenames are built from the filename prefix in `MALLOC_TRACE_ALL` variable, Unix epoch timestamp, process identifier (PID), and a random suffix to cope with `exec()`'ed program having the same PID (could also handle PID reusing).

*BEWARE* that a process calling `exec()` won't have a chance to call [muntrace()] (or `exit()`), so its output file could be truncated. In fact it's likely to be empty. It's usually harmless, as most process `fork()` before calling `exec()`. Anyway if the file was not truncated, [mtrace] would have shown many memory leak, so not having the file is quite expected.

[glibc]:        https://www.gnu.org/software/libc/ "GNU C library"
[debug]:        https://www.gnu.org/software/libc/manual/html_node/Allocation-Debugging.html "Allocation Debugging"
[MALLOC_TRACE]: https://www.gnu.org/software/libc/manual/html_node/Tracing-malloc.html "How to install the tracing functionality"
[mtrace()]:     https://www.gnu.org/software/libc/manual/html_node/Tracing-malloc.html "How to install the tracing functionality"
[muntrace()]:   https://www.gnu.org/software/libc/manual/html_node/Tracing-malloc.html "How to install the tracing functionality"
[mtrace]:       https://www.gnu.org/software/libc/manual/html_node/Interpreting-the-traces.html#Interpreting-the-traces "Interpreting the traces"
