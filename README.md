# memcached protocol parser

This memcached protocol parser implements the server-side memcached protocol handling procedure to make it easy to build memcached-compatible servers.

This implementation is used in [mimicached](https://github.com/yasukata/mimicached), a cache server that mimics the behavior of memcached.

An important goal of this implementation is to minimize the dependencies on external components including CPUs, OSes, libraries, and compilers so that it can be used on a wide range of platforms. To minimize the dependencies on compilers, this implementation complies with the C89 and C++98 standards.

### compilation test

The following program is to check if the compilation works without external libraries and compiler-specific features.

<details>

<summary>please click here to show the program</summary>

```c
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned long	uint64_t;
typedef unsigned long	uintptr_t;

#ifdef __cplusplus
#define NULL (0)
#else
#define NULL ((void *) 0)
#endif

int printf_nothing(const char *format, ...) { (void) format; return 0; }
#define MP_OPS_DEBUG_PRINTF printf_nothing

#define MP_OPS_UTIL_TIME_NS(__o) (0)

static void mp_ops_clear_response(void *opaque) { (void) opaque; }
#define MP_OPS_CLEAR_RESPONSE mp_ops_clear_response
static long mp_ops_push_response(void *opaque, const char *msg, uint64_t len) { return 0; (void) opaque; (void) msg; (void) len; }
#define MP_OPS_PUSH_RESPONSE mp_ops_push_response
static void mp_ops_kv_cmd(void *mem, const uint8_t *key, uint64_t key_len, uint8_t *cmd, void *opaque) { (void) mem; (void) key; (void) key_len; (void) cmd; (void) opaque; }
#define MP_OPS_KV_CMD mp_ops_kv_cmd
static void mp_ops_kv_flush_all(uint64_t expr_sec, void *opaque) { (void) expr_sec; (void) opaque; }
#define MP_OPS_KV_FLUSH_ALL  mp_ops_kv_flush_all
static int mp_ops_allow_shutdown(void *opaque) { return 0; (void) opaque; }
#define MP_OPS_ALLOW_SHUTDOWN mp_ops_allow_shutdown
static void mp_ops_shutdown(uint64_t op, void *opaque) { (void) op; (void) opaque; }
#define MP_OPS_SHUTDOWN mp_ops_shutdown

#define MP_TEST_HOOK() do { } while (0)

#include "main.c"

void _start(void) {
	(void) parse_memcached_request(NULL, NULL);
}
```

</details>

Supposedly, we can compile the program above, saved as a file named ```stub.c```, can be compiled by the following commands.

```
gcc -Werror -Wextra -Wall -pedantic -ansi -std=c89 -nostartfiles -nodefaultlibs -nostdlib -nostdinc stub.c
```

```
g++ -Werror -Wextra -Wall -pedantic -ansi -std=c++98 -nostartfiles -nodefaultlibs -nostdlib -nostdinc stub.c
```

Note that the program above is just for checking whether the compilartion can be performed or not, and thus, does not work properly when it is executed.
