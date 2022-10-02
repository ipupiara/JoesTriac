#ifndef JOBCOMPLETESCREENVIEW_HPP
#define JOBCOMPLETESCREENVIEW_HPP

#include <gui_generated/jobcompletescreen_screen/jobCompleteScreenViewBase.hpp>
#include <gui/jobcompletescreen_screen/jobCompleteScreenPresenter.hpp>

class jobCompleteScreenView : public jobCompleteScreenViewBase
{
public:
    jobCompleteScreenView();
    virtual ~jobCompleteScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // JOBCOMPLETESCREENVIEW_HPP
