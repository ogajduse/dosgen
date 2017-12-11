/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 11 "trafgen_parser.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h>
#include <errno.h>
#include <stdbool.h>
#include <libgen.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <linux/if_ether.h>

#include "xmalloc.h"
#include "trafgen_parser.tab.h"
#include "trafgen_conf.h"
#include "trafgen_proto.h"
#include "trafgen_l2.h"
#include "trafgen_l3.h"
#include "trafgen_l4.h"
#include "built_in.h"
#include "die.h"
#include "str.h"
#include "csum.h"
#include "cpp.h"

#define YYERROR_VERBOSE		0
#define YYDEBUG			0
#define YYENABLE_NLS		1
#define YYLTYPE_IS_TRIVIAL	1
#define ENABLE_NLS		1

extern FILE *yyin;
extern int yylex(void);
extern void yy_scan_string(char *);
extern void yylex_destroy();
extern void yyerror(const char *);
extern int yylineno;
extern char *yytext;

extern struct packet *packets;
extern size_t plen;

#define packet_last		(plen - 1)

#define payload_last		(packets[packet_last].len - 1)

extern struct packet_dyn *packet_dyn;
extern size_t dlen;

#define packetd_last		(dlen - 1)

#define packetdc_last		(packet_dyn[packetd_last].clen - 1)
#define packetdr_last		(packet_dyn[packetd_last].rlen - 1)
#define packetds_last		(packet_dyn[packetd_last].slen - 1)

static int our_cpu, min_cpu = -1, max_cpu = -1;

static struct proto_hdr *hdr;

static inline int test_ignore(void)
{
	if (min_cpu < 0 && max_cpu < 0)
		return 0;
	else if (max_cpu >= our_cpu && min_cpu <= our_cpu)
		return 0;
	else
		return 1;
}

static inline void __init_new_packet_slot(struct packet *slot)
{
	slot->payload = NULL;
	slot->len = 0;
}

static inline void __init_new_counter_slot(struct packet_dyn *slot)
{
	slot->cnt = NULL;
	slot->clen = 0;
}

static inline void __init_new_randomizer_slot(struct packet_dyn *slot)
{
	slot->rnd = NULL;
	slot->rlen = 0;
}

static inline void __init_new_csum_slot(struct packet_dyn *slot)
{
	slot->csum = NULL;
	slot->slen = 0;
}

static inline void __setup_new_counter(struct counter *c, uint8_t start,
				       uint8_t stop, uint8_t stepping,
				       int type)
{
	c->min = start;
	c->max = stop;
	c->inc = stepping;
	c->val = (type == TYPE_INC) ? start : stop;
	c->off = payload_last;
	c->type = type;
}

static inline void __setup_new_randomizer(struct randomizer *r)
{
	r->off = payload_last;
}

static inline void __setup_new_csum16(struct csum16 *s, off_t from, off_t to,
				      enum csum which)
{
	s->off = payload_last - 1;
	s->from = from;
	s->to = to;
	s->which = which;
}

static void realloc_packet(void)
{
	if (test_ignore())
		return;

	plen++;
	packets = xrealloc(packets, plen * sizeof(*packets));

	__init_new_packet_slot(&packets[packet_last]);

	dlen++;
	packet_dyn = xrealloc(packet_dyn, dlen * sizeof(*packet_dyn));

	__init_new_counter_slot(&packet_dyn[packetd_last]);
	__init_new_randomizer_slot(&packet_dyn[packetd_last]);
	__init_new_csum_slot(&packet_dyn[packetd_last]);
}

struct packet *current_packet(void)
{
	return &packets[packet_last];
}

static void set_byte(uint8_t val)
{
	struct packet *pkt = &packets[packet_last];

	if (test_ignore())
		return;

	pkt->len++;
	pkt->payload = xrealloc(pkt->payload, pkt->len);
	pkt->payload[payload_last] = val;
}

static void set_multi_byte(uint8_t *s, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		set_byte(s[i]);
}

void set_fill(uint8_t val, size_t len)
{
	size_t i;
	struct packet *pkt = &packets[packet_last];

	if (test_ignore())
		return;

	pkt->len += len;
	pkt->payload = xrealloc(pkt->payload, pkt->len);
	for (i = 0; i < len; ++i)
		pkt->payload[payload_last - i] = val;
}

static void __set_csum16_dynamic(size_t from, size_t to, enum csum which)
{
	struct packet *pkt = &packets[packet_last];
	struct packet_dyn *pktd = &packet_dyn[packetd_last];

	pkt->len += 2;
	pkt->payload = xrealloc(pkt->payload, pkt->len);

	pktd->slen++;
	pktd->csum = xrealloc(pktd->csum, pktd->slen * sizeof(struct csum16));

	__setup_new_csum16(&pktd->csum[packetds_last], from, to, which);
}

static void __set_csum16_static(size_t from, size_t to, enum csum which __maybe_unused)
{
	struct packet *pkt = &packets[packet_last];
	uint16_t sum;
	uint8_t *psum;

	sum = htons(calc_csum(pkt->payload + from, to - from));
	psum = (uint8_t *) &sum;

	set_byte(psum[0]);
	set_byte(psum[1]);
}

static inline bool is_dynamic_csum(enum csum which)
{
	switch (which) {
	case CSUM_UDP:
	case CSUM_TCP:
	case CSUM_UDP6:
	case CSUM_TCP6:
		return true;
	default:
		return false;
	}
}

static void set_csum16(size_t from, size_t to, enum csum which)
{
	struct packet *pkt = &packets[packet_last];
	struct packet_dyn *pktd = &packet_dyn[packetd_last];

	if (test_ignore())
		return;

	if (to < from) {
		size_t tmp = to;

		to = from;
		from = tmp;
	}

	bug_on(!(from < to));

	if (packet_dyn_has_elems(pktd) || to >= pkt->len || is_dynamic_csum(which))
		__set_csum16_dynamic(from, to, which);
	else
		__set_csum16_static(from, to, which);
}

