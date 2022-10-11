#ifndef ALARMCONFIGSCREENVIEW_HPP
#define ALARMCONFIGSCREENVIEW_HPP

#include <gui_generated/alarmconfigscreen_screen/alarmConfigScreenViewBase.hpp>
#include <gui/alarmconfigscreen_screen/alarmConfigScreenPresenter.hpp>

class alarmConfigScreenView : public alarmConfigScreenViewBase
{
public:
    alarmConfigScreenView();
    virtual ~alarmConfigScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void backNSaveButtonPressed();

    virtual void cancelButtonPressed();

    virtual void numPressed(uint8_t value);
    void toggleCursor();

protected:
    static const uint16_t TIMEVALUETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar timeValueTextBuffer[TIMEVALUETEXT_SIZE];


    uint16_t  timeValueMin;
    uint8_t    alarmNeeded;
    void      setValArray(uint16_t val);
    void      recalcTimeMin();
    void      printCurrentValueTimeOnScreen();
   uint8_t   valPos = 0;
   uint8_t   valArray[2];

   uint8_t getDefinesAlarmNeeded();
   uint32_t getDefinesAlarmTime();
   void buttonPressed(uint8_t val);
};

#endif // ALARMCONFIGSCREENVIEW_HPP
