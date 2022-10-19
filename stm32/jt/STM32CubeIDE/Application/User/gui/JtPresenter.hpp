/*
 * JtPresenter.hpp
 *
 *  Created on: Oct 13, 2022
 *      Author: diego
 */

#ifndef APPLICATION_USER_GUI_JTPRESENTER_HPP_
#define APPLICATION_USER_GUI_JTPRESENTER_HPP_


#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <mainJt.h>

class JtPresenter: public touchgfx::Presenter , public ModelListener
{
public:
	JtPresenter();
	virtual ~JtPresenter();

	  uint32_t getCalibLow();
	  uint32_t getCalibHigh();
	  uint32_t getZeroPotiPos();

	   void setCalibHigh(uint32_t cH);
	   void setCalibLow(uint32_t cL);
	   void setZeroPotiPos(uint32_t cL);

	    void   saveCalibValues();

	    void   resetCache();


protected:

};

#endif /* APPLICATION_USER_GUI_JTPRESENTER_HPP_ */