static void set_rnd(size_t len)
{
	size_t i;
	struct packet *pkt = &packets[packet_last];

	if (test_ignore())
		return;

	pkt->len += len;
	pkt->payload = xrealloc(pkt->payload, pkt->len);
	for (i = 0; i < len; ++i)
		pkt->payload[payload_last - i] = (uint8_t) rand();
}

static void set_sequential_inc(uint8_t start, size_t len, uint8_t stepping)
{
	size_t i;
	struct packet *pkt = &packets[packet_last];

	if (test_ignore())
		return;

	pkt->len += len;
	pkt->payload = xrealloc(pkt->payload, pkt->len);
	for (i = 0; i < len; ++i) {
		off_t off = len - 1 - i;

		pkt->payload[payload_last - off] = start;
		start += stepping;
	}
}

static void set_sequential_dec(uint8_t start, size_t len, uint8_t stepping)
{
	size_t i;
	struct packet *pkt = &packets[packet_last];

	if (test_ignore())
		return;

	pkt->len += len;
	pkt->payload = xrealloc(pkt->payload, pkt->len);
	for (i = 0; i < len; ++i) {
		int off = len - 1 - i;

		pkt->payload[payload_last - off] = start;
		start -= stepping;
	}
}

static void set_dynamic_rnd(void)
{
	struct packet *pkt = &packets[packet_last];
	struct packet_dyn *pktd = &packet_dyn[packetd_last];

	if (test_ignore())
		return;

	pkt->len++;
	pkt->payload = xrealloc(pkt->payload, pkt->len);

	pktd->rlen++;
	pktd->rnd = xrealloc(pktd->rnd, pktd->rlen * sizeof(struct randomizer));

	__setup_new_randomizer(&pktd->rnd[packetdr_last]);
}

static void set_dynamic_incdec(uint8_t start, uint8_t stop, uint8_t stepping,
			       int type)
{
	struct packet *pkt = &packets[packet_last];
	struct packet_dyn *pktd = &packet_dyn[packetd_last];

	if (test_ignore())
		return;

	pkt->len++;
	pkt->payload = xrealloc(pkt->payload, pkt->len);

	pktd->clen++;
	pktd->cnt = xrealloc(pktd->cnt, pktd->clen * sizeof(struct counter));

	__setup_new_counter(&pktd->cnt[packetdc_last], start, stop, stepping, type);
}

static void proto_add(enum proto_id pid)
{
	hdr = proto_header_init(pid);
}


#line 398 "trafgen/trafgen_parser.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "trafgen_parser.tab.h".  */
#ifndef YY_YY_TRAFGEN_TRAFGEN_PARSER_TAB_H_INCLUDED
# define YY_YY_TRAFGEN_TRAFGEN_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    K_COMMENT = 258,
    K_FILL = 259,
    K_RND = 260,
    K_SEQINC = 261,
    K_SEQDEC = 262,
    K_DRND = 263,
    K_DINC = 264,
    K_DDEC = 265,
    K_WHITE = 266,
    K_CPU = 267,
    K_CSUMIP = 268,
    K_CSUMUDP = 269,
    K_CSUMTCP = 270,
    K_CSUMUDP6 = 271,
    K_CSUMTCP6 = 272,
    K_CONST8 = 273,
    K_CONST16 = 274,
    K_CONST32 = 275,
    K_CONST64 = 276,
    K_DADDR = 277,
    K_SADDR = 278,
    K_ETYPE = 279,
    K_OPER = 280,
    K_SHA = 281,
    K_SPA = 282,
    K_THA = 283,
    K_TPA = 284,
    K_REQUEST = 285,
    K_REPLY = 286,
    K_PTYPE = 287,
    K_HTYPE = 288,
    K_PROT = 289,
    K_TTL = 290,
    K_DSCP = 291,
    K_ECN = 292,
    K_TOS = 293,
    K_LEN = 294,
    K_ID = 295,
    K_FLAGS = 296,
    K_FRAG = 297,
    K_IHL = 298,
    K_VER = 299,
    K_CSUM = 300,
    K_DF = 301,
    K_MF = 302,
    K_SPORT = 303,
    K_DPORT = 304,
    K_SEQ = 305,
    K_ACK_SEQ = 306,
    K_DOFF = 307,
    K_CWR = 308,
    K_ECE = 309,
    K_URG = 310,
    K_ACK = 311,
    K_PSH = 312,
    K_RST = 313,
    K_SYN = 314,
    K_FIN = 315,
    K_WINDOW = 316,
    K_URG_PTR = 317,
    K_TPID = 318,
    K_TCI = 319,
    K_PCP = 320,
    K_DEI = 321,
    K_1Q = 322,
    K_1AD = 323,
    K_ETH = 324,
    K_VLAN = 325,
    K_ARP = 326,
    K_IP4 = 327,
    K_UDP = 328,
    K_TCP = 329,
    number = 330,
    string = 331,
    mac = 332,
    ip4_addr = 333
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 343 "trafgen_parser.y" /* yacc.c:355  */

	struct in_addr ip4_addr;
	long long int number;
	uint8_t bytes[256];
	char *str;

#line 524 "trafgen/trafgen_parser.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TRAFGEN_TRAFGEN_PARSER_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 541 "trafgen/trafgen_parser.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   474

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  167
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  465

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   333

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    87,    88,     2,
      78,    79,    85,    84,    75,    83,     2,    86,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    82,     2,
      90,    97,    91,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    80,     2,    81,    92,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,    89,    77,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      93,    94,    95,    96
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   380,   380,   381,   382,   383,   387,   391,   392,   396,
     397,   401,   402,   403,   407,   408,   411,   418,   433,   443,
     444,   448,   449,   453,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   473,   475,   477,
     479,   481,   483,   485,   487,   489,   491,   493,   495,   497,
     502,   507,   509,   514,   519,   527,   532,   534,   536,   538,
     540,   545,   547,   552,   554,   559,   561,   570,   572,   577,
     579,   584,   585,   586,   587,   588,   589,   593,   597,   601,
     602,   603,   607,   608,   612,   614,   616,   621,   625,   629,
     630,   631,   635,   636,   640,   642,   644,   646,   648,   650,
     652,   657,   661,   662,   663,   667,   669,   671,   673,   675,
     677,   679,   681,   683,   685,   687,   691,   695,   699,   700,
     701,   705,   707,   709,   711,   713,   715,   717,   719,   721,
     723,   725,   727,   729,   730,   731,   733,   738,   742,   746,
     747,   748,   752,   754,   756,   758,   763,   767,   771,   772,
     773,   777,   779,   781,   783,   785,   787,   788,   789,   790,
     791,   792,   793,   794,   795,   797,   799,   804
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "K_COMMENT", "K_FILL", "K_RND",
  "K_SEQINC", "K_SEQDEC", "K_DRND", "K_DINC", "K_DDEC", "K_WHITE", "K_CPU",
  "K_CSUMIP", "K_CSUMUDP", "K_CSUMTCP", "K_CSUMUDP6", "K_CSUMTCP6",
  "K_CONST8", "K_CONST16", "K_CONST32", "K_CONST64", "K_DADDR", "K_SADDR",
  "K_ETYPE", "K_OPER", "K_SHA", "K_SPA", "K_THA", "K_TPA", "K_REQUEST",
  "K_REPLY", "K_PTYPE", "K_HTYPE", "K_PROT", "K_TTL", "K_DSCP", "K_ECN",
  "K_TOS", "K_LEN", "K_ID", "K_FLAGS", "K_FRAG", "K_IHL", "K_VER",
  "K_CSUM", "K_DF", "K_MF", "K_SPORT", "K_DPORT", "K_SEQ", "K_ACK_SEQ",
  "K_DOFF", "K_CWR", "K_ECE", "K_URG", "K_ACK", "K_PSH", "K_RST", "K_SYN",
  "K_FIN", "K_WINDOW", "K_URG_PTR", "K_TPID", "K_TCI", "K_PCP", "K_DEI",
  "K_1Q", "K_1AD", "K_ETH", "K_VLAN", "K_ARP", "K_IP4", "K_UDP", "K_TCP",
  "','", "'{'", "'}'", "'('", "')'", "'['", "']'", "':'", "'-'", "'+'",
  "'*'", "'/'", "'%'", "'&'", "'|'", "'<'", "'>'", "'^'", "number",
  "string", "mac", "ip4_addr", "'='", "$accept", "packets",
  "inline_comment", "cpu_delim", "delimiter_nowhite", "noenforce_white",
  "skip_white", "packet", "payload", "delimiter", "elem_delimiter", "elem",
  "expression", "fill", "const", "rnd", "csum", "seqinc", "seqdec", "drnd",
  "dinc", "ddec", "proto", "eth_proto", "eth", "eth_param_list",
  "eth_type", "eth_field", "vlan_proto", "vlan", "vlan_param_list",
  "vlan_type", "vlan_field", "arp_proto", "arp_param_list", "arp_field",
  "arp", "ip4_proto", "ip4_param_list", "ip4_field", "ip4", "udp_proto",
  "udp_param_list", "udp_field", "udp", "tcp_proto", "tcp_param_list",
  "tcp_field", "tcp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,    44,   123,   125,    40,    41,
      91,    93,    58,    45,    43,    42,    47,    37,    38,   124,
      60,    62,    94,   330,   331,   332,   333,    61
};
# endif

#define YYPACT_NINF -343

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-343)))

