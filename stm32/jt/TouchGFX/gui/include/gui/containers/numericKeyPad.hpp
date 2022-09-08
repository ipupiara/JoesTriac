#ifndef NUMERICKEYPAD_HPP
#define NUMERICKEYPAD_HPP

#include <gui_generated/containers/numericKeyPadBase.hpp>

class numericKeyPad : public numericKeyPadBase
{
public:
    numericKeyPad();
    virtual ~numericKeyPad() {}

    virtual void initialize();
protected:
};

#endif // NUMERICKEYPAD_HPP
