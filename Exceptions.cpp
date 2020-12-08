#include "Exceptions.h"


AvlTreeException::AvlTreeException(std::string messege):Exception(){
    this->message = messege;
}

//*************************************** Avl Exception Implementations *********************************************
 
NullArgument::NullArgument():
    AvlTreeException("Error: nullptr was givven as argument."){
}


const char* NullArgument::what() const noexcept{
    return message.c_str();
}       
//----------------------------------------------------------------------------------------------------------------------

