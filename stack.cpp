#include <iostream>
#include "stack.hpp"

STACK::STACK(int size):capacity(size)
{
    BLOCK=new STACK_BLOCK{};
    STACK::active_stack=BLOCK;
    active_stack->arr=new int[capacity];
    BLOCK->stack_number=1;
}
void STACK::push(const int data)
{
 if(STACK::capacity>STACK::active_stack->top){
    STACK::active_stack->arr[active_stack->top]=data;
    if(STACK::active_stack->top<STACK::capacity)
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
 int value=temp->arr[temp->top-1];
 temp->arr[temp->top]=0;
 temp->top-=1;
 return value;
}
else
{
    int value=STACK::active_stack->arr[active_stack->top-1];
    STACK::active_stack->arr[active_stack->top]=0;
    STACK::active_stack->top-=1;
    return value;
}   
}
STACK::~STACK()
{
    STACK_BLOCK* temp;
    while(STACK::active_stack->prev!=BLOCK){     
    temp= STACK::active_stack;
    STACK::active_stack=STACK::active_stack->prev;
    delete[] temp->arr;
    temp->arr=nullptr;
    delete temp;
    }
    if(STACK::active_stack->prev==BLOCK){
        STACK::active_stack->prev=nullptr;
        temp=nullptr;
        delete[] BLOCK->arr;
        BLOCK->arr=nullptr;
        delete BLOCK;
        delete[] STACK::active_stack->arr;
        active_stack->arr=nullptr;
        delete STACK::active_stack;
        BLOCK=nullptr;
        STACK::active_stack=nullptr;
        return;
    }
    delete[] STACK::active_stack->arr;
    active_stack->arr=nullptr;
    delete[] BLOCK->arr;
    BLOCK->arr=nullptr;
    delete STACK::active_stack;
    delete BLOCK;
    STACK::active_stack=nullptr;
    BLOCK= nullptr;
}



