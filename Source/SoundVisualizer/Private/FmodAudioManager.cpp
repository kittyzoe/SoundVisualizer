// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundVisualizer/Public/FmodAudioManager.h"


#include <Misc/Paths.h>
#include <Misc/FileHelper.h>


FSongPlayFinished UFmodAudioManager::TootSongPlayEndDelegate;


UFmodAudioManager::UFmodAudioManager()
{


#if 0
    TootSongPlayEndDelegate.AddLambda([ ](const FString& txt){  // this is a method
        UE_LOG(LogTemp, Warning, TEXT("TXT is : %s") , *txt);
        thizClz->SongPlayEnd(txt);

    });
#else
    TootSongPlayEndDelegate.AddUniqueDynamic(this, &UFmodAudioManager::SongPlayEnd);


#endif


}




UFmodAudioManager::~UFmodAudioManager()
{

}



int32 UFmodAudioManager::InitializeManager()
{

    mSoundManager = std::unique_ptr<SoundManagerFmod>(new SoundManagerFmod());
    int rsl = mSoundManager->initialize();
    if(rsl > 0)
    {
        return rsl;
    }

    return 0;
}

int32 UFmodAudioManager::InitLinerSpectrum(const int32 numBars)
{
  return  mSoundManager->initializeLinerSpectrum(numBars);
}

int32 UFmodAudioManager::InitLogSpectrum(const int32 numBars)
{
    return mSoundManager->initializeLogSpectrum(numBars);
}

void UFmodAudioManager::InitBeatDetector()
{
    mSoundManager->initializeBeatDetector();
}

void UFmodAudioManager::FecthLinerSpectrum(TArray<float> &freqValues, const int32 bars)
{
    freqValues.Init(0.0, bars);
    mSoundManager->fetchLinerSpectrum(freqValues.GetData());
}

void UFmodAudioManager::FecthLogSpectrum(TArray<float> &freqValues, const int32 bars)
{
    freqValues.Init(0.0 , bars);
    mSoundManager->fetchLogSpectrum(freqValues.GetData());
}

void UFmodAudioManager::FecthBeat(TArray<float> &freqValues, TArray<float> &freqAverageValues, bool &isBoss, bool &isLowM)
{
    freqValues.Init( 0.0 , 2);
    freqAverageValues.Init(0.0  , 2);
    mSoundManager->getBeat(freqValues.GetData(), freqAverageValues.GetData(), isBoss , isLowM);
}

void UFmodAudioManager::UpdFmodSystem()
{
    mSoundManager->updFFTData();
}

int32 UFmodAudioManager::playSong(const FString &Pathname)
{
   // FString songsPath = Pathname ;// FPaths::ProjectContentDir() + "RawSongs/";

    //currSongName = "xiaosan.wav";
    //currSongName = "hoidaysale.mp3";
    // currSongName = "A Drop A Day - Fairy Dust.wav";

   // FString songFile(songsPath + currSongName);

    uint8 *songData;
    unsigned int songDatLen = 0;

    TArray<uint8> rawSongMemData;
    FFileHelper::LoadFileToArray(rawSongMemData, *Pathname);

    songData = rawSongMemData.GetData();
    songDatLen = rawSongMemData.Num() * sizeof(uint8);

    int32 rsl = mSoundManager->loadSndFromMemory(reinterpret_cast<char*>(songData), songDatLen);
    if(rsl > 0){
        return rsl;
    }

    mSoundManager->playSnd();



    return 0;
}

void UFmodAudioManager::PauseSong(bool isPause)
{
    mSoundManager->pauseSnd(isPause);


}

const FString &UFmodAudioManager::GetSongName() const
{
    return currSongName;
}

void UFmodAudioManager::SongPlayEndEvt(const FString &SongName) // this is static func
{

      TootSongPlayEndDelegate.Broadcast(SongName);


}
