// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundVisualizer/Public/FmodAudioManager.h"


#include <Misc/Paths.h>
#include <Misc/FileHelper.h>
#include <string>

FSongPlayFinished UFmodAudioManager::TootSongPlayEndDelegate;

UFmodAudioManager::UFmodAudioManager()
    : mCurrSongName(""),
      mOldSongName("")
{

#if 0
    TootSongPlayEndDelegate.AddLambda([ ](const FString& txt){  // this is a method
        UE_LOG(LogTemp, Warning, TEXT("TXT is : %s") , *txt);
        thizClz->SongPlayEnd(txt);

    });
#else

     TootSongPlayEndDelegate.Clear();
    TootSongPlayEndDelegate.AddUniqueDynamic(this, &UFmodAudioManager::SongPlayEndFMod);


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

int32 UFmodAudioManager::FetchMediaDuration(const FString &Pathname)
{
    return mSoundManager->fetchPlayerMediaDurationOnly(TCHAR_TO_UTF8(*Pathname));
}

void UFmodAudioManager::UpdFmodSystem()
{
    mSoundManager->updFFTData();
}

int32 UFmodAudioManager::PlaySong(const FString &Pathname)
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

  //  wcscmp(  (wchar_t*)  mCurrSongName.Data.AllocatorInstance.Data , L"" ) == 0;

    mSoundManager->playSnd();
    mCurrSongName = Pathname;

#if 0
    std::string tmpPn = TCHAR_TO_UTF8(*Pathname);

    std::vector<char> tmpStr ( tmpPn.begin(),  tmpPn.end());  // copy, has 2 strings in memory
    tmpStr.push_back('\0');

    oldSongName = FString( UTF8_TO_TCHAR( tmpStr.data() ) );
#endif

    return 0;
}

void UFmodAudioManager::StopSong(const FString &Pathname)
{
     mSoundManager->stopSnd();
}

void UFmodAudioManager::PauseSong(bool isPause)
{
    mSoundManager->pauseSnd(isPause);
}

void UFmodAudioManager::SongPlayEndFMod()
{
    unsigned int timePos = mSoundManager->fetchPlayerCurrTimePos();
    unsigned int duration = mSoundManager->fetchPlayerMediaDuration();

    if(timePos / 10 == duration / 10){
        //UE_LOG(LogTemp, Warning, TEXT("old song: %s") , *mOldSongName);
        // UE_LOG(LogTemp, Warning, TEXT("curr song: %s , pos ms: %ld") , *mCurrSongName , timePos);
        //  UE_LOG(LogTemp, Warning, TEXT("old song: ..... ..")  );

        SongPlayEnd(mOldSongName);
    }
}

// const FString &UFmodAudioManager::GetSongName() const
// {
//     return currSongName;
// }

void UFmodAudioManager::SongPlayEndEvt() // this is static func
{
      TootSongPlayEndDelegate.Broadcast(  );
}
