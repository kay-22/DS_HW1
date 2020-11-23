#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>

//*************************************************************************************
/* A base class representing its inheritors. Each derrived class is made for
   handling  adifferent error by printing the most detailed massege possible  */

    class Exception : public std::exception{
        public:
        virtual const char* what() const noexcept = 0;
        virtual ~Exception(){};

    };
//***********************************************************************************************************
/* A mid-base class, derrived from Exception and representing its inheritors from GraphException.
   Each derrived class is made for handling a different situation in its own way and according to Exception  */

    class AvlTreeException : public Exception {
    
    protected:

        std::string message;

    public:

        AvlTreeException(std::string message);
        virtual const char* what() const noexcept = 0;
        virtual ~AvlTreeException(){};
    };
//********************************************************************************


//*********************************** AvlTree Error classes ****************************************

    class NullArgument : public AvlTreeException{
    public:
        NullArgument();
        const char* what() const noexcept override;
    };


    
//------------------------------------- end of AvlTree Error classes -------------------------------------------



#endif    // EXCEPTIONS_H_


