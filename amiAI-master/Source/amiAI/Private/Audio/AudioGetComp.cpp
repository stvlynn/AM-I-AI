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
	// 创建并初始化音频捕获
	AudioCapture = NewObject<UAudioCapture>();
	if (AudioCapture)
	{
		// 使用 Lambda 表达式添加音频生成的回调函数
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
	// 打开默认音频流并开始捕获音频
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

// 回调函数：处理捕获到的音频数据
void UAudioGetComp::OnAudioGenerate(const float* InAudio, int32 NumSamples)
{
	// 将捕获的音频数据添加到缓冲区
	AudioBuffer.Append(InAudio, NumSamples);
}

// 保存音频为WAV文件
void UAudioGetComp::SaveWavFile()
{
	if (AudioBuffer.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No audio data to save."));
		return;
	}

	// 获取音频的采样率和通道数
	const int32 NumChannels = 2;  // Mono
	const int32 SampleRate = 44100; // 通常采样率

	// 包装缓冲区，以便PCM写入器可以理解它
	Audio::FSampleBuffer SampleBuffer(AudioBuffer.GetData(), AudioBuffer.Num(), NumChannels, SampleRate);

	// 创建PCM写入器
	Audio::FSoundWavePCMWriter PCMWriter;

	// WAV文件路径
	FString FilePath = FPaths::ProjectSavedDir() + "SaveWav";

	// 开始写入WAV文件
	PCMWriter.BeginWriteToWavFile(SampleBuffer, "CapturedAudio", FilePath);

	UE_LOG(LogTemp, Log, TEXT("WAV file saved to: %s"), *FilePath);
}

