/*
 *
 * Copyright 2025 Kenichi Yasukata
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ____MP__CONF_MAX_KEY_LEN
#define ____MP__CONF_MAX_KEY_LEN (250)
#endif

#ifndef ____MP__CONF_MAX_VAL_LEN
#define ____MP__CONF_MAX_VAL_LEN (1UL << 20) /* 1 MiB limit defined by memcached */
#endif

#ifndef ____MP__CONF_ENDIAN
#define ____MP__CONF_ENDIAN (1) /* little 1, big 2 */
#endif

#define __mp_bsw16(_n) ((((_n) >> 8) | ((_n) << 8)) & 0xffff)
#define __mp_bsw32(_n) (((((_n) >> 24) & 0xff) | (((_n) >> 8) & 0xff00) | (((_n) << 8) & 0xff0000) | (((_n) << 24) & 0xff000000)) & 0xffffffff)
#define __mp_bsw64(_n) ((((uint64_t) __mp_bsw32(((uint64_t)(_n)) & 0xffffffff)) << 32) | __mp_bsw32((uint64_t)(_n) >> 32))

#if ____MP__CONF_ENDIAN == 1
#define __mp_htons(_n) __mp_bsw16(_n)
#define __mp_htonl(_n) __mp_bsw32(_n)
#define __mp_htonll(_n) __mp_bsw64(_n)
#define __mp_ntohs(_n) __mp_bsw16(_n)
#define __mp_ntohl(_n) __mp_bsw32(_n)
#define __mp_ntohll(_n) __mp_bsw64(_n)
#elif ____MP__CONF_ENDIAN == 2
#define __mp_htons(_n) (_n)
#define __mp_htonl(_n) (_n)
#define __mp_ntohs(_n) (_n)
#define __mp_ntohl(_n) (_n)
#else
#error "invalid CONF_ENDIAN: it should be either 1 (little) or 2 (big)"
#endif

#ifndef MP_ERR_EAGAIN
#define MP_ERR_EAGAIN (11)
#endif

#ifndef MP_ERR_ENOMEM
#define MP_ERR_ENOMEM (12)
#endif

#ifndef MP_ERR_EINVAL
#define MP_ERR_EINVAL (22)
#endif

#ifndef mp_assert
#define mp_assert(__c) do { if (__c) {} } while (0)
#endif

#ifndef mp_memcmp
static __inline__ uint64_t ____mp_memcmp(const void *a, const void *b, uint64_t l)
{
	uint64_t i;
	for (i = 0; i < l; i++) {
		if (((uint8_t *) a)[i] != ((uint8_t *) b)[i])
			break;
	}
	if (i == l)
		return 0;
	else
		return i;
}
#define mp_memcmp ____mp_memcmp
#endif

#ifndef mp_memcpy
#define mp_memcpy(__dst, __src, __n) \
	do { \
		uint64_t __i; \
		for (__i = 0; __i < (uint64_t)(__n); __i++) \
			((uint8_t *)(__dst))[__i] = ((uint8_t *)(__src))[__i]; \
	} while (0)
#endif

#ifndef mp_memmove
#define mp_memmove(__dst, __src, __n) \
	do { \
		uint64_t ____i; \
		for (____i = (uint64_t)(__n); ____i != 0; ____i--) \
			((uint8_t *)(__dst))[____i] = ((uint8_t *)(__src))[____i]; \
	} while (0)
#endif

#ifndef mp_memset
#define mp_memset(__dst, __c, __n) \
	do { \
		uint64_t ____i; \
		for (____i = 0; ____i < (__n); ____i++) \
			((uint8_t *)(__dst))[____i] = (__c); \
	} while (0)
#endif

#ifndef mp_strlen
static __inline__ uint64_t ____mp_strlen(void *s)
{
	uint64_t i = 0;
	while (1) {
		if (((uint8_t *) s)[i++] == '\0')
			break;
	}
	return i;
}
#define mp_strlen ____mp_strlen
#endif

#define STR2INT(__b, __l, __r, __m, __e) \
	do { \
		(__e) = 0; \
		(__r) = 0; \
		(__m) = 0; \
		{ \
			uint64_t ____i; \
			for (____i = 0; !(__e) && ____i < (__l); ____i++) { \
				uint64_t ____base = 1; \
				{ \
					uint64_t ____j; \
					for (____j = (__l) - ____i; ____j > 1; ____j--) \
						____base *= 10; \
				} \
				switch ((__b)[____i]) { \
				case '-': \
					if (____i != 0) \
						(__e) = (uint64_t) -1; \
					else \
						(__m) = (uint64_t) -1; \
					break; \
				case '0': \
					if ((!(__m) && ____i == 0 && (__l) != 1) || ((__m) && ____i == 1)) \
						(__e) = (uint64_t) -1; \
					break; \
				case '1': \
					(__r) += ____base; \
					break; \
				case '2': \
					(__r) += ____base * 2; \
					break; \
				case '3': \
					(__r) += ____base * 3; \
					break; \
				case '4': \
					(__r) += ____base * 4; \
					break; \
				case '5': \
					(__r) += ____base * 5; \
					break; \
				case '6': \
					(__r) += ____base * 6; \
					break; \
				case '7': \
					(__r) += ____base * 7; \
					break; \
				case '8': \
					(__r) += ____base * 8; \
					break; \
				case '9': \
					(__r) += ____base * 9; \
					break; \
				default: /* invalid */ \
					(__e) = (uint64_t) -1; \
					break; \
				} \
			} \
		} \
	} while (0)

#define UINT2STR(__b, __l, __r, __n, __e) \
	do { \
		uint64_t ____c = 0; \
		uint8_t ____t[21]; \
		{ \
			uint64_t ____n = (__n); \
			do { \
				mp_assert(____c < 21); \
				switch (____n % 10) { \
				case 0: \
					____t[sizeof(____t) - 1 - ____c] = '0'; \
					break; \
				case 1: \
					____t[sizeof(____t) - 1 - ____c] = '1'; \
					break; \
				case 2: \
					____t[sizeof(____t) - 1 - ____c] = '2'; \
					break; \
				case 3: \
					____t[sizeof(____t) - 1 - ____c] = '3'; \
					break; \
				case 4: \
					____t[sizeof(____t) - 1 - ____c] = '4'; \
					break; \
				case 5: \
					____t[sizeof(____t) - 1 - ____c] = '5'; \
					break; \
				case 6: \
					____t[sizeof(____t) - 1 - ____c] = '6'; \
					break; \
				case 7: \
					____t[sizeof(____t) - 1 - ____c] = '7'; \
					break; \
				case 8: \
					____t[sizeof(____t) - 1 - ____c] = '8'; \
					break; \
				case 9: \
					____t[sizeof(____t) - 1 - ____c] = '9'; \
					break; \
				} \
				____n /= 10; \
				____c++; \
			} while (____n); \
		} \
		{ \
			if ((__l) < ____c) { \
				(__e) = (uint64_t) -1; \
				(__r) = 0; \
			} else { \
				mp_memcpy((__b), &____t[sizeof(____t) - ____c], ____c); \
				(__e) = 0; \
				(__r) = ____c; \
			} \
		} \
	} while (0)

#define DECODE_BASE64(__s, __sl, __b, __bl, __l, __e) \
do { \
	uint64_t ____i, ____x; \
	(__e) = 0; \
	(__l) = 0; \
	if ((__sl) % 4 == 0) { \
		for (____i = 0, ____x = 0; ____i < (__sl); ____i++) { \
			uint64_t ____c; \
			switch ((__s)[____i]) { \
			case 'A': \
				____c = 0; \
				break; \
			case 'B': \
				____c = 1; \
				break; \
			case 'C': \
				____c = 2; \
				break; \
			case 'D': \
				____c = 3; \
				break; \
			case 'E': \
				____c = 4; \
				break; \
			case 'F': \
				____c = 5; \
				break; \
			case 'G': \
				____c = 6; \
				break; \
			case 'H': \
				____c = 7; \
				break; \
			case 'I': \
				____c = 8; \
				break; \
			case 'J': \
				____c = 9; \
				break; \
			case 'K': \
				____c = 10; \
				break; \
			case 'L': \
				____c = 11; \
				break; \
			case 'M': \
				____c = 12; \
				break; \
			case 'N': \
				____c = 13; \
				break; \
			case 'O': \
				____c = 14; \
				break; \
			case 'P': \
				____c = 15; \
				break; \
			case 'Q': \
				____c = 16; \
				break; \
			case 'R': \
				____c = 17; \
				break; \
			case 'S': \
				____c = 18; \
				break; \
			case 'T': \
				____c = 19; \
				break; \
			case 'U': \
				____c = 20; \
				break; \
			case 'V': \
				____c = 21; \
				break; \
			case 'W': \
				____c = 22; \
				break; \
			case 'X': \
				____c = 23; \
				break; \
			case 'Y': \
				____c = 24; \
				break; \
			case 'Z': \
				____c = 25; \
				break; \
			case 'a': \
				____c = 26; \
				break; \
			case 'b': \
				____c = 27; \
				break; \
			case 'c': \
				____c = 28; \
				break; \
			case 'd': \
				____c = 29; \
				break; \
			case 'e': \
				____c = 30; \
				break; \
			case 'f': \
				____c = 31; \
				break; \
			case 'g': \
				____c = 32; \
				break; \
			case 'h': \
				____c = 33; \
				break; \
			case 'i': \
				____c = 34; \
				break; \
			case 'j': \
				____c = 35; \
				break; \
			case 'k': \
				____c = 36; \
				break; \
			case 'l': \
				____c = 37; \
				break; \
			case 'm': \
				____c = 38; \
				break; \
			case 'n': \
				____c = 39; \
				break; \
			case 'o': \
				____c = 40; \
				break; \
			case 'p': \
				____c = 41; \
				break; \
			case 'q': \
				____c = 42; \
				break; \
			case 'r': \
				____c = 43; \
				break; \
			case 's': \
				____c = 44; \
				break; \
			case 't': \
				____c = 45; \
				break; \
			case 'u': \
				____c = 46; \
				break; \
			case 'v': \
				____c = 47; \
				break; \
			case 'w': \
				____c = 48; \
				break; \
			case 'x': \
				____c = 49; \
				break; \
			case 'y': \
				____c = 50; \
				break; \
			case 'z': \
				____c = 51; \
				break; \
			case '0': \
				____c = 52; \
				break; \
			case '1': \
				____c = 53; \
				break; \
			case '2': \
				____c = 54; \
				break; \
			case '3': \
				____c = 55; \
				break; \
			case '4': \
				____c = 56; \
				break; \
			case '5': \
				____c = 57; \
				break; \
			case '6': \
				____c = 58; \
				break; \
			case '7': \
				____c = 59; \
				break; \
			case '8': \
				____c = 60; \
				break; \
			case '9': \
				____c = 61; \
				break; \
			case '+': \
				____c = 62; \
				break; \
			case '/': \
				____c = 63; \
				break; \
			case '=': \
				____c = 0; \
				break; \
			default: \
				(__e) = -1; \
				break; \
			} \
			if ((__e) == (uint64_t) -1) \
				break; \
			switch (____i % 4) { \
			case 0: \
				____x  = ____c << 18; \
				break; \
			case 1: \
				____x |= ____c << 12; \
				break; \
			case 2: \
				____x |= ____c <<  6; \
				break; \
			case 3: \
				____x |= ____c <<  0; \
				if ((__l) < (__bl)) \
					(__b)[(__l)++] = 0xff & (____x >> 16); \
				if (!((____i == (__sl) - 1) && (__s)[____i - 1] == '=')) { \
					if ((__l) < (__bl)) \
						(__b)[(__l)++] = 0xff & (____x >>  8); \
					if (!((____i == (__sl) - 1) && (__s)[____i] == '=')) { \
						if ((__l) < (__bl)) \
							(__b)[(__l)++] = 0xff & (____x >>  0); \
					} \
				} \
				break; \
			} \
		} \
	} else { \
		(__e) = (uint64_t) -1; \
	} \
} while (0)

#define FORWARD_HEAD(__m, __i, __j, __c) \
	do { \
		uint64_t ____i = (__i), ____j = (__j), ____c = 0; \
		while (!(____i == MPR_RING_TAIL_IDX(__m) && ____j == MPR_RING_TAIL_OFF(__m))) { \
			uint64_t ____l = (____i == (__i) ? MPR_SLOT_LEN((__m), ____i) - (__j) : MPR_SLOT_LEN((__m), ____i)); \
			if (____c + ____l > (__c)) \
				____l = (__c) - ____c; \
			____c += ____l; \
			____j += ____l; \
			if (____j == MPR_SLOT_LEN(__m, ____i)) { \
				if (!(____i == MPR_RING_TAIL_IDX(__m) && ____j == MPR_RING_TAIL_OFF(__m))) { \
					____j = 0; \
					if (++____i == MPR_RING_NUM_SLOT(__m)) \
						____i = 0; \
				} \
			} \
			if (____c == (__c)) \
				break; \
		} \
		(__i) = ____i; \
		(__j) = ____j; \
	} while (0)

#define EXTRACT_DATA(__m, __i, __j, __o, __b, __l) \
	do { \
		uint64_t ____i = (__i), ____j = (__j); \
		(__l) = 0; \
		while (!(____i == MPR_RING_TAIL_IDX(__m) && ____j == MPR_RING_TAIL_OFF(__m))) { \
			if ((__l) == (__o)) { \
				break; \
			} \
			{ \
				uint64_t ____l = (____i == MPR_RING_TAIL_IDX(__m) ? MPR_RING_TAIL_OFF(__m) - ____j : MPR_SLOT_LEN((__m), ____i) - ____j); \
				if ((__o) - (__l) < ____l) \
					____l = (__o) - (__l); \
				mp_memcpy(&(__b)[(__l)], &((uint8_t *) MPR_SLOT_PTR((__m), ____i))[____j], ____l); \
				____j += ____l; \
				if (____j == MPR_SLOT_LEN((__m), ____i)) { \
					if (!(____i == MPR_RING_TAIL_IDX(__m) && ____j == MPR_RING_TAIL_OFF(__m))) { \
						____j = 0; \
						____i = (____i + 1 == MPR_RING_NUM_SLOT(__m) ? 0 : ____i + 1); \
					} \
				} \
				(__l) += ____l; \
			} \
		} \
		(__i) = ____i; \
		(__j) = ____j; \
	} while (0)

#define EXTRACT_VAL_FLOAT(__m, __i, __j, __l, __a, __b, __f, __n, __e) \
	do { \
		uint64_t ____msglen = 0; \
		char ____msg[40]; /* 64-bit max 18446744073709551615, 20 bytes * 2 */ \
		(__n) = 0; \
		(__e) = 0; \
		(__a) = 0; \
		(__b) = 0; \
		(__f) = 0; \
		if ((__l) <= 40) { \
			EXTRACT_DATA(__m, __i, __j, __l, ____msg, ____msglen); \
			{ \
				uint64_t ____dot = (uint64_t) -1, ____i; \
				for (____i = 0; ____i < ____msglen; ____i++) { \
					if (____msg[____i] == '.') { \
						if (____dot == (uint64_t) -1) { \
							____dot = ____i; \
							break; \
						} \
					} \
				} \
				if ((____dot != (uint64_t) -1) && ((____dot == 0 && ____msglen > 0) || ((____msglen > 0) && (____msglen - 1 == ____dot)))) \
					(__e) = (uint64_t) -1; \
				else { \
					if (____dot == (uint64_t) -1) { \
						(__f) = 0; \
						STR2INT(____msg, ____msglen, (__a), (__n), (__e)); \
					} else { \
						STR2INT(____msg, ____dot, (__a), (__n), (__e)); \
						if ((__e) == 0) { \
							uint64_t ____c; \
							for (____c = ____dot + 1; ____c < ____msglen; ____c++) { \
								if (____msg[____c] != '0') \
									break; \
							} \
							{ \
								uint64_t ____n; \
								STR2INT(&____msg[____c], ____msglen - ____c, (__b), ____n, (__e)); \
								if (____n) { \
									(__n) = (uint64_t) -1; \
									(__e) = (uint64_t) -1; \
								} \
							} \
							if ((__e) == 0) \
								(__f) = ____msglen - ____dot - 1; \
						} \
					} \
				} \
			} \
		} else \
			(__e) = (uint64_t) -1; \
	} while (0)

#define EXTRACT_VAL(__m, __i, __j, __l, __v, __n, __e) \
	do { \
		(__e) = (__n) = 0; \
		if (__l > 20) \
			(__e) = (uint64_t) -1; \
		else { \
			uint64_t ____msglen = 0; \
			char ____msg[20]; /* 64-bit max 18446744073709551615, 20 bytes */ \
			(__n) = 0; \
			(__e) = 0; \
			EXTRACT_DATA(__m, __i, __j, __l, ____msg, ____msglen); \
			STR2INT(____msg, ____msglen, (__v), (__n), (__e)); \
		} \
	} while (0)

#define EXTRACT_NOREPLY(__m, __i, __j, __e) \
	do { \
		uint64_t ____msglen = 0; \
		char ____msg[7]; /* noreply is 7 bytes */ \
		(__e) = 0; \
		EXTRACT_DATA(__m, __i, __j, 7, ____msg, ____msglen); \
		if ((__e) != (uint64_t) -1 && ____msglen == 7) { \
			if (!mp_memcmp(____msg, "noreply", ____msglen)) \
				(__e) = 0; \
			else \
				(__e) = -1; \
		} else \
			(__e) = -1; \
	} while (0)