#define YYTABLE_NINF -14

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -343,     6,  -343,  -343,  -343,   -67,    -1,  -343,  -343,   -78,
    -343,    15,  -343,   246,     4,  -343,   -44,   -39,   -21,   -16,
     -15,    -8,     7,    13,    16,    19,    20,    28,    30,    31,
      41,    42,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,    14,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,  -343,  -343,  -343,    45,  -343,    48,  -343,    51,  -343,
      52,  -343,    53,  -343,    55,    -9,  -343,  -343,   -13,    43,
      44,    46,    49,   -55,    58,    59,    60,    61,    62,    65,
      66,    32,    32,    32,    32,    57,    67,    83,   246,  -343,
     -20,    82,   357,   134,    79,   234,    -1,    56,    17,    84,
      17,    17,  -343,    85,    17,    17,    17,    17,    17,    17,
      17,    32,    32,  -343,   218,   245,   259,   273,  -343,  -343,
      96,    96,  -343,  -343,    86,    96,    17,  -343,    96,  -343,
      96,    96,    96,  -343,  -343,    87,    96,    17,    96,    96,
      96,    96,    96,  -343,  -343,    96,    96,   103,    17,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,    96,  -343,  -343,   104,    17,    96,    96,    96,
      96,   105,    17,    96,    96,    96,    96,    96,    96,  -343,
    -343,  -343,  -343,  -343,  -343,  -343,  -343,    96,    96,   107,
      17,   111,    80,  -343,  -343,    95,  -343,    97,   101,  -343,
     115,   130,   135,   153,   154,   164,   165,   289,  -343,  -343,
      32,    32,    32,    32,    32,    32,    32,    99,    47,    32,
    -343,  -343,  -343,  -343,   171,   172,  -343,   173,   -20,   176,
     177,   183,   184,  -343,   201,    82,   202,   203,   214,   215,
     216,   217,   224,  -343,   357,   225,   226,   228,   244,   256,
     257,   258,   272,   274,   294,   295,   296,   297,   298,  -343,
     134,   299,   300,   301,   302,  -343,    79,   305,   306,   307,
     308,   309,   310,   311,   312,  -343,   234,    -1,    -1,   121,
      -4,    -3,     2,     9,   291,   331,   332,   333,   334,  -343,
    -343,  -343,  -343,  -343,  -343,  -343,  -343,    32,    32,  -343,
      96,    96,    96,  -343,    96,    96,    96,    96,    96,  -343,
      96,    96,    96,    96,    96,    96,    96,  -343,    96,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    96,
      96,    96,  -343,    96,    96,    96,    96,  -343,    96,    96,
      96,    96,    96,    96,    96,    96,  -343,   246,   338,  -343,
    -343,   322,  -343,   323,  -343,   324,  -343,   325,  -343,  -343,
    -343,  -343,  -343,  -343,  -343,   326,   328,   327,   335,   336,
     337,   339,   340,    24,   329,   330,   341,   342,   344,   346,
     345,   347,   349,   351,   352,   353,   354,   355,   356,   358,
     359,   360,   361,   362,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,    14,    -1,   348,   390,
     391,   392,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,  -343,  -343,  -343,  -343,   363,   246,  -343,  -343,  -343,
    -343,  -343,    14,   395,  -343
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     6,     5,     0,    11,     4,     3,     0,
      12,     9,    13,     0,     0,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,    88,   116,   137,   146,   167,    24,    25,
      36,    11,    19,    26,    34,    27,    33,    29,    30,    28,
      31,    32,    35,    71,     0,    72,     0,    73,     0,    74,
       0,    75,     0,    76,     0,     0,     7,     8,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    22,    21,     0,     0,    20,
      79,    89,   102,   118,   139,   148,    11,     0,     0,     0,
       0,     0,    65,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,     0,     0,     0,     0,    16,    23,
      14,    14,    82,    83,     0,    14,    80,    93,    14,    92,
      14,    14,    14,    95,    96,     0,    14,    90,    14,    14,
      14,    14,    14,   108,   109,    14,    14,     0,   103,    14,
      14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
      14,    14,    14,   133,   134,     0,   119,    14,    14,    14,
      14,     0,   140,    14,    14,    14,    14,    14,    14,   156,
     157,   158,   159,   160,   161,   162,   163,    14,    14,     0,
     149,     0,     0,    22,    21,     0,    55,     0,     0,    66,
       0,     0,     0,     0,     0,     0,     0,     0,    48,    51,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      52,    53,    54,    15,     0,     0,    77,     0,    79,     0,
       0,     0,     0,    87,     0,    89,     0,     0,     0,     0,
       0,     0,     0,   101,   102,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   117,
     118,     0,     0,     0,     0,   138,   139,     0,     0,     0,
       0,     0,     0,     0,     0,   147,   148,    11,    11,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    49,
      39,    38,    40,    41,    42,    43,    44,     0,     0,    45,
      14,    14,    14,    81,    14,    14,    14,    14,    14,    91,
      14,    14,    14,    14,    14,    14,    14,   104,    14,    14,
      14,    14,    14,    14,    14,    14,    14,    14,    14,    14,
      14,    14,   120,    14,    14,    14,    14,   141,    14,    14,
      14,    14,    14,    14,    14,    14,   150,     0,     0,    50,
      61,     0,    63,     0,    67,     0,    69,     0,    56,    58,
      57,    60,    59,    46,    47,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,    11,     0,     0,
       0,     0,    84,    85,    86,   100,    97,    98,    99,    94,
     105,   106,   107,   112,   114,   113,   115,   111,   110,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   135,
     122,   121,   136,   144,   145,   142,   143,   165,   151,   152,
     153,   154,   155,   164,   166,     0,     0,    62,    64,    68,
      70,    18,    11,     0,    17
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -343,  -343,   418,  -343,    -6,   -40,  -109,  -343,  -342,    -5,
    -343,   343,    29,  -343,  -343,  -343,  -343,  -343,  -343,  -343,
    -343,  -343,  -343,  -343,  -343,   197,  -343,  -343,  -343,  -343,
     199,  -343,  -343,  -343,   191,  -343,  -343,  -343,   190,  -343,
    -343,  -343,   207,  -343,  -343,  -343,   198,  -343,  -343
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    40,    68,   194,    13,   224,     8,    41,    88,
      89,    42,   114,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,   124,   125,   126,    55,    56,
     135,   136,   137,    57,   147,   148,    58,    59,   165,   166,
      60,    61,   171,   172,    62,    63,   189,   190,    64
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      12,    87,   120,   121,   122,   406,     2,   193,   193,     3,
      10,     9,   225,   193,   123,    14,   227,     4,     5,   229,
     193,   230,   231,   232,   102,    85,    15,   234,   193,   236,
     237,   238,   239,   240,    69,    86,   241,   242,   103,    70,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,   420,   421,   191,    71,   261,   262,
     263,   264,    72,    73,   267,   268,   269,   270,   271,   272,
      74,    11,    11,    96,    11,   350,   352,    11,   273,   274,
      97,   354,     6,    65,    11,    75,    66,    67,   356,    11,
      12,    76,    11,   195,    77,   197,   198,    78,    79,   200,
     201,   202,   203,   204,   205,   206,    80,   223,    81,    82,
     111,   115,   116,   117,   462,   112,   127,   422,   167,    83,
      84,   228,   128,    90,   168,   113,    91,   169,   170,    92,
      93,    94,   235,    95,   -12,   192,    98,    99,   298,   100,
     207,   208,   101,   244,   -13,   129,   130,   131,   132,   133,
     134,   104,   105,   106,   107,   108,   149,   150,   109,   110,
     118,   260,   278,   196,   199,   226,   233,   266,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   243,   259,   265,   276,   275,   277,   279,   297,
     280,   365,   366,   367,   281,   368,   369,   370,   371,   372,
     349,   373,   374,   375,   376,   377,   378,   379,   282,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   283,   394,   395,   396,   397,   284,   398,
     399,   400,   401,   402,   403,   404,   405,   347,   348,   290,
     291,   292,   293,   294,   295,   296,   285,   286,   299,     3,
      16,    17,    18,    19,    20,    21,    22,   287,   288,    23,
      24,    25,    26,    27,    28,    29,    30,    31,   300,   301,
     302,    12,    12,   304,   305,   351,   353,   355,   357,   173,
     306,   307,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   209,   308,   310,
     311,   210,   211,   212,   213,   214,   215,   216,   217,   218,
     219,   312,   313,   314,   315,    32,    33,    34,    35,    36,
      37,   316,   318,   319,   220,   320,   363,   364,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   221,    38,
      39,   321,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   222,   322,   323,   324,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   455,   456,   289,   325,
     358,   326,   210,   211,   212,   213,   214,   215,   216,   217,
     218,   219,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   327,   328,   329,   330,   331,   333,   334,   335,   336,
      86,    12,   338,   339,   340,   341,   342,   343,   344,   345,
     359,   360,   361,   362,   407,   408,   409,   410,   411,     7,
     414,   412,   463,   413,   423,   303,   424,   457,   415,   416,
     417,   119,   418,   419,   309,   317,   425,   427,   426,   428,
     461,   429,   431,   430,   432,   433,   434,   435,   436,   437,
     332,   438,   439,   440,   441,   442,    86,   443,   444,   445,
     446,   447,   448,   449,   450,   451,   452,   453,   454,   458,
     459,   460,   464,   337,   346
};

