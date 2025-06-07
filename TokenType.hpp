enum class TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  //
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // LITERALS
  IDENTIFIER,
  STRING,
  NUMBER,

  // KEYWORDS
  AND,
  CLASS,
  ELSE,
  // adicionados, pois algum OS pode não compreender o false/true
  MY_FALSE,
  MY_TRUE,
  //
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  VAR,
  WHILE,

  // eof
  MY_EOF
};
