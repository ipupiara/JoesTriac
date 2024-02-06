#include <gui/containers/astrolabiumContainer.hpp>
#include <stdlib.h>
#include <math.h>


astrolabiumContainer::astrolabiumContainer()
{

}

void astrolabiumContainer::initialize()
{
    astrolabiumContainerBase::initialize();
}

void astrolabiumContainer::update(pJoesPresenterEventT  pMsg )
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
