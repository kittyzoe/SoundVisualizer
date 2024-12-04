// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundVisualizer/Public/ScanOneFolder.h"

#include <filesystem>

#include "HAL/FileManager.h"


namespace fs = std::filesystem;

UScanOneFolder::UScanOneFolder()
{
}

UScanOneFolder::~UScanOneFolder()
{
}

bool UScanOneFolder::FindFoldersOnly(const FString& StartDirectory, TArray<FString>& FoundFilenames)
{

    //IFileManager::Get().FindFilesRecursive(FoundFilenames, *StartDirectory, TEXT("*.*"), false, false, false);

   // UE_LOG(LogTemp, Warning, TEXT("=========         StartDirectory : %s") , *StartDirectory);

    // IFileManager::Get().FindFiles( FoundFilenames, *StartDirectory) ;

    for (const auto &it  : fs::directory_iterator(*StartDirectory)) {
        if(it.is_directory()){
            FoundFilenames.Add(FString(it.path().c_str()));
        }
    }


    return FoundFilenames.Num() > 0;

}

