#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Tokenizer
//

typedef enum {
  TK_NUM,
  TK_RESERVED,
  TK_EOF
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  char *str;
  int val;
};

Token *token;
char *user_input;

// error
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// error_at
void error_at(char *loc, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = loc - user_input;

  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, " ");
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
}

// consume
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    return false;

  token = token->next;
  return true;
}

// expect
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "expected '%c'", op);
  
  token = token->next;
}

// expect_number
int expect_number() {
  if (token->kind != TK_NUM)
    error_at(token->str, "expected a number");

  int val = token->val;
  token = token->next;
  return val;
}

// at_eof
bool at_eof() {
  return token->kind != TK_EOF;
}

// new_token
Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

// tokenize
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (strchr("+-*/()", *p)) {
      cur = new_token(TK_RESERVED, cur, p);
      p++;
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "invalid token");
  }
  cur = new_token(TK_EOF, cur, p);
  
  return head.next;
}

// 
//  Parser
//

typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_NUM
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

// new_node

// new_binary

// new_num

// expr

// mul

// primary

// gen


// main
int main(int argc, char **argv)
{
  if (argc != 2) {
    error("%s: invalid number of arguments\n" , argv[0]);
  }

  // " 12 + 34 - 5 "
  user_input = argv[1];
  token = tokenize(user_input);
  Node *node = expr();
  // "5+7"
  // 5のnode生成 -> +の判定 -> 7のnode生成 -> +のnode生成 & リンクリストの連結(5と+と7)
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  gen(node);

  printf("  pop rax\n");
  printf("  ret\n");

  return 0;
}