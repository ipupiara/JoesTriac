#ifndef CONFIGDEBUGSCREENVIEW_HPP
#define CONFIGDEBUGSCREENVIEW_HPP

#include <gui_generated/configdebugscreen_screen/configDebugScreenViewBase.hpp>
#include <gui/configdebugscreen_screen/configDebugScreenPresenter.hpp>

class configDebugScreenView : public configDebugScreenViewBase
{
public:
    configDebugScreenView();
    virtual ~configDebugScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void cancelButtonPressed();
    virtual void saveButtonPressed();

protected:

	uint8_t  pidPrintB;
	uint8_t  infoPrintB;
	uint8_t  extiCheckB;
    touchgfx::RadioButtonGroup<2> extiRadioButtonGroup;    //  todo check if more groups can be used this way ?????  else  never-touch-gfx
    touchgfx::RadioButtonGroup<2> pidRadioButtonGroup;
    touchgfx::RadioButtonGroup<2> infoRadioButtonGroup;
    touchgfx::RadioButton printPidOnButton;
    touchgfx::RadioButton printPidOffButton;
     touchgfx::RadioButton extiCheckOnButton;
     touchgfx::RadioButton extiCheckOffButton;
     touchgfx::RadioButton infoPrintfOnButton;
     touchgfx::RadioButton infoPrintfOffButton;
};

#endif // CONFIGDEBUGSCREENVIEW_HPP
