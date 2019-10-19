////////////////////////////////////////////////////////////////////////////////
// Module Name:  stack_machine.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures" 
// provided by  the School of Software Engineering of the Faculty 
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

#include "stack_machine.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

namespace xi {

//==============================================================================
// Free functions -- helpers
//==============================================================================


bool isNumber(std::string s)
{
    int sz = s.size();
    for (int i = 0; i < sz; ++i)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
    }

    return true;
}

int convertToInt(std::string s)
{
    int res = 0;
    int sz = s.size();
    for (int i = 0; i < sz; ++i)
    {
        res *= 10;
        res += s[i] - '0';
    }

    return res;
}


//==============================================================================
// class PlusOp
//==============================================================================


int PlusOp::operation(char op, int a, int b, int /*c*/) // < just commented unused argument. This does not affect the code anyhow.
{
    if(op != '+')
        throw std::logic_error("Operation other than Plus (+) are not supported");

    // here we just ignore unused operands
    return a + b;
}

IOperation::Arity PlusOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class MinusOp
//==============================================================================


int MinusOp::operation(char op, int a, int b, int /*c*/)
{
    if(op != '-')
        throw std::logic_error("Operation other than Minus (-) are not supported");

    return a - b;
}

IOperation::Arity MinusOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class ChoiceOp
//==============================================================================


int ChoiceOp::operation(char op, int a, int b, int c)
{
    if(op != '?')
        throw std::logic_error("Operation other than Choice (?) are not supported");

    if (a != 0)
        return b;

    return c;
}

IOperation::Arity ChoiceOp::getArity() const
{
    return arTre;
}


//==============================================================================
// class AndOp
//==============================================================================


int AndOp::operation(char op, int a, int b, int /*c*/)
{
    if (op != '&')
        throw std::logic_error("Operation other than And (&) are not supported");

    return a & b;
}

IOperation::Arity AndOp::getArity() const
{
    return arDue;
}


//==============================================================================
// class StackMachine
//==============================================================================


void StackMachine::registerOperation(char symb, IOperation* oper)
{
    SymbolToOperMapConstIter it = _opers.find(symb);
    if(it != _opers.end())
    {
        std::string s1 = "An operation ";
        s1 += symb;
        s1 += " is already registered";
        throw std::logic_error(s1);
    }

    _opers[symb] = oper;
}

IOperation* StackMachine::getOperation(char symb)
{
    SymbolToOperMapConstIter it = _opers.find(symb);
    if(it == _opers.end())
    {
        std::string s1 = "An operation ";
        s1 += symb;
        s1 += " is not registered";
        throw std::logic_error(s1);
    }

    return it -> second;
}

int StackMachine::calculate(const std::string &expr, bool clearStack)
{
    if (clearStack)
        getStack().clear();

    std::vector<std::string> vector;
    int sz = expr.size();
    int i = 0;
    while (i < sz)
    {
        std::string res;
        while (i < sz && expr[i] != ' ')
        {
            res += expr[i];
            ++i;
        }

        if (!res.empty())
            vector.push_back(res);

        ++i;
    }

    sz = vector.size();
    for (i = 0; i < sz; ++i)
    {
        if (isNumber(vector[i]))
            _s.push(convertToInt(vector[i]));
        else
        {
            int opSize = vector[i].size();
            if (opSize > 1)
                throw std::logic_error("An operation is not registered");

            char symb = vector[i][0];
            IOperation* oper = getOperation(symb);
            switch (oper -> getArity())
            {
                case IOperation::Arity::arUno:
                    int a;
                    a = _s.pop();
                    _s.push(oper -> operation(symb, a));
                    break;

                case IOperation::Arity::arDue:
                    a = _s.pop();
                    int b;
                    b = _s.pop();
                    _s.push(oper -> operation(symb, b, a));
                    break;

                case IOperation::arTre:
                    a = _s.pop();
                    b = _s.pop();
                    int c;
                    c = _s.pop();
                    _s.push(oper -> operation(symb, c, b, a));
                    break;
            }
        }
    }

    return getStack().top();
}

} // namespace xi
