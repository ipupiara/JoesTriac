/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef ALARMCONFIGSCREENVIEWBASE_HPP
#define ALARMCONFIGSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/alarmconfigscreen_screen/alarmConfigScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/containers/numericKeyPad.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ToggleButton.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>

class alarmConfigScreenViewBase : public touchgfx::View<alarmConfigScreenPresenter>
{
public:
    alarmConfigScreenViewBase();
    virtual ~alarmConfigScreenViewBase() {}
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void backNSaveButtonPressed()
    {
        // Override and implement this function in alarmConfigScreen
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
    numericKeyPad numericKeyPad1;
    touchgfx::TextArea textArea1;
    touchgfx::TextArea alarmTimeText;
    touchgfx::ToggleButton toggleButton1;
    touchgfx::TextArea textArea2;
    touchgfx::Line cursor;
    touchgfx::PainterRGB565 cursorPainter;
    touchgfx::ButtonWithLabel backNSaveButton;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<alarmConfigScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // ALARMCONFIGSCREENVIEWBASE_HPP