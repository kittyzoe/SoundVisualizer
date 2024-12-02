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



class SoundManagerFmod
{
public:
    SoundManagerFmod();
    ~SoundManagerFmod();


    int initialize();
    int loadSndFromPathname(std::string sndPathname);
    int loadSndFromMemory(char* memPtr, unsigned int memSz);
    void playSnd();
    void pauseSnd(bool isPause = false);

    //
    int initializeLinerSpectrum(int maxBands);
    int initializeLogSpectrum(int maxBands);
    void fetchLinerSpectrum(float* data);
    void fetchLogSpectrum(float* data);
    void updFFTData();



private:
    FMOD::System* mSystem;
    FMOD::Channel* mChannel;
    FMOD::Sound* mSound;
    FMOD::DSP* mDsp;


    int winRectSz;

    std::vector<int> mBarsLinerAlgorithm;
    std::vector<int> mBarsLogAlgorithm;
};

#endif // SOUNDMANAGERFMOD_H
