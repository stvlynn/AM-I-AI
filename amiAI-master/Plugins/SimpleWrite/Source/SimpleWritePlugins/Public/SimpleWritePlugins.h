//All rights reserved HWSLandDFTX8.The SimpleWritePlugins is expected to be published in 2024.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FSimpleWritePluginsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
