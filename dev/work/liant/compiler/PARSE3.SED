/^[ ]*PAR_AC[ ]*([ ]*\(.*\)[ ]*)[ ]*:[ ]*CALL[ ]*\([a-zA-Z_][a-zA-Z0-9_]*\)[ ]*;[ ]*GOTO[ ]*PARSE_RET[ ]*;.*$/d
s/^[ ]*PAR_AC[ ]*([ ]*\(.*\)[ ]*)[ ]*:[ ]*\(.*\)GOTO[ ]*PARSE_RET_TRUE[ ]*;[ ]*GOTO[ ]*PARSE_RET[ ]*;[ ]*$/PAR_ACTION_\1: PROC INTERNAL; \2 PAR_FOUND = TRUE; END;/
s/^[ ]*PAR_AC[ ]*([ ]*\(.*\)[ ]*)[ ]*:[ ]*\(.*\)GOTO[ ]*PARSE_RET[ ]*;[ ]*$/PAR_ACTION_\1: PROC INTERNAL; \2 END;/
s/^[ ]*PAR_AC[ ]*([ ]*\(.*\)[ ]*)[ ]*:[ ]*\(.*\)[ ]*$/PAR_ACTION_\1: PROC INTERNAL; \2/
s/^\(.*\)GOTO[ ]*PARSE_RET_TRUE[ ]*;[ ]*GOTO[ ]*PARSE_RET[ ]*;[ ]*$/\1 PAR_FOUND = TRUE; END;/
s/^\(.*\)GOTO[ ]*PARSE_RET[ ]*;[ ]*/\1 END;/
