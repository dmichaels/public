-- ANSI-C lextab.lex - Lexical analyzer (FSM); LEXTABLE input file --

------------------------------------------------------------------------
-- This product is the property of Language Processors, Inc. and is    -
-- licensed pursuant to a written license agreement.  No portion of    -
-- this product may be reproduced without the written permission of    -
-- Language Processors, Inc. except pursuant to the license agreement. -
------------------------------------------------------------------------

------------------------------------------------------------------------
--
--  LPI EDIT HISTORY
--
--  11.21.89  DGM  Added proper support for the C++ pointer-to-member
--                 operators ".*" and "->*".
--
--  08.29.89  DGM  Added C++ support.  Just needed to add the the scope
--                 operator token "::".  Each of the class member
--                 operators ".*" and "->*" are each handled as separate
--                 tokens; although I can't find any any documentation,
--                 they each seem to be simply two operators which can
--                 be used together, rather than a new operator.
--                 Q: Why is "question" a class & not a "simple_token"?
--
--  03.10.89  DGM  Fixed bug which caused "1E++" to be lexed as "1E+"
--                 (FLOAT_CONSTANT -- wrong) and "+"; it should be
--                 lexed as "1E+" (PP_NUMBER) and "+".
--
--  01.16.89  DGM  Fixed bug (typo) in "maybe_identifier" state which.
--                 caused an identifer beginning with an "L" followed
--                 by a digit to be lexed as separate tokens.
--
--  08.18.88  DGM  Totally redone for ANSI-C.
--                 Also, to improve speed efficiency, modified the
--                 LEXTABLE utility to produce a (larger) direct access
--                 table (NEXT_STATE) when the #DIRECT_ACCESS_TABLE
--                 directive is specified in this input file.
--
------------------------------------------------------------------------

------------------------------------------------------------------------
#DIRECT_ACCESS_TABLE
------------------------------------------------------------------------

------------------------------------------------------------------------
#STATES
------------------------------------------------------------------------

------------------------------------------------------------
-- The initial state (must be fully specified!)
------------------------------------------------------------

initial:
                @@              error
                @backslash      error

                @newline        *

                @other_letter   +identifier
                @hex_letter     +identifier
                @e_letter       +identifier
                @f_letter       +identifier
                @l_up_letter    +maybe_identifier
                @l_lo_letter    +identifier
                @u_letter       +identifier
                @x_letter       +identifier
                @dollar_letter  +check_dollar

                @dec_digit      +decimal_constant
                @oct_digit      +decimal_constant
                @zero_digit     +octal_constant
                @dot            +dot

                @squote         +char_constant
                @dquote         +string_literal

                @simple_token   simple_token
                @question       simple_token

                @plus           plus
                @minus          minus
                @star           star
                @slash          slash
                @mod            mod
                @and            and
                @or             or
                @xor            xor
                @greater        greater
                @less           less
                @equal          equal
                @not            not
                @colon          colon

                @pp_stringize   pp_stringize

------------------------------------------------------------
-- Identifiers
--
-- The "maybe" state is necessary because when an "L" is
-- found, it could be a wide-string-literal or an identifier.
------------------------------------------------------------

identifier:                     (IDENTIFIER)
                @other_letter   +*
                @hex_letter     +*
                @e_letter       +*
                @f_letter       +*
                @l_lo_letter    +*
                @l_up_letter    +*
                @u_letter       +*
                @x_letter       +*
                @dec_digit      +*
                @oct_digit      +*
                @zero_digit     +*
                @dollar_letter  +check_dollar
                @@              past_end_of_identifier

maybe_identifier:               (IDENTIFIER)
                @other_letter   +identifier
                @hex_letter     +identifier
                @e_letter       +identifier
                @f_letter       +identifier
                @l_lo_letter    +identifier
                @l_up_letter    +identifier
                @u_letter       +identifier
                @x_letter       +identifier
                @dec_digit      +identifier
                @oct_digit      +identifier
                @zero_digit     +identifier
                @dollar_letter  +check_dollar
                @dquote         +string_literal
                @squote         +char_constant
                @@              past_end_of_identifier

