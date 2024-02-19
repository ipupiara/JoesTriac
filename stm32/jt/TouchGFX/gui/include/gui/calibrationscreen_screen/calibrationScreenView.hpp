#ifndef CALIBRATIONSCREENVIEW_HPP
#define CALIBRATIONSCREENVIEW_HPP

#include <gui_generated/calibrationscreen_screen/calibrationScreenViewBase.hpp>
#include <gui/calibrationscreen_screen/calibrationScreenPresenter.hpp>

class calibrationScreenView : public calibrationScreenViewBase
{
public:
    calibrationScreenView();
    virtual ~calibrationScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    void updateAmpsValues(float adcV, uint32_t adc, uint16_t triacDelay);
    void updateTriacDelay(uint32_t val);
    void updateDesiredAmps(float amps);

    virtual void plus100ButtonPressed()
     {
    	sendTriacDelayCalibCorrection(100);
     }

     virtual void plus10ButtonPressed()
     {
    	 sendTriacDelayCalibCorrection(10);
     }

     virtual void plus1ButtonPressed()
     {
    	 sendTriacDelayCalibCorrection(1);
     }

     virtual void minus100ButtonPressed()
     {
    	 sendTriacDelayCalibCorrection(-100);
     }

     virtual void minus10ButtonPressed()
     {
    	 sendTriacDelayCalibCorrection(-10);
     }

     virtual void minus1ButtonPressed()
     {
    	 sendTriacDelayCalibCorrection(-1);
     }

     virtual void abortButtonPressed();
     virtual void continueButtonPressed();
     virtual void skipButtonPressed();


protected:
    void sendTriacDelayCalibCorrection(int32_t val);

    static const uint16_t ADCVALUETEXT_SIZE = 10;  // stm32 spaghetticode does no more create textBuffers
    touchgfx::Unicode::UnicodeChar ampereTextBuffer[ADCVALUETEXT_SIZE];

};

#endif // CALIBRATIONSCREENVIEW_HPP
