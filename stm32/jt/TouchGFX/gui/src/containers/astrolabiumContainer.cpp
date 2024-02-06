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
	 if (this->isVisible())  {
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
