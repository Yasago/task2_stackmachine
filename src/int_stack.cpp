////////////////////////////////////////////////////////////////////////////////
// Module Name:  int_stack.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures" 
// provided by  the School of Software Engineering of the Faculty 
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////


#include "int_stack.h"

// Added necessary header
#include <stdexcept>

namespace xi {


//==============================================================================
// class IntStack
//==============================================================================


IntStack::IntStack(size_t sz)
{
    _stack = new int[sz];
    _ssize = sz;
    _head = -1;
}

IntStack::~IntStack()
{
    delete[] _stack;
    _stack = nullptr;
    _ssize = 0;
    _head = -1;
}

void IntStack::push(int el)
{
    if (isFull())
        throw std::logic_error("No more elements can be placed onto the stack due to its overflow");

    _stack[++_head] = el;
}

int IntStack::pop()
{
    if (isEmpty())
        throw std::logic_error("No elements stored in the stack");

    return _stack[_head--];
}

int IntStack::top()
{
    if (isEmpty())
        throw std::logic_error("No elements stored in the stack");

    return _stack[_head];
}

void IntStack::clear()
{
    _head = -1;
}

bool IntStack::isEmpty() const
{
    return _head == -1;
}

bool IntStack::isFull() const
{
    return _head == _ssize - 1;
}

} // namespace xi

