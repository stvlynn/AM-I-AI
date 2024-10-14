//All rights reserved HWSLandDFTX8.The SimpleWritePlugins is expected to be published in 2024.


#include "WriteLibrary.h"
#include "Misc/FileHelper.h"
#include "Containers/StringView.h"
#include "HAL/PlatformFileManager.h"

#pragma region Create
bool UWriteLibrary::CreateFile(const FString& FilePath)
{
	//Checks if the file already exists,Already exists; returns false	
	if (IFileManager::Get().FileExists(*FilePath))
	{
		return false;
	}
	auto File = IFileManager::Get().CreateFileWriter(*FilePath);
	if (File)
	{
		File->Close();
		return true;
	}
	return false;
}

bool UWriteLibrary::ReCreateFile(const FString& FilePath)
{
	//Checks if the file already exists,delete
	if (IFileManager::Get().FileExists(*FilePath))
	{
		IFileManager::Get().Delete(*FilePath);
	}
	auto File = IFileManager::Get().CreateFileWriter(*FilePath);
	if (File)
	{
		File->Close();
		return true;
	}
	return false;
}
#pragma endregion Create
#pragma region Write
bool UWriteLibrary::OverWriteStringInFile(const FString& Content, const FString& FilePath)
{
	FString ExistingContent;
	if (FFileHelper::LoadFileToString(ExistingContent, *FilePath))
	{
		//over
		ExistingContent = Content;
		return FFileHelper::SaveStringToFile(ExistingContent, *FilePath);
	}
	return false;
}
bool UWriteLibrary::AppendStringToFile(const FString& Content, const FString& FilePath)
{
	FString ExistingContent;
	if (FFileHelper::LoadFileToString(ExistingContent, *FilePath))
	{
		//over
		ExistingContent += Content;
		return FFileHelper::SaveStringToFile(ExistingContent, *FilePath);
	}
	return false;
}
#pragma endregion Write
#pragma region Read

bool UWriteLibrary::GetStringInFile(const FString& FilePath, FString& Content)
{
	FString ExistingContent;
	if (FFileHelper::LoadFileToString(ExistingContent, *FilePath))
	{
		Content = ExistingContent;
		return true;
	}
	return false;
}

FString UWriteLibrary::GetLineFromFile(const FString& FilePath, int32 LineNumber)
{
	TArray<FString> Lines;
	FString Result;
	if (FFileHelper::LoadFileToStringArray(Lines, *FilePath))
	{
		if (LineNumber >= 0 && LineNumber < Lines.Num())
		{
			Result = Lines[LineNumber];
		}
	}
	return Result;
}
#pragma endregion Read
#pragma region Other
bool UWriteLibrary::FileIsOnlyRead(const FString& FilePath)
{
	return IFileManager::Get().IsReadOnly(*FilePath);
}

FString UWriteLibrary::LineBreak()
{
	return "\n";
}
bool UWriteLibrary::CheckFileExists(const FString& FilePath)
{
	return FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath);
}
bool UWriteLibrary::DeleteFile(const FString& FilePath)
{
	if (FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*FilePath))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to delete file: %s"), *FilePath);
		return false;
	}
}
int64 UWriteLibrary::GetFileSize(const FString& FilePath)
{
	int64 FileSize = -1;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*FilePath))
	{
		FileSize = PlatformFile.FileSize(*FilePath);
	}
	return FileSize;
}
bool UWriteLibrary::MoveFile(const FString& SourcePath, const FString& DestinationPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.FileExists(*SourcePath) && PlatformFile.MoveFile(*DestinationPath, *SourcePath))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to move file"));
		return false;
	}
}
bool UWriteLibrary::CopyFile(const FString& SourcePath, const FString& DestinationPath)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// source is exists,and copy success
	if (PlatformFile.FileExists(*SourcePath) && PlatformFile.CopyFile(*DestinationPath, *SourcePath))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SourcePath no exists  Or Copy Failed"))
		return false;
	}
}
TArray<FString> UWriteLibrary::GetFilesInDirectory(const FString& DirectoryPath)
{
	TArray<FString> Files;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	// get all file in path
	PlatformFile.FindFiles(Files, *DirectoryPath, nullptr);
	return Files;
}

#pragma endregion Other