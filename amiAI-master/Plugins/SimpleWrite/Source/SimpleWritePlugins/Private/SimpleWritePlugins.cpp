//All rights reserved HWSLandDFTX8.The SimpleWritePlugins is expected to be published in 2024.

#include "SimpleWritePlugins.h"

#define LOCTEXT_NAMESPACE "FSimpleWritePluginsModule"

void FSimpleWritePluginsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSimpleWritePluginsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleWritePluginsModule, SimpleWritePlugins)