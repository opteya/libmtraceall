mtraceall_CFLAGS += -Werror
mtraceall_CFLAGS += -Wall
mtraceall_CFLAGS += -Wextra
mtraceall_CFLAGS += -O2
mtraceall_CFLAGS += -fPIC
mtraceall_CFLAGS += -g
mtraceall_CFLAGS += $(CFLAGS)

mtraceall_CPPFLAGS += -DPIC
mtraceall_CPPFLAGS += $(CPPFLAGS)

mtraceall_LDFLAGS += $(LDFLAGS)

.PHONY: all
all: libmtraceall.so

mtraceall.o: mtraceall.c
	$(CC) $(mtraceall_CFLAGS) $(mtraceall_CPPFLAGS) -c -o $@ $^

libmtraceall.so: mtraceall.o
	$(CC) -shared $(mtraceall_CFLAGS) $(mtraceall_LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) mtraceall.o libmtraceall.so
