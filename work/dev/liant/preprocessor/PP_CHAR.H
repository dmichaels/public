/* CPP pp_char.h - Source character set definitions */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  04.25.89  DGM  Added END_BUFFER_TOKEN.
 *  08.10.88  DGM  Original.
 *
 ***********************************************************************/

/* --------------------------------------------------------
 * Definitions for both source and execution character sets
 * ------------------------------------------------------ */

/* Lower case English alphabet characters */

#define LOWER_A_CHAR		'a'		/* ascii: 0x61 */
#define LOWER_B_CHAR		'b'		/* ascii: 0x62 */
#define LOWER_C_CHAR		'c'		/* ascii: 0x63 */
#define LOWER_D_CHAR		'd'		/* ascii: 0x64 */
#define LOWER_E_CHAR		'e'		/* ascii: 0x65 */
#define LOWER_F_CHAR		'f'		/* ascii: 0x66 */
#define LOWER_G_CHAR		'g'		/* ascii: 0x67 */
#define LOWER_H_CHAR		'h'		/* ascii: 0x68 */
#define LOWER_I_CHAR		'i'		/* ascii: 0x69 */
#define LOWER_J_CHAR		'j'		/* ascii: 0x6A */
#define LOWER_K_CHAR		'k'		/* ascii: 0x6B */
#define LOWER_L_CHAR		'l'		/* ascii: 0x6C */
#define LOWER_M_CHAR		'm'		/* ascii: 0x6D */
#define LOWER_N_CHAR		'n'		/* ascii: 0x6E */
#define LOWER_O_CHAR		'o'		/* ascii: 0x6F */
#define LOWER_P_CHAR		'p'		/* ascii: 0x70 */
#define LOWER_Q_CHAR		'q'		/* ascii: 0x71 */
#define LOWER_R_CHAR		'r'		/* ascii: 0x72 */
#define LOWER_S_CHAR		's'		/* ascii: 0x73 */
#define LOWER_T_CHAR		't'		/* ascii: 0x74 */
#define LOWER_U_CHAR		'u'		/* ascii: 0x75 */
#define LOWER_V_CHAR		'v'		/* ascii: 0x76 */
#define LOWER_W_CHAR		'w'		/* ascii: 0x77 */
#define LOWER_X_CHAR		'x'		/* ascii: 0x78 */
#define LOWER_Y_CHAR		'y'		/* ascii: 0x79 */
#define LOWER_Z_CHAR		'z'		/* ascii: 0x7A */

/* Upper case English alphabet characters */

#define UPPER_A_CHAR		'A'		/* ascii: 0x41 */
#define UPPER_B_CHAR		'B'		/* ascii: 0x42 */
#define UPPER_C_CHAR		'C'		/* ascii: 0x43 */
#define UPPER_D_CHAR		'D'		/* ascii: 0x44 */
#define UPPER_E_CHAR		'E'		/* ascii: 0x45 */
#define UPPER_F_CHAR		'F'		/* ascii: 0x46 */
#define UPPER_G_CHAR		'G'		/* ascii: 0x47 */
#define UPPER_H_CHAR		'H'		/* ascii: 0x48 */
#define UPPER_I_CHAR		'I'		/* ascii: 0x49 */
#define UPPER_J_CHAR		'J'		/* ascii: 0x4A */
#define UPPER_K_CHAR		'K'		/* ascii: 0x4B */
#define UPPER_L_CHAR		'L'		/* ascii: 0x4C */
#define UPPER_M_CHAR		'M'		/* ascii: 0x4D */
#define UPPER_N_CHAR		'N'		/* ascii: 0x4E */
#define UPPER_O_CHAR		'O'		/* ascii: 0x4F */
#define UPPER_P_CHAR		'P'		/* ascii: 0x50 */
#define UPPER_Q_CHAR		'Q'		/* ascii: 0x51 */
#define UPPER_R_CHAR		'R'		/* ascii: 0x52 */
#define UPPER_S_CHAR		'S'		/* ascii: 0x53 */
#define UPPER_T_CHAR		'T'		/* ascii: 0x54 */
#define UPPER_U_CHAR		'U'		/* ascii: 0x55 */
#define UPPER_V_CHAR		'V'		/* ascii: 0x56 */
#define UPPER_W_CHAR		'W'		/* ascii: 0x57 */
#define UPPER_X_CHAR		'X'		/* ascii: 0x58 */
#define UPPER_Y_CHAR		'Y'		/* ascii: 0x59 */
#define UPPER_Z_CHAR		'Z'		/* ascii: 0x5A */

/* Decimal digits */

#define ZERO_CHAR		'0'		/* ascii: 0x30 */
#define ONE_CHAR		'1'		/* ascii: 0x31 */
#define TWO_CHAR		'2'		/* ascii: 0x32 */
#define THREE_CHAR		'3'		/* ascii: 0x33 */
#define FOUR_CHAR		'4'		/* ascii: 0x34 */
#define FIVE_CHAR		'5'		/* ascii: 0x35 */
#define SIX_CHAR		'6'		/* ascii: 0x36 */
#define SEVEN_CHAR		'7'		/* ascii: 0x37 */
#define EIGHT_CHAR		'8'		/* ascii: 0x38 */
#define NINE_CHAR		'9'		/* ascii: 0x39 */

/* Other graphic characters */

#define BANG_CHAR		'!'		/* ascii: 0x21 */
#define DQUOTE_CHAR		'"'		/* ascii: 0x22 */
#define POUND_CHAR		'#'		/* ascii: 0x23 */
#define DOLLAR_CHAR		'$'		/* ascii: 0x24 */
#define PERCENT_CHAR		'%'		/* ascii: 0x25 */
#define AMPERSAND_CHAR		'&'		/* ascii: 0x26 */
#define SQUOTE_CHAR		'\''		/* ascii: 0x27 */
#define LPAREN_CHAR		'('		/* ascii: 0x28 */
#define RPAREN_CHAR		')'		/* ascii: 0x29 */
#define STAR_CHAR		'*'		/* ascii: 0x2A */
#define PLUS_CHAR		'+'		/* ascii: 0x2B */
#define COMMA_CHAR		','		/* ascii: 0x2C */
#define MINUS_CHAR		'-'		/* ascii: 0x2D */
#define DOT_CHAR		'.'		/* ascii: 0x2E */
#define SLASH_CHAR		'/'		/* ascii: 0x2F */
#define COLON_CHAR		':'		/* ascii: 0x3A */
#define SEMICOLON_CHAR		';'		/* ascii: 0x3B */
#define LANGLE_CHAR		'<'		/* ascii: 0x3C */
#define EQUAL_CHAR		'='		/* ascii: 0x3D */
#define RANGLE_CHAR		'>'		/* ascii: 0x3E */
#define QUESTION_CHAR		'?'		/* ascii: 0x3F */
#define LSQUARE_CHAR		'['		/* ascii: 0x5B */
#define BACKSLASH_CHAR		'\\'		/* ascii: 0x5C */
#define RSQUARE_CHAR		']'		/* ascii: 0x5D */
#define CARAT_CHAR		'^'		/* ascii: 0x5E */
#define UNDERSCORE_CHAR		'_'		/* ascii: 0x5F */
#define LCURLY_CHAR		'{'		/* ascii: 0x7B */
#define PIPE_CHAR		'|'		/* ascii: 0x7C */
#define RCURLY_CHAR		'}'		/* ascii: 0x7D */
#define TILDE_CHAR		'~'		/* ascii: 0x7E */

/* Non-graphic characters */

#define NEWLINE_CHAR		'\n'		/* ascii: 0x0A */
#define SPACE_CHAR		' '		/* ascii: 0x20 */
#define HORIZONTAL_TAB_CHAR	'\t'		/* ascii: 0x09 */
#define VERTICAL_TAB_CHAR	'\v'		/* ascii: 0x0B */
#define FORMFEED_CHAR		'\f'		/* ascii: 0x0C */

/* --------------------------------------------------------
 * Other execution character set only character definitions
 * ------------------------------------------------------ */

#define ALERT_CHAR		(0x07)		/* CTRL-G bell */
#define BACKSPACE_CHAR		'\b'		/* ascii: 0x08 */
#define RETURN_CHAR		'\r'		/* ascii: 0x0D */
#define NULL_CHAR		'\0'		/* ascii: 0x00 */

/* --------------------------------
 * Special (pseudo) character codes
 * ------------------------------ */

#define BEGIN_PP_DIRECTIVE_CHAR	  (-1)	/* Begin preprocessing directive */
#define END_PP_DIRECTIVE_CHAR	  (-2)	/* End preprocessing directive */
#define WHITE_SPACE_CHAR	  (-3)	/* White-space indicator */
#define GRAY_SPACE_CHAR		  (-4)	/* Gray-space indicator */
#define END_BUFFER_CHAR		  (-5)	/* End of internal buffer */

#define EOF_CHAR		(-101)	/* End-of-file (kosher) */
#define EOF_IN_LINE_CHAR	(-102)	/* EOF in logical line */
#define EOF_IN_COMMENT_CHAR	(-103)	/* EOF in comment */
#define EOF_IN_DIRECTIVE_CHAR	(-104)	/* EOF in directive line */
#define EOF_IN_ARGUMENT_CHAR	(-105)	/* EOF in macro function argument */
#define EOF_IN_SKIP_GROUP_CHAR	(-106)	/* EOF in excluded group */
#define ERROR_CHAR		(-107)	/* Read error */

