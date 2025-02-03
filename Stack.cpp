#include "Stack.h"
#include <iostream>
//stack for backtracking

// Constructor to initialize an empty stack
Stack::Stack() {
    // TODO: Your code here
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(int province) {
    // TODO: Your code here

    if(top < MAX_SIZE-1) { // Stack is not full
        top++;
        data[top] = province;
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if (isEmpty()) {
        return -1;
    }
    return data[top--];
}

// Returns the top province without removing it
int Stack::peek() const {
    // TODO: Your code here
    if(isEmpty() == false) {
        return data[top];
    }
    return -1;
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    // TODO: Your code here
    if(top == -1) {
        return true;
    }
    return false;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    // TODO: Your code here

    if(isEmpty() == false) {
        return top + 1;
    }
    return 0;
}
