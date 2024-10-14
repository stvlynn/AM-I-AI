// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncConversation.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Dom/JsonObject.h"
#include "JsonObjectConverter.h"


void UAsyncConversation::Activate()
{

}

UAsyncConversation* UAsyncConversation::SendAIHttp(UObject* WorldContextObject, const FString& Url, const FString& JsonString, const FString& Verb)
{
	//创建唯一标识符
	FString RequestId = FGuid::NewGuid().ToString();
	UAsyncConversation* AsyncHttpActionNode = NewObject<UAsyncConversation>();
	AsyncHttpActionNode->RequestId = RequestId;
	//Set
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(Verb);
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetContentAsString(JsonString);
	//set back
	HttpRequest->OnProcessRequestComplete().BindLambda
	(
		[AsyncHttpActionNode, RequestId](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
		{
			if (AsyncHttpActionNode->RequestId == RequestId)
			{
				if (bWasSuccessful)
				{
					AsyncHttpActionNode->Success.Broadcast(Response->GetContentAsString());
				}
				else
				{
					AsyncHttpActionNode->Failed.Broadcast(TEXT("Request failed")); 
				}
			}
		}
	);
	//send
	HttpRequest->ProcessRequest();
	return AsyncHttpActionNode;
}

