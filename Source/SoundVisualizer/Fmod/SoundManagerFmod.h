/**************************************************************************
**   Copyright @ 2024 TOOTzoe.com
**   Special keywords: thor 11/30/2024 2024
**   Environment variables:
**
**
**
**   E-mail : toot@tootzeo.com
**   Tel    : 13802205042
**   Website: http://www.tootzoe.com
**
**************************************************************************/



#ifdef Q_CC_MSVC
#pragma execution_character_set("UTF-8")
#endif


//------------------------------------------------------------------------



#ifndef SOUNDMANAGERFMOD_H
#define SOUNDMANAGERFMOD_H


#include <string>
#include <vector>


namespace FMOD {

    class System;
    class Channel;
    class Sound;
    class DSP;
}

typedef std::vector<std::vector<float> > FFTHistoryContainer;

class SoundManagerFmod
{
public:
    SoundManagerFmod();
    ~SoundManagerFmod();


    int initialize();
    int loadSndFromPathname(std::string sndPathname);
    int loadSndFromMemory(char* memPtr, unsigned int memSz);
    void keepMusicPathname(std::string sndPathname);
    void playSnd();
    void stopSnd();
    void pauseSnd(bool isPause = false);


    //
    int initializeLinerSpectrum(int maxBands);
    int initializeLogSpectrum(int maxBands);
    void fetchLinerSpectrum(float* data);
    void fetchLogSpectrum(float* data);
    void updFFTData();
    unsigned int fetchPlayerCurrTimePos();
    unsigned int fetchPlayerMediaDuration();

    //
    void initializeBeatDetector();
    void getBeat(float* spectrum, float* averageSpecturm , bool& isBass , bool& isLowM);



private:

   // static FMOD_R


private:
    FMOD::System* mSystem;
    FMOD::Channel* mChannel;
    FMOD::Sound* mSound;
    FMOD::DSP* mDsp;


    int winRectSz;
    float mSampleingFreq;

    std::vector<int> mBarsLinerAlgorithm;
    std::vector<int> mBarsLogAlgorithm;




    int mFFTHistoryMaxSize;
    std::vector<int> mBeatDetector_bandLimits;
    FFTHistoryContainer mFFTHistory_linear;
    FFTHistoryContainer mFFTHistory_log;
    FFTHistoryContainer mFFTHistory_beatDetector;


    //
    //std::vector<char> currMusicPathname;
    std::string currMusicPathname;

};

#endif // SOUNDMANAGERFMOD_H
