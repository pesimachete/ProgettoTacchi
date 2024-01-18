#include "Manager.h"

#include<vector>


template <typename T>
void Manager::clearMemory(std::vector<T>& allocated) {
    // Iterates through each element of the passed vector.
    // Deallocates the element, and finally resets the vector's length to zero. 
    auto i = allocated.begin();
    for (; i != allocated.end(); ++i) {
        delete(*i);
    }
    allocated.resize(0);
}


NumExprManager::~NumExprManager()   {
    // Calls the clearMemory function to deallocate memory for numerical expressions.
    clearMemory(NEallocated);
}

BoolExprManager::~BoolExprManager()  {
    // Calls the clearMemory function to deallocate memory for boolean expressions.
    clearMemory(BEallocated); 
}
StatementManager::~StatementManager() {
    // Calls the clearMemory function to deallocate memory for statements.
    clearMemory(Sallocated);
}
BlockManager::~BlockManager() {
    // Calls the clearMemory function to deallocate memory for blocks.
    clearMemory(Ballocated);
}

ProgramManager::~ProgramManager() {
    // Calls the clearMemory function to deallocate memory for programs.
    clearMemory(Pallocated);
}