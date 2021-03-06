#ifndef LEXER_H
#define LEXER_H

#include "../include.h"

struct LEXER{
	file_p file;
	char current;
	bool processed;
};

enum tok_type_t{
	TERMINAL,
	NON_TERMINAL,
	SEMI_COLON,
	COLON,
	OR,
	END,
	UNIDENTIFIED_TOK
};

struct TOKEN{
	char* value;
	enum tok_type_t type;
	size_t lineNum;
	size_t characterNum;
};

typedef struct LEXER* lexer_p;
typedef struct TOKEN* tok_p;

lexer_p init_lexer(file_p f);
void delLexer(lexer_p lex);

tok_p getTok(char* value, enum tok_type_t type);
tok_p dupTok(tok_p token);
void delTok(tok_p token);

void pushTok(tok_p tok);
tok_p popTok();

tok_p lexer_next_token(lexer_p lex);


#endif