/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef REQUESTSTOPSCREENVIEWBASE_HPP
#define REQUESTSTOPSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/requeststopscreen_screen/requestStopScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ButtonWithIcon.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <gui/containers/GaugeWithGoal.hpp>

class requestStopScreenViewBase : public touchgfx::View<requestStopScreenPresenter>
{
public:
    requestStopScreenViewBase();
    virtual ~requestStopScreenViewBase();
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void continueButtonPressed()
    {
        // Override and implement this function in requestStopScreen
    }
    virtual void abortButtonPressed()
    {
        // Override and implement this function in requestStopScreen
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box box1;
    touchgfx::TextAreaWithOneWildcard timeValueText;
    touchgfx::ButtonWithIcon abortButton;
    touchgfx::TextAreaWithOneWildcard setAmpereText;
    touchgfx::TextArea textArea1;
    touchgfx::TextArea textArea2;
    touchgfx::TextAreaWithOneWildcard currentTimeText;
    touchgfx::TextAreaWithOneWildcard currentAmpereText;
    touchgfx::BoxProgress boxProgress1;
    touchgfx::ButtonWithIcon continueButton;
    touchgfx::TextArea textArea4;
    touchgfx::TextAreaWithOneWildcard secondsb4ReturnText;
    GaugeWithGoal ampGauge;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TIMEVALUETEXT_SIZE = 20;
    touchgfx::Unicode::UnicodeChar timeValueTextBuffer[TIMEVALUETEXT_SIZE];
    static const uint16_t SETAMPERETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar setAmpereTextBuffer[SETAMPERETEXT_SIZE];
    static const uint16_t CURRENTTIMETEXT_SIZE = 20;
    touchgfx::Unicode::UnicodeChar currentTimeTextBuffer[CURRENTTIMETEXT_SIZE];
    static const uint16_t CURRENTAMPERETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar currentAmpereTextBuffer[CURRENTAMPERETEXT_SIZE];
    static const uint16_t SECONDSB4RETURNTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar secondsb4ReturnTextBuffer[SECONDSB4RETURNTEXT_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<requestStopScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // REQUESTSTOPSCREENVIEWBASE_HPP
