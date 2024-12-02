// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundVisualizer/Public/FmodAudioManager.h"


#include <Misc/Paths.h>
#include <Misc/FileHelper.h>


UFmodAudioManager::UFmodAudioManager()
{

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

void UFmodAudioManager::UpdFmodSystem()
{
    mSoundManager->updFFTData();
}

int32 UFmodAudioManager::playSong()
{
    FString songsPath = FPaths::ProjectContentDir() + "RawSongs/";

    //currSongName = "xiaosan.wav";
    //currSongName = "hoidaysale.mp3";
     currSongName = "A Drop A Day - Fairy Dust.wav";

    FString songFile(songsPath + currSongName);

    uint8 *songData;
    unsigned int songDatLen = 0;

    TArray<uint8> rawSongMemData;
    FFileHelper::LoadFileToArray(rawSongMemData, *songFile);

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
