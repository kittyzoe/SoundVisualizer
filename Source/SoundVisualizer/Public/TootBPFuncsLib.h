// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TootBPFuncsLib.generated.h"

/**
 * 
 */
UCLASS()
class SOUNDVISUALIZER_API UTootBPFuncsLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
    public:

     UFUNCTION(BlueprintCallable, BlueprintPure , Category = TOOTFuncsLib , meta=(WorldContext= "WorldContext"))
    static  bool ListFolderOnly(const FString& StartDirectory, TArray<FString>& FoundFilenames);



	
	
};
