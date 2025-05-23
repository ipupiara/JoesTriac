/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef SETAMPERESCREENVIEWBASE_HPP
#define SETAMPERESCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/setamperescreen_screen/setAmpereScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/mixins/ClickListener.hpp>
#include <gui/containers/numericKeyPad.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class setAmpereScreenViewBase : public touchgfx::View<setAmpereScreenPresenter>
{
public:
    setAmpereScreenViewBase();
    virtual ~setAmpereScreenViewBase();
    virtual void setupScreen();

    /*
     * Virtual Action Handlers
     */
    virtual void backSaveButtonPressed()
    {
        // Override and implement this function in setAmpereScreen
    }
    virtual void numButtonPressed(uint8_t value)
    {
        // Override and implement this function in setAmpereScreen
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
    touchgfx::ClickListener< touchgfx::TextAreaWithOneWildcard > setAmpereText;
    numericKeyPad numericKeyPad1;
    touchgfx::ButtonWithLabel backNSaveButton;
    touchgfx::Line cursor;
    touchgfx::PainterRGB565 cursorPainter;

    /*
     * Wildcard Buffers
     */
    static const uint16_t SETAMPERETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar setAmpereTextBuffer[SETAMPERETEXT_SIZE];

private:

    /*
     * Canvas Buffer Size
     */
    static const uint32_t CANVAS_BUFFER_SIZE = 12000;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];

    /*
     * Callback Declarations
     */
    touchgfx::Callback<setAmpereScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<setAmpereScreenViewBase, uint8_t> numericKeyPad1NumPressedCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void numericKeyPad1NumPressedCallbackHandler(uint8_t value);

};

#endif // SETAMPERESCREENVIEWBASE_HPP
