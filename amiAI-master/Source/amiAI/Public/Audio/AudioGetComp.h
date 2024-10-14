// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AudioCapture.h"
#include "Sound/SampleBufferIO.h"
#include "AudioGetComp.generated.h"

class UAudioCapture;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AMIAI_API UAudioGetComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAudioGetComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
		void StartCapture();

	UFUNCTION(BlueprintCallable, Category = "AudioCapture")
		void StopCapture();

private:
	UAudioCapture* AudioCapture;
	TArray<float> AudioBuffer;

	void OnAudioGenerate(const float* InAudio, int32 NumSamples);
	void SaveWavFile();

};