static const yytype_uint16 yycheck[] =
{
       6,    41,    22,    23,    24,   347,     0,    11,    11,     3,
      11,    78,   121,    11,    34,    93,   125,    11,    12,   128,
      11,   130,   131,   132,    79,    11,    11,   136,    11,   138,
     139,   140,   141,   142,    78,    41,   145,   146,    93,    78,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    30,    31,    96,    78,   167,   168,
     169,   170,    78,    78,   173,   174,   175,   176,   177,   178,
      78,    75,    75,    82,    75,    79,    79,    75,   187,   188,
      93,    79,    76,    79,    75,    78,    82,    83,    79,    75,
      96,    78,    75,    98,    78,   100,   101,    78,    78,   104,
     105,   106,   107,   108,   109,   110,    78,    11,    78,    78,
      78,    82,    83,    84,   456,    83,    34,    93,    39,    78,
      78,   126,    40,    78,    45,    93,    78,    48,    49,    78,
      78,    78,   137,    78,    77,    79,    93,    93,    91,    93,
     111,   112,    93,   148,    77,    63,    64,    65,    66,    67,
      68,    93,    93,    93,    93,    93,    22,    23,    93,    93,
      77,   166,    82,    79,    79,    79,    79,   172,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    79,    79,    79,   190,    79,    76,    93,    90,
      93,   300,   301,   302,    93,   304,   305,   306,   307,   308,
      79,   310,   311,   312,   313,   314,   315,   316,    93,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,    93,   333,   334,   335,   336,    93,   338,
     339,   340,   341,   342,   343,   344,   345,   277,   278,   210,
     211,   212,   213,   214,   215,   216,    93,    93,   219,     3,
       4,     5,     6,     7,     8,     9,    10,    93,    93,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    97,    97,
      97,   277,   278,    97,    97,   280,   281,   282,   283,    45,
      97,    97,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    79,    97,    97,
      97,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    97,    97,    97,    97,    69,    70,    71,    72,    73,
      74,    97,    97,    97,    79,    97,   297,   298,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    79,    93,
      94,    97,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    79,    97,    97,    97,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,   406,   407,    79,    97,
      79,    97,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    97,    97,    97,    97,    97,    97,    97,    97,    97,
     406,   407,    97,    97,    97,    97,    97,    97,    97,    97,
      79,    79,    79,    79,    76,    93,    93,    93,    93,     1,
      93,    95,   462,    95,    95,   228,    96,    79,    93,    93,
      93,    88,    93,    93,   235,   244,    95,    93,    96,    93,
      77,    96,    93,    96,    93,    93,    93,    93,    93,    93,
     260,    93,    93,    93,    93,    93,   462,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    79,
      79,    79,    77,   266,   276
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    99,     0,     3,    11,    12,    76,   100,   105,    78,
      11,    75,   102,   103,    93,    11,     4,     5,     6,     7,
       8,     9,    10,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    69,    70,    71,    72,    73,    74,    93,    94,
     100,   106,   109,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   126,   127,   131,   134,   135,
     138,   139,   142,   143,   146,    79,    82,    83,   101,    78,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    78,
      78,    78,    78,    78,    78,    11,   102,   103,   107,   108,
      78,    78,    78,    78,    78,    78,    82,    93,    93,    93,
      93,    93,    79,    93,    93,    93,    93,    93,    93,    93,
      93,    78,    83,    93,   110,   110,   110,   110,    77,   109,
      22,    23,    24,    34,   123,   124,   125,    34,    40,    63,
      64,    65,    66,    67,    68,   128,   129,   130,    25,    26,
      27,    28,    29,    30,    31,    32,    33,   132,   133,    22,
      23,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,   136,   137,    39,    45,    48,
      49,   140,   141,    45,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   144,
     145,   103,    79,    11,   102,   107,    79,   107,   107,    79,
     107,   107,   107,   107,   107,   107,   107,   110,   110,    79,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      79,    79,    79,    11,   104,   104,    79,   104,   107,   104,
     104,   104,   104,    79,   104,   107,   104,   104,   104,   104,
     104,   104,   104,    79,   107,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,    79,
     107,   104,   104,   104,   104,    79,   107,   104,   104,   104,
     104,   104,   104,   104,   104,    79,   107,    76,    82,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    79,
     110,   110,   110,   110,   110,   110,   110,    90,    91,   110,
      97,    97,    97,   123,    97,    97,    97,    97,    97,   128,
      97,    97,    97,    97,    97,    97,    97,   132,    97,    97,
      97,    97,    97,    97,    97,    97,    97,    97,    97,    97,
      97,    97,   136,    97,    97,    97,    97,   140,    97,    97,
      97,    97,    97,    97,    97,    97,   144,   103,   103,    79,
      79,   107,    79,   107,    79,   107,    79,   107,    79,    79,
      79,    79,    79,   110,   110,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   106,    76,    93,    93,
      93,    93,    95,    95,    93,    93,    93,    93,    93,    93,
      30,    31,    93,    95,    96,    95,    96,    93,    93,    96,
      96,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,   103,   103,    79,    79,    79,
      79,    77,   106,   103,    77
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,    99,    99,    99,   100,   101,   101,   102,
     102,   103,   103,   103,   104,   104,   105,   105,   105,   106,
     106,   107,   107,   108,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     111,   112,   112,   112,   112,   113,   114,   114,   114,   114,
     114,   115,   115,   116,   116,   117,   117,   118,   118,   119,
     119,   120,   120,   120,   120,   120,   120,   121,   122,   123,
     123,   123,   124,   124,   125,   125,   125,   126,   127,   128,
     128,   128,   129,   129,   130,   130,   130,   130,   130,   130,
     130,   131,   132,   132,   132,   133,   133,   133,   133,   133,
     133,   133,   133,   133,   133,   133,   134,   135,   136,   136,
     136,   137,   137,   137,   137,   137,   137,   137,   137,   137,
     137,   137,   137,   137,   137,   137,   137,   138,   139,   140,
     140,   140,   141,   141,   141,   141,   142,   143,   144,   144,
     144,   145,   145,   145,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   146
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     1,     1,     1,     1,
       2,     0,     1,     1,     0,     1,     5,    13,    11,     1,
       2,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     4,     4,     2,     3,
       6,     4,     4,     4,     4,     4,     6,     6,     6,     6,
       6,     6,     8,     6,     8,     3,     4,     6,     8,     6,
       8,     1,     1,     1,     1,     1,     1,     4,     1,     0,
       1,     3,     1,     1,     5,     5,     5,     4,     1,     0,
       1,     3,     1,     1,     5,     1,     1,     5,     5,     5,
       5,     4,     0,     1,     3,     5,     5,     5,     1,     1,
       5,     5,     5,     5,     5,     5,     1,     4,     0,     1,
       3,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     1,     1,     5,     5,     1,     4,     0,
       1,     3,     5,     5,     5,     5,     1,     4,     0,     1,
       3,     5,     5,     5,     5,     5,     1,     1,     1,     1,
       1,     1,     1,     1,     5,     5,     5,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 380 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1932 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 381 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1938 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 382 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1944 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 383 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1950 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 387 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1956 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 391 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1962 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 392 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1968 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 396 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1974 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 397 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1980 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 401 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1986 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 402 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1992 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 403 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 1998 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 407 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2004 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 408 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2010 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 411 "trafgen_parser.y" /* yacc.c:1646  */
    {
			min_cpu = max_cpu = -1;

			proto_packet_finish();

			realloc_packet();
		}
#line 2022 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 418 "trafgen_parser.y" /* yacc.c:1646  */
    {
			min_cpu = (yyvsp[-10].number);
			max_cpu = (yyvsp[-8].number);

			if (min_cpu > max_cpu) {
				int tmp = min_cpu;

				min_cpu = max_cpu;
				max_cpu = tmp;
			}

			proto_packet_finish();

			realloc_packet();
		}
#line 2042 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 433 "trafgen_parser.y" /* yacc.c:1646  */
    {
			min_cpu = max_cpu = (yyvsp[-8].number);

			proto_packet_finish();

			realloc_packet();
		}
#line 2054 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 443 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2060 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 444 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2066 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 448 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2072 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 449 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2078 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 453 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2084 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 457 "trafgen_parser.y" /* yacc.c:1646  */
    { set_byte((uint8_t) (yyvsp[0].number)); }
#line 2090 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 458 "trafgen_parser.y" /* yacc.c:1646  */
    { set_multi_byte((uint8_t *) (yyvsp[0].str) + 1, strlen((yyvsp[0].str)) - 2); }
#line 2096 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 459 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2102 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 460 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2108 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 461 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2114 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 462 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2120 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 463 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2126 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 464 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2132 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 465 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2138 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 466 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2144 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 467 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2150 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 468 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_header_finish(hdr); }
#line 2156 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 469 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2162 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 474 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[0].number); }
#line 2168 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 476 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) + (yyvsp[0].number); }
#line 2174 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 478 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) - (yyvsp[0].number); }
#line 2180 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 480 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) * (yyvsp[0].number); }
#line 2186 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 482 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) / (yyvsp[0].number); }
#line 2192 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 484 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) % (yyvsp[0].number); }
#line 2198 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 486 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) & (yyvsp[0].number); }
#line 2204 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 488 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) | (yyvsp[0].number); }
#line 2210 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 490 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-2].number) ^ (yyvsp[0].number); }
#line 2216 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 492 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-3].number) << (yyvsp[0].number); }
#line 2222 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 494 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-3].number) >> (yyvsp[0].number); }
#line 2228 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 496 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = -1 * (yyvsp[0].number); }
#line 2234 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 498 "trafgen_parser.y" /* yacc.c:1646  */
    { (yyval.number) = (yyvsp[-1].number);}
#line 2240 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 503 "trafgen_parser.y" /* yacc.c:1646  */
    { set_fill((yyvsp[-3].number), (yyvsp[-1].number)); }
#line 2246 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 508 "trafgen_parser.y" /* yacc.c:1646  */
    { set_byte((uint8_t) (yyvsp[-1].number)); }
#line 2252 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 509 "trafgen_parser.y" /* yacc.c:1646  */
    {
			uint16_t __c = cpu_to_be16((uint16_t) (yyvsp[-1].number));

			set_multi_byte((uint8_t *) &__c, sizeof(__c));
		}
#line 2262 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 514 "trafgen_parser.y" /* yacc.c:1646  */
    {
			uint32_t __c = cpu_to_be32((uint32_t) (yyvsp[-1].number));

			set_multi_byte((uint8_t *) &__c, sizeof(__c));
		}
#line 2272 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 519 "trafgen_parser.y" /* yacc.c:1646  */
    {
			uint64_t __c = cpu_to_be64((uint64_t) (yyvsp[-1].number));

			set_multi_byte((uint8_t *) &__c, sizeof(__c));
		}
#line 2282 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 528 "trafgen_parser.y" /* yacc.c:1646  */
    { set_rnd((yyvsp[-1].number)); }
#line 2288 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 533 "trafgen_parser.y" /* yacc.c:1646  */
    { set_csum16((yyvsp[-3].number), (yyvsp[-1].number), CSUM_IP); }
#line 2294 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 535 "trafgen_parser.y" /* yacc.c:1646  */
    { set_csum16((yyvsp[-3].number), (yyvsp[-1].number), CSUM_TCP); }
#line 2300 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 537 "trafgen_parser.y" /* yacc.c:1646  */
    { set_csum16((yyvsp[-3].number), (yyvsp[-1].number), CSUM_UDP); }
#line 2306 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 539 "trafgen_parser.y" /* yacc.c:1646  */
    { set_csum16((yyvsp[-3].number), (yyvsp[-1].number), CSUM_TCP6); }
#line 2312 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 541 "trafgen_parser.y" /* yacc.c:1646  */
    { set_csum16((yyvsp[-3].number), (yyvsp[-1].number), CSUM_UDP6); }
#line 2318 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 546 "trafgen_parser.y" /* yacc.c:1646  */
    { set_sequential_inc((yyvsp[-3].number), (yyvsp[-1].number), 1); }
#line 2324 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 548 "trafgen_parser.y" /* yacc.c:1646  */
    { set_sequential_inc((yyvsp[-5].number), (yyvsp[-3].number), (yyvsp[-1].number)); }
#line 2330 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 553 "trafgen_parser.y" /* yacc.c:1646  */
    { set_sequential_dec((yyvsp[-3].number), (yyvsp[-1].number), 1); }
#line 2336 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 555 "trafgen_parser.y" /* yacc.c:1646  */
    { set_sequential_dec((yyvsp[-5].number), (yyvsp[-3].number), (yyvsp[-1].number)); }
#line 2342 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 560 "trafgen_parser.y" /* yacc.c:1646  */
    { set_dynamic_rnd(); }
#line 2348 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 562 "trafgen_parser.y" /* yacc.c:1646  */
    {
			int i, max = (yyvsp[-1].number);
			for (i = 0; i < max; ++i)
				set_dynamic_rnd();
		}
#line 2358 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 571 "trafgen_parser.y" /* yacc.c:1646  */
    { set_dynamic_incdec((yyvsp[-3].number), (yyvsp[-1].number), 1, TYPE_INC); }
#line 2364 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 573 "trafgen_parser.y" /* yacc.c:1646  */
    { set_dynamic_incdec((yyvsp[-5].number), (yyvsp[-3].number), (yyvsp[-1].number), TYPE_INC); }
#line 2370 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 578 "trafgen_parser.y" /* yacc.c:1646  */
    { set_dynamic_incdec((yyvsp[-3].number), (yyvsp[-1].number), 1, TYPE_DEC); }
#line 2376 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 580 "trafgen_parser.y" /* yacc.c:1646  */
    { set_dynamic_incdec((yyvsp[-5].number), (yyvsp[-3].number), (yyvsp[-1].number), TYPE_DEC); }
#line 2382 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 584 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2388 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 585 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2394 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 586 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2400 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 587 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2406 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 588 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2412 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 589 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2418 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 593 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2424 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 597 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_add(PROTO_ETH); }
#line 2430 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 601 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2436 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 602 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2442 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 603 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2448 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 607 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2454 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 608 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2460 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 613 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_bytes(hdr, ETH_DST_ADDR, (yyvsp[0].bytes)); }
#line 2466 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 615 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_bytes(hdr, ETH_SRC_ADDR, (yyvsp[0].bytes)); }
#line 2472 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 617 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ETH_TYPE, (yyvsp[0].number)); }
#line 2478 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 621 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2484 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 625 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_add(PROTO_VLAN); }
#line 2490 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 629 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2496 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 630 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2502 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 631 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2508 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 635 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2514 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 641 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_TPID, (yyvsp[0].number)); }
#line 2520 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 643 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_TPID, ETH_P_8021Q); }
#line 2526 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 645 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_TPID, ETH_P_8021AD); }
#line 2532 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 647 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_TCI, (yyvsp[0].number)); }
#line 2538 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 649 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_PCP, (yyvsp[0].number)); }
#line 2544 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 651 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_DEI, (yyvsp[0].number)); }
#line 2550 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 653 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, VLAN_VID, (yyvsp[0].number)); }
#line 2556 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 657 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2562 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 661 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2568 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 662 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2574 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 663 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2580 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 668 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_OPER, ARPOP_REQUEST); }
#line 2586 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 670 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_OPER, ARPOP_REPLY); }
#line 2592 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 672 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_OPER, (yyvsp[0].number)); }
#line 2598 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 674 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_OPER, ARPOP_REQUEST); }
#line 2604 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 676 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_OPER, ARPOP_REPLY); }
#line 2610 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 678 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_HTYPE, (yyvsp[0].number)); }
#line 2616 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 680 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, ARP_PTYPE, (yyvsp[0].number)); }
#line 2622 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 682 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_bytes(hdr, ARP_SHA, (yyvsp[0].bytes)); }
#line 2628 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 684 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_bytes(hdr, ARP_THA, (yyvsp[0].bytes)); }
#line 2634 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 686 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u32(hdr, ARP_SPA, (yyvsp[0].ip4_addr).s_addr); }
#line 2640 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 688 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u32(hdr, ARP_TPA, (yyvsp[0].ip4_addr).s_addr); }
#line 2646 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 691 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_add(PROTO_ARP); }
#line 2652 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 695 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2658 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 699 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2664 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 700 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2670 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 701 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2676 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 706 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_VER, (yyvsp[0].number)); }
#line 2682 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 708 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_IHL, (yyvsp[0].number)); }
#line 2688 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 710 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u32(hdr, IP4_DADDR, (yyvsp[0].ip4_addr).s_addr); }
#line 2694 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 712 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u32(hdr, IP4_SADDR, (yyvsp[0].ip4_addr).s_addr); }
#line 2700 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 714 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_PROTO, (yyvsp[0].number)); }
#line 2706 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 716 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_TTL, (yyvsp[0].number)); }
#line 2712 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 718 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_DSCP, (yyvsp[0].number)); }
#line 2718 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 720 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_ECN, (yyvsp[0].number)); }
#line 2724 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 722 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_u8(hdr, IP4_TOS, (yyvsp[0].number)); }
#line 2730 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 724 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_LEN, (yyvsp[0].number)); }
#line 2736 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 726 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_ID, (yyvsp[0].number)); }
#line 2742 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 728 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_FLAGS, (yyvsp[0].number)); }
#line 2748 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 729 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_DF, 1); }
#line 2754 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 730 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_MF, 1); }
#line 2760 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 732 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_FRAG_OFFS, (yyvsp[0].number)); }
#line 2766 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 734 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, IP4_CSUM, (yyvsp[0].number)); }
#line 2772 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 738 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_add(PROTO_IP4); }
#line 2778 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 742 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2784 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 746 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2790 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 747 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2796 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 748 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2802 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 753 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, UDP_SPORT, (yyvsp[0].number)); }
#line 2808 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 755 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, UDP_DPORT, (yyvsp[0].number)); }
#line 2814 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 757 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, UDP_LEN, (yyvsp[0].number)); }
#line 2820 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 759 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, UDP_CSUM, (yyvsp[0].number)); }
#line 2826 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 763 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_add(PROTO_UDP); }
#line 2832 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 767 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2838 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 771 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2844 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 772 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2850 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 773 "trafgen_parser.y" /* yacc.c:1646  */
    { }
#line 2856 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 778 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_SPORT, (yyvsp[0].number)); }
#line 2862 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 780 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_DPORT, (yyvsp[0].number)); }
#line 2868 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 782 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be32(hdr, TCP_SEQ, (yyvsp[0].number)); }
#line 2874 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 784 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be32(hdr, TCP_ACK_SEQ, (yyvsp[0].number)); }
#line 2880 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 786 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_DOFF, (yyvsp[0].number)); }
#line 2886 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 787 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_CWR, 1); }
#line 2892 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 788 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_ECE, 1); }
#line 2898 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 789 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_URG, 1); }
#line 2904 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 790 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_ACK, 1); }
#line 2910 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 791 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_PSH, 1); }
#line 2916 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 792 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_RST, 1); }
#line 2922 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 793 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_SYN, 1); }
#line 2928 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 794 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_FIN, 1); }
#line 2934 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 796 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_WINDOW, (yyvsp[0].number)); }
#line 2940 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 798 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_CSUM, (yyvsp[0].number)); }
#line 2946 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 800 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_field_set_be16(hdr, TCP_URG_PTR, (yyvsp[0].number)); }
#line 2952 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 804 "trafgen_parser.y" /* yacc.c:1646  */
    { proto_add(PROTO_TCP); }
#line 2958 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
    break;


#line 2962 "trafgen/trafgen_parser.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 807 "trafgen_parser.y" /* yacc.c:1906  */


static void finalize_packet(void)
{
	/* XXX hack ... we allocated one packet pointer too much */
	plen--;
	dlen--;
}

static void dump_conf(void)
{
	size_t i, j;

	for (i = 0; i < plen; ++i) {
		printf("[%zu] pkt\n", i);
		printf(" len %zu cnts %zu rnds %zu\n",
		       packets[i].len,
		       packet_dyn[i].clen,
		       packet_dyn[i].rlen);

		printf(" payload ");
		for (j = 0; j < packets[i].len; ++j)
			printf("%02x ", packets[i].payload[j]);
		printf("\n");

		for (j = 0; j < packet_dyn[i].clen; ++j)
			printf(" cnt%zu [%u,%u], inc %u, off %jd type %s\n", j,
			       packet_dyn[i].cnt[j].min,
			       packet_dyn[i].cnt[j].max,
			       packet_dyn[i].cnt[j].inc,
			       (intmax_t)packet_dyn[i].cnt[j].off,
			       packet_dyn[i].cnt[j].type == TYPE_INC ?
			       "inc" : "dec");

		for (j = 0; j < packet_dyn[i].rlen; ++j)
			printf(" rnd%zu off %jd\n", j,
			       (intmax_t)packet_dyn[i].rnd[j].off);
	}
}

void cleanup_packets(void)
{
	size_t i;

	for (i = 0; i < plen; ++i) {
		if (packets[i].len > 0)
			xfree(packets[i].payload);
	}

	free(packets);

	for (i = 0; i < dlen; ++i) {
		free(packet_dyn[i].cnt);
		free(packet_dyn[i].rnd);
	}

	free(packet_dyn);
}

void compile_packets(char *file, bool verbose, unsigned int cpu,
		     bool invoke_cpp, char *const cpp_argv[])
{
	char tmp_file[128];
	int ret = -1;

	memset(tmp_file, 0, sizeof(tmp_file));
	our_cpu = cpu;

	if (invoke_cpp) {
		if (cpp_exec(file, tmp_file, sizeof(tmp_file), cpp_argv)) {
			fprintf(stderr, "Failed to invoke C preprocessor!\n");
			goto err;
		}
		file = tmp_file;
	}

	if (!strncmp("-", file, strlen("-")))
		yyin = stdin;
	else
		yyin = fopen(file, "r");
	if (!yyin) {
		fprintf(stderr, "Cannot open %s: %s!\n", file, strerror(errno));
		goto err;
	}

	realloc_packet();
	if (yyparse() != 0)
		goto err;
	finalize_packet();

	if (our_cpu == 0 && verbose)
		dump_conf();

	ret = 0;
err:
	if (yyin && yyin != stdin)
		fclose(yyin);

	if (invoke_cpp)
		unlink(tmp_file);
	if (ret)
		die();
}

void compile_packets_str(char *str, bool verbose, unsigned int cpu)
{
	int ret = 1;

	our_cpu = cpu;
	realloc_packet();

	yy_scan_string(str);
	if (yyparse() != 0)
		goto err;

	finalize_packet();
	if (our_cpu == 0 && verbose)
		dump_conf();

	ret = 0;
err:
	yylex_destroy();

	if (ret)
		die();
}

void yyerror(const char *err)
{
	fprintf(stderr, "Syntax error at line %d, char '%s': %s\n", yylineno, yytext, err);
}
