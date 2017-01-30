#include "SPEngine/SPException.h"

SPException::SPException(std::string p_ErrMess)
{

    this->m_ErrMess = "EXCEPTION - " + p_ErrMess;
}

SPException::~SPException() throw()
{

}

const char * SPException::what() const throw()
{
    return this->m_ErrMess.c_str();
}

void SPException::add(std::string p_ContentToAdd) throw()
{
    this->m_ErrMess += "\n" + p_ContentToAdd;
}
