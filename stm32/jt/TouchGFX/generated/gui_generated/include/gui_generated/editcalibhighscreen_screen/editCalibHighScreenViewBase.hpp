/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef EDITCALIBHIGHSCREENVIEWBASE_HPP
#define EDITCALIBHIGHSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/editcalibhighscreen_screen/editCalibHighScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <gui/containers/numericKeyPad.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class editCalibHighScreenViewBase : public touchgfx::View<editCalibHighScreenPresenter>
{
public:
    editCalibHighScreenViewBase();
    virtual ~editCalibHighScreenViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box box1;
    touchgfx::TextArea calibLowText;
    touchgfx::TextArea calibHighText;
    touchgfx::ButtonWithLabel calibLowButton;
    touchgfx::ButtonWithLabel backButton;
    numericKeyPad numericKeyPad1;
    touchgfx::Line cursor;
    touchgfx::PainterRGB565 cursorPainter;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<editCalibHighScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

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

#endif // EDITCALIBHIGHSCREENVIEWBASE_HPP