// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "amiAIGIS.generated.h"

/**
 * 
 */
UCLASS()
class AMIAI_API UamiAIGIS : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	FString ToAIUrl = "http://127.0.0.1:80";
};
