#include <gui/runscreen_screen/runScreenView.hpp>
#include <stdlib.h>
#include <math.h>
#include <mainJt.h>

runScreenView::runScreenView()
{

}

void runScreenView::setupScreen()
{
    runScreenViewBase::setupScreen();

    weldingTimeSec = presenter->getWeldingTimeSec();
    weldingAmps    = presenter->getWeldingAmps();

	ampGauge.setValue(0);
	ampGauge.setGoalValue(weldingAmps);
	ampGauge.invalidate();

	Unicode::snprintfFloat(setAmpereTextBuffer, 7, "%06.2f", weldingAmps);
	setAmpereText.setWildcard(setAmpereTextBuffer);
	setAmpereText.invalidate();

	Unicode::snprintfFloat(currentAmpereTextBuffer, 7, "%06.2f",0.0);
	currentAmpereText.setWildcard(currentAmpereTextBuffer);
	currentAmpereText.invalidate();

	uint8_t  minVal = uint8_t( weldingTimeSec / 60);
	uint8_t  secVal = (uint8_t) ( weldingTimeSec % 60);
	Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	currentTimeText.setWildcard(currentTimeTextBuffer);
	currentTimeText.invalidate();

	Unicode::snprintf(timeValueTextBuffer, 6, "%02d:%02d", minVal, secVal);
	timeValueText.setWildcard(timeValueTextBuffer);
	timeValueText.invalidate();

	 boxProgress1.setValue(0);
	 boxProgress1.invalidate();

	 showAstrolabium(false);
}

void runScreenView::tearDownScreen()
{
    runScreenViewBase::tearDownScreen();
}

void runScreenView::update(pJoesPresenterEventT  pMsg )
{

	// todo bug: amps does not show / calculate delay decimal places in setamperetext

	Unicode::snprintfFloat(currentAmpereTextBuffer, 7, "%6.2f",pMsg->evData.runScreenData.amps);
	currentAmpereText.setWildcard(currentAmpereTextBuffer);
	currentAmpereText.invalidate();

//	 int ampValue = ((int)( amps  ));

		ampGauge.setValue(pMsg->evData.runScreenData.amps);
		ampGauge.invalidate();

	 uint8_t  minVal = uint8_t( pMsg->evData.runScreenData.secondsRemaining  / 60);
	 uint8_t  secVal = (uint8_t) ( pMsg->evData.runScreenData.secondsRemaining % 60);
	 Unicode::snprintf(currentTimeTextBuffer, 6, "%02d:%02d", minVal, secVal);
	 currentTimeText.setWildcard(currentTimeTextBuffer);
	 currentTimeText.invalidate();

	 float wTime = weldingTimeSec;
	 float rTime = pMsg->evData.runScreenData.secondsRemaining;
	 float  remain = (wTime - rTime)/ wTime;
	 uint32_t boxPro = 100 * remain;
	 boxProgress1.setValue(boxPro);
	 boxProgress1.invalidate();

	 if (astroBorder.isVisible())  {
		 Unicode::snprintf(adcValueTextBuffer, 6, "%04d", pMsg->evData.runScreenData.adcValue);
		 adcValueText.setWildcard(adcValueTextBuffer);
		 adcValueText.invalidate();

		 Unicode::snprintfFloat(adcVoltageTextBuffer, 6, "%01.3f", pMsg->evData.runScreenData.adcVoltage);
		 adcVoltageText.setWildcard(adcVoltageTextBuffer);
		 adcVoltageText.invalidate();

		 Unicode::snprintf(delayTextBuffer, 6, "%04d", pMsg->evData.runScreenData.triacDelay);
		 delayText.setWildcard(delayTextBuffer);
		 delayText.invalidate();

		 Unicode::snprintf(amtMissedZpTextBuffer, 6, "%04d", pMsg->evData.runScreenData.amtMissZp);
		 amtMissedZpText.setWildcard(amtMissedZpTextBuffer);
		 amtMissedZpText.invalidate();

		 Unicode::snprintf(maxMissedZpTextBuffer, 6, "%03d", pMsg->evData.runScreenData.maxMissZp);
		 maxMissedZpText.setWildcard(maxMissedZpTextBuffer);
		 maxMissedZpText.invalidate();

		 Unicode::snprintf(amtExtiTotalTextBuffer, AMTEXTITOTALTEXT_SIZE, "%016d", pMsg->evData.runScreenData.extiEvTotal);
		 amtExtiTotalText.setWildcard(amtExtiTotalTextBuffer);
		 amtExtiTotalText.invalidate();

		 Unicode::snprintf(illigelExtiTextBuffer, ILLIGELEXTITEXT_SIZE, "%010d", pMsg->evData.runScreenData.amtIllExti);
		 illigelExtiText.setWildcard(illigelExtiTextBuffer);
		 illigelExtiText.invalidate();

		 Unicode::snprintf(amtSyncWrongTextBuffer, 6, "%04d", pMsg->evData.runScreenData.amtWrongSyn);
		 amtSyncWrongText.setWildcard(amtSyncWrongTextBuffer);
		 amtSyncWrongText.invalidate();

		 Unicode::snprintf(amtSeqErrTextBuffer, 6, "%04d", pMsg->evData.runScreenData.amtSeqErr);
		 amtSeqErrText.setWildcard(amtSeqErrTextBuffer);
		 amtSeqErrText.invalidate();
	 }
}

void runScreenView::showAstrolabium(bool vis)     // this is bullshit, but how do else with stm touchgfx and it's "rich" user manual
{
	adcValueText.setVisible(vis);
	adcVoltageText.setVisible(vis);
	delayText.setVisible(vis);
	amtMissedZpText.setVisible(vis);
	maxMissedZpText.setVisible(vis);
	amtExtiTotalText.setVisible(vis);
	illigelExtiText.setVisible(vis);
	amtSyncWrongText.setVisible(vis);
	amtSeqErrText.setVisible(vis);
	textArea3.setVisible(vis);
	textArea4.setVisible(vis);
	textArea5.setVisible(vis);
	textArea6.setVisible(vis);
	textArea7.setVisible(vis);
	textArea9.setVisible(vis);
	textArea10.setVisible(vis);
	textArea11.setVisible(vis);
	textArea12.setVisible(vis);
	astroBorder.setVisible(vis);

	adcValueText.invalidate();
	adcVoltageText.invalidate();
	delayText.invalidate();
	amtMissedZpText.invalidate();
	maxMissedZpText.invalidate();
	amtExtiTotalText.invalidate();
	illigelExtiText.invalidate();
	amtSyncWrongText.invalidate();
	amtSeqErrText.invalidate();
	textArea3.invalidate();
	textArea4.invalidate();
	textArea5.invalidate();
	textArea6.invalidate();
	textArea7.invalidate();
	textArea9.invalidate();
	textArea10.invalidate();
	textArea11.invalidate();
	textArea12.invalidate();
	astroBorder.invalidate();
	astroBorder.invalidate();         // hope this works since all is under this border
}


void runScreenView::stopButtonPressed()
{
	presenter->stopButtonPressed();
}

void runScreenView::astroButtonPressed()
{
	if (astroBorder.isVisible()) {
		showAstrolabium(false);
	} else {
		showAstrolabium(true);
	}
}

