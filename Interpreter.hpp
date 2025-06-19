#pragma once

#include "Token.hpp"
#include "Expr.hpp"
#include "Debug.hpp"
#include "Stmt.hpp"
#include "Environment.hpp"

class Interpreter : public ExprVisitor, public Statement::StmtVisitor {
  public:
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    Interpreter();
    //void interpret(std::shared_ptr<Expr> expr);
    void interpret(std::vector<std::shared_ptr<Statement::Stmt>> &statements);
    void execute(std::shared_ptr<Statement::Stmt> statement);
    void executeBlock(const std::vector<std::shared_ptr<Statement::Stmt>> &statements, std::shared_ptr<Env> new_env);

    std::any visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<Statement::Print> stmt) override;   

    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;


    std::any visitVarStmt(std::shared_ptr<Statement::Var> stmt) override;
    std::any visitBlockStmt(std::shared_ptr<Statement::Block> stmt) override;

    std::shared_ptr<Env> global{new Env};

  private:
    void checkNumberOperand(const Token& oper, const std::any& operand);
    void checkNumberOperands(const Token& oper, const std::any& left, const std::any& right);
    bool isTruthy(const std::any& object);
    bool isEqual(const std::any& a, const std::any& b);
    std::string stringify(const std::any& object);
    std::any evaluate(std::shared_ptr<Expr> expr);

    std::unordered_map<std::shared_ptr<Expr>, int> locals;
    std::shared_ptr<Env> curr_env = global;
};