------------------------------------------------------------
-- Octal integer constants
--
-- Note that 0 is lexed as an OCTAL_CONSTANT_TOKEN.
------------------------------------------------------------

octal_constant:                 (OCTAL_CONSTANT)
                @x_letter       +hex_prefix
                @oct_digit      +*
                @dec_digit      +pp_number_or_float
                @zero_digit     +*
                @dot            +float_fraction
                @e_letter       +float_e
                @l_up_letter    +octal_long_suffix
                @l_lo_letter    +octal_long_suffix
                @u_letter       +octal_unsigned_suffix
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @f_letter       +pp_number
                @@              past_end_of_token

octal_long_suffix:              (OCTAL_CONSTANT)
                @u_letter       +octal_unsigned_suffix
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @f_letter       +pp_number
                @x_letter       +pp_number
                @l_up_letter    +pp_number
                @l_lo_letter    +pp_number
                @dot            +pp_number
                @@              past_end_of_token

octal_unsigned_suffix:          (OCTAL_CONSTANT)
                @l_up_letter    +octal_long_suffix
                @l_lo_letter    +octal_long_suffix
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @f_letter       +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @dot            +pp_number
                @@              past_end_of_token

pp_number_or_float:             (PP_NUMBER)
                @oct_digit      +*
                @dec_digit      +*
                @zero_digit     +*
                @e_letter       +float_fraction
                @dot            +float_fraction
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @f_letter       +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @l_up_letter    +pp_number
                @l_lo_letter    +pp_number
                @@              past_end_of_token

------------------------------------------------------------
-- Decimal integer constants
------------------------------------------------------------

decimal_constant:               (DECIMAL_CONSTANT)
                @oct_digit      +*
                @dec_digit      +*
                @zero_digit     +*
                @dot            +float_fraction
                @e_letter       +float_e
                @l_up_letter    +decimal_long_suffix
                @l_lo_letter    +decimal_long_suffix
                @u_letter       +decimal_unsigned_suffix
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @f_letter       +pp_number
                @x_letter       +pp_number
                @@              past_end_of_token

decimal_long_suffix:            (DECIMAL_CONSTANT)
                @u_letter       +decimal_unsigned_suffix
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @f_letter       +pp_number
                @x_letter       +pp_number
                @l_up_letter    +pp_number
                @l_lo_letter    +pp_number
                @dot            +pp_number
                @@              past_end_of_token

decimal_unsigned_suffix:         (DECIMAL_CONSTANT)
                @l_up_letter    +decimal_long_suffix
                @l_lo_letter    +decimal_long_suffix
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @f_letter       +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @dot            +pp_number
                @@              past_end_of_token

------------------------------------------------------------
-- Hex integer constants
------------------------------------------------------------

hex_prefix:                     (PP_NUMBER)
                @oct_digit      +hex_constant
                @dec_digit      +hex_constant
                @zero_digit     +hex_constant
                @hex_letter     +hex_constant
                @e_letter       +hex_constant_e
                @f_letter       +hex_constant
                @other_letter   +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @l_up_letter    +pp_number
                @l_lo_letter    +pp_number
                @dot            +pp_number
                @@              past_end_of_token

hex_constant:                   (HEX_CONSTANT)
                @oct_digit      +*
                @dec_digit      +*
                @zero_digit     +*
                @hex_letter     +*
                @e_letter       +hex_constant_e
                @f_letter       +*
                @l_up_letter    +hex_long_suffix
                @l_lo_letter    +hex_long_suffix
                @u_letter       +hex_unsigned_suffix
                @other_letter   +pp_number
                @x_letter       +pp_number
                @dot            +pp_number
                @@              past_end_of_token

hex_constant_e:                 (HEX_CONSTANT)
                @oct_digit      +hex_constant
                @dec_digit      +hex_constant
                @zero_digit     +hex_constant
                @hex_letter     +hex_constant
                @e_letter       +hex_constant
                @f_letter       +hex_constant
                @l_up_letter    +hex_long_suffix
                @l_lo_letter    +hex_long_suffix
                @u_letter       +hex_unsigned_suffix
                @other_letter   +pp_number
                @x_letter       +pp_number
                @dot            +pp_number
                @plus           +pp_number
                @minus          +pp_number
                @@              past_end_of_token
        