#define REMAIN_LENGTH(__m, __i, __j, __l) \
	do { \
		uint64_t ____i = (__i), ____j = (__j); \
		(__l) = 0; \
		while (!(____i == MPR_RING_TAIL_IDX(__m) && ____j == MPR_RING_TAIL_OFF(__m))) { \
			{ \
				uint32_t ____l = (____i == MPR_RING_TAIL_IDX(__m) ? MPR_RING_TAIL_OFF(__m) - ____j : MPR_SLOT_LEN((__m), ____i) - ____j); \
				____j += ____l; \
				if (____j == MPR_SLOT_LEN((__m), ____i)) { \
					if (!(____i == MPR_RING_TAIL_IDX(__m) && ____j == MPR_RING_TAIL_OFF(__m))) { \
						____j = 0; \
						____i = (____i + 1 == MPR_RING_NUM_SLOT(__m) ? 0 : ____i + 1); \
					} \
				} \
				(__l) += ____l; \
			} \
		} \
	} while (0)

#define CMD_SET					    (1)
#define CMD_ADD			    		    (2)
#define CMD_REPLACE		    		    (3)
#define CMD_APPEND		    		    (4)
#define CMD_PREPEND		    		    (5)
#define CMD_CAS			    		    (6)
#define CMD_GET			    		    (7)
#define CMD_GETS		    		    (8)
#define CMD_GAT			    		    (9)
#define CMD_GATS		    		    (10)
#define CMD_DELETE		    		    (11)
#define CMD_INCR		    		    (12)
#define CMD_DECR		    		    (13)
#define CMD_TOUCH		    		    (14)
#define CMD_ME			    		    (15)
#define CMD_MG			    		    (16)
#define CMD_MS			    		    (17)
#define CMD_MD			    		    (18)
#define CMD_MA			    		    (19)
#define CMD_MN			    		    (20)
#define CMD_SLABS		    		    (21)
#define CMD_SLABS__REASSIGN	    		    (22)
#define CMD_SLABS__AUTOMOVE	    		    (23)
#define CMD_LRU			    		    (24)
#define CMD_LRU__TUNE		    		    (25)
#define CMD_LRU__MODE		    		    (26)
#define CMD_LRU__TEMP_TTL	    		    (27)
#define CMD_LRU_CRAWLER				    (28)
#define CMD_LRU_CRAWLER__ENABLE	    		    (29)
#define CMD_LRU_CRAWLER__DISABLE    		    (30)
#define CMD_LRU_CRAWLER__SLEEP	    		    (31)
#define CMD_LRU_CRAWLER__TOCRAWL		    (32)
#define CMD_LRU_CRAWLER__CRAWL			    (33)
#define CMD_LRU_CRAWLER__METADUMP		    (34)
#define CMD_LRU_CRAWLER__MGDUMP	    		    (35)
#define CMD_WATCH		    		    (36)
#define CMD_STATS		    		    (44)
#define CMD_STATS__SETTINGS			    (45)
#define CMD_STATS__ITEMS			    (46)
#define CMD_STATS__SIZES			    (47)
#define CMD_STATS__SLABS			    (48)
#define CMD_STATS__CONNS			    (49)
#define CMD_STATS__SSL_HANDSHAKE_ERRORS		    (50)
#define CMD_STATS__SSL_MIN_VERSION		    (51)
#define CMD_STATS__SSL_NEW_SESSIONS		    (52)
#define CMD_STATS__TIME_SINCE_SERVER_CERT_REFRESH   (53)
#define CMD_FLUSH_ALL				    (54)
#define CMD_CACHE_MEMLIMIT			    (55)
#define CMD_SHUTDOWN				    (56)
#define CMD_VERSION			    	    (57)
#define CMD_QUIT			    	    (58)
#define CMD_MISBEHAVE			    	    (59)

#define UDP_REQ(____udp_hdr) ((uint16_t)(__mp_ntohs(*((uint16_t *)&udp_hdr[0])))) /* request id */
#define UDP_SEQ(____udp_hdr) ((uint16_t)(__mp_ntohs(*((uint16_t *)&udp_hdr[2])))) /* sequence number */
#define UDP_CNT(____udp_hdr) ((uint16_t)(__mp_ntohs(*((uint16_t *)&udp_hdr[4])))) /* total number of datagrams */
#define UDP_RSV(____udp_hdr) ((uint16_t)(__mp_ntohs(*((uint16_t *)&udp_hdr[6])))) /* reserved and not used, it must be zero */

/*
 * Memcached Parser Request (MPR)
 *
 * uint64_t     err_line;
 * uint64_t     mode_binary;
 * uint64_t     udp_hdr;
 * uint64_t     ring_num_slots;
 * uint64_t     ring_head_idx;
 * uint64_t     ring_head_off;
 * uint64_t     ring_tail_idx;
 * uint64_t     ring_tail_off;
 * --------- slot[0] ------------
 * uint64_t     ptr;
 * uint64_t     len;
 * --------- slot[1] ------------
 * uint64_t     ptr;
 * uint64_t     len;
 * ------
 *  ...
 * --- slot[ring_num_slots-1] ---
 * uint64_t     ptr;
 * uint64_t     len;
 */

#define MPR_ERRLINE(____m)         (((uint64_t *)(____m))[0])
#define MPR_MODE_BINARY(____m)     (((uint64_t *)(____m))[1])
#define MPR_UDPHDR(____m)          (((uint64_t *)(____m))[2])
#define MPR_RING_NUM_SLOT(____m)   (((uint64_t *)(____m))[3])
#define MPR_RING_HEAD_IDX(____m)   (((uint64_t *)(____m))[4])
#define MPR_RING_HEAD_OFF(____m)   (((uint64_t *)(____m))[5])
#define MPR_RING_TAIL_IDX(____m)   (((uint64_t *)(____m))[6])
#define MPR_RING_TAIL_OFF(____m)   (((uint64_t *)(____m))[7])
#define MPR_SLOT_PTR(____m, ____i) (((uint64_t *)(____m))[8 + 2 * ____i])
#define MPR_SLOT_LEN(____m, ____i) (((uint64_t *)(____m))[8 + 2 * ____i + 1])
#define MPR_MEM_SIZE(____c) (8 * sizeof(uint64_t) + (____c) * sizeof(uint64_t) * 2)

#define MC_KV_CMD_OPFLAG_ERROR                    (1UL <<  0)
#define MC_KV_CMD_OPFLAG_FOUND                    (1UL <<  1)
#define MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS           (1UL <<  2)
#define MC_KV_CMD_OPFLAG_UPDATE                   (1UL <<  3)
#define MC_KV_CMD_OPFLAG_DELETE                   (1UL <<  4)
#define MC_KV_CMD_OPFLAG_CAS                      (1UL <<  5)
#define MC_KV_CMD_OPFLAG_SET_FLAG                 (1UL <<  6)
#define MC_KV_CMD_OPFLAG_SET_EXPTIME              (1UL <<  7)
#define MC_KV_CMD_OPFLAG_SET_CAS_UNIQUE_IF_NEW    (1UL <<  8)
#define MC_KV_CMD_OPFLAG_INVALIDATE               (1UL <<  9)
#define MC_KV_CMD_OPFLAG_INVALIDATE_WITH_CAS      (1UL << 10)
#define MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE          (1UL << 11)
#define MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE_IF_STALE (1UL << 12)
#define MC_KV_CMD_OPFLAG_NUMERICAL                (1UL << 13)
#define MC_KV_CMD_OPFLAG_NUMERICAL_INCREMENT      (1UL << 14)
#define MC_KV_CMD_OPFLAG_CONCAT                   (1UL << 15)
#define MC_KV_CMD_OPFLAG_CONCAT_APPEND            (1UL << 16)
#define MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE          (1UL << 17)
#define MC_KV_CMD_OPFLAG_COPY_VAL                 (1UL << 18)
#define MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE         (1UL << 19)

#define MC_KV_EXTRA_FLAG_STALE (1UL << 0)
#define MC_KV_EXTRA_FLAG_WON   (1UL << 1)

#define MP_KV_CMD_OPFLAGS(__c)         (((uint64_t *)(__c))[ 0])
#define MP_KV_CMD_VAL_LEN(__c)         (((uint64_t *)(__c))[ 1])
#define MP_KV_CMD_VAL_PTR_0(__c)       (((uint64_t *)(__c))[ 2])
#define MP_KV_CMD_VAL_PTR_1(__c)       (((uint64_t *)(__c))[ 3])
#define MP_KV_CMD_FLAGS(__c)           (((uint64_t *)(__c))[ 4])
#define MP_KV_CMD_CAS_UNIQUE(__c)      (((uint64_t *)(__c))[ 5])
#define MP_KV_CMD_ADD_TIME(__c)        (((uint64_t *)(__c))[ 6])
#define MP_KV_CMD_LAST_ACCESS(__c)     (((uint64_t *)(__c))[ 7])
#define MP_KV_CMD_EXPTIME(__c)         (((uint64_t *)(__c))[ 8])
#define MP_KV_CMD_CLS(__c)             (((uint64_t *)(__c))[ 9])
#define MP_KV_CMD_EXTRA_FLAGS(__c)     (((uint64_t *)(__c))[10])
#define MP_KV_CMD_INCDEC_VAL(__c)      (((uint64_t *)(__c))[11])
#define MP_KV_CMD_NEW_INCDEC_VAL(__c)  (((uint64_t *)(__c))[12])
#define MP_KV_CMD_RECACHE_EXPTIME(__c) (((uint64_t *)(__c))[13])
#define MP_KV_CMD_NEW_CAS_UNIQUE(__c)  (((uint64_t *)(__c))[14])
#define MP_KV_CMD_ERRNO(__c)           (((uint64_t *)(__c))[15])
#define MP_KV_CMD_VAL(__c)             (((void **)(__c))[16])
#define MP_KV_CMD_SIZE (sizeof(uint64_t) * 17)

#define CLEAR_RESPONSE() MP_OPS_CLEAR_RESPONSE(opaque)

#define PUSH_RESPONSE(__d, __l) \
	do { \
		if (MP_OPS_PUSH_RESPONSE(opaque, (const char *)(__d), (__l))) \
			return 0; \
	} while (0)

#define PUSH_RESPONSE_BINARY(__magic, __opcode, __key_length, __extras_length, __data_type, __status, __total_body_length, __bin_opaque_be, __cas_unique, __extras, __key, __val, __msg, __msglen) \
	do { \
		struct { \
			uint8_t magic; \
			uint8_t opcode; \
			uint16_t key_length_be; \
			uint8_t extras_length; \
			uint8_t data_type; \
			uint16_t status_be; \
			uint32_t total_body_length_be; \
			uint32_t opaque_be; \
			uint64_t cas_be; \
		} response; \
		response.magic = (__magic); \
		response.opcode = (__opcode); \
		response.key_length_be = __mp_htons(__key_length); \
		response.extras_length = (__extras_length); \
		response.data_type = (__data_type); \
		response.status_be = __mp_htons(__status); \
		response.total_body_length_be = __mp_htonl(__total_body_length); \
		response.opaque_be = __bin_opaque_be; \
		response.cas_be = __mp_htonll(__cas_unique); \
		PUSH_RESPONSE(&response, sizeof(response)); \
		if (__extras_length) \
			PUSH_RESPONSE((__extras), (__extras_length)); \
		if (__key_length) { \
			mp_assert((__key) != NULL); \
			PUSH_RESPONSE((__key), (__key_length)); \
		} \
		if ((__total_body_length) - (__extras_length) - (__key_length) - (__msglen)) { \
			mp_assert((__val) != NULL); \
			PUSH_RESPONSE((__val), (__total_body_length) - (__extras_length) - (__key_length) - (__msglen)); \
		} \
		if (__msglen) { \
			mp_assert((__msg) != NULL); \
			PUSH_RESPONSE((__msg), (__msglen)); \
		} \
	} while (0)

/* parser entry point */

