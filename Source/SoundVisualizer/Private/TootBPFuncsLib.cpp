// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundVisualizer/Public/TootBPFuncsLib.h"

#include <filesystem>

namespace fs = std::filesystem;

bool UTootBPFuncsLib::ListFolderOnly(const FString &StartDirectory, TArray<FString> &FoundFilenames)
{
    for (const auto &it  : fs::directory_iterator(*StartDirectory)) {
        if(it.is_directory()){
            FoundFilenames.Add(FString(it.path().c_str()));
        }
    }

    return FoundFilenames.Num() > 0;
}
