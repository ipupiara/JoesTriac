#ifndef REQUESTSTOPSCREENVIEW_HPP
#define REQUESTSTOPSCREENVIEW_HPP

#include <gui_generated/requeststopscreen_screen/requestStopScreenViewBase.hpp>
#include <gui/requeststopscreen_screen/requestStopScreenPresenter.hpp>

class requestStopScreenView : public requestStopScreenViewBase
{
public:
    requestStopScreenView();
    virtual ~requestStopScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // REQUESTSTOPSCREENVIEW_HPP