static long parse_memcached_request(void *mpr, void *opaque)
{
	long ret = 0;
	if (!((MPR_RING_HEAD_IDX(mpr) == MPR_RING_TAIL_IDX(mpr)) && (MPR_RING_HEAD_OFF(mpr) == MPR_RING_TAIL_OFF(mpr)))) {
		uint64_t ring_cur_idx = MPR_RING_HEAD_IDX(mpr), ring_cur_off = MPR_RING_HEAD_OFF(mpr);
		switch (MPR_MODE_BINARY(mpr)) {
		case 0x80: /* magic byte of request packet */
			/*
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_MAGICN="\x80"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_OPCODE="\x07"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_KEYLEN="\x00\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_EXTLEN="\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_DATYPE="\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_RSRVED="\x00\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_TOTLEN="\x00\x00\x00\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_OPAQUE="\x00\x00\x00\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUIT_REQSTR="${QUIT_MAGICN}${QUIT_OPCODE}${QUIT_KEYLEN}${QUIT_EXTLEN}${QUIT_DATYPE}${QUIT_RSRVED}${QUIT_TOTLEN}${QUIT_OPAQUE}${QUIT_CASUNI}"
			 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
			 */
			{ /* binary */
				uint8_t req_opcode;
				uint16_t req_key_length;
				uint8_t req_extras_length;
				uint8_t req_data_type;
				uint16_t req_vbucket_id;
				uint32_t req_total_body_length;
				uint32_t req_opaque_be;
				uint64_t req_cas;
				{
					struct { /* 24 bytes */
						uint8_t magic;
						uint8_t opcode;
						uint16_t key_length_be;
						uint8_t extras_length;
						uint8_t data_type;
						uint16_t vbucket_id_be;
						uint32_t total_body_length_be;
						uint32_t opaque_be;
						uint64_t cas_be;
					} _req, *req = &_req;
					mp_memset(req, 0, sizeof(*req));
					{
						uint64_t l;
						EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, sizeof(*req), (uint8_t *) req, l);
						if (l < sizeof(*req))
							return MP_ERR_EAGAIN;
					}
					mp_assert(req->magic == 0x80);
					req_opcode = req->opcode;
					req_key_length = __mp_ntohs(req->key_length_be);
					req_extras_length = req->extras_length;
					req_data_type = req->data_type;
					req_vbucket_id = __mp_ntohs(req->vbucket_id_be);
					req_total_body_length = __mp_ntohl(req->total_body_length_be);
					req_opaque_be = req->opaque_be;
					req_cas = __mp_ntohll(req->cas_be);
					(void) req_vbucket_id;
				}
				{
					uint64_t remain;
					REMAIN_LENGTH(mpr, ring_cur_idx, ring_cur_off, remain);
					if (remain < req_total_body_length)
						return MP_ERR_EAGAIN;
				}
				if (req_key_length > ____MP__CONF_MAX_KEY_LEN) {
					MP_TEST_HOOK();
					/*
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the key length specified in the binary request header is larger than the 250-byte limit"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x00" # get
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\xff" # 255 byte key
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\xff"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="$(printf "\\\x%02x" {1..255})"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${KEYSTR}"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}" "${TESTID}"
					 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
					 */
					PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, 0, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
					FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
					ret = -1;
					break;
				}
				switch (req_opcode) {
				case 0x00: /* get */
				case 0x09: /* getq */
				case 0x0c: /* getk */
				case 0x0d: /* getkq */
				case 0x1d: /* gat */
				case 0x1e: /* gatq */
					{
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint32_t exptime = 0;
						{
							uint64_t kl;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, req_key_length, key, kl);
							mp_assert(kl == req_key_length);
						}
						if (req_opcode == 0x1d /* gat */ || req_opcode == 0x1e /* gatq */) {
							uint64_t tmp;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, 4, &exptime, tmp);
							mp_assert(tmp == 4);
						}
						if ((req_opcode == 0x00 /* get */
								|| req_opcode == 0x09 /* getq */
								|| req_opcode == 0x0c /* getk */
								|| req_opcode == 0x0d /* getkq */)
								&& req_extras_length) {
							MP_TEST_HOOK(); /* the binary request header for get,getq,getk,getq has an extra field */
							PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
						} else if ((req_opcode == 0x1d /* gat */ || req_opcode == 0x1e /* gatq */)
								&& req_extras_length != 4) {
							MP_TEST_HOOK(); /* the extra field for the binary request header for gat,gatq is not 4 bytes */
							PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
						} else {
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							if (req_opcode == 0x1d /* gat */ || req_opcode == 0x1e /* gatq */) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_SET_EXPTIME;
								MP_KV_CMD_EXPTIME(cmd) = exptime;
							}
							MP_OPS_KV_CMD(mpr, key, req_key_length, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								uint32_t flags = MP_KV_CMD_FLAGS(cmd);
								PUSH_RESPONSE_BINARY(0x81, req_opcode, ((req_opcode == 0x0c /* getk */) || (req_opcode == 0x0d /* getkq */) ? req_key_length : 0),
										sizeof(flags), req_data_type, 0x0 /* no error */, sizeof(flags) + ((req_opcode == 0x0c /* getk */) || (req_opcode == 0x0d /* getkq */) ? req_key_length : 0) + MP_KV_CMD_VAL_LEN(cmd),
										req_opaque_be, MP_KV_CMD_CAS_UNIQUE(cmd),
										&flags, key, MP_KV_CMD_VAL(cmd), NULL, 0);
							} else {
								MP_TEST_HOOK(); /* binary get,getq,getk,getq commands send not found */
								PUSH_RESPONSE_BINARY(0x81, req_opcode, ((req_opcode == 0x0c /* getk */) || (req_opcode == 0x0d /* getkq */) ? req_key_length : 0),
										0, req_data_type, 0x1 /* key not found */, 9 + ((req_opcode == 0x0c /* getk */) || (req_opcode == 0x0d /* getkq */) ? req_key_length : 0),
										req_opaque_be, 0,
										NULL, key, NULL, "Not found", 9);
							}
						}
					}
					break;
				case 0x02: /* add */
				case 0x12: /* addq */
				case 0x0e: /* append */
				case 0x19: /* appendq */
				case 0x0f: /* prepend */
				case 0x1a: /* prependq */
				case 0x03: /* replace */
				case 0x13: /* replaceq */
				case 0x01: /* set */
				case 0x11: /* setq */
					if (req_total_body_length - req_extras_length - req_key_length > ____MP__CONF_MAX_VAL_LEN) {
						/*
						 * XXX:
						 * the 1 MiB limit is applied for value length + implementation-specific metadata
						 * so, the acceptable value size is smaller than 1 MiB and specific to implementation and configuration
						 */
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the value length specified in the binary request header is larger than the 1MiB limit"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x02" # add
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01" # 1 byte key
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x10\x00\x0a"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x00$(for i in {1..4096}; do printf "\\\x%02x" {0..255}; done)" # 1Mib + 1
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, 0, 0x3 /* Value too large */, 10, req_opaque_be, 0, NULL, NULL, NULL, "Too large.", 10);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
					} else if (req_extras_length != 8) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="extra fields or binary add,addq,append,appendq,prepend,prependq,replace,replaceq,set,setq commands is not 8 bytes"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x02" # add
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01" # 1 byte key
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x04" # 4 byte extras
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x06"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22" # 1 byte
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint32_t flags, exptime;
						{
							uint8_t extras[8];
							uint64_t l;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, 8, extras, l);
							mp_assert(l == 8);
							flags = __mp_ntohl(*((uint32_t *) &extras[0]));
							exptime = __mp_ntohl(*((uint32_t *) &extras[4]));
						}
						{
							uint64_t kl;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, req_key_length, key, kl);
							mp_assert(kl == req_key_length);
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							switch (req_opcode) {
							case 0x02: /* add */
							case 0x12: /* addq */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS | MC_KV_CMD_OPFLAG_SET_FLAG | MC_KV_CMD_OPFLAG_SET_EXPTIME;
								break;
							case 0x01: /* set */
							case 0x11: /* setq */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS | MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_SET_FLAG | MC_KV_CMD_OPFLAG_SET_EXPTIME | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case 0x03: /* replace */
							case 0x13: /* replaceq */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_SET_FLAG | MC_KV_CMD_OPFLAG_SET_EXPTIME | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case 0x0e: /* append */
							case 0x19: /* appendq */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_CONCAT | MC_KV_CMD_OPFLAG_CONCAT_APPEND | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case 0x0f: /* prepend */
							case 0x1a: /* prependq */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_CONCAT | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							}
							if (req_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CAS;
								MP_KV_CMD_CAS_UNIQUE(cmd) = req_cas;
							}
							MP_KV_CMD_FLAGS(cmd) = flags;
							MP_KV_CMD_EXPTIME(cmd) = exptime;
							MP_KV_CMD_VAL_PTR_0(cmd) = ring_cur_idx;
							MP_KV_CMD_VAL_PTR_1(cmd) = ring_cur_off;
							MP_KV_CMD_VAL_LEN(cmd) = req_total_body_length - req_extras_length - req_key_length;
							MP_OPS_KV_CMD(mpr, key, req_key_length, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_ERROR) {
								if (MP_KV_CMD_ERRNO(cmd) == MP_ERR_ENOMEM) {
									MP_TEST_HOOK(); /* the cache has no sufficient space to store a new item  */
									/* XXX: NO TEST */
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x82 /* Out of memory */,
											13, req_opaque_be, 0, NULL, NULL, NULL, "Out of memory", 13);
								} else
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x81 /* Unknown command */,
											0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
							} else if (((req_opcode == 0x02 /* add */ || req_opcode == 0x12 /* addq */) && !(MP_KV_CMD_OPFLAGS(cmd) & (MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS)))
										|| ((req_opcode == 0x01 /* set */ || req_opcode == 0x11 /* setq */) && (!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS) || !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE)))
										|| ((!(req_opcode == 0x02 /* add */ || req_opcode == 0x12 /* addq */) && !(req_opcode == 0x01 /* set */ || req_opcode == 0x11 /* setq */)) && !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE))) {
								switch (req_opcode) {
								case 0x02: /* add */
								case 0x0e: /* append */
								case 0x0f: /* prepend */
								case 0x03: /* replace */
								case 0x01: /* set */
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */,
											0, req_opaque_be, MP_KV_CMD_CAS_UNIQUE(cmd), NULL, NULL, NULL, NULL, 0);
									break;
								case 0x12: /* addq */
								case 0x19: /* appendq */
								case 0x1a: /* prependq */
								case 0x13: /* replaceq */
								case 0x11: /* setq */
									MP_TEST_HOOK();
									/*
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary setq does not send reply"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x11" # setq
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0a"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									break;
								}
							} else {
								if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
									if (req_cas || (req_opcode == 0x02 /* add */ || req_opcode == 0x12 /* addq */)) {
										MP_TEST_HOOK();
										/*
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary storage commands with cas command did not updated item because data exist"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x01" # set
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0a"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\xff\xff\xff\xff"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary storage commands with add command did not updated item because data exist"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x02" # add
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0a"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary storage commands with addq command did not updated item because data exist"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x12" # addq
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0a"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x2 /* key exists */,
												20, req_opaque_be, 0, NULL, NULL, NULL,
												"Data exists for key.", 20);
									} else
										mp_assert(0);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary add,addq,append,appendq,prepend,prependq,replace,replaceq,set,setq commands do not find key"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x03" # replace
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0a"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x12"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									/*
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary add,addq,append,appendq,prepend,prependq,replace,replaceq,set,setq commands do not find key"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x13" # replaceq
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0a"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x12"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x22"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x1 /* key not found */,
											9, req_opaque_be, 0, NULL, NULL, NULL,
											"Not found", 9);
									if (req_opcode == 0x02 /* add */ || req_opcode == 0x12 /* addq */
											|| req_opcode == 0x01 /* set */ || req_opcode == 0x11 /* setq */)
										mp_assert(0);
								}
							}
							FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length - req_extras_length - req_key_length);
						}
					}
					break;
				case 0x1c: /* touch */
					if (req_extras_length != 4) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="extra field for binary touch command is not 4 bytes"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x1c"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08" # 8 byte extras
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x09"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint32_t exptime;
						{
							uint8_t extras[4];
							uint64_t l;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, 4, extras, l);
							mp_assert(l == 4);
							exptime = __mp_ntohl(*((uint32_t *) extras));
						}
						{
							uint64_t kl;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, req_key_length, key, kl);
							mp_assert(kl == req_key_length);
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_SET_EXPTIME | MC_KV_CMD_OPFLAG_COPY_VAL;
							if (req_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CAS;
								MP_KV_CMD_CAS_UNIQUE(cmd) = req_cas;
							}
							MP_KV_CMD_EXPTIME(cmd) = exptime;
							MP_KV_CMD_VAL_PTR_0(cmd) = ring_cur_idx;
							MP_KV_CMD_VAL_PTR_1(cmd) = ring_cur_off;
							MP_KV_CMD_VAL_LEN(cmd) = req_total_body_length - req_extras_length - req_key_length;
							MP_OPS_KV_CMD(mpr, key, req_key_length, cmd, opaque);
							if (!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE))
								PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */,
										0, req_opaque_be, MP_KV_CMD_CAS_UNIQUE(cmd), NULL, NULL, NULL, NULL, 0);
							else {
								if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND)
									mp_assert(0);
								else {
									MP_TEST_HOOK();
									/*
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary touch command send not found"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x1c"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x04"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x05"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x12"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x1 /* key not found */,
											9, req_opaque_be, 0, NULL, NULL, NULL,
											"Not found", 9);
								}
							}
						}
					}
					break;
				case 0x04: /* delete */
				case 0x14: /* deleteq */
					if (!req_key_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="key length for binary delete commands is 0"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else if (req_total_body_length - req_extras_length - req_key_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="value length for binary delete command is specified"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x02"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=""
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x12"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else if (req_extras_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary delete,deleteq commands have extra fields"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08" # 8 byte extras
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x09"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						{
							uint64_t kl;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, req_key_length, key, kl);
							mp_assert(kl == req_key_length);
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_DELETE;
							if (req_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CAS;
								MP_KV_CMD_CAS_UNIQUE(cmd) = req_cas;
							}
							MP_OPS_KV_CMD(mpr, key, req_key_length, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								mp_assert(!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_DELETE));
								if (req_opcode != 0x14 /* deleteq */)
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */, 0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
								else {
									MP_TEST_HOOK();
									/*
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary deleteq command does not send reply"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x14"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=""
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
									 */
								}
							} else {
								MP_TEST_HOOK();
								/*
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary delete command send not found"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x04"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x01"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=""
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x12"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
								 */
								/*
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary deleteq command send not found"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x14"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x01"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=""
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x12"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x1 /* key not found */,
										9, req_opaque_be, 0, NULL, NULL, NULL,
										"Not found", 9);
							}
						}
					}
					break;
				case 0x05: /* increment */
				case 0x06: /* decrement */
				case 0x15: /* incrementq */
				case 0x16: /* decrementq */
					if (!req_key_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="key length for binary increment,incrementq,decrement,decrementq commands is 0"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x05"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x14"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x14"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else if (req_total_body_length - req_extras_length - req_key_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="value length for binary increment,incrementq,decrement,decrementq commands is specified"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x05"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x14"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x16"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else if (req_extras_length != 20) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="extra fields for binary increment,incrementq,decrement,decrementq commands are not 20 bytes"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x05"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08" # 8 byte extras
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x09"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						uint64_t amount_to_add, initial_value;
						uint32_t exptime;
						{
							uint8_t tmp[20];
							uint64_t l;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, 20, tmp, l);
							mp_assert(l == 20);
							amount_to_add = __mp_ntohll(*((uint64_t *)&tmp[0]));
							initial_value = __mp_ntohll(*((uint64_t *)&tmp[8]));
							exptime       = __mp_ntohl(*((uint32_t *)&tmp[16]));
						}
						{
							uint8_t key[____MP__CONF_MAX_KEY_LEN];
							{
								uint64_t kl;
								EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, req_key_length, key, kl);
								mp_assert(kl == req_key_length);
							}
							if (exptime == 0xffffffff) {
								MP_TEST_HOOK();
								/*
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary increment,incrementq,decrement,decrementq commands must return not found if specified expiration time is 0xffffffff and counter is 0"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x05"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x14"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x15"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x01"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\x00\x00\x00\x00\x00\x00\x00\x00"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\xff\xff\xff\xff"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x1 /* key not found */,
										9, req_opaque_be, 0, NULL, NULL, NULL,
										"Not found", 9);
							} else {
								uint8_t cmd[MP_KV_CMD_SIZE];
								mp_memset(cmd, 0, sizeof(cmd));
								MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE | MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS | MC_KV_CMD_OPFLAG_NUMERICAL;
								if (req_opcode == 0x05 /* increment */ || req_opcode == 0x15 /* incrementq */)
									MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_NUMERICAL_INCREMENT;
								MP_KV_CMD_INCDEC_VAL(cmd) = amount_to_add;
								MP_KV_CMD_NEW_INCDEC_VAL(cmd) = initial_value;
								MP_KV_CMD_EXPTIME(cmd) = exptime;
								MP_OPS_KV_CMD(mpr, key, req_key_length, cmd, opaque);
								if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_ERROR) {
									MP_TEST_HOOK();
									/*
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary increment,incrementq,decrement,decrementq commands could not update value because item was not a number"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x11" # setq
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0c"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x41\x42\x43" # ABC
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x05"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x14"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x15"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x31"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\x00\x00\x00\x00\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\x00\x00\x00\x00"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS+="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x6 /* incr/decr on non-numeric value */, 46, req_opaque_be, 0, NULL, NULL, NULL, "Non-numeric server-side value for incr or decr", 46);
								} else {
									if (!(req_opcode == 0x15 /* incrementq */ || req_opcode == 0x16 /* decrementq */)) {
										uint64_t r, minus, e;
										STR2INT((uint8_t *) MP_KV_CMD_VAL(cmd), MP_KV_CMD_VAL_LEN(cmd), r, minus, e);
										mp_assert(!(minus || e));
										r = __mp_htonll(r);
										PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */, sizeof(r), req_opaque_be, MP_KV_CMD_CAS_UNIQUE(cmd), NULL, NULL, NULL, &r, sizeof(r));
									} else {
										MP_TEST_HOOK();
										/*
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x11" # setq
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x0c"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: VALSTR="\x31\x32\x33" # 123
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}${VALSTR}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary incrementq command does not return result"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x15"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x14"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x15"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\x00\x00\x00\x00\x00\x00\x00\x01"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\x00\x00\x00\x00\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS+="\x00\x00\x00\x00"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS+="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
										 */
									}
								}
							}
						}
					}
					break;
				case 0x07: /* quit */
				case 0x17: /* quitq */
					if (req_total_body_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary quit command has extra field, key, or value"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x07" # quit
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01" # key length 1
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary quitq command has extra field, key, or value"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x07" # quit
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01" # key length 1
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						if (req_opcode == 0x07 /* quit */) {
							PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */, 0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
						} else {
							MP_TEST_HOOK(); /* binary quitq command does not send reply */
						}
						ret = -1;
					}
					break;
				case 0x08: /* flush */
				case 0x18: /* flushq */
					if (req_key_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="key for binary flush,flushq command is specified"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x08"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08" # 8 byte extras
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x08"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x81 /* invalid arguments */, 15, req_opaque_be, 0, NULL, NULL, NULL, "Unknown command", 15);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else if (req_extras_length != 4) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="extra fields for binary flush,flushq command is not 4 bytes"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x08"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x08" # 8 byte extras
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x08"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						uint32_t expr_sec;
						{
							uint8_t tmp[4];
							uint64_t l;
							EXTRACT_DATA(mpr, ring_cur_idx, ring_cur_off, 4, tmp, l);
							mp_assert(l == 4);
							expr_sec = *((uint32_t *) tmp);
						}
						MP_OPS_KV_FLUSH_ALL(expr_sec, opaque);
						if (req_opcode == 0x08 /* flush */)
							PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */, 0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
						else {
							MP_TEST_HOOK();
							/*
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary flushq command does not send reply"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x18"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x04"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x04"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
							 */
						}
					}
					break;
				case 0x0a: /* no-op */
					if (req_total_body_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary no-op command have extra field, key, or value"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x0a"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */, 0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
					break;
				case 0x0b: /* version */
					if (req_total_body_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary version command have extra field, key, or value"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x0b"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x0 /* no error */, 6, req_opaque_be, 0, NULL, NULL, NULL, "1.6.38", 6);
					break;
				case 0x10: /* stat */
					if (req_total_body_length != req_key_length) {
						/*
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary stat command has value"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x10"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x02" # supposedly has value, but this does not
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=""
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x22"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x0b" # version
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: EXTRAS=""
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS+="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						/* XXX: this is accepted */
					}
					if (req_extras_length) {
						MP_TEST_HOOK();
						/*
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: T_INFO="binary stat command has extra fiedld"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: MAGICN="\x80"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPCODE="\x10"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYLEN="\x00\x01"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTLEN="\x04"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: DATYPE="\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: RSRVED="\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: TOTLEN="\x00\x00\x00\x05"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: OPAQUE="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: CASUNI="\x00\x00\x00\x00\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: EXTRAS="\x00\x00\x00\x00"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: KEYSTR="\x11"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: QUERYS="${MAGICN}${OPCODE}${KEYLEN}${EXTLEN}${DATYPE}${RSRVED}${TOTLEN}${OPAQUE}${CASUNI}${EXTRAS}${KEYSTR}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: test_binary_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_BINARY_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x4 /* invalid arguments */, 17, req_opaque_be, 0, NULL, NULL, NULL, "Invalid arguments", 17);
						FORWARD_HEAD(mpr, ring_cur_idx, ring_cur_off, req_total_body_length);
						ret = -1;
					} else {
						/* XXX: not implemented */
					}
					break;
				case 0x1b: /* verbosity */
					break;
				default:
					MP_TEST_HOOK(); /* unknown binary command */
					PUSH_RESPONSE_BINARY(0x81, req_opcode, 0, 0, req_data_type, 0x81 /* Unknown command */,
							0, req_opaque_be, 0, NULL, NULL, NULL, NULL, 0);
					break;
				}
			}
			break;
		default:
			/*
			 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUIT_REQSTR="quit\r\n"
			 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
			 */
			{ /* text */
				struct word {
					uint64_t ptr[2];
					uint64_t len;
				};
				struct word wl[32];
				uint64_t cmd_type = 0, word_num = 0;
				{ /* fetch and parse the first line and determine the comand */
					char prev = ' ', prevprev = ' ', line_closed = 0, second_line = 0;
					uint64_t n = 0;
					while (!(ring_cur_idx == MPR_RING_TAIL_IDX(mpr) && ring_cur_off == MPR_RING_TAIL_OFF(mpr))) {
						n++;
						if (word_num == 32) {
							/*
							 * XXX
							 * the default implementation can handle many words for multi get
							 * but this implementation does not
							 */
							return -MP_ERR_ENOMEM; /* XXX: close connection */
						}
						switch (((char *) MPR_SLOT_PTR(mpr, ring_cur_idx))[ring_cur_off]) {
						case ' ':
							if (prev != ' ' && prev != '\r' && prev != '\n') {
								wl[word_num].len = n - 1;
								n = 0;
								word_num++;
							}
							break;
						case '\n':
							if (prev == '\r') {
								if (prevprev != ' ') {
									wl[word_num].len = n - 2;
									n = 0;
									word_num++;
								}
								line_closed = 1;
							}
							break;
						default:
							if (prev == ' ' || prev == '\r' || prev == '\n') {
								wl[word_num].ptr[0] = ring_cur_idx;
								wl[word_num].ptr[1] = ring_cur_off;
							}
							break;
						}
						prevprev = prev;
						prev = ((char *) MPR_SLOT_PTR(mpr, ring_cur_idx))[ring_cur_off];
						if (++ring_cur_off == MPR_SLOT_LEN(mpr, ring_cur_idx)) {
							ring_cur_off = 0;
							if (++ring_cur_idx == MPR_RING_NUM_SLOT(mpr))
								ring_cur_idx = 0;
						}
						if (line_closed) {
							if (!second_line) {
								if (14 < wl[0].len)
									cmd_type = (uint64_t) -1;
								else {
									uint64_t cmdlen = 0;
									char cmd[14 /* cache_memlimit (14 bytes at maximum) */ ];
									EXTRACT_DATA(mpr, wl[0].ptr[0], wl[0].ptr[1], wl[0].len, cmd, cmdlen);
									switch (cmd[0]) {
									/*
									 * add
									 * append
									 */
									case 'a':
										switch (cmdlen) {
										case 3:
											if (!mp_memcmp(cmd, "add", cmdlen))
												cmd_type = CMD_ADD;
											break;
										case 6:
											if (!mp_memcmp(cmd, "append", cmdlen))
												cmd_type = CMD_APPEND;
											break;
										}
										break;
									/*
									 * cache_memlimit
									 * cas
									 */
									case 'c':
										switch (cmdlen) {
										case 14:
											if (!mp_memcmp(cmd, "cache_memlimit", cmdlen))
												cmd_type = CMD_CACHE_MEMLIMIT;
											break;
										case 3:
											if (!mp_memcmp(cmd, "cas", cmdlen))
												cmd_type = CMD_CAS;
											break;
										}
										break;
									/*
									 * decr
									 * delete
									 */
									case 'd':
										switch (cmdlen) {
										case 4:
											if (!mp_memcmp(cmd, "decr", cmdlen))
												cmd_type = CMD_DECR;
											break;
										case 6:
											if (!mp_memcmp(cmd, "delete", cmdlen))
												cmd_type = CMD_DELETE;
											break;
										}
									break;
									/*
									 * flush_all
									 */
									case 'f':
										switch (cmdlen) {
										case 9:
											if (!mp_memcmp(cmd, "flush_all", cmdlen))
												cmd_type = CMD_FLUSH_ALL;
											break;
										}
										break;
									/*
									 * gat
									 * gats
									 * get
									 * gets
									 */
									case 'g':
										switch (cmdlen) {
										case 3:
											if (!mp_memcmp(cmd, "get", cmdlen)) {
												cmd_type = CMD_GET;
												break;
											}
											if (!mp_memcmp(cmd, "gat", cmdlen))
												cmd_type = CMD_GAT;
											break;
										case 4:
											if (!mp_memcmp(cmd, "gets", cmdlen)) {
												cmd_type = CMD_GETS;
												break;
											}
											if (!mp_memcmp(cmd, "gats", cmdlen))
												cmd_type = CMD_GATS;
											break;
										}
										break;
									/*
									 * incr
									 */
									case 'i':
										switch (cmdlen) {
										case 4:
											if (!mp_memcmp(cmd, "incr", cmdlen))
												cmd_type = CMD_INCR;
											break;
										}
										break;
									/*
									 * lru
									 */
									case 'l':
										switch (cmdlen) {
										case 3:
											if (!mp_memcmp(cmd, "lru", cmdlen))
												cmd_type = CMD_LRU;
											break;
										case 11:
											if (!mp_memcmp(cmd, "lru_crawler", cmdlen))
												cmd_type = CMD_LRU_CRAWLER;
											break;
										}
										break;
									/*
									 * ma
									 * md
									 * me
									 * mg
									 * misbehave
									 * mn
									 * ms
									 */
									case 'm':
										switch (cmdlen) {
										case 2:
											switch (cmd[1]) {
											case 'a':
												cmd_type = CMD_MA;
												break;
											case 'd':
												cmd_type = CMD_MD;
												break;
											case 'e':
												cmd_type = CMD_ME;
												break;
											case 'g':
												cmd_type = CMD_MG;
												break;
											case 'n':
												cmd_type = CMD_MN;
												break;
											case 's':
												cmd_type = CMD_MS;
												break;
											}
											break;
										case 9:
											if (!mp_memcmp(cmd, "misbehave", cmdlen))
												cmd_type = CMD_MISBEHAVE;
											break;
										}
										break;
									/*
									 * prepend
									 */
									case 'p':
										switch (cmdlen) {
										case 7:
											if (!mp_memcmp(cmd, "prepend", cmdlen))
												cmd_type = CMD_PREPEND;
											break;
										}
										break;
									/*
									 * quit
									 */
									case 'q':
										switch (cmdlen) {
										case 4:
											if (!mp_memcmp(cmd, "quit", cmdlen))
												cmd_type = CMD_QUIT;
											break;
										}
										break;
									/*
									 * replace
									 */
									case 'r':
										switch (cmdlen) {
										case 7:
											switch (cmd[1]) {
											case 'e':
												if (!mp_memcmp(cmd, "replace", cmdlen))
													cmd_type = CMD_REPLACE;
												break;
											}
											break;
										}
										break;
									/*
									 * set
									 * shutdown
									 * slabs
									 * stats
									 */
									case 's':
										switch (cmdlen) {
										case 3:
											if (!mp_memcmp(cmd, "set", cmdlen))
												cmd_type = CMD_SET;
											break;
										case 8:
											if (!mp_memcmp(cmd, "shutdown", cmdlen))
												cmd_type = CMD_SHUTDOWN;
											break;
										case 5:
											if (!mp_memcmp(cmd, "slabs", cmdlen)) {
												cmd_type = CMD_SLABS;
												break;
											}
											if (!mp_memcmp(cmd, "stats", cmdlen)) {
												cmd_type = CMD_STATS;
												break;
											}
											break;
										}
										break;
									/*
									 * touch
									 */
									case 't':
										switch (cmdlen) {
										case 5:
											if (!mp_memcmp(cmd, "touch", cmdlen))
												cmd_type = CMD_TOUCH;
											break;
										}
										break;
									/*
									 * version
									 */
									case 'v':
										switch (cmdlen) {
										case 7:
											if (!mp_memcmp(cmd, "version", cmdlen))
												cmd_type = CMD_VERSION;
											break;
										}
										break;
									/*
									 * watch
									 */
									case 'w':
										switch (cmdlen) {
										case 5:
											if (!mp_memcmp(cmd, "watch", cmdlen))
												cmd_type = CMD_WATCH;
											break;
										}
										break;
									}
									switch (cmd_type) {
									case CMD_SET:
									case CMD_ADD:
									case CMD_REPLACE:
									case CMD_APPEND:
									case CMD_PREPEND:
										if (word_num == 5 || word_num == 6) {
											line_closed = 0;
											second_line = 1;
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words consisting the set,add,replace,prepend commands is neither 6 nor 7"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 5 noreply dummy\r\n\a b c\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											cmd_type = -1;
										}
										break;
									case CMD_CAS:
										if (word_num == 6 || word_num == 7) {
											line_closed = 0;
											second_line = 1;
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words consisting the cas command is neither 7 or 8"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 5 1 noreply dummy\r\n\a b c\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											cmd_type = -1;
										}
										break;
									case CMD_MS:
										if (word_num >= 3) {
											line_closed = 0;
											second_line = 1;
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words consisting the ms command is fewer than 2"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms\r\n\a\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											cmd_type = -1;
										}
										break;
									}
								}
							} else
								break;
						}
						if (line_closed)
							break;
					}
					if (!line_closed)
						return 0;
				}
				switch (cmd_type) {
				/* storage commands */
				case CMD_SET:
				case CMD_ADD:
				case CMD_REPLACE:
				case CMD_APPEND:
				case CMD_PREPEND:
				case CMD_CAS:
					/* XXX: no need to check the number of words because it is done during the line check above */
					if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the key length specified for cas is longer then the 250-byte limit"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas $(printf "a%.0s" {0..250}) 0 0 1 2\r\n\a\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
						ring_cur_idx = wl[word_num - 1].ptr[0];
						ring_cur_off = wl[word_num - 1].ptr[1];
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl, flags, exptime, bytes, cas_unique, noreply = 0, immediate_expire = 0;
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, flags, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag specified for set,add,replace,append,prepend,cas commands is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a -1 0 1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag specified for set,add,replace,append,prepend,cas commands is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a a 0 1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							} else if (4294967295UL < flags) {
								/* XXX: protocol specification defines flags to be 16-bit unsigned integer, but new versions allocate a bigger size integer */
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag specified for set,add,replace,append,prepend,cas commands is bigger than 4294967295 (this is accepted by newer version)"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 4294967296 0 1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* XXX: this is accepted */
							}
						}
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[3].ptr[0], wl[3].ptr[1], wl[3].len, exptime, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="expiration time specified for set,add,replace,append,prepend,cas commands is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 -1 1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* XXX: if exptime is minus, the item immediately expires */
								immediate_expire = 1;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="expiration time specified for set,add,replace,append,prepend,cas commands is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 a 1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							}
						}
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[4].ptr[0], wl[4].ptr[1], wl[4].len, bytes, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value length specified for set,add,replace,append,prepend,cas commands is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 -1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value length specified for set,add,replace,append,prepend,cas commands is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 a\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							}
						}
						if (cmd_type == CMD_CAS) {
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[5].ptr[0], wl[5].ptr[1], wl[5].len, cas_unique, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cas value specified for cas command is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 1 -1\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cas value specified for cas command is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 1 a\r\n\a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							}
						} else
							cas_unique = 0;
						if (cmd_type == CMD_CAS ? (word_num == 8) : (word_num == 7)) {
							uint64_t e = 0;
							if (cmd_type == CMD_CAS) {
								if (wl[6].len != 7) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="for the cas command, the word length of 7th token is not 7 bytes, it has to be noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 1 1 noreplyx\r\n\a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/* XXX: this is accepted */
								} else {
									EXTRACT_NOREPLY(mpr, wl[6].ptr[0], wl[6].ptr[1], e);
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="for the cas command, the word length of 7th token is 7 bytes, but it does not match noreply"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 1 1 noreplx\r\n\a\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/* this is not considered an error */
									} else
										noreply = 1;
								}
							} else {
								if (wl[5].len != 7) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="for the set,add,replace,append,prepend commands, the word length of 6th token is not 7 bytes, it should be noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1 noreplyx\r\n\a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/* this is not considered an error */
								} else {
									EXTRACT_NOREPLY(mpr, wl[5].ptr[0], wl[5].ptr[1], e);
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="for the set,add,replace,append,prepend commands, the word length of 6th token is 7 bytes, it does not match noreply"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1 noreplx\r\n\a\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/* this is not considered an error */
									} else
										noreply = 1;
								}
							}
						}
						MP_OPS_DEBUG_PRINTF("flags %lu exptime %lu (immediate_expire %s) bytes %lu cas_unique %lu noreply %lu\n",
								flags, exptime, (immediate_expire ? "yes" : "no"), bytes, cas_unique, noreply);
						if (bytes > ____MP__CONF_MAX_VAL_LEN) { /* delete the key if exist */
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_DELETE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the value length specified for set,add,replace,append,prepend,cas commands is larger than 1 MiB"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1048577\r\n$(printf "a%.0s" {1..1048577})\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("SERVER_ERROR object too large for cache\r\n", 41);
						} else {
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_FLAG | MC_KV_CMD_OPFLAG_SET_EXPTIME;
							if (immediate_expire)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
							switch (cmd_type) {
							case CMD_ADD:
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS;
								break;
							case CMD_SET:
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS | MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case CMD_REPLACE:
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case CMD_APPEND:
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_CONCAT | MC_KV_CMD_OPFLAG_CONCAT_APPEND | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case CMD_PREPEND:
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_CONCAT | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							case CMD_CAS:
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_CAS | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								break;
							}
							MP_KV_CMD_VAL_PTR_0(cmd) = wl[word_num - 1].ptr[0];
							MP_KV_CMD_VAL_PTR_1(cmd) = wl[word_num - 1].ptr[1];
							MP_KV_CMD_VAL_LEN(cmd) = wl[word_num - 1].len;
							MP_KV_CMD_FLAGS(cmd) = flags;
							MP_KV_CMD_EXPTIME(cmd) = exptime;
							MP_KV_CMD_CAS_UNIQUE(cmd) = cas_unique;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_ERROR) {
								if (MP_KV_CMD_ERRNO(cmd) == MP_ERR_ENOMEM) {
									MP_TEST_HOOK(); /* out of memory for the value for set,add,replace,append,prepend,cas commands */
									/* XXX: NO TEST */
									PUSH_RESPONSE("SERVER_ERROR out of memory storing object\r\n", 43);
								} else
									PUSH_RESPONSE("ERROR\r\n", 7);
							} else if (((cmd_type == CMD_ADD) && !(MP_KV_CMD_OPFLAGS(cmd) & (MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS)))
										|| ((cmd_type == CMD_SET) && (!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS) || !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE)))
										|| (((cmd_type != CMD_ADD) && (cmd_type != CMD_SET)) && !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE))) {
								if (!noreply)
									PUSH_RESPONSE("STORED\r\n", 8);
								else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="set,add,replace,append,prepend,cas commands do not print stored because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1 noreply\r\na\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							} else {
								if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
									switch (cmd_type) {
									case CMD_SET:
									case CMD_REPLACE:
									case CMD_APPEND:
									case CMD_PREPEND:
										mp_assert(0);
										break;
									case CMD_ADD:
										if (!noreply) {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="add command print not_stored"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="add a 0 0 1\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											PUSH_RESPONSE("NOT_STORED\r\n", 12);
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="add command does not print not_stored because of noreply"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="add a 0 0 1 noreply\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
										}
										break;
									case CMD_CAS:
										if (!noreply) {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cas command print exists"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 1 100000\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											PUSH_RESPONSE("EXISTS\r\n", 8);
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cas command does not print exists because of noreply"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas a 0 0 1 100000 noreply\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
										}
										break;
									}
								} else {
									switch (cmd_type) {
									case CMD_ADD:
									case CMD_SET:
										mp_assert(0);
									case CMD_REPLACE:
									case CMD_APPEND:
									case CMD_PREPEND:
										if (!noreply) {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="replace,append,prepend commands print not_stored"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="replace b 0 0 1\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											PUSH_RESPONSE("NOT_STORED\r\n", 12);
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="replace,append,prepend commands do not print not_stored because of noreply"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="replace b 0 0 1 noreply\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
										}
										break;
									case CMD_CAS:
										if (!noreply) {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cas command print not_found"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas b 0 0 1 10000\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											PUSH_RESPONSE("NOT_FOUND\r\n", 11);
										} else {
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cas command does not print not_found because of noreply"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cas b 0 0 1 10000 noreply\r\na\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
										}
										break;
									}
								}
							}
						}
					}
					break;
				/* retrieval commands */
				case CMD_GET:
				case CMD_GETS:
				case CMD_GAT:
				case CMD_GATS:
					if (word_num < 2) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="get,gets,gat,gats commands less than 2 words"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="get\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						uint64_t exptime = 0, word_start = 1, immediate_expire = 0;
						if (cmd_type == CMD_GAT || cmd_type == CMD_GATS) {
							uint64_t minus, e;
							EXTRACT_VAL(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, exptime, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="expiration time for gat,gats commands is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="gat -1 a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								immediate_expire = 1;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="expiration time for gat,gats commands is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="gat a a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid exptime argument\r\n", 39);
								break;
							}
							word_start = 2;
						}
						{
							uint64_t c;
							for (c = word_start; c < word_num; c++) {
								uint8_t key[____MP__CONF_MAX_KEY_LEN];
								uint64_t kl;
								if (wl[c].len > ____MP__CONF_MAX_KEY_LEN) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for get,gets,gat,gats commands is bigger than 250 bytes"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="get a $(printf "a%.0s" {0..250})\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									CLEAR_RESPONSE();
									PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
									break;
								}
								EXTRACT_DATA(mpr, wl[c].ptr[0], wl[c].ptr[1], wl[c].len, key, kl);
								{
									uint8_t cmd[MP_KV_CMD_SIZE];
									mp_memset(cmd, 0, sizeof(cmd));
									if (cmd_type == CMD_GAT || cmd_type == CMD_GATS) {
										MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_SET_EXPTIME | MC_KV_CMD_OPFLAG_COPY_VAL;
										if (immediate_expire)
											MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
									}
									MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
									if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
										PUSH_RESPONSE("VALUE ", 6);
										PUSH_RESPONSE((const char *) key, kl);
										PUSH_RESPONSE(" ", 1);
										{
											char buf[21];
											uint64_t r, e;
											UINT2STR(buf, sizeof(buf), r, MP_KV_CMD_FLAGS(cmd), e);
											mp_assert(!e);
											PUSH_RESPONSE(buf, r);
										}
										PUSH_RESPONSE(" ", 1);
										{
											char buf[21];
											uint64_t r, e;
											UINT2STR(buf, sizeof(buf), r, MP_KV_CMD_VAL_LEN(cmd), e);
											mp_assert(!e);
											PUSH_RESPONSE(buf, r);
										}
										if (cmd_type == CMD_GETS || cmd_type == CMD_GATS) {
											PUSH_RESPONSE(" ", 1);
											{
												char buf[21];
												uint64_t r, e;
												UINT2STR(buf, sizeof(buf), r, MP_KV_CMD_CAS_UNIQUE(cmd), e);
												mp_assert(!e);
												PUSH_RESPONSE(buf, r);
											}
										}
										PUSH_RESPONSE("\r\n", 2);
										PUSH_RESPONSE((const char *) MP_KV_CMD_VAL(cmd), MP_KV_CMD_VAL_LEN(cmd));
										PUSH_RESPONSE("\r\n", 2);
									}
								}
							}
							if (c == word_num)
								PUSH_RESPONSE("END\r\n", 5);
						}
					}
					break;
				case CMD_DELETE:
					if (word_num != 2 && word_num != 3) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for delete command is neither 2 nor 3"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="delete a noreply a\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format.  Usage: delete <key> [noreply]\r\n", 70);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for delete command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="delete $(printf "a%.0s" {0..250})\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl, noreply = 0;
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						if (word_num == 3) {
							if (wl[2].len != 7) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for delete is not 7, it should be noreply"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="delete a noreplyx\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format.  Usage: delete <key> [noreply]\r\n", 70);
								break;
							} else {
								uint64_t e;
								EXTRACT_NOREPLY(mpr, wl[2].ptr[0], wl[2].ptr[1], e);
								if (e) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for delete is 7, but does not match noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="delete a noreplx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR bad command line format.  Usage: delete <key> [noreply]\r\n", 70);
									break;
								} else
									noreply = 1;
							}
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_DELETE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								mp_assert(!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_DELETE));
								if (!noreply) {
									MP_TEST_HOOK(); /* delete command prints deleted */
									PUSH_RESPONSE("DELETED\r\n", 9);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="delete command does not print deleted because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\na\r\ndelete a noreply\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							} else {
								if (!noreply) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="delete command prints not_found"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="delete a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("NOT_FOUND\r\n", 11);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="delete command does not print not_found because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="delete a noreply\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							}
						}
					}
					break;
				case CMD_INCR:
				case CMD_DECR:
					if (word_num != 3 && word_num != 4) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for incr,decr commands is neither 3 nor 4"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="incr a 1 noreply 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for incr,decr command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="incr $(printf "a%.0s" {0..250}) 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl, val, noreply = 0;
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, val, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for incr,decr commands is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\n1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS+="incr a -1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid numeric delta argument\r\n", 45);
								break;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for incr,decr commands is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="incr a a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid numeric delta argument\r\n", 45);
								break;
							}
						}
						if (word_num == 4) {
							if (wl[3].len != 7) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 4th word for incr,decr commands is not 7, it should be noreply"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="incr a 1 noreplyx\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* this is not considered error */
							} else {
								uint64_t e = 0;
								EXTRACT_NOREPLY(mpr, wl[3].ptr[0], wl[3].ptr[1], e);
								if (e == (uint64_t) -1) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 4th word for incr,decr is 7, but does not match noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="incr a 1 noreplx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/* this is not considered error */
								} else
									noreply = 1;
							}
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
							if (cmd_type == CMD_INCR)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_NUMERICAL | MC_KV_CMD_OPFLAG_NUMERICAL_INCREMENT | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
							else
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_NUMERICAL | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
							MP_KV_CMD_INCDEC_VAL(cmd) = val;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_ERROR) {
								if (MP_KV_CMD_ERRNO(cmd) == MP_ERR_EINVAL) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the value updated by incr,decr commands is not a number"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\na\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS+="incr a 1\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR cannot increment or decrement non-numeric value\r\n", 62);
								} else
									PUSH_RESPONSE("ERROR\r\n", 7);
							} else if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								if (!noreply) {
									PUSH_RESPONSE((const char *) MP_KV_CMD_VAL(cmd), MP_KV_CMD_VAL_LEN(cmd));
									PUSH_RESPONSE("\r\n", 2);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="incr,decr commands does not send reply because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\n1\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS+="incr a 1 noreply\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							} else {
								if (!noreply) {
									MP_TEST_HOOK(); /* incr,decr commands print not_found */
									PUSH_RESPONSE("NOT_FOUND\r\n", 11);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="incr,decr commands does not send reply because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="incr b 1 noreply\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							}
						}
					}
					break;
				case CMD_TOUCH:
					if (word_num != 3 && word_num != 4) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for touch command is neither 3 nor 4"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch a 1000 noreply 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for touch command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch $(printf "a%.0s" {0..250}) 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl, exptime, noreply = 0, immediate_expire = 0;
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, exptime, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for touch command is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch a -1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								immediate_expire = 1;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for touch command is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch a a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid exptime argument\r\n", 39);
								break;
							}
						}
						if (word_num == 4) {
							if (wl[3].len != 7) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 4th word for touch command is not 7, it should be noreply"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch a 1000 noreplyx\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* this is not considered error */
							} else {
								uint64_t e = 0;
								EXTRACT_NOREPLY(mpr, wl[3].ptr[0], wl[3].ptr[1], e);
								if (e == (uint64_t) -1) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 4th word for touch command is 7, but does not match noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch a 1000 noreplx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/* this is not considered error */
								} else
									noreply = 1;
							}
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_SET_EXPTIME | MC_KV_CMD_OPFLAG_COPY_VAL;
							MP_KV_CMD_EXPTIME(cmd) = exptime;
							if (immediate_expire)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								if (!noreply) {
									MP_TEST_HOOK(); /* touch command prints touched */
									PUSH_RESPONSE("TOUCHED\r\n", 9);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="touch command does not print touched because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\na\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS+="touch a 1000 noreply\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							} else {
								if (!noreply) {
									MP_TEST_HOOK(); /* touch command prints not_found */
									PUSH_RESPONSE("NOT_FOUND\r\n", 11);
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="touch command prints not_found because of noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="touch b 1000 noreply\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
								}
							}
						}
					}
					break;
				case CMD_ME:
					if (word_num < 2) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for me command is less than 2"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="me \r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for me command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="me $(printf "a%.0s" {0..250})\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl;
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						if (word_num >= 3) {
							if (wl[2].len != 1) {
								MP_TEST_HOOK();
								/*
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the flag length for me command is not 1"
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS="me a aa\r\n"
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: 
								 */
								/* XXX: this is accepted, option is just ignored */
							} else {
								uint8_t flag[1]; /* 'b' one byte */
								uint8_t l = 0;
								EXTRACT_DATA(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, flag, l);
								if (flag[0] != 'b') {
									MP_TEST_HOOK();
									/*
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the flag length for me command is 1, but it is not b"
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS="me a a\r\n"
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									/* XXX: this is accepted, option is just ignored */
								} else {
									uint8_t tmp[____MP__CONF_MAX_KEY_LEN];
									uint64_t l, e;
									DECODE_BASE64(key, kl, tmp, sizeof(tmp), l, e);
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for me command cannot be decoded for base64"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="me a b\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
										break;
									}
									mp_assert(l < ____MP__CONF_MAX_KEY_LEN); /* same buffer space must be enough */
									mp_memcpy(key, tmp, l);
									kl = l;
								}
							}
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								PUSH_RESPONSE("ME ", 3);
								PUSH_RESPONSE((const char *) key, kl);
								if (!MP_KV_CMD_EXPTIME(cmd))
									PUSH_RESPONSE(" exp=-1", 7);
								else {
									PUSH_RESPONSE(" exp=", 5);
									{
										char tmp[21];
										uint64_t r, e;
										UINT2STR(tmp, sizeof(tmp), r, MP_KV_CMD_EXPTIME(cmd) - MP_OPS_UTIL_TIME_NS(opaque), e);
										mp_assert(!e);
										PUSH_RESPONSE(tmp, r);
									}
								}
								{
									PUSH_RESPONSE(" la=", 4);
									{
										char tmp[21];
										uint64_t r, e;
										UINT2STR(tmp, sizeof(tmp), r, (MP_OPS_UTIL_TIME_NS(opaque) - MP_KV_CMD_LAST_ACCESS(cmd)) / 1000000000UL, e);
										mp_assert(!e);
										PUSH_RESPONSE(tmp, r);
									}
								}
								PUSH_RESPONSE(" cas=", 5);
								{
									char tmp[21];
									uint64_t r, e;
									UINT2STR(tmp, sizeof(tmp), r, MP_KV_CMD_CAS_UNIQUE(cmd), e);
									mp_assert(!e);
									PUSH_RESPONSE(tmp, r);
								}
								if (MP_KV_CMD_LAST_ACCESS(cmd) != MP_KV_CMD_ADD_TIME(cmd))
									PUSH_RESPONSE(" fetch=yes", 10);
								else
									PUSH_RESPONSE(" fetch=no", 9);
								PUSH_RESPONSE(" cls=", 5);
								{
									char tmp[21];
									uint64_t r, e;
									UINT2STR(tmp, sizeof(tmp), r, MP_KV_CMD_CLS(cmd), e);
									mp_assert(!e);
									PUSH_RESPONSE(tmp, r);
								}
								PUSH_RESPONSE(" size=", 6);
								{
									char tmp[21];
									uint64_t r, e;
									UINT2STR(tmp, sizeof(tmp), r, kl + MP_KV_CMD_VAL_LEN(cmd), e);
									mp_assert(!e);
									PUSH_RESPONSE(tmp, r);
								}
								PUSH_RESPONSE("\r\n", 6);
							} else
								PUSH_RESPONSE("EN\r\n", 4);
						}
					}
					break;
				case CMD_MG:
					if (word_num < 2) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for mg command is less than 2"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg  \r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for mg command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg $(printf "a%.0s" {0..250})\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl;
						char b_base64 = 0;
						char c_return_cas = 0;
						char f_return_client_flags = 0;
						char h_return_hit_or_not = 0;
						char k_return_key = 0;
						char l_return_time_la_sec = 0;
						char O_opaque = 0; char O_opaque_buf[31]; uint64_t O_opaque_buf_len = 0;
						char q_noreply = 0;
						char s_return_item_size = 0;
						char t_return_item_ttl = 0;
						char u_dont_bump_item_lru = 0;
						char v_return_value = 0;
						char E_new_cas = 0; uint64_t E_new_cas_val = 0;
						char N_vivify_on_miss = 0; uint64_t N_vivify_on_miss_val = 0; uint64_t immediate_expire = 0;
						char R_win_for_recache = 0; uint64_t R_win_for_recache_val = 0;
						char T_update_ttl = 0; uint64_t T_update_ttl_val = 0;
						char flags_order[16];
						{
							uint64_t i, found_error = 0;
							for (i = 2; i < word_num && !found_error; i++) {
								uint64_t l;
								char tmp[32]; /* O(token) 1 + 31 bytes token */
								if (wl[i].len > sizeof(tmp)) {
									EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], 1, tmp, l);
									switch (tmp[0]) {
									case 'O':
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="opaque length for mg command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a O$(printf "1%.0s" {0..31})\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR opaque token too long\r\n", 36);
										break;
									default:
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag length for mg command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a T$(printf "1%.0s" {0..31})\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
										break;
									}
									found_error = 1;
									break;
								} /* other single-byte options ignore content from tmp[1] */
								EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], wl[i].len, tmp, l);
								flags_order[i - 2] = tmp[0];
								switch (tmp[0]) {
								case 'b':
									if (b_base64) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="b flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a b b\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										b_base64 = 1;
									break;
								case 'c':
									if (c_return_cas) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="c flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a c c\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										c_return_cas = 1;
									break;
								case 'f':
									if (f_return_client_flags) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="f flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a f f\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										f_return_client_flags = 1;
									break;
								case 'h':
									if (h_return_hit_or_not) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="h flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a h h\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										h_return_hit_or_not = 1;
									break;
								case 'k':
									if (k_return_key) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="k flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a k k\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										k_return_key = 1;
									break;
								case 'l':
									if (l_return_time_la_sec) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="l flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a l l\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										l_return_time_la_sec = 1;
									break;
								case 'q':
									if (q_noreply) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="q flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a q q\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										q_noreply = 1;
									break;
								case 's':
									if (s_return_item_size) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="s flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a s s\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										s_return_item_size = 1;
									break;
								case 't':
									if (t_return_item_ttl) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="t flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a t t\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										t_return_item_ttl = 1;
									break;
								case 'u':
									if (u_dont_bump_item_lru) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="u flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a u u\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										u_dont_bump_item_lru = 1;
									break;
								case 'v':
									if (v_return_value) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="v flag for mg command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a v v\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										v_return_value = 1;
									break;
								case 'O':
								case 'E':
								case 'N':
								case 'R':
								case 'T':
									if (l == 1 && tmp[0] != 'O' /* O does not need to take subsequent bytes */) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for mg command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a E\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="N flag for mg command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a N\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="R flag for mg command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a R\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for mg command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a T\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
										found_error = 1;
									} else {
										switch (tmp[0]) {
										case 'O':
											if (O_opaque) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="O flag for mg command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a O O\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												O_opaque = 1;
												O_opaque_buf_len = l - 1;
												mp_memcpy(O_opaque_buf, &tmp[1], O_opaque_buf_len);
											}
											break;
										case 'E':
											if (E_new_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for mg command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a E1 E1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												E_new_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, E_new_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for mg command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a E-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for mg command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a Ea\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										case 'N':
											if (N_vivify_on_miss) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="N flag for mg command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a N1 N1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												N_vivify_on_miss = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, N_vivify_on_miss_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for N flag for mg command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a N-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														/* XXX: this is accepted */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK(); /* value for N flag for mg command is not a number */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										case 'R':
											if (R_win_for_recache) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="R flag for mg command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a R1 R1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												R_win_for_recache = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, R_win_for_recache_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for R flag for mg command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a R-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														/* XXX: this is accepted */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for R flag for mg command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a Ra\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										case 'T':
											if (T_update_ttl) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for mg command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a T1 T1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												T_update_ttl = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, T_update_ttl_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for mg command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a T-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														/* XXX: this is accepted */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for mg command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a Ta\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										}
									}
									break;
								default:
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined flag for mg command"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR invalid flag\r\n", 27);
									found_error = 1;
									break;
								}
							}
							if (found_error)
								break;
						}
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						if (b_base64) {
							uint8_t tmp[____MP__CONF_MAX_KEY_LEN];
							uint64_t l, e;
							DECODE_BASE64(key, kl, tmp, sizeof(tmp), l, e);
							if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for mg command cannot be decoded for base64"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="mg a b\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR error decoding key\r\n", 33);
								break;
							}
							mp_assert(l < ____MP__CONF_MAX_KEY_LEN); /* same buffer space must be enough */
							mp_memcpy(key, tmp, l);
							kl = l;
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE_IF_STALE | MC_KV_CMD_OPFLAG_COPY_VAL;
							if (E_new_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_CAS_UNIQUE_IF_NEW;
								MP_KV_CMD_NEW_CAS_UNIQUE(cmd) = E_new_cas_val;
							}
							if (R_win_for_recache) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE;
								MP_KV_CMD_RECACHE_EXPTIME(cmd) = R_win_for_recache_val;
							}
							if (N_vivify_on_miss) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS|MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE|MC_KV_CMD_OPFLAG_SET_EXPTIME;
								MP_KV_CMD_EXPTIME(cmd) = N_vivify_on_miss_val;
							}
							if (T_update_ttl) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE|MC_KV_CMD_OPFLAG_SET_EXPTIME;
								MP_KV_CMD_EXPTIME(cmd) = T_update_ttl_val; /* T_update_ttl_val has higher priority than N_vivify_on_miss_val */
							}
							if (immediate_expire)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & (MC_KV_CMD_OPFLAG_FOUND) || (N_vivify_on_miss && !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS))) {
								if (v_return_value) {
									PUSH_RESPONSE("VA ", 3);
									{
										char tmp[21];
										uint64_t r, e;
										UINT2STR(tmp, sizeof(tmp), r, MP_KV_CMD_VAL_LEN(cmd), e);
										mp_assert(!e);
										PUSH_RESPONSE(tmp, r);
									}
								} else
									PUSH_RESPONSE("HD", 2);
								{
									uint64_t i;
									for (i = 0; i < word_num - 2; i++) {
										switch (flags_order[i]) {
										case 'c':
											{
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_CAS_UNIQUE(cmd), e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 'c';
												PUSH_RESPONSE(tmp, 2 + r);
											}
											break;
										case 'f':
											{
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_FLAGS(cmd), e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 'f';
												PUSH_RESPONSE(tmp, 2 + r);
											}
											break;
										case 'h':
											if (MP_KV_CMD_LAST_ACCESS(cmd) != MP_KV_CMD_ADD_TIME(cmd))
												PUSH_RESPONSE(" h1", 3);
											else
												PUSH_RESPONSE(" h0", 3);
											break;
										case 'k':
											PUSH_RESPONSE(" k", 2);
											PUSH_RESPONSE((const char *) key, kl);
											break;
										case 'l':
											{
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, (MP_OPS_UTIL_TIME_NS(opaque) - MP_KV_CMD_LAST_ACCESS(cmd)) / 1000000000UL, e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 'l';
												PUSH_RESPONSE(tmp, 2 + r);
											}
											break;
										case 's':
											{
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_VAL_LEN(cmd), e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 's';
												PUSH_RESPONSE(tmp, 2 + r);
											}
											break;
										case 't':
											if (!MP_KV_CMD_EXPTIME(cmd))
												PUSH_RESPONSE(" t-1\0", 4);
											else {
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_EXPTIME(cmd), e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 't';
												PUSH_RESPONSE(tmp, 2 + r);
											}
											break;
										case 'O':
											PUSH_RESPONSE(" O", 2);
											PUSH_RESPONSE(O_opaque_buf, O_opaque_buf_len);
											break;
										case 'N':
										case 'R':
										case 'T':
											/* nothing to be done here */
											break;
										}
									}
								}
								if (MP_KV_CMD_EXTRA_FLAGS(cmd) & MC_KV_EXTRA_FLAG_WON) {
									if (MP_KV_CMD_OPFLAGS(cmd) & (MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE_IF_STALE|MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE))
										PUSH_RESPONSE(" Z", 2);
								}
								if (MP_KV_CMD_EXTRA_FLAGS(cmd) & MC_KV_EXTRA_FLAG_STALE)
									PUSH_RESPONSE(" X", 2);
								if (MP_KV_CMD_EXTRA_FLAGS(cmd) & MC_KV_EXTRA_FLAG_WON) {
									if (!(MP_KV_CMD_OPFLAGS(cmd) & (MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE_IF_STALE|MC_KV_CMD_OPFLAG_WIN_FOR_RECACHE)))
										PUSH_RESPONSE(" W", 2);
								}
								if (v_return_value) {
									PUSH_RESPONSE("\r\n", 2);
									PUSH_RESPONSE((const char *) MP_KV_CMD_VAL(cmd), MP_KV_CMD_VAL_LEN(cmd));
								}
								if (!u_dont_bump_item_lru) {
									/* TODO: update lru */
								}
								PUSH_RESPONSE("\r\n", 2);
							} else {
								PUSH_RESPONSE("EN\r\n", 4);
								if (O_opaque) {
									PUSH_RESPONSE(" O", 2);
									PUSH_RESPONSE(O_opaque_buf, O_opaque_buf_len);
								}
							}
						}
					}
					break;
				case CMD_MD:
					if (word_num < 2) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for md command is less than 2"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for md command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md $(printf "a%.0s" {0..250})\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else if (word_num > 11) { /* 9 options maximum */
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for md command is bigger than 11"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a 0 1 2 3 4 5 6 7 8 9\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl;
						char b_base64 = 0;
						char C_cas = 0; uint64_t C_cas_val = 0;
						char E_new_cas = 0; uint64_t E_new_cas_val = 0;
						char I_invalidate = 0;
						char k_return_key = 0;
						char O_opaque = 0; char O_opaque_buf[31]; uint64_t O_opaque_buf_len = 0;
						char q_noreply = 0;
						char T_update_ttl = 0; uint64_t T_update_ttl_val = 0; uint64_t immediate_expire = 0;
						char x_remove_value_leave_item = 0;
						char flags_order[9];
						{
							uint64_t i, found_error = 0;
							for (i = 2; i < word_num && !found_error; i++) {
								uint64_t l;
								char tmp[32]; /* O(token) 1 + 31 bytes token */
								if (wl[i].len > sizeof(tmp)) {
									EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], 1, tmp, l);
									switch (tmp[0]) {
									case 'O':
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="opaque length for md command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a O$(printf "1%.0s" {0..31})\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR opaque token too long\r\n", 36);
										break;
									default:
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag length for md command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a T$(printf "1%.0s" {0..31})\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										break;
									}
									found_error = 1;
									break;
								} /* other single-byte options ignore content from tmp[1] */
								EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], wl[i].len, tmp, l);
								flags_order[i - 2] = tmp[0];
								switch (tmp[0]) {
								case 'b':
									if (b_base64) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="b flag for md command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a b b\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										b_base64 = 1;
									break;
								case 'I':
									if (I_invalidate) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="I flag for md command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a I I\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										I_invalidate = 1;
									break;
								case 'k':
									if (k_return_key) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="k flag for md command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a k k\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										k_return_key = 1;
									break;
								case 'q':
									if (q_noreply) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="q flag for md command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a q q\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										q_noreply = 1;
									break;
								case 'x':
									if (x_remove_value_leave_item) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="x flag for md command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a x x\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										x_remove_value_leave_item = 1;
									break;
								case 'O':
								case 'C':
								case 'E':
								case 'T':
									if (l == 1 && tmp[0] != 'O') {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="C flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a C\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a E\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a T\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else {
										switch (tmp[0]) {
										case 'O':
											if (O_opaque) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="O flag for md command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a O O\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												O_opaque = 1;
												O_opaque_buf_len = l - 1;
												mp_memcpy(O_opaque_buf, &tmp[1], O_opaque_buf_len);
											}
											break;
										case 'C':
											if (C_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="C flag for md command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a C1 C1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												C_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, C_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for C flag for md command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a C-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													} if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for C flag for md command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a Ca\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'E':
											if (E_new_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for md command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a E1 E1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												E_new_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, E_new_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for md command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a E-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for md command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a Ea\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'T':
											if (T_update_ttl) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for md command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a T1 T1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												T_update_ttl = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, T_update_ttl_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for md command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a T-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														/* XXX: this is accepted */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for md command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a Ta\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										}
									}
									break;
								default:
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined flag for md command"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
									found_error = 1;
									break;
								}
							}
							if (found_error)
								break;
						}
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						if (b_base64) {
							uint8_t tmp[____MP__CONF_MAX_KEY_LEN];
							uint64_t l, e;
							DECODE_BASE64(key, kl, tmp, sizeof(tmp), l, e);
							if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for md command cannot be decoded for base64"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="md a b\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
								break;
							}
							mp_assert(l < ____MP__CONF_MAX_KEY_LEN); /* same buffer space must be enough */
							mp_memcpy(key, tmp, l);
							kl = l;
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							if (C_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CAS;
								MP_KV_CMD_CAS_UNIQUE(cmd) = C_cas_val;
							}
							if (E_new_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_CAS_UNIQUE_IF_NEW;
								MP_KV_CMD_NEW_CAS_UNIQUE(cmd) = E_new_cas_val;
							}
							if (T_update_ttl) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_EXPTIME;
								MP_KV_CMD_EXPTIME(cmd) = T_update_ttl_val;
							}
							if (I_invalidate)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_INVALIDATE | MC_KV_CMD_OPFLAG_COPY_VAL;
							else
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_DELETE;
							if (x_remove_value_leave_item)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE;
							if (immediate_expire)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
								if (!(MP_KV_CMD_OPFLAGS(cmd) & (MC_KV_CMD_OPFLAG_DELETE|MC_KV_CMD_OPFLAG_INVALIDATE)))
									PUSH_RESPONSE("HD", 2);
								else
									PUSH_RESPONSE("EX", 2);
							} else
								PUSH_RESPONSE("NF", 2);
							{
								uint64_t i;
								for (i = 0; i < word_num - 2; i++) {
									switch (flags_order[i]) {
									case 'k':
										PUSH_RESPONSE(" k", 2);
										PUSH_RESPONSE((const char *) key, kl);
										break;
									case 'O':
										PUSH_RESPONSE(" O", 2);
										PUSH_RESPONSE(O_opaque_buf, O_opaque_buf_len);
										break;
									}
								}
							}
							PUSH_RESPONSE("\r\n", 2);
						}
					}
					break;
				case CMD_MA:
					if (word_num < 2) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for ma command is less than 2"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for ma command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma $(printf "a%.0s" {0..250})\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
					} else if (word_num > 16) { /* 14 options maximum */
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for ma command is bigger than 16"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a 0 1 2 3 4 5 6 7 8 9 1 2 3 4 5\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN + 1];
						uint64_t kl;
						char b_base64 = 0;
						char C_cas = 0; uint64_t C_cas_val = 0; uint64_t immediate_expire = 0;
						char E_new_cas = 0; uint64_t E_new_cas_val = 0;
						char N_vivify_on_miss = 0; uint64_t N_vivify_on_miss_val = 0;
						char J_initial_vivify = 0; uint64_t J_initial_vivify_val = 0; /* 0 by default */
						char D_delta = 0; uint64_t D_delta_val = 1; /* increment 1 by default */
						char T_update_ttl = 0; uint64_t T_update_ttl_val = 0;
						char M_mode = 0; char M_mode_val = 'I'; /* indrement by default */
						char O_opaque = 0; char O_opaque_buf[31]; uint64_t O_opaque_buf_len = 0;
						char q_noreply = 0;
						char t_return_item_ttl = 0;
						char c_return_cas = 0;
						char v_return_value = 0;
						char k_return_key = 0;
						char flags_order[14];
						{
							uint64_t i, found_error = 0;
							for (i = 2; i < word_num && !found_error; i++) {
								uint64_t l;
								char tmp[32]; /* O(token) 1 + 31 bytes token */
								if (wl[i].len > sizeof(tmp)) {
									EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], 1, tmp, l);
									switch (tmp[0]) {
									case 'O':
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="opaque length for ma command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a O$(printf "1%.0s" {0..31})\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR opaque token too long\r\n", 36);
										break;
									default:
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag length for ma command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a T$(printf "1%.0s" {0..31})\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										break;
									}
									found_error = 1;
									break;
								} /* other single-byte options ignore content from tmp[1] */
								EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], wl[i].len, tmp, l);
								flags_order[i - 2] = tmp[0];
								switch (tmp[0]) {
								case 'b':
									if (b_base64) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="b flag for ma command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a b b\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										b_base64 = 1;
									break;
								case 'q':
									if (q_noreply) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="q flag for ma command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a q q\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										q_noreply = 1;
									break;
								case 't':
									if (t_return_item_ttl) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="q flag for ma command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a t t\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										t_return_item_ttl = 1;
									break;
								case 'c':
									if (c_return_cas) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="c flag for ma command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a c c\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										c_return_cas = 1;
									break;
								case 'v':
									if (v_return_value) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="v flag for ma command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a v v\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										v_return_value = 1;
									break;
								case 'k':
									if (k_return_key) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="k flag for ma command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a k k\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else
										k_return_key = 1;
									break;
								case 'C':
								case 'E':
								case 'N':
								case 'J':
								case 'D':
								case 'T':
								case 'M':
								case 'O':
									if (l == 1 && tmp[0] != 'O') {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="C flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a C\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a E\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="N flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a N\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="J flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a J\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="D flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a D\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a T\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="M flag for md command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a M\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
										found_error = 1;
									} else {
										switch (tmp[0]) {
										case 'C':
											if (C_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="C flag for ma command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a C1 C1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												C_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, C_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for C flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a C-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for C flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a Ca\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'E':
											if (E_new_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for ma command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a E1 E1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												E_new_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, E_new_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a E-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK(); /* value for E flag for ma command is not a number */
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a Ea\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'N':
											if (N_vivify_on_miss) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="N flag for ma command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a N1 N1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												N_vivify_on_miss = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, N_vivify_on_miss_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for N flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a N-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for N flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a Na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'J':
											if (D_delta || J_initial_vivify) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="D,J flags for ma command are duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a J1 J1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												J_initial_vivify = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, J_initial_vivify_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for J flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a J-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for J flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a Ja\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'D':
											if (D_delta || J_initial_vivify) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="D,J flags for ma command are duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a D1 D1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												D_delta = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, D_delta_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for D flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a D-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for D flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a Da\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'T':
											if (T_update_ttl) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for ma command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a T1 T1\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												T_update_ttl = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, T_update_ttl_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for ma command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\n1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS+="ma a T-1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="set a 0 0 1\r\n1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS+="ma a Ta\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
														found_error = 1;
													}
												}
											}
											break;
										case 'M':
											if (M_mode) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="M flag for ma command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a MI MI\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												M_mode = 1;
												if (l != 2) {
													MP_TEST_HOOK();
													/*
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="length of M flag for ma command is not 2 bytes"
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a MII\r\n"
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
													 */
													PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
													found_error = 1;
												} else {
													switch (tmp[1]) {
													case 'I':
													case '+':
													case 'D':
													case '-':
														M_mode_val = tmp[1];
														break;
													default:
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined character for M flag for ma command"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a Ma\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid mode for ma M token\r\n", 42);
														found_error = 1;
														break;
													}
												}
											}
											break;
										case 'O':
											if (O_opaque) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="O flag for ma command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a O O\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
												found_error = 1;
											} else {
												O_opaque = 1;
												O_opaque_buf_len = l - 1;
												mp_memcpy(O_opaque_buf, &tmp[1], O_opaque_buf_len);
											}
											break;
										}
									}
									break;
								default:
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined flag for ma command"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
									found_error = 1;
									break;
								}
							}
							if (found_error)
								break;
						}
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						if (b_base64) {
							uint8_t tmp[____MP__CONF_MAX_KEY_LEN];
							uint64_t l, e;
							DECODE_BASE64(key, kl, tmp, sizeof(tmp), l, e);
							if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for ma command cannot be decoded for base64"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ma a b\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid or duplicate flag\r\n", 40);
								break;
							}
							mp_assert(l < ____MP__CONF_MAX_KEY_LEN); /* same buffer space must be enough */
							mp_memcpy(key, tmp, l);
							kl = l;
						}
						{
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_OPFLAGS(cmd) = MC_KV_CMD_OPFLAG_UPDATE;
							if (D_delta)
								MP_KV_CMD_INCDEC_VAL(cmd) = D_delta_val;
							if (J_initial_vivify)
								MP_KV_CMD_NEW_INCDEC_VAL(cmd) = J_initial_vivify_val;
							else
								MP_KV_CMD_NEW_INCDEC_VAL(cmd) = 0; /* default value */
							if (C_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CAS;
								MP_KV_CMD_CAS_UNIQUE(cmd) = C_cas_val;
							}
							if (E_new_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_CAS_UNIQUE_IF_NEW;
								MP_KV_CMD_NEW_CAS_UNIQUE(cmd) = E_new_cas_val;
							}
							if (N_vivify_on_miss) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS;
								MP_KV_CMD_EXPTIME(cmd) = N_vivify_on_miss_val;
							}
							if (T_update_ttl) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_EXPTIME;
								MP_KV_CMD_EXPTIME(cmd) = T_update_ttl_val;
							}
							if (M_mode_val == 'I' || M_mode_val == '+')
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_NUMERICAL | MC_KV_CMD_OPFLAG_NUMERICAL_INCREMENT;
							else
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_NUMERICAL;
							if (immediate_expire)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE) || (N_vivify_on_miss && !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS))) {
								if (v_return_value) {
									PUSH_RESPONSE("VA ", 3);
									{
										char tmp[21];
										uint64_t r, e;
										UINT2STR(tmp, sizeof(tmp), r, MP_KV_CMD_VAL_LEN(cmd), e);
										mp_assert(!e);
										PUSH_RESPONSE(tmp, r);
									}
								} else
									PUSH_RESPONSE("HD", 2);
								{
									uint64_t i;
									for (i = 0; i < word_num - 2; i++) {
										switch (flags_order[i]) {
										case 'O':
											PUSH_RESPONSE(" O", 2);
											PUSH_RESPONSE(O_opaque_buf, O_opaque_buf_len);
											break;
										case 't':
											if (!MP_KV_CMD_EXPTIME(cmd))
												PUSH_RESPONSE(" t-1\0", 4);
											else {
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_EXPTIME(cmd), e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 't';
												PUSH_RESPONSE(tmp, r);
											}
											break;
										case 'c':
											{
												char tmp[23];
												uint64_t r, e;
												UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_CAS_UNIQUE(cmd), e);
												mp_assert(!e);
												tmp[0] = ' ';
												tmp[1] = 'c';
												PUSH_RESPONSE(tmp, r);
											}
											break;
										case 'k':
											PUSH_RESPONSE(" k", 2);
											PUSH_RESPONSE((const char *) key, kl);
											break;
										}
									}
								}
								if (v_return_value) {
									PUSH_RESPONSE("\r\n", 2);
									PUSH_RESPONSE((const char *) MP_KV_CMD_VAL(cmd), MP_KV_CMD_VAL_LEN(cmd));
								}
								PUSH_RESPONSE("\r\n", 2);
							} else { /* not found */
								PUSH_RESPONSE("NF\r\n", 4);
								if (O_opaque) {
									PUSH_RESPONSE(" O", 2);
									PUSH_RESPONSE(O_opaque_buf, O_opaque_buf_len);
								}
							}
						}
					}
					break;
				case CMD_MS:
					if (wl[1].len > ____MP__CONF_MAX_KEY_LEN) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for ms command is bigger than 250 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms $(printf "a%.0s" {0..250}) 1\r\na\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
						ring_cur_idx = wl[word_num - 1].ptr[0];
						ring_cur_off = wl[word_num - 1].ptr[1];
					} else {
						uint8_t key[____MP__CONF_MAX_KEY_LEN];
						uint64_t kl, bytes;
						char b_base64 = 0;
						char c_return_cas = 0;
						char C_cas = 0; uint64_t C_cas_val = 0;
						char E_new_cas = 0; uint64_t E_new_cas_val = 0;
						char F_flags = 0; uint64_t F_flags_val = 0; /* 32 bit */
						char I_invalidate = 0;
						char k_return_key = 0;
						char O_opaque = 0; char O_opaque_buf[31]; uint64_t O_opaque_buf_len = 0;
						char q_noreply = 0;
						char s_return_item_size = 0;
						char T_update_ttl = 0; uint64_t T_update_ttl_val = 0; uint64_t immediate_expire = 0;
						char M_mode = 0; char M_mode_val = 'S';
						char N_vivify_on_miss = 0; uint64_t N_vivify_on_miss_val = 0;
						char flags_order[13];
						{
							uint64_t i, found_error = 0;
							for (i = 3; i < word_num - 1 && !found_error; i++) {
								uint64_t l;
								char tmp[32]; /* O(token) 1 + 31 bytes token */
								if (wl[i].len > sizeof(tmp)) {
									EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], 1, tmp, l);
									switch (tmp[0]) {
									case 'O':
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="opaque length for ms command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 O$(printf "1%.0s" {0..31})\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR opaque token too long\r\n", 36);
										break;
									default:
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flag length for ms command is larger than 31 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 a$(printf "1%.0s" {0..31})\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR invalid flag\r\n", 27);
										break;
									}
									found_error = 1;
									break;
								} /* other single-byte options ignore content from tmp[1] */
								EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], wl[i].len, tmp, l);
								flags_order[i - 3] = tmp[0];
								switch (tmp[0]) {
								case 'b':
									if (b_base64) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="b flag for ms command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 b b\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										b_base64 = 1;
									break;
								case 'c':
									if (c_return_cas) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="c flag for ms command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 c c\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										c_return_cas = 1;
									break;
								case 'I':
									if (I_invalidate) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="I flag for ms command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 I I\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										I_invalidate = 1;
									break;
								case 'k':
									if (k_return_key) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="k flag for ms command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 k k\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										k_return_key = 1;
									break;
								case 'q':
									if (q_noreply) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="q flag for ms command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 q q\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										q_noreply = 1;
									break;
								case 's':
									if (s_return_item_size) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="s flag for ms command is duplicated"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 s s\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
										found_error = 1;
									} else
										s_return_item_size = 1;
									break;
								case 'C':
								case 'E':
								case 'F':
								case 'O':
								case 'T':
								case 'M':
								case 'N':
									if (l == 1 && tmp[0] != 'O') {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="C flag for ms command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 C\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for ms command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 E\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for ms command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 T\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="N flag for ms command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 N\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="F flag for ms command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 F\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="M flag for ms command does not have subsequent value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 M\r\na\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										switch (tmp[0]) {
										case 'C':
										case 'E':
										case 'T':
										case 'N':
											PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
											break;
										case 'F':
											PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
											break;
										case 'M':
											PUSH_RESPONSE("CLIENT_ERROR incorrect length for M token\r\n", 43);
											break;
										}
										found_error = 1;
									} else {
										switch (tmp[0]) {
										case 'C':
											if (C_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="C flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 C1 C1\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												C_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, C_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for C flag for ms command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 C-1\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for C flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 Ca\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										case 'E':
											if (E_new_cas) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="E flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 E1 E1\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												E_new_cas = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, E_new_cas_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for ms command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 E-1\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for E flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 Ea\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										case 'F':
											if (F_flags) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="F flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 F1 F1\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												F_flags = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, F_flags, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for F flag for ms command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 F-1\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
														found_error = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for F flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 Fa\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
														found_error = 1;
													}
												}
											}
											break;
										case 'O':
											if (O_opaque) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="O flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 O O\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												O_opaque = 1;
												O_opaque_buf_len = l - 1;
												mp_memcpy(O_opaque_buf, &tmp[1], O_opaque_buf_len);
											}
											break;
										case 'T':
											if (T_update_ttl) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="T flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 T1 T1\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												T_update_ttl = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, T_update_ttl_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for ms command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 T-1\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for T flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 Ta\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										case 'M':
											if (M_mode) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="M flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 ME ME\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												M_mode = 1;
												if (l != 2) {
													MP_TEST_HOOK();
													/*
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="length of M flag for ms command is not 2 bytes"
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 MEE\r\na\r\n"
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
													 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
													 */
													PUSH_RESPONSE("CLIENT_ERROR incorrect length for M token\r\n", 43);
													found_error = 1;
												} else {
													switch (tmp[1]) {
													case 'E':
													case 'A':
													case 'P':
													case 'R':
													case 'S':
														M_mode_val = tmp[1];
														break;
													default:
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined character for M flag for ms command"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 MX\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR invalid mode for ms M token\r\n", 42);
														found_error = 1;
														break;
													}
												}
											}
											break;
										case 'N':
											if (N_vivify_on_miss) {
												MP_TEST_HOOK();
												/*
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="N flag for ms command is duplicated"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 N1 N1\r\na\r\n"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
												 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
												 */
												PUSH_RESPONSE("CLIENT_ERROR duplicate flag\r\n", 29);
												found_error = 1;
											} else {
												N_vivify_on_miss = 1;
												{
													uint64_t minus, e;
													STR2INT(&tmp[1], l - 1, N_vivify_on_miss_val, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for N flag for ms command is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 N-1\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														immediate_expire = 1;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value for N flag for ma command is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 Na\r\na\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("CLIENT_ERROR bad token in command line format\r\n", 47);
														found_error = 1;
													}
												}
											}
											break;
										}
									}
									break;
								default:
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined flag for ms command"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 a\r\na\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR invalid flag\r\n", 27);
									found_error = 1;
									break;
								}
							}
							if (found_error)
								break;
						}
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, key, kl);
						if (b_base64) {
							uint8_t tmp[____MP__CONF_MAX_KEY_LEN];
							uint64_t l, e;
							DECODE_BASE64(key, kl, tmp, sizeof(tmp), l, e);
							if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="key for ms command cannot be decoded for base64"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a 1 b\r\na\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR error decoding key\r\n", 33);
								break;
							}
							mp_assert(l < ____MP__CONF_MAX_KEY_LEN); /* same buffer space must be enough */
							mp_memcpy(key, tmp, l);
							kl = l;
						}
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, bytes, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value length for ms command is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a -1\r\na\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="value length for ms command is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="ms a a\r\na\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
								ring_cur_idx = wl[word_num - 1].ptr[0];
								ring_cur_off = wl[word_num - 1].ptr[1];
								break;
							}
						}
						{
							uint8_t do_vivify_on_miss = 0;
							uint8_t cmd[MP_KV_CMD_SIZE];
							mp_memset(cmd, 0, sizeof(cmd));
							MP_KV_CMD_VAL_PTR_0(cmd) = wl[word_num - 1].ptr[0];
							MP_KV_CMD_VAL_PTR_1(cmd) = wl[word_num - 1].ptr[1];
							MP_KV_CMD_VAL_LEN(cmd) = bytes;
							if (M_mode_val == 'E') /* ADD */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS;
							else if (M_mode_val == 'S') { /* SET */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS | MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
								do_vivify_on_miss = 1;
							} else /* REPLACE APPEND PREPEND */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_UPDATE | MC_KV_CMD_OPFLAG_BUMP_CAS_UNIQUE;
							if (M_mode_val == 'A') /* APPEND */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CONCAT | MC_KV_CMD_OPFLAG_CONCAT_APPEND;
							else if (M_mode_val == 'P') /* PREPEND */
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CONCAT;
							if (F_flags) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_FLAG;
								MP_KV_CMD_FLAGS(cmd) = F_flags_val;
							}
							if (C_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_CAS;
								MP_KV_CMD_CAS_UNIQUE(cmd) = C_cas_val;
							}
							if (E_new_cas) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_CAS_UNIQUE_IF_NEW;
								MP_KV_CMD_NEW_CAS_UNIQUE(cmd) = E_new_cas_val;
							}
							if (N_vivify_on_miss) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS;
								MP_KV_CMD_EXPTIME(cmd) = N_vivify_on_miss_val;
								do_vivify_on_miss = 1;
							}
							if (T_update_ttl) {
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_SET_EXPTIME;
								MP_KV_CMD_EXPTIME(cmd) = T_update_ttl_val;
							}
							if (I_invalidate)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_INVALIDATE_WITH_CAS;
							if (immediate_expire)
								MP_KV_CMD_OPFLAGS(cmd) |= MC_KV_CMD_OPFLAG_IMMEDIATE_EXPIRE;
							MP_OPS_KV_CMD(mpr, key, kl, cmd, opaque);
							if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_ERROR) {
								if (MP_KV_CMD_ERRNO(cmd) == MP_ERR_ENOMEM) {
									MP_TEST_HOOK(); /* out of memory for the value for ms command */
									PUSH_RESPONSE("SERVER_ERROR out of memory storing object\r\n", 43);
								} else
									PUSH_RESPONSE("ERROR\r\n", 7);
							} else if (!(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_UPDATE) || (do_vivify_on_miss && !(MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_VIVIFY_ON_MISS)))
								PUSH_RESPONSE("HD", 2);
							else {
								if (MP_KV_CMD_OPFLAGS(cmd) & MC_KV_CMD_OPFLAG_FOUND) {
									if (C_cas)
										PUSH_RESPONSE("EX", 2);
									else
										PUSH_RESPONSE("NS", 2);
								} else {
									if (C_cas)
										PUSH_RESPONSE("NF", 4);
									else
										PUSH_RESPONSE("NS", 4);
								}
							}
							{
								uint64_t i;
								for (i = 0; i < word_num - 4; i++) {
									switch (flags_order[i]) {
									case 'O':
										PUSH_RESPONSE(" O", 2);
										PUSH_RESPONSE(O_opaque_buf, O_opaque_buf_len);
										break;
									case 'c':
										{
											char tmp[23];
											uint64_t r, e;
											UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_CAS_UNIQUE(cmd), e);
											mp_assert(!e);
											tmp[0] = ' ';
											tmp[1] = 'c';
											PUSH_RESPONSE(tmp, r);
										}
										break;
									case 't':
										if (!MP_KV_CMD_EXPTIME(cmd))
											PUSH_RESPONSE(" t-1\0", 4);
										else {
											char tmp[23];
											uint64_t r, e;
											UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_EXPTIME(cmd), e);
											mp_assert(!e);
											tmp[0] = ' ';
											tmp[1] = 't';
											PUSH_RESPONSE(tmp, r);
										}
										break;
									case 'k':
										PUSH_RESPONSE(" k", 2);
										PUSH_RESPONSE(key, kl);
										break;
									case 's':
										{
											char tmp[23];
											uint64_t r, e;
											UINT2STR(&tmp[2], sizeof(tmp) - 2, r, MP_KV_CMD_VAL_LEN(cmd), e);
											mp_assert(!e);
											tmp[0] = ' ';
											tmp[1] = 's';
											PUSH_RESPONSE(tmp, r);
										}
										break;
									}
								}
								PUSH_RESPONSE("\r\n", 2);
							}
						}
					}
					break;
				case CMD_MN:
					PUSH_RESPONSE("MN\r\n", 2);
					break;
				case CMD_SLABS:
					if (word_num < 3) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for slab command is less than 3"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (word_num > 5) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for slab command is more than 5"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign 1 2 3 4\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len != 8) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word for slab command is not 8"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassig 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						uint64_t sub_cmd_type = (uint64_t) -1;
						{
							uint64_t msglen = 0;
							char msg[8]; /* reassign and automove are 8 bytes */
							EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, msg, msglen);
							switch (msg[0]) {
							case 'r':
								if (!mp_memcmp(msg, "reassign", msglen))
									sub_cmd_type = CMD_SLABS__REASSIGN;
								break;
							case 'a':
								if (!mp_memcmp(msg, "automove", msglen))
									sub_cmd_type = CMD_SLABS__AUTOMOVE;
								break;
							}
						}
						switch (sub_cmd_type) {
						case CMD_SLABS__REASSIGN:
							if (word_num != 4) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for slab reassign is not 4"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign 1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
							} else {
								uint64_t source_class, dest_class;
								{
									uint64_t minus, e = 0;
									EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, source_class, minus, e);
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="source class for slab reassign command is not a number"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign a 1\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
										break;
									} else if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="source class for slab reassign command is less than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign -1 1\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("BADCLASS invalid src or dst class id\r\n", 38);
										break;
									}
								}
								{
									uint64_t minus, e = 0;
									EXTRACT_VAL(mpr, wl[3].ptr[0], wl[3].ptr[1], wl[3].len, dest_class, minus, e);
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="destination class for slab reassign command is not a number"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign 1 a\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
										break;
									} else if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="destination class for slab reassign command is less than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign 1 -1\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("BADCLASS invalid src or dst class id\r\n", 38);
										break;
									}
								}
								if (source_class == dest_class) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="source and destination classes for slab reassign command are the same"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassign 1 1\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("SAME src and dst class are identical\r\n", 38);
									break;
								}
								{
									/*
									 * XXX: not implemented
									 *
									 * potential replies
									 *
									 * OK, BUSY, BADCLASS ,NOSPARE, NOTFULL, UNSAFE
									 */
									PUSH_RESPONSE("OK\r\n", 4);
								}
							}
							break;
						case CMD_SLABS__AUTOMOVE:
							{
								uint64_t indicator;
								{
									uint64_t minus, e = 0;
									EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, indicator, minus, e);
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="indicator for slab automove command is not a number"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs automove a\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
										break;
									} else if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="indicator for slab automove command is less than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs automove -1\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
										break;
									} else if (indicator > 2) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="indicator for slab automove command is bigger than 2"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs automove 3\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									}
								}
								switch (indicator) {
								/*
								 * XXX: not implemented
								 */
								case 0:
									break;
								case 1:
									break;
								case 2:
									break;
								}
								PUSH_RESPONSE("OK\r\n", 4);
							}
							break;
						default:
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word for slab command is 8, but undefined"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="slabs reassigx 1\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("ERROR\r\n", 7);
							break;
						}
					}
					break;
				case CMD_LRU:
					if (wl[1].len > 8) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word for lru command is bigger than 8"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru temp_ttla 10\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						uint64_t msglen = 0;
						char msg[8]; /* temp_ttl is 8 bytes */
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, msg, msglen);
						switch (msglen) {
						case 4:
							switch (msg[0]) {
							case 't':
								if (!mp_memcmp(msg, "tune", msglen))
									cmd_type = CMD_LRU__TUNE;
								break;
							case 'm':
								if (!mp_memcmp(msg, "mode", msglen))
									cmd_type = CMD_LRU__MODE;
								break;
							}
							break;
						case 8:
							if (!mp_memcmp(msg, "temp_ttl", msglen))
								cmd_type = CMD_LRU__TEMP_TTL;
							break;
						}
						switch (cmd_type) {
						case CMD_LRU__TUNE:
							if (word_num < 6) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru tune is less than 6"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 20 0.1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
							} else {
								uint64_t percent_hot, percent_warm, max_hot_factor, max_warm_age_factor;
								{
									uint64_t minus, e = 0;
									EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, percent_hot, minus, e);
									if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="percent hot for lru tune is smaller than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune -1 20 0.1 4.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									} else if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="percent hot for lru tune is not a number"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune a 20 0.1 4.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									}
								}
								{
									uint64_t minus, e = 0;
									EXTRACT_VAL(mpr, wl[3].ptr[0], wl[3].ptr[1], wl[3].len, percent_warm, minus, e);
									if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="percent warm for lru tune is smaller than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 -1 0.1 4.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									} else if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="percent warm for lru tune is not a number"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 a 0.1 4.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									}
								}
								{
									uint64_t minus, e = 0, integer, dicimal, factor;
									EXTRACT_VAL_FLOAT(mpr, wl[4].ptr[0], wl[4].ptr[1], wl[4].len, integer, dicimal, factor, minus, e);
									max_hot_factor = integer * 100;
									{
										uint64_t i;
										for (i = factor; i > 2; i--)
											dicimal /= 10;
									}
									max_hot_factor += dicimal;
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="max hot factor for lru tune is invalid float value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 20 0.1.1 4.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									} else if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="max hot factor for lru tune is less than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 20 -1 4.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR hot/warm age factors must be greater than 0\r\n", 51);
										break;
									}
								}
								{
									uint64_t minus, e = 0, integer, dicimal, factor;
									EXTRACT_VAL_FLOAT(mpr, wl[5].ptr[0], wl[5].ptr[1], wl[5].len, integer, dicimal, factor, minus, e);
									max_warm_age_factor = integer * 100;
									{
										uint64_t i;
										for (i = factor; i > 2; i--)
											dicimal /= 10;
									}
									max_warm_age_factor += dicimal;
									if (e) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="max warm factor for lru tune is invalid float value"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 20 0.1 4.0.0\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									} else if (minus) {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="max warm factor for lru tune is less than 0"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru tune 10 20 0.1 -1\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR hot/warm age factors must be greater than 0\r\n", 51);
										break;
									}
								}
								{
									/* XXX: not implemented */
								}
								PUSH_RESPONSE("OK\r\n", 4);
							}
							break;
						case CMD_LRU__MODE:
							if (word_num < 3) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru mode is less than 3"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru mode\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
							} else if (wl[2].len > 9) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for lru mode is bigger than 9 bytes"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru mode segmenteda\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
							} else {
								uint64_t msglen = 0;
								char msg[9]; /* segmented is 9 bytes */
								EXTRACT_DATA(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, msg, msglen);
								if (msglen == 4 && !mp_memcmp(msg, "flat", msglen)) {
									/* XXX: not implemented */
								} else if (msglen == 9 && !mp_memcmp(msg, "segmented", msglen)) {
									/* XXX: not implemented */
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru mode"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru mode unknown\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("ERROR\r\n", 7);
									break;
								}
							}
							break;
						case CMD_LRU__TEMP_TTL:
							if (word_num < 3) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru temp ttl is less than 3"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru temp_ttl\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
							} else {
								uint64_t ttl;
								{
									uint64_t minus, e = 0;
									EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, ttl, minus, e);
									if (e) {
										MP_TEST_HOOK(); /* ttl for lru temp ttl is not a number */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru temp ttl is less than 3"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru temp_ttl a\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("ERROR\r\n", 7);
										break;
									}
									if (minus) {
										/*
										 * XXX: not implemented
										 * disable temp lru
										 */
									} else {
										/* XXX: not implemented */
									}
								}
							}
							break;
						default:
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word for lru command is undefined"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru temp_ttx 1\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("ERROR\r\n", 7);
							break;
						}
					}
					break;
				case CMD_LRU_CRAWLER:
					if (word_num < 2) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawler is less than 2"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else if (wl[1].len > 8) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word for for lru crawler is bigger than 8 bytes"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadumpa\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						uint64_t msglen = 0;
						char msg[8]; /* metadump are 8 bytes */
						EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, msg, msglen);
						switch (msglen) {
						case 6:
							switch (msg[0]) {
							case 'e':
								if (!mp_memcmp(msg, "enable", msglen))
									cmd_type = CMD_LRU_CRAWLER__ENABLE;
								break;
							case 'm':
								if (!mp_memcmp(msg, "mgdump", msglen))
									cmd_type = CMD_LRU_CRAWLER__MGDUMP;
								break;
							}
							break;
						case 5:
							switch (msg[0]) {
							case 's':
								if (!mp_memcmp(msg, "sleep", msglen))
									cmd_type = CMD_LRU_CRAWLER__SLEEP;
								break;
							case 'c':
								if (!mp_memcmp(msg, "crawl", msglen))
									cmd_type = CMD_LRU_CRAWLER__CRAWL;
								break;
							}
							break;
						case 7:
							switch (msg[0]) {
							case 'd':
								if (!mp_memcmp(msg, "disable", msglen))
									cmd_type = CMD_LRU_CRAWLER__DISABLE;
								break;
							case 't':
								if (!mp_memcmp(msg, "tocrawl", msglen))
									cmd_type = CMD_LRU_CRAWLER__TOCRAWL;
								break;
							}
							break;
						case 8:
							if (!mp_memcmp(msg, "metadump", msglen))
								cmd_type = CMD_LRU_CRAWLER__METADUMP;
							break;
						}
					}
					switch (cmd_type) {
					case CMD_LRU_CRAWLER__ENABLE:
					case CMD_LRU_CRAWLER__DISABLE:
						if (word_num != 2) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawler enable,disable is not 2"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler disable 1\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("ERROR\r\n", 7);
						} else {
							/* XXX: not implemented */
						}
						break;
					case CMD_LRU_CRAWLER__SLEEP:
						if (word_num < 3) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawler sleep is less than 3"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler sleep\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("ERROR\r\n", 7);
						} else {
							uint64_t microseconds;
							{
								uint64_t minus, e;
								EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, microseconds, minus, e);
								if (minus) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="microseconds for lru crawler sleep is smaller than 0"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler sleep -1\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
									break;
								} else if (e) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="microseconds for lru crawler sleep is not a number"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler sleep a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
									break;
								}
							}
							{
								/* XXX: not implemented */
							}
						}
						break;
					case CMD_LRU_CRAWLER__TOCRAWL:
						if (word_num < 3) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawle tocrawl is less than 3"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler tocrawl\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("ERROR\r\n", 7);
						} else {
							uint64_t val_32u;
							{
								uint64_t minus, e;
								EXTRACT_VAL(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, val_32u, minus, e);
								if (minus) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="val for lru crawle tocrawl is smaller than 0"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler tocrawl -1\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
									break;
								} else if (e) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="val for lru crawle tocrawl is not a number"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler tocrawl a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR bad command line format\r\n", 38);
									break;
								}
							}
							{
								/* XXX: not implemented */
							}
						}
						break;
					case CMD_LRU_CRAWLER__CRAWL:
					case CMD_LRU_CRAWLER__METADUMP:
					case CMD_LRU_CRAWLER__MGDUMP:
						if (word_num != 3) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawler crawl is not 3"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawler metadump is not 3"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadump\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for lru crawler mgdump is not 3"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("ERROR\r\n", 7);
						} else if (wl[2].len > 62) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word lru crawler crawl is bigger than 62"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl $(printf "a%.0s" {0..62})\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word lru crawler metadump is bigger than 62"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadump $(printf "1%.0s" {0..62})\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word lru crawler mgdump is bigger than 62"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump $(printf "1%.0s" {0..62})\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
						} else {
							uint64_t msglen;
							char msg[62]; /* int(20 bytes),int(20 bytes),int(20 bytes) max 62 bytes */
							EXTRACT_DATA(mpr, wl[2].ptr[0], wl[2].ptr[1], wl[2].len, msg,  msglen);
							mp_assert(msglen > 0);
							switch (msg[0]) {
							case 'a':
								if (msglen == 3) {
									if (!mp_memcmp(msg, "all", msglen)) {
										switch (cmd_type) {
										case CMD_LRU_CRAWLER__CRAWL:
											{
												/* XXX: not implemented */
											}
											break;
										case CMD_LRU_CRAWLER__METADUMP:
											{
												/* XXX: not implemented */
											}
											break;
										case CMD_LRU_CRAWLER__MGDUMP:
											{
												/* XXX: not implemented */
											}
										default:
											mp_assert(0);
											break;
										}
									} else {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler crawl begins with a but its message length is not 3 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl abc\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler metadump begins with a but its message length is not 3 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadump abc\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler mgdump begins with a but its message length is not 3 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump abc\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
									}
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler crawl begins with a"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl alll\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler metadump begins with a"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadump alll\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler mgdump begins with a"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump alll\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
								}
								break;
							case 'h':
								if (msglen == 4) {
									if (!mp_memcmp(msg, "hash", msglen)) {
										switch (cmd_type) {
										case CMD_LRU_CRAWLER__METADUMP:
											{
												/* XXX: not implemented */
											}
											break;
										case CMD_LRU_CRAWLER__MGDUMP:
											{
												/* XXX: not implemented */
											}
											break;
										case CMD_LRU_CRAWLER__CRAWL:
											MP_TEST_HOOK();
											/*
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="hash command for lru_crawler crawl"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl hash\r\n"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
											 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
											 */
											PUSH_RESPONSE("ERROR an unknown error happened\r\n", 33);
											break;
										default:
											mp_assert(0);
											break;
										}
									} else {
										MP_TEST_HOOK();
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler crawl begins with h but its message length is not 4 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl hasx\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler metadump begins with h but its message length is not 4 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadump hasx\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										/*
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler mgdump begins with h but its message length is not 4 bytes"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump hasx\r\n"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
										 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
										 */
										PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
									}
								} else {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler crawl begins with h"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler crawl hashx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler metadump begins with h"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler metadump hashx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined command for lru_crawler mgdump begins with h"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump hashx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
								}
								break;
							case '-':
							case '0':
							case '1':
							case '2':
							case '3':
							case '4':
							case '5':
							case '6':
							case '7':
							case '8':
							case '9':
								{ /* validation */
									uint64_t i, c, found_error = 0;
									for (i = 0, c = 0; i < msglen + 1 && !found_error; i++) {
										if (msg[i] == ',' || i == msglen) {
											if (i - c > 1) {
												uint64_t tmp;
												{
													uint64_t minus, e;
													STR2INT(&msg[c], i - c, tmp, minus, e);
													if (minus) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="class id for lru crawler mgdump is smaller than 0"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump -1\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
														found_error = 1;
														break;
													} else if (e) {
														MP_TEST_HOOK();
														/*
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="class id for lru crawler mgdump is not a number"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump 1b\r\n"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
														 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
														 */
														PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
														found_error = 1;
														break;
													}
												}
											}
											c = i;
										}
									}
									if (found_error)
										break;
								}
								{ /* handle classids */
									uint64_t i, c;
									for (i = 0, c = 0; i < msglen + 1; i++) {
										if (msg[i] == ',' || i == msglen) {
											if (i - c > 1) {
												uint64_t classid;
												{
													uint64_t minus, err;
													STR2INT(&msg[c], i - c, classid, minus, err);
													mp_assert(!minus & !err);
												}
												switch (cmd_type) {
												case CMD_LRU_CRAWLER__CRAWL:
													{
														/* XXX: not implemented */
													}
													break;
												case CMD_LRU_CRAWLER__METADUMP:
													{
														/* XXX: not implemented */
													}
													break;
												case CMD_LRU_CRAWLER__MGDUMP:
													{
														/* XXX: not implemented */
													}
													break;
												}
											}
											c = i;
										}
									}
								}
								PUSH_RESPONSE("OK\r\n", 4);
								break;
							default:
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="class id for lru crawler is invalid character"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="lru_crawler mgdump b\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("BADCLASS invalid class id\r\n", 27);
								break;
							}
						}
					}
					break;
				case CMD_WATCH:
					{
						uint64_t i, found_error = 0;
						for (i = 1; i < word_num && !found_error; i++) {
							uint64_t msglen = 0;
							char msg[11]; /* proxyevents are 11 bytes */
							if (wl[i].len > 11) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of a word for watch command is bigger than 11 bytes"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="watch proxyeventsa\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
								found_error = 1;
								break;
							}
							EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], wl[i].len, msg, msglen);
							{
								uint64_t handled = 1;
								switch (msglen) {
								case 8:
									if (!mp_memcmp(msg, "fetchers", msglen)) {
										/* XXX: not implemented */
									} else
										handled = 0;
									break;
								case 9:
									switch (msg[0]) {
									case 'm':
										if (!mp_memcmp(msg, "mutations", msglen)) {
											/* XXX: not implemented */
										} else
											handled = 0;
										break;
									case 'e':
										if (!mp_memcmp(msg, "evictions", msglen)) {
											/* XXX: not implemented */
										} else
											handled = 0;
										break;
									case 'p':
										if (!mp_memcmp(msg, "proxyreqs", msglen)) {
											/* XXX: not implemented */
										} else
											handled = 0;
										break;
									case 'd':
										if (!mp_memcmp(msg, "deletions", msglen)) {
											/* XXX: not implemented */
										} else
											handled = 0;
										break;
									}
									break;
								case 10:
									if (!mp_memcmp(msg, "connevents", msglen)) {
										/* XXX: not implemented */
									} else
										handled = 0;
									break;
								case 5:
									if (!mp_memcmp(msg, "items", msglen)) {
										/* XXX: not implemented */
									} else
										handled = 0;
									break;
								default:
									handled = 0;
									break;
								}
								if (!handled) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined sub command for watch command, length can be 8,9,10,11"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="watch 12345678\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("ERROR\r\n", 7);
								}
							}
						}

					}
					break;
				case CMD_STATS:
					{
						uint64_t i, found_error = 0;
						for (i = 1; i < word_num && !found_error; i++) {
							uint64_t msglen;
							char msg[30]; /* time_since_server_cert_refresh are 30 bytes */
							if (wl[i].len > 30) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of a word for stats command is bigger than 30 bytes"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="stats time_since_server_cert_refresha\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
								found_error = 1;
								break;
							}
							EXTRACT_DATA(mpr, wl[i].ptr[0], wl[i].ptr[1], wl[i].len, msg, msglen);
							{
								uint64_t sub_cmd_type = (uint64_t) -1;
								switch (msglen) {
								case 5:
									switch (msg[1]) {
									case 't':
										if (!mp_memcmp(msg, "items", msglen))
											sub_cmd_type = CMD_STATS__ITEMS;
										break;
									case 'i':
										if (!mp_memcmp(msg, "sizes", msglen))
											sub_cmd_type = CMD_STATS__SIZES;
										break;
									case 'l':
										if (!mp_memcmp(msg, "slabs", msglen))
											sub_cmd_type = CMD_STATS__SLABS;
										break;
									case 'o':
										if (!mp_memcmp(msg, "conns", msglen))
											sub_cmd_type = CMD_STATS__CONNS;
										break;
									}
									break;
								case 8:
									if (!mp_memcmp(msg, "settings", msglen))
										sub_cmd_type = CMD_STATS__SETTINGS;
									break;
								case 15:
									if (!mp_memcmp(msg, "ssl_min_version", msglen))
										sub_cmd_type = CMD_STATS__SSL_MIN_VERSION;
									break;
								case 16:
									if (!mp_memcmp(msg, "ssl_new_sessions", msglen))
										sub_cmd_type = CMD_STATS__SSL_NEW_SESSIONS;
									break;
								case 20:
									if (!mp_memcmp(msg, "ssl_handshake_errors", msglen))
										sub_cmd_type = CMD_STATS__SSL_HANDSHAKE_ERRORS;
									break;
								case 30:
									if (!mp_memcmp(msg, "TIME_SINCE_SERVER_CERT_REFRESH", msglen))
										sub_cmd_type = CMD_STATS__TIME_SINCE_SERVER_CERT_REFRESH;
									break;
								default:
									sub_cmd_type = -1;
									break;
								}
								switch (sub_cmd_type) { /* XXX: not implemented */
								case CMD_STATS__SETTINGS:
									break;
								case CMD_STATS__ITEMS:
									break;
								case CMD_STATS__SIZES:
									break;
								case CMD_STATS__SLABS:
									break;
								case CMD_STATS__CONNS:
									break;
								case CMD_STATS__SSL_HANDSHAKE_ERRORS:
									break;
								case CMD_STATS__SSL_MIN_VERSION:
									break;
								case CMD_STATS__SSL_NEW_SESSIONS:
									break;
								case CMD_STATS__TIME_SINCE_SERVER_CERT_REFRESH:
									break;
								default:
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="undefined sub command for stats command"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="stats a\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("ERROR\r\n", 7);
									found_error = 1;
									break;
								}
							}
						}
					}
					break;
				case CMD_FLUSH_ALL:
					if (word_num != 2 && word_num != 3) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for flush all command is neither 2 nor 3"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all 0 noreply 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						uint64_t expr_sec, noreply = 0;
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, expr_sec, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="expiration time specified for flush_all command is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all -1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* this is not considered error */
								expr_sec = 0;
							} else if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="expiration time specified for flush_all command is not a number"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid exptime argument\r\n", 39);
								break;
							}
						}
						if (word_num == 3) {
							if (wl[2].len != 7) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for flush all command is not 7, it should be noreply"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all 0 norepl\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* this is not considered error */
							} else {
								uint64_t e = 0;
								EXTRACT_NOREPLY(mpr, wl[2].ptr[0], wl[2].ptr[1], e);
								if (e) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for flush all command is 7, but does not match noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all 0 noreplx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/* this is not considered error */
								} else
									noreply = 1;
							}
						}
						MP_OPS_KV_FLUSH_ALL(expr_sec, opaque);
						if (!noreply) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flush all command prints ok"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all 0\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("OK\r\n", 4);
						} else {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="flush all command does not print ok because of noreply"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="flush_all 0 noreply\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
						}
					}
					break;
				case CMD_CACHE_MEMLIMIT:
					if (word_num != 2 && word_num != 3) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for cache memlimit command is neither 2 nor 3"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit 128 noreply 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						uint64_t limit_mb, noreply = 0;
						{
							uint64_t minus, e = 0;
							EXTRACT_VAL(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, limit_mb, minus, e);
							if (minus) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="limit for cache memlimit command is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit -1\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
								break;
							} if (e) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="limit for cache memlimit command is smaller than 0"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit a\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("ERROR\r\n", 7);
								break;
							}
						}
						if (word_num == 3) {
							if (wl[2].len != 7) {
								MP_TEST_HOOK();
								/*
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for cache memlimit command is not 7, it should be noreply"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit 128 norepl\r\n"
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
								 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
								 */
								/* this is not considered error */
							} else {
								uint64_t e;
								EXTRACT_NOREPLY(mpr, wl[2].ptr[0], wl[2].ptr[1], e);
								if (e) {
									MP_TEST_HOOK();
									/*
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the length of 3rd word for cache memlimit command is 7, but does not match noreply"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit 128 noreplx\r\n"
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}" 1
									 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
									 */
									/* this is not considered error */
								} else
									noreply = 1;
							}
						}
						if (!noreply) {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cache_memlimit command prints ok"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit 128\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("OK\r\n", 4);
						} else {
							MP_TEST_HOOK();
							/*
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="cache_memlimit command does not print ok because of noreply"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="cache_memlimit 128 noreply\r\n"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
							 */
						}
					}
					break;
				case CMD_SHUTDOWN:
					if (MP_OPS_ALLOW_SHUTDOWN(opaque)) {
						if (word_num > 2) {
							MP_TEST_HOOK();
							/*
							 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the number of words for quit command is neither 1 nor 2"
							 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS="shutdown graceful 2\r\n"
							 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
							 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: 
							 */
							PUSH_RESPONSE("CLIENT_ERROR invalid shutdown mode\r\n", 36);
						} else if (word_num == 2) {
							if (wl[1].len != 8) {
								MP_TEST_HOOK();
								/*
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word is not 8"
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS="shutdown gracefu\r\n"
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
								 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: 
								 */
								PUSH_RESPONSE("CLIENT_ERROR invalid shutdown mode\r\n", 36);
							} else {
								uint64_t msglen = 0;
								char msg[8]; /* graceful is 8 bytes */
								EXTRACT_DATA(mpr, wl[1].ptr[0], wl[1].ptr[1], wl[1].len, msg, msglen);
								if (!mp_memcmp(msg, "graceful", msglen))
									mp_ops_shutdown(1, opaque);
								else {
									MP_TEST_HOOK();
									/*
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: T_INFO="the length of 2nd word is 8, but does not match graceful"
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: QUERYS="shutdown gracefuf\r\n"
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
									 * __NOT_TESTED_BINARY_QUERY_ERROR_RESPONSE: 
									 */
									PUSH_RESPONSE("CLIENT_ERROR invalid shutdown mode\r\n", 36);
								}
							}
						} else
							mp_ops_shutdown(0, opaque);
					} else {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="shutdown command is not enabled"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="shutdown\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR: shutdown not enabled\r\n", 29);
					}
					break;
				case CMD_VERSION:
					PUSH_RESPONSE("VERSION 1.6.38\r\n", 16);
					break;
				case CMD_QUIT:
					return -1;
					break;
				case CMD_MISBEHAVE:
					if (word_num != 1) {
						MP_TEST_HOOK();
						/*
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: T_INFO="the number of words for misbehave command is not 1"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: QUERYS="misbehave 1\r\n"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: test_text_request "${T_INFO}" "${QUERYS}${QUIT_REQSTR}" "${TESTID}"
						 * TEST_TEXT_QUERY_ERROR_RESPONSE: 
						 */
						PUSH_RESPONSE("ERROR\r\n", 7);
					} else {
						/* XXX: not implemented */
					}
					break;
				default:
					PUSH_RESPONSE("ERROR\r\n", 7);
					break;
				}
			}
			break;
		}
		MPR_RING_HEAD_IDX(mpr) = ring_cur_idx;
		MPR_RING_HEAD_OFF(mpr) = ring_cur_off;
	}
	return ret;
}

