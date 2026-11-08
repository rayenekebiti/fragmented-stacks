#include <iostream>
#include "stack.hpp"

STACK::STACK(int size):capacity(size)
{
    if(capacity<=0)
    {
        throw std::out_of_range("stack size can't be less or equal 0");
    }
    BLOCK=new STACK_BLOCK{};
    STACK::active_stack=BLOCK;
    active_stack->arr=new int[capacity];
    BLOCK->stack_number=1;
}
void STACK::push(const int data)
{
 if(STACK::capacity>STACK::active_stack->top){
    STACK::active_stack->arr[active_stack->top]=data;
    STACK::active_stack->top+=1;
 }
 else
 {
    STACK_BLOCK* new_stack=new STACK_BLOCK{};
    new_stack->arr=new int[STACK::capacity];
    new_stack->prev=STACK::active_stack;
    STACK::active_stack=new_stack;
    new_stack->stack_number=STACK::active_stack->prev->stack_number+1;
    STACK::push(data);
 }
}
int STACK::pop(){
if(STACK::active_stack->top==0&&active_stack->prev==nullptr){
throw std::runtime_error("can't pop into empty stack");
}
else if(active_stack->top==0){
 STACK_BLOCK* temp= STACK::active_stack->prev;
 delete[] STACK::active_stack->arr;
 active_stack->arr=nullptr;
 delete STACK::active_stack;
 STACK::active_stack=temp;
 temp->top-=1;
 int value=temp->arr[temp->top];
 temp->arr[temp->top]=0;
 return value;
}
else
{
    STACK::active_stack->top-=1;
    int value=STACK::active_stack->arr[active_stack->top];
    STACK::active_stack->arr[active_stack->top]=0;
    return value;
}   
}
STACK::~STACK()
{
    STACK_BLOCK* temp;
    while(STACK::active_stack!=nullptr){     
    temp= STACK::active_stack;
    STACK::active_stack=STACK::active_stack->prev;
    delete[] temp->arr;
    temp->arr=nullptr;
    delete temp;
    }
    BLOCK=nullptr;
}



