// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/AudioGetComp.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Sound/SampleBufferIO.h"
#include "AudioDevice.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundWaveProcedural.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAudioGetComp::UAudioGetComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAudioGetComp::BeginPlay()
{
	Super::BeginPlay();
	// ��������ʼ����Ƶ����
	AudioCapture = NewObject<UAudioCapture>();
	if (AudioCapture)
	{
		// ʹ�� Lambda ���ʽ�����Ƶ���ɵĻص�����
		AudioCapture->AddGeneratorDelegate([this](const float* InAudio, int32 NumSamples)
			{
				OnAudioGenerate(InAudio, NumSamples);
			});
	}
	// ...
	
}


// Called every frame
void UAudioGetComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAudioGetComp::StartCapture()
{
	// ��Ĭ����Ƶ������ʼ������Ƶ
	if (AudioCapture->OpenDefaultAudioStream())
	{
		if (AudioCapture && !AudioCapture->IsCapturingAudio())
		{
			AudioBuffer.Empty();
			AudioCapture->StartCapturingAudio();
		}
	}
}

void UAudioGetComp::StopCapture()
{
	if (AudioCapture && AudioCapture->IsCapturingAudio())
	{
		UE_LOG(LogTemp, Log, TEXT("end end end end end end"));
		AudioCapture->StopCapturingAudio();
		SaveWavFile();
		AudioBuffer.Reset();
	}
}

// �ص������������񵽵���Ƶ����
void UAudioGetComp::OnAudioGenerate(const float* InAudio, int32 NumSamples)
{
	// ���������Ƶ������ӵ�������
	AudioBuffer.Append(InAudio, NumSamples);
}

// ������ƵΪWAV�ļ�
void UAudioGetComp::SaveWavFile()
{
	if (AudioBuffer.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No audio data to save."));
		return;
	}

	// ��ȡ��Ƶ�Ĳ����ʺ�ͨ����
	const int32 NumChannels = 2;  // Mono
	const int32 SampleRate = 44100; // ͨ��������

	// ��װ���������Ա�PCMд�������������
	Audio::FSampleBuffer SampleBuffer(AudioBuffer.GetData(), AudioBuffer.Num(), NumChannels, SampleRate);

	// ����PCMд����
	Audio::FSoundWavePCMWriter PCMWriter;

	// WAV�ļ�·��
	FString FilePath = FPaths::ProjectSavedDir() + "SaveWav";

	// ��ʼд��WAV�ļ�
	PCMWriter.BeginWriteToWavFile(SampleBuffer, "CapturedAudio", FilePath);

	UE_LOG(LogTemp, Log, TEXT("WAV file saved to: %s"), *FilePath);
}

