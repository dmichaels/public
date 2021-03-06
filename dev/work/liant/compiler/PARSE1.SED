s/^[ ]*PAR_AC[ ]*([ ]*\(.*\)[ ]*)[ ]*:[ ]*CALL[ ]*\([a-zA-Z_][a-zA-Z0-9_]*\)[ ]*;[ ]*GOTO[ ]*PARSE_RET[ ]*;.*$/    PAR_ACTION (\1) = \2;/
s/^[ ]*PAR_AC[ ]*([ ]*\(.*\)[ ]*)[ ]*:[ ]*\(.*\)[ ]*$/    PAR_ACTION (\1) = PAR_ACTION_\1;/
