#ifndef SPEXCEPTION_H
#define SPEXCEPTION_H

#include <stdexcept>
#include <string>

class SPException : public std::exception
{
    public:
        SPException(std::string p_ErrMess);
        virtual ~SPException() throw();
        virtual const char* what() const throw();
        virtual void add(std::string p_ContentToAdd) throw();
    protected:
    private:
        std::string m_ErrMess;
};

#endif // SPEXCEPTION_H
