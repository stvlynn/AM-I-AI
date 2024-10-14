//All rights reserved HWSLandDFTX8.The SimpleWritePlugins is expected to be published in 2024.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WriteLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLEWRITEPLUGINS_API UWriteLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
#pragma region Create
	/**
	 * create a file
	 * If the file exists, false is returned
	 * create failed ,return false
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Create")
		static bool CreateFile(const FString& FilePath);
	/**
	 * Re create file
	 * If the file exists,will delete old file ,create new file
	 * create failed ,return false
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Create")
		static bool ReCreateFile(const FString& FilePath);
#pragma endregion Create

#pragma region Write
	/**
	 * Overwrite string to file
	 * write failed : return false
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Write")
		static bool OverWriteStringInFile(const FString& Content, const FString& FilePath);
	/**
	 * Append String To File End
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Write")
	static bool AppendStringToFile(const FString& Content, const FString& FilePath);
#pragma endregion Write

#pragma region Read
	/**
	 * get string in file
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Read")
		static bool GetStringInFile(const FString& FilePath, FString& Content);
	/**
	 * File is only read?
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Read")
		static FString GetLineFromFile(const FString& FilePath, int32 LineNumber);
#pragma endregion Read
	
#pragma region Other
	/**
	 * File is only read?
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWritePlugins|other")
		static bool FileIsOnlyRead(const FString& FilePath);
	/**
	* add line break
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWritePlugins|other")
		static FString LineBreak();
	/**
	 * Check File is Exists
	 * yes,return true
	 * no ,return false
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWritePlugins|Other")
		static bool CheckFileExists(const FString& FilePath);
	/**
	 * Delete file
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Other")
		static bool DeleteFile(const FString& FilePath);
	/**
	 * Get file size : In bytes
	 * if file no vaild ,return -1;
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWritePlugins|Other")
		static int64 GetFileSize(const FString& FilePath);
	/**
	 * Move File to other file
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Other")
		static bool MoveFile(const FString& SourcePath, const FString& DestinationPath);
	/**
	 * copy file
	 */
	UFUNCTION(BlueprintCallable, Category = "SimpleWritePlugins|Other")
		static bool CopyFile(const FString& SourcePath, const FString& DestinationPath);
	/**
	 * Gets a list of all files in the specified directory
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "SimpleWritePlugins|Other")
		static TArray<FString> GetFilesInDirectory(const FString& DirectoryPath);
#pragma endregion Other
	
};
