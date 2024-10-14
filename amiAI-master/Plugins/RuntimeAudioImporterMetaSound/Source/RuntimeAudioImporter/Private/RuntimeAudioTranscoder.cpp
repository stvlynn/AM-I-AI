﻿// Georgy Treshchev 2024.


#include "RuntimeAudioTranscoder.h"

#include "RuntimeAudioImporterLibrary.h"
#include "Codecs/RAW_RuntimeCodec.h"

void URuntimeAudioTranscoder::TranscodeRAWDataFromBuffer(TArray<uint8> RAWDataFrom, ERuntimeRAWAudioFormat RAWFormatFrom, ERuntimeRAWAudioFormat RAWFormatTo, const FOnRAWDataTranscodeFromBufferResult& Result)
{
	TranscodeRAWDataFromBuffer(TArray64<uint8>(MoveTemp(RAWDataFrom)), RAWFormatFrom, RAWFormatTo, FOnRAWDataTranscodeFromBufferResultNative::CreateLambda([Result](bool bSucceeded, const TArray64<uint8>& RAWData)
	{
		Result.ExecuteIfBound(bSucceeded, TArray<uint8>(RAWData));
	}));
}

void URuntimeAudioTranscoder::TranscodeRAWDataFromBuffer(TArray64<uint8> RAWDataFrom, ERuntimeRAWAudioFormat RAWFormatFrom, ERuntimeRAWAudioFormat RAWFormatTo, const FOnRAWDataTranscodeFromBufferResultNative& Result)
{
	if (IsInGameThread())
	{
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [RAWDataFrom = MoveTemp(RAWDataFrom), RAWFormatFrom, RAWFormatTo, Result]() mutable
		{
			TranscodeRAWDataFromBuffer(MoveTemp(RAWDataFrom), RAWFormatFrom, RAWFormatTo, Result);
		});
		return;
	}

	auto ExecuteResult = [Result](bool bSucceeded, TArray64<uint8>&& AudioData)
	{
		AsyncTask(ENamedThreads::GameThread, [Result, bSucceeded, AudioData]()
		{
			Result.ExecuteIfBound(bSucceeded, AudioData);
		});
	};

	TArray64<uint8> IntermediateRAWBuffer;

	// Transcoding of all formats to unsigned 8-bit PCM format (intermediate)
	switch (RAWFormatFrom)
	{
	case ERuntimeRAWAudioFormat::Int8:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<int8, uint8>(RAWDataFrom, IntermediateRAWBuffer);
			break;
		}
	case ERuntimeRAWAudioFormat::UInt8:
		{
			IntermediateRAWBuffer = MoveTemp(RAWDataFrom);
			break;
		}
	case ERuntimeRAWAudioFormat::Int16:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<int16, uint8>(RAWDataFrom, IntermediateRAWBuffer);
			break;
		}
	case ERuntimeRAWAudioFormat::UInt16:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint16, uint8>(RAWDataFrom, IntermediateRAWBuffer);
			break;
		}
	case ERuntimeRAWAudioFormat::Int32:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<int32, uint8>(RAWDataFrom, IntermediateRAWBuffer);
			break;
		}
	case ERuntimeRAWAudioFormat::UInt32:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint32, uint8>(RAWDataFrom, IntermediateRAWBuffer);
			break;
		}
	case ERuntimeRAWAudioFormat::Float32:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<float, uint8>(RAWDataFrom, IntermediateRAWBuffer);
			break;
		}
	}

	TArray64<uint8> RAWData_To;

	// Transcoding unsigned 8-bit PCM to the specified format
	switch (RAWFormatTo)
	{
	case ERuntimeRAWAudioFormat::Int8:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint8, int8>(IntermediateRAWBuffer, RAWData_To);
			break;
		}
	case ERuntimeRAWAudioFormat::UInt8:
		{
			RAWData_To = MoveTemp(IntermediateRAWBuffer);
			break;
		}
	case ERuntimeRAWAudioFormat::Int16:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint8, int16>(IntermediateRAWBuffer, RAWData_To);
			break;
		}
	case ERuntimeRAWAudioFormat::UInt16:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint8, uint16>(IntermediateRAWBuffer, RAWData_To);
			break;
		}
	case ERuntimeRAWAudioFormat::Int32:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint8, int32>(IntermediateRAWBuffer, RAWData_To);
			break;
		}
	case ERuntimeRAWAudioFormat::UInt32:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint8, uint32>(IntermediateRAWBuffer, RAWData_To);
			break;
		}
	case ERuntimeRAWAudioFormat::Float32:
		{
			FRAW_RuntimeCodec::TranscodeRAWData<uint8, float>(IntermediateRAWBuffer, RAWData_To);
			break;
		}
	}

	ExecuteResult(true, MoveTemp(RAWData_To));
}

void URuntimeAudioTranscoder::TranscodeRAWDataFromFile(const FString& FilePathFrom, ERuntimeRAWAudioFormat RAWFormatFrom, const FString& FilePathTo, ERuntimeRAWAudioFormat RAWFormatTo, const FOnRAWDataTranscodeFromFileResult& Result)
{
	TranscodeRAWDataFromFile(FilePathFrom, RAWFormatFrom, FilePathTo, RAWFormatTo, FOnRAWDataTranscodeFromFileResultNative::CreateLambda([Result](bool bSucceeded)
	{
		Result.ExecuteIfBound(bSucceeded);
	}));
}

void URuntimeAudioTranscoder::TranscodeRAWDataFromFile(const FString& FilePathFrom, ERuntimeRAWAudioFormat RAWFormatFrom, const FString& FilePathTo, ERuntimeRAWAudioFormat RAWFormatTo, const FOnRAWDataTranscodeFromFileResultNative& Result)
{
	if (IsInGameThread())
	{
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [FilePathFrom, RAWFormatFrom, FilePathTo, RAWFormatTo, Result]()
		{
			TranscodeRAWDataFromFile(FilePathFrom, RAWFormatFrom, FilePathTo, RAWFormatTo, Result);
		});
		return;
	}

	auto ExecuteResult = [Result](bool bSucceeded)
	{
		AsyncTask(ENamedThreads::GameThread, [Result, bSucceeded]()
		{
			Result.ExecuteIfBound(bSucceeded);
		});
	};

	TArray64<uint8> RAWBufferFrom;
	if (!RuntimeAudioImporter::LoadAudioFileToArray(RAWBufferFrom, *FilePathFrom))
	{
		UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Something went wrong when reading RAW data on the path '%s'"), *FilePathFrom);
		ExecuteResult(false);
		return;
	}

	TranscodeRAWDataFromBuffer(MoveTemp(RAWBufferFrom), RAWFormatFrom, RAWFormatTo, FOnRAWDataTranscodeFromBufferResultNative::CreateLambda([Result, ExecuteResult, FilePathTo](bool bSucceeded, const TArray64<uint8>& RAWBufferTo)
	{
		if (!bSucceeded)
		{
			UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Something went wrong when transcoding RAW data from buffer to save to the path '%s'"), *FilePathTo);
			ExecuteResult(false);
			return;
		}

		if (!FFileHelper::SaveArrayToFile(RAWBufferTo, *FilePathTo))
		{
			UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Something went wrong when saving RAW data to the path '%s'"), *FilePathTo);
			ExecuteResult(false);
			return;
		}

		ExecuteResult(true);
	}));
}

void URuntimeAudioTranscoder::TranscodeEncodedDataFromBuffer(TArray<uint8> EncodedDataFrom, ERuntimeAudioFormat EncodedFormatFrom, ERuntimeAudioFormat EncodedFormatTo, uint8 Quality, const FRuntimeAudioExportOverrideOptions& OverrideOptions, const FOnEncodedDataTranscodeFromBufferResult& Result)
{
	TranscodeEncodedDataFromBuffer(TArray64<uint8>(MoveTemp(EncodedDataFrom)), EncodedFormatFrom, EncodedFormatTo, Quality, OverrideOptions, FOnEncodedDataTranscodeFromBufferResultNative::CreateLambda([Result](bool bSucceeded, const TArray64<uint8>& EncodedData)
	{
		Result.ExecuteIfBound(bSucceeded, TArray<uint8>(EncodedData));
	}));
}

void URuntimeAudioTranscoder::TranscodeEncodedDataFromBuffer(TArray64<uint8> EncodedDataFrom, ERuntimeAudioFormat EncodedFormatFrom, ERuntimeAudioFormat EncodedFormatTo, uint8 Quality, const FRuntimeAudioExportOverrideOptions& OverrideOptions, const FOnEncodedDataTranscodeFromBufferResultNative& Result)
{
	if (IsInGameThread())
	{
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [EncodedDataFrom = MoveTemp(EncodedDataFrom), EncodedFormatFrom, EncodedFormatTo, Quality, OverrideOptions, Result]() mutable
		{
			TranscodeEncodedDataFromBuffer(MoveTemp(EncodedDataFrom), EncodedFormatFrom, EncodedFormatTo, Quality, OverrideOptions, Result);
		});
		return;
	}

	auto ExecuteResult = [Result](bool bSucceeded, TArray64<uint8>&& AudioData)
	{
		AsyncTask(ENamedThreads::GameThread, [Result, bSucceeded, AudioData]()
		{
			Result.ExecuteIfBound(bSucceeded, AudioData);
		});
	};

	FDecodedAudioStruct DecodedAudioInfo;
	{
		FEncodedAudioStruct EncodedAudioInfo(MoveTemp(EncodedDataFrom), EncodedFormatFrom);
		if (!URuntimeAudioImporterLibrary::DecodeAudioData(MoveTemp(EncodedAudioInfo), DecodedAudioInfo))
		{
			UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Failed to decode audio data"));
			ExecuteResult(false, TArray64<uint8>());
			return;
		}
	}

	// Check if the number of channels and the sampling rate of the sound wave and desired override options are not the same
	if (OverrideOptions.IsOverriden() && (DecodedAudioInfo.SoundWaveBasicInfo.SampleRate != OverrideOptions.SampleRate || DecodedAudioInfo.SoundWaveBasicInfo.NumOfChannels != OverrideOptions.NumOfChannels))
	{
		Audio::FAlignedFloatBuffer WaveData(DecodedAudioInfo.PCMInfo.PCMData.GetView().GetData(), DecodedAudioInfo.PCMInfo.PCMData.GetView().Num());

		// Resampling if needed
		if (OverrideOptions.IsSampleRateOverriden() && DecodedAudioInfo.SoundWaveBasicInfo.SampleRate != OverrideOptions.SampleRate)
		{
			Audio::FAlignedFloatBuffer ResamplerOutputData;
			if (!FRAW_RuntimeCodec::ResampleRAWData(WaveData, DecodedAudioInfo.SoundWaveBasicInfo.NumOfChannels, DecodedAudioInfo.SoundWaveBasicInfo.SampleRate, OverrideOptions.SampleRate, ResamplerOutputData))
			{
				UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Unable to resample audio data to the overriden sample rate. Resampling failed"));
				ExecuteResult(false, TArray64<uint8>());
				return;
			}

			WaveData = MoveTemp(ResamplerOutputData);
			DecodedAudioInfo.SoundWaveBasicInfo.SampleRate = OverrideOptions.SampleRate;
		}

		// Mixing the channels if needed
		if (OverrideOptions.IsNumOfChannelsOverriden() && DecodedAudioInfo.SoundWaveBasicInfo.NumOfChannels != OverrideOptions.NumOfChannels)
		{
			Audio::FAlignedFloatBuffer WaveDataTemp;
			if (!FRAW_RuntimeCodec::MixChannelsRAWData(WaveData, OverrideOptions.SampleRate, DecodedAudioInfo.SoundWaveBasicInfo.NumOfChannels, OverrideOptions.NumOfChannels, WaveDataTemp))
			{
				UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Unable to mix audio channels to the overriden number of channels. Mixing failed"));
				ExecuteResult(false, TArray64<uint8>());
				return;
			}
			WaveData = MoveTemp(WaveDataTemp);
			DecodedAudioInfo.SoundWaveBasicInfo.NumOfChannels = OverrideOptions.NumOfChannels;
		}

		DecodedAudioInfo.PCMInfo.PCMNumOfFrames = WaveData.Num() / DecodedAudioInfo.SoundWaveBasicInfo.NumOfChannels;
		DecodedAudioInfo.PCMInfo.PCMData = FRuntimeBulkDataBuffer<float>(WaveData);
	}

	FEncodedAudioStruct EncodedAudioInfo;
	EncodedAudioInfo.AudioFormat = EncodedFormatTo;
	{
		if (!URuntimeAudioImporterLibrary::EncodeAudioData(MoveTemp(DecodedAudioInfo), EncodedAudioInfo, 100))
		{
			UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Failed to encode audio data"));
			ExecuteResult(false, TArray64<uint8>());
			return;
		}
	}

	ExecuteResult(true, TArray64<uint8>(EncodedAudioInfo.AudioData.GetView().GetData(), EncodedAudioInfo.AudioData.GetView().Num()));
}

void URuntimeAudioTranscoder::TranscodeEncodedDataFromFile(const FString& FilePathFrom, ERuntimeAudioFormat EncodedFormatFrom, const FString& FilePathTo, ERuntimeAudioFormat EncodedFormatTo, uint8 Quality, const FRuntimeAudioExportOverrideOptions& OverrideOptions, const FOnEncodedDataTranscodeFromFileResult& Result)
{
	TranscodeEncodedDataFromFile(FilePathFrom, EncodedFormatFrom, FilePathTo, EncodedFormatTo, Quality, OverrideOptions, FOnEncodedDataTranscodeFromFileResultNative::CreateLambda([Result](bool bSucceeded)
	{
		Result.ExecuteIfBound(bSucceeded);
	}));
}

void URuntimeAudioTranscoder::TranscodeEncodedDataFromFile(const FString& FilePathFrom, ERuntimeAudioFormat EncodedFormatFrom, const FString& FilePathTo, ERuntimeAudioFormat EncodedFormatTo, uint8 Quality, const FRuntimeAudioExportOverrideOptions& OverrideOptions, const FOnEncodedDataTranscodeFromFileResultNative& Result)
{
	if (IsInGameThread())
	{
		AsyncTask(ENamedThreads::AnyBackgroundHiPriTask, [FilePathFrom, EncodedFormatFrom, FilePathTo, EncodedFormatTo, Quality, OverrideOptions, Result]()
		{
			TranscodeEncodedDataFromFile(FilePathFrom, EncodedFormatFrom, FilePathTo, EncodedFormatTo, Quality, OverrideOptions, Result);
		});
		return;
	}

	auto ExecuteResult = [Result](bool bSucceeded)
	{
		AsyncTask(ENamedThreads::GameThread, [Result, bSucceeded]()
		{
			Result.ExecuteIfBound(bSucceeded);
		});
	};

	TArray64<uint8> AudioBuffer;
	if (!RuntimeAudioImporter::LoadAudioFileToArray(AudioBuffer, *FilePathFrom))
	{
		UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Something went wrong when reading audio data on the path '%s' for transcoding"), *FilePathFrom);
		ExecuteResult(false);
		return;
	}

	TranscodeEncodedDataFromBuffer(MoveTemp(AudioBuffer), EncodedFormatFrom, EncodedFormatTo, Quality, OverrideOptions, FOnEncodedDataTranscodeFromBufferResultNative::CreateLambda([ExecuteResult, FilePathTo](bool bSucceeded, const TArray64<uint8>& EncodedData)
	{
		if (!bSucceeded)
		{
			UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Something went wrong when transcoding audio data from buffer to save to the path '%s'"), *FilePathTo);
			ExecuteResult(false);
			return;
		}

		if (!FFileHelper::SaveArrayToFile(EncodedData, *FilePathTo))
		{
			UE_LOG(LogRuntimeAudioImporter, Error, TEXT("Something went wrong when saving audio data to the path '%s' after transcoding"), *FilePathTo);
			ExecuteResult(false);
			return;
		}

		ExecuteResult(true);
	}));
}