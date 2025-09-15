#pragma once
#include <functional>
#include <memory>
#include <vector>
#include <stdexcept>

class Boolean_Function
{
public:

    enum class Type
    {
        NONE,
        VAR,
        FUNC,

        VAR_VAR,
        VAR_FUNC,
        FUNC_FUNC,

        TAUT,
        CONT
    };

    enum class Operation
    {
        NONE,
        AND,
        OR,
        NOT
    };

    using variable_repr_T = int;

    Boolean_Function(bool binary) { if (binary) type = Type::TAUT; else type = Type::CONT;};
    Boolean_Function(Operation operation, variable_repr_T term) : type(Type::VAR), operation(operation), var1(term)
    {
        if (operation != Operation::NOT)
            throw std::runtime_error("Boolean function with single term must have operation::NOT");
    }
    Boolean_Function(Operation operation, std::shared_ptr<Boolean_Function> term) : type(Type::FUNC), operation(operation), func1(term)
    {
        if (operation != Operation::NOT)
            throw std::runtime_error("Boolean function with single term must have operation::NOT");
    }
    Boolean_Function(variable_repr_T term1, Operation operation, variable_repr_T term2) : type(Type::VAR_VAR), operation(operation), var1(term1), var2(term2) {}
    Boolean_Function(variable_repr_T term1, Operation operation, std::shared_ptr<Boolean_Function> term2) : type(Type::VAR_FUNC), operation(operation), var1(term1), func1(term2) {}
    Boolean_Function(std::shared_ptr<Boolean_Function> term1, Operation operation, variable_repr_T term2) : type(Type::VAR_FUNC), operation(operation), var1(term2), func1(term1) {}
    Boolean_Function(std::shared_ptr<Boolean_Function> term1, Operation operation, std::shared_ptr<Boolean_Function> term2) : type(Type::FUNC_FUNC), operation(operation), func1(term1), func2(term2) {}

    bool eval(std::function<bool(variable_repr_T)> eval_variable)
    {

        switch (type)
        {
        case Type::TAUT:
            return true;
        case Type::CONT:
            return false;
        case Type::VAR:
            return !eval_variable(var1);
        case Type::FUNC:
            return !func1->eval(eval_variable);
        case Type::VAR_VAR:
            switch (operation)
            {
            case Operation::AND:
                return eval_variable(var1) && eval_variable(var2);
            case Operation::OR:
                return eval_variable(var1) || eval_variable(var2);
            default:
                throw std::runtime_error("Bad Operation Type");
            }
        case Type::VAR_FUNC:
            switch (operation)
            {
            case Operation::AND:
                return eval_variable(var1) && func1->eval(eval_variable);
            case Operation::OR:
                return eval_variable(var1) || func1->eval(eval_variable);
            default:
                throw std::runtime_error("Bad Operation Type");
            }
        case Type::FUNC_FUNC:
            switch (operation)
            {
            case Operation::AND:
                return  func2->eval(eval_variable) && func1->eval(eval_variable);
            case Operation::OR:
                return  func2->eval(eval_variable) || func1->eval(eval_variable);
            default:
                throw std::runtime_error("Bad Operation Type");
            }    
        default:
            throw std::runtime_error("Unrecognized Function Type");
        }
    };

private:
    Type type{ Type::NONE };
    Operation operation{ Operation::NONE };

    [[maybe_unused]] variable_repr_T var1{};
    [[maybe_unused]] variable_repr_T var2{};

    [[maybe_unused]] std::shared_ptr < Boolean_Function > func1{ NULL };
    [[maybe_unused]] std::shared_ptr < Boolean_Function > func2{ NULL };
};
