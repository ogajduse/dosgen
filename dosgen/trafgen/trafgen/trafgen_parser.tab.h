/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 343 "trafgen_parser.y" /* yacc.c:1909  */

	struct in_addr ip4_addr;
	long long int number;
	uint8_t bytes[256];
	char *str;

#line 140 "trafgen/trafgen_parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TRAFGEN_TRAFGEN_PARSER_TAB_H_INCLUDED  */
