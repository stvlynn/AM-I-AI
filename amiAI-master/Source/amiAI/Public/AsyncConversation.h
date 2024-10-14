// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncConversation.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHttpAsyncDelegate, FString, AiReturn);

class IHttpRequest;
class IHttpResponse;

/**
 * 
 */
UCLASS()
class AMIAI_API UAsyncConversation : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FHttpAsyncDelegate Success;
	UPROPERTY(BlueprintAssignable)
	FHttpAsyncDelegate Failed;
	//±êÊ¶·û
	FString RequestId;
protected:
	virtual void Activate()override;
#pragma region Function
public:
	//send ai,input json string
	UFUNCTION(BlueprintCallable, Category = "Http", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
		static UAsyncConversation* SendAIHttp(UObject* WorldContextObject, const FString& Url, const FString& JsonString, const FString& Verb);
#pragma endregion

};
