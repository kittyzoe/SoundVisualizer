// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ScanOneFolder.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class SOUNDVISUALIZER_API UScanOneFolder : public UObject
{
      GENERATED_BODY()

public:
    UScanOneFolder();
    ~UScanOneFolder();

    UFUNCTION(BlueprintCallable, Category = "TOOT")
    bool FindFoldersOnly(const FString &StartDirectory, TArray<FString> &FoundFilenames);

   private:



};
