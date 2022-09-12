#ifndef NUMERICKEYPAD_HPP
#define NUMERICKEYPAD_HPP

#include <gui_generated/containers/numericKeyPadBase.hpp>

class numericKeyPad : public numericKeyPadBase
{
public:
    numericKeyPad();
    virtual ~numericKeyPad() {}

    virtual void button0Pressed()
    {
    	buttonPressed(0);
    }

    virtual void button1Pressed()
    {
    	buttonPressed(1);
    }

    virtual void button2Pressed()
    {
        buttonPressed(2);
    }

    virtual void button3Pressed()
    {
    	buttonPressed(3);
    }

    virtual void button4Pressed()
    {
    	buttonPressed(4);
    }

    virtual void button5Pressed()
    {
    	buttonPressed(5);
    }

    virtual void button6Pressed()
    {
    	buttonPressed(6);
    }

    virtual void button7Pressed()
    {
    	buttonPressed(7);
    }

    virtual void button8Pressed()
    {
    	buttonPressed(8);
    }

    virtual void button9Pressed()
    {
    	buttonPressed(9);
    }


    virtual void initialize();
protected:
    void    buttonPressed(uint8_t val);
};

#endif // NUMERICKEYPAD_HPP
