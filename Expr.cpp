#include "Expr.hpp"

Binary::Binary(std::shared_ptr<Expr> left, Token oper,
               std::shared_ptr<Expr> right)
    : left{std::move(left)}, oper{std::move(oper)}, right{std::move(right)} {}

std::any Binary::accept(ExprVisitor &visitor) {
  return visitor.visitBinaryExpr(shared_from_this());
}

Grouping::Grouping(std::shared_ptr<Expr> expression)
    : expression{std::move(expression)} {}

std::any Grouping::accept(ExprVisitor &visitor) {
  return visitor.visitGroupingExpr(shared_from_this());
}

Literal::Literal(std::any value) : value{std::move(value)} {}

std::any Literal::accept(ExprVisitor &visitor) {
  return visitor.visitLiteralExpr(shared_from_this());
}

Unary::Unary(Token oper, std::shared_ptr<Expr> right)
    : oper{std::move(oper)}, right{std::move(right)} {}

std::any Unary::accept(ExprVisitor &visitor) {
  return visitor.visitUnaryExpr(shared_from_this());
}

Variable::Variable(Token name) : name(name) {}

std::any Variable::accept(ExprVisitor &visitor) {
  return visitor.visitVariableExpr(shared_from_this());
}

Assign::Assign(Token name, std::shared_ptr<Expr> value)
    : name{std::move(name)}, value{std::move(value)} {}

std::any Assign::accept(ExprVisitor &visitor) {
  return visitor.visitAssignExpr(shared_from_this());
}

Logical::Logical(std::shared_ptr<Expr> left, Token oper,
                 std::shared_ptr<Expr> right)
    : left{std::move(left)}, oper{std::move(oper)}, right{std::move(right)} {}

std::any Logical::accept(ExprVisitor &visitor) {
  return visitor.visitLogicalExpr(shared_from_this());
}

Call::Call(std::shared_ptr<Expr> callee, Token paren,
           std::vector<std::shared_ptr<Expr>> arguments)
    : callee{std::move(callee)}, paren{std::move(paren)},
      arguments{std::move(arguments)} {}

std::any Call::accept(ExprVisitor &visitor) {
  return visitor.visitCallExpr(shared_from_this());
}