hex_long_suffix:                (HEX_CONSTANT)
                @u_letter       +hex_unsigned_suffix
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @f_letter       +pp_number
                @x_letter       +pp_number
                @l_up_letter    +pp_number
                @l_lo_letter    +pp_number
                @dot            +pp_number
                @@              past_end_of_token

hex_unsigned_suffix:            (HEX_CONSTANT)
                @l_up_letter    +hex_long_suffix
                @l_lo_letter    +hex_long_suffix
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @f_letter       +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @dot            +pp_number
                @@              past_end_of_token

------------------------------------------------------------
-- Floating constants
------------------------------------------------------------

float_fraction:                 (FLOAT_CONSTANT)
                @oct_digit      +*
                @dec_digit      +*
                @zero_digit     +*
                @e_letter       +float_e
                @f_letter       +float_suffix
                @l_up_letter    +float_suffix
                @l_lo_letter    +float_suffix
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @@              past_end_of_token

float_sign:                     (PP_NUMBER)
                @oct_digit      +float_exponent
                @dec_digit      +float_exponent
                @zero_digit     +float_exponent
                @@              past_end_of_token

float_e:
                @plus           +float_sign
                @minus          +float_sign

float_exponent:                 (FLOAT_CONSTANT)
                @oct_digit      +*
                @dec_digit      +*
                @zero_digit     +*
                @f_letter       +float_suffix
                @l_up_letter    +float_suffix
                @l_lo_letter    +float_suffix
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @@              past_end_of_token

float_suffix:                   (FLOAT_CONSTANT)
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number_e
                @u_letter       +pp_number
                @x_letter       +pp_number
                @dot            +pp_number
                @@              past_end_of_token

------------------------------------------------------------
-- Preprocessing numbers
------------------------------------------------------------

pp_number:                      (PP_NUMBER)
                @oct_digit      +*
                @dec_digit      +*
                @zero_digit     +*
                @other_letter   +*
                @hex_letter     +*
                @e_letter       +pp_number_e
                @f_letter       +*
                @u_letter       +*
                @x_letter       +*
                @l_up_letter    +*
                @l_lo_letter    +*
                @dot            +*
                @@              past_end_of_token

pp_number_e:                    (PP_NUMBER)
                @plus           +pp_number
                @minus          +pp_number
                @oct_digit      +pp_number
                @dec_digit      +pp_number
                @zero_digit     +pp_number
                @other_letter   +pp_number
                @hex_letter     +pp_number
                @e_letter       +pp_number
                @f_letter       +pp_number
                @u_letter       +pp_number
                @x_letter       +pp_number
                @l_up_letter    +pp_number
                @l_lo_letter    +pp_number
                @dot            +pp_number
                @@              past_end_of_token

------------------------------------------------------------
-- Puncuator and operators
------------------------------------------------------------

dot:                            (DOT)
                @dot            dot_dot
                @oct_digit      +float_fraction
                @dec_digit      +float_fraction
                @zero_digit     +float_fraction
                @@              past_end_of_token

dot_dot:
                @dot            ellipsis (ELLIPSIS)
                @star           dot_star (DOT_STAR)
                @@              double_dot

plus:                           (PLUS)
                @plus           plus_plus (PLUS_PLUS)
                @equal          plus_equal (PLUS_EQUAL)
                @@              past_end_of_token

minus:                          (MINUS)
                @minus          minus_minus (MINUS_MINUS)
                @equal          minus_equal (MINUS_EQUAL)
                @greater        pointer
                @@              past_end_of_token

pointer:                        (POINTER)
                @star           pointer_star (POINTER_STAR)
                @@              past_end_of_token

star:                           (STAR)
                @equal          star_equal (STAR_EQUAL)
                @@              past_end_of_token

slash:                          (SLASH)
                @equal          slash_equal (SLASH_EQUAL)
                @@              past_end_of_token

mod:                            (MOD)
                @equal          mod_equal (MOD_EQUAL)
                @@              past_end_of_token

and:                            (AND)
                @and            and_and (AND_AND)
                @equal          and_equal (AND_EQUAL)
                @@              past_end_of_token

or:                             (OR)
                @or             or_or (OR_OR)
                @equal          or_equal (OR_EQUAL)
                @@              past_end_of_token

xor:                            (XOR)
                @equal          xor_equal (XOR_EQUAL)
                @@              past_end_of_token

less:                           (LESS_THAN)
                @equal          less_equal (LESS_EQUAL)
                @less           less_less
                @@              past_end_of_token

less_less:                      (LSHIFT)
                @equal          lshift_equal (LSHIFT_EQUAL)
                @@              past_end_of_token

greater:                        (GREATER_THAN)
                @equal          greater_equal (GREATER_EQUAL)
                @greater        greater_greater
                @@              past_end_of_token

greater_greater:                (RSHIFT)
                @equal          rshift_equal (RSHIFT_EQUAL)
                @@              past_end_of_token

equal:                          (EQUAL)
                @equal          equal_equal (EQUAL_EQUAL)
                @@              past_end_of_token

not:                            (NOT)
                @equal          not_equal (NOT_EQUAL)
                @@              past_end_of_token

pp_stringize:                   (PP_STRINGIZE)
                @pp_stringize   pp_token_paste (PP_TOKEN_PASTE)
                @@              past_end_of_token

colon:                          (COLON)
                @colon          colon_colon (COLON_COLON)
                @@              past_end_of_token

------------------------------------------------------------
-- String literal
------------------------------------------------------------

string_literal:                 (STRING_LITERAL)
                @backslash      +string_literal_escape
                @newline        +illegal_string_literal
                @dquote         +end_of_string_literal
                @@              +*

string_literal_escape:
                @dquote         +string_literal
                @@              +string_literal

------------------------------------------------------------
-- Character constant
------------------------------------------------------------

char_constant:                  (CHAR_CONSTANT)
                @backslash      +char_constant_escape
                @newline        +illegal_char_constant
                @squote         +end_of_char_constant
                @@              +*

char_constant_escape:
                @squote         +char_constant
                @@              +char_constant

------------------------------------------------------------------------
#CLASSES
------------------------------------------------------------------------

newline:
                012             -- newline

other_letter:
                g h i j k m n o p q r s t v w y z
                G H I J K M N O P Q R S T V W Y Z
                _
hex_letter:
                a b c d
                A B C D
e_letter:
                e E
f_letter:
                f F
u_letter:
                u U
x_letter:
                x X
l_up_letter:
                L
l_lo_letter:
                l
dollar_letter:
                $               -- A letter when "-dollar" specified

zero_digit:
                0

oct_digit:
                1 2 3 4 5 6 7

dec_digit:
                8 9

simple_token:                   -- These are always single-byte tokens

                (  )            -- parentheses
                [  ]            -- square brackets
                {  }            -- curly brackets
                ~               -- tilde
                ,               -- comma
                ;               -- semicolon

pp_stringize:
                043             -- # (preprocessor directive)

plus:
                +
minus:
                -
star:
                *
slash:
                /
mod:
                %
and:
                &
or:
                |
xor:
                ^
greater:
                >
less:
                <
equal:
                =
not:
                !
dot:
                .
squote:
                '
dquote:
                "
question:
                ?
colon:
                :
backslash:
                \

------------------------------------------------------------------------
#BYTES
------------------------------------------------------------------------

        .       = dot
        ,       = comma
        :       = colon
        ;       = semicolon
        ?       = question
        (       = lparen
        )       = rparen
        [       = lsquare
        ]       = rsquare
        {       = lcurly
        }       = rcurly
        %       = mod
        *       = star
        /       = slash
        !       = not
        ~       = tilde
        +       = plus
        -       = minus
        <       = less_than
        >       = greater_than
        =       = equal
        &       = and
        |       = or
        ^       = xor
        043     = pp_stringize      -- # (preprocessing directive)

