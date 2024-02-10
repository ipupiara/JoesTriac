#ifndef ASTROLABIUMCONTAINER_HPP
#define ASTROLABIUMCONTAINER_HPP

#include <gui_generated/containers/astrolabiumContainerBase.hpp>
#include <stdint.h>
#include <mainJt.h>

//#include <gui/astrolabiumContainer/astrolabiumPresenter.hpp>

class astrolabiumContainer : public astrolabiumContainerBase
{
public:
    astrolabiumContainer();
    virtual ~astrolabiumContainer() {}

    virtual void initialize();
    void update(pJoesPresenterEventT  pMsg );
protected:
};

#endif // ASTROLABIUMCONTAINER_HPP
