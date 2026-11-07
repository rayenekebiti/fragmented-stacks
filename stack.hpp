#ifndef STACK_HPP
#define STACK_HPP
struct STACK_BLOCK{
    STACK_BLOCK* prev=nullptr;
    int top=0;
    int *arr;
    unsigned int stack_number;
    };
class STACK{
    STACK_BLOCK* BLOCK;
    STACK_BLOCK* active_stack;
    const int capacity;
public:
    STACK(const int size);
    ~STACK();
    void push(const int data);
    int pop();
};
#endif STACK_HPP
