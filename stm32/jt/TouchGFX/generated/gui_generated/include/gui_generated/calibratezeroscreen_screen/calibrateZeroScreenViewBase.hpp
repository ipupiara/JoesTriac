/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef CALIBRATEZEROSCREENVIEWBASE_HPP
#define CALIBRATEZEROSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/calibratezeroscreen_screen/calibrateZeroScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>

class calibrateZeroScreenViewBase : public touchgfx::View<calibrateZeroScreenPresenter>
{
public:
    calibrateZeroScreenViewBase();
    virtual ~calibrateZeroScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void abortPressed()
    {
        // Override and implement this function in calibrateZeroScreen
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
    touchgfx::TextArea textArea1;
    touchgfx::TextArea textArea2;
    touchgfx::TextArea textArea3;
    touchgfx::TextAreaWithOneWildcard zeroPotiPosText;
    touchgfx::TextArea textArea4;
    touchgfx::TextAreaWithOneWildcard textArea5;
    touchgfx::ButtonWithLabel abortButton;

    /*
     * Wildcard Buffers
     */
    static const uint16_t ZEROPOTIPOSTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar zeroPotiPosTextBuffer[ZEROPOTIPOSTEXT_SIZE];
    static const uint16_t TEXTAREA5_SIZE = 10;
    touchgfx::Unicode::UnicodeChar textArea5Buffer[TEXTAREA5_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<calibrateZeroScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // CALIBRATEZEROSCREENVIEWBASE_HPP