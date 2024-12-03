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



#include "SoundManagerFmod.h"

#include <fmod.hpp>

#include <string>

using namespace std;

SoundManagerFmod::SoundManagerFmod()
    : mSystem(nullptr),
      mChannel(nullptr),
      mSound(nullptr),
      mDsp(nullptr),
      winRectSz(1024),
      mSampleingFreq(0),
      mFFTHistoryMaxSize(0)
{

}


SoundManagerFmod::~SoundManagerFmod()
{
    if(mSound){
        mChannel->removeDSP(mDsp);
        mSound->release();
    }

    if(mSystem)
    {
        mDsp->release();
        mSystem->close();
        mSystem->release();
    }
}

int SoundManagerFmod::initialize()
{
    FMOD_RESULT rsl =   FMOD::System_Create(&mSystem);
    if(rsl != FMOD_OK)
    {
        return rsl;
    }else{
        mSystem->init(1,FMOD_INIT_NORMAL, nullptr);
        mSystem->createDSPByType(FMOD_DSP_TYPE_FFT, &mDsp);
        mDsp->setParameterInt(FMOD_DSP_FFT_WINDOWTYPE , FMOD_DSP_FFT_WINDOW_TRIANGLE);
        mDsp->setParameterInt(FMOD_DSP_FFT_WINDOWSIZE , winRectSz);
    }

    return 0;

}

int SoundManagerFmod::loadSndFromPathname(std::string sndPathname)
{

    FMOD_RESULT rsl = mSystem->createSound(sndPathname.c_str(), FMOD_LOOP_NORMAL | FMOD_CREATESAMPLE, 0, &mSound );
    return rsl;
}

int SoundManagerFmod::loadSndFromMemory(char *memPtr, unsigned int memSz)
{
    FMOD_CREATESOUNDEXINFO sndInfo = {0};
    sndInfo.cbsize = sizeof(sndInfo);
    sndInfo.length = memSz;


    FMOD_RESULT rsl = mSystem->createSound(memPtr, FMOD_OPENMEMORY | FMOD_LOOP_NORMAL , &sndInfo, &mSound);
    return rsl;
}

void SoundManagerFmod::playSnd()
{
    mSystem->playSound(mSound , 0 , false, &mChannel );

    mChannel->getFrequency(&mSampleingFreq);
    mFFTHistoryMaxSize = mSampleingFreq / winRectSz;

    mChannel->addDSP(0, mDsp);
    mDsp->setActive(true);

}

void SoundManagerFmod::pauseSnd(bool isPause)
{
    bool isPaused;
    mChannel->getPaused(&isPaused);

    if(isPaused && !isPause)
    {
        mChannel->setPaused(false);

    }else if(!isPaused && isPause){
        mChannel->setPaused(true);

    }

}

int SoundManagerFmod::initializeLinerSpectrum(int maxBands)
{
    int barSampleWidth = winRectSz / 2 / maxBands;


    UE_LOG(LogTemp, Warning, TEXT("max bands : %d.........barSampleWidth = %d ") , maxBands , barSampleWidth);

    mBarsLinerAlgorithm.clear();

    for(int i = 0 ; i < barSampleWidth ; i ++){
        mBarsLinerAlgorithm.push_back(barSampleWidth);
    }

    UE_LOG(LogTemp, Warning, TEXT("mBarsLinerAlgorithm size: %d") , mBarsLinerAlgorithm.size());

    return mBarsLinerAlgorithm.size(); // effective count

}

int SoundManagerFmod::initializeLogSpectrum(int maxBands)
{

    std::vector<int>freqOctaves;

    freqOctaves.push_back(0);
    for (int i = 0; i < 13; ++i) {

        freqOctaves.push_back((int)((44100 / 2) / (float)pow(2, 12 - i)));
    }

    int bandW = 44100 / winRectSz;
    int bandPerOctave = maxBands / 12 ; // octaves

    mBarsLogAlgorithm.clear();
    for (int o = 0; o < 12; ++o) {
        int idxLow = freqOctaves[o] / bandW;
        int idxHigh = freqOctaves[o + 1] / bandW;
        int octaveIdxes = idxHigh - idxLow;

        if (octaveIdxes > 0) {
            if(octaveIdxes <= bandPerOctave){
                for (int c = 0; c < octaveIdxes; ++c) {
                    mBarsLogAlgorithm.push_back(1);
                }
            }else{
                for (int c = 0; c < bandPerOctave; ++c) {
                    mBarsLogAlgorithm.push_back(octaveIdxes / bandPerOctave);
                }
            }

        }

    }

     UE_LOG(LogTemp, Warning, TEXT("mBarsLogAlgorithm sz = %d  ")  , mBarsLogAlgorithm.size());

     return mBarsLogAlgorithm.size();
}

void SoundManagerFmod::fetchLinerSpectrum(float *data)
{
    FMOD_DSP_PARAMETER_FFT* fftData = nullptr;
    FMOD_RESULT rsl = mDsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void **)&fftData, 0,0,0);

    if(fftData){
        int len = fftData ->length / 2;
        int channels = fftData->numchannels;

       // UE_LOG(LogTemp, Warning, TEXT("len: %d chs: %d") , len, channels);

        if(len > 0){
            int idxFFT = 0;
            for(int i = 0 ; i < mBarsLinerAlgorithm.size() ; i ++){
                for(int f = 0  ; f < mBarsLinerAlgorithm[i]; f ++){
                    for(int c= 0 ;  c < channels ; c ++){
                        data[i] += fftData->spectrum[c][idxFFT];
                    }
                    idxFFT ++;
                }

                data[i] /= (float)(mBarsLinerAlgorithm[i] * channels);
            }
        }
    }

}

void SoundManagerFmod::fetchLogSpectrum(float *data)
{


   // UE_LOG(LogTemp, Warning, TEXT("mBarsLogAlgorithm.size = %d") , mBarsLogAlgorithm.size());


    FMOD_DSP_PARAMETER_FFT* fftData = nullptr;
    FMOD_RESULT rsl = mDsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA, (void**)&fftData , 0 ,  0 , 0 );




    if (fftData) {

        int len = fftData->length / 2;
        int channels = fftData->numchannels;

        if (len  > 0) {
            int idxFFT = 0 ;
            for (int idx = 0; idx < mBarsLogAlgorithm.size(); ++idx) {

                for (int f = 0; f < mBarsLogAlgorithm[idx]; ++f) {
                    for (int c = 0; c < channels; ++c) {
                        data[idx] += fftData->spectrum[c][idxFFT];
                    }
                    idxFFT ++;
                }

              data[idx] /= (float)(mBarsLogAlgorithm[idx] * channels);

            }
        }
    }

}

void SoundManagerFmod::updFFTData()
{
    // UE_LOG(LogTemp, Warning, TEXT("Nextloop here.........."));
    mSystem->update();
}

void SoundManagerFmod::initializeBeatDetector()
{
    int bandSz = mSampleingFreq / winRectSz;

    mBeatDetector_bandLimits.clear();
    mBeatDetector_bandLimits.reserve(4);  // bass + lowMidRang * 2 // call reserve first just for perfermence

    // BASS : 60Hz ~ 130Hz (kick drum)
    mBeatDetector_bandLimits.push_back(60 / bandSz);
    mBeatDetector_bandLimits.push_back(130 / bandSz);
    //
    // Low Midrange 301Hz ~ 750 Hz (snare Drum)
    mBeatDetector_bandLimits.push_back(301 / bandSz);
    mBeatDetector_bandLimits.push_back(750 / bandSz);


    mFFTHistory_beatDetector.clear();

}

static void fillAverageSpectrum(float* averageSpectrum , int numBands , const FFTHistoryContainer& fftHistory)
{
    for (FFTHistoryContainer::const_iterator it    =  fftHistory.cbegin() ; it !=   fftHistory.cend() ; ++it) {
        const std::vector<float>& tmpVal = *it;

        for (int i = 0; i < tmpVal.size(); ++i) {
            averageSpectrum[i] += tmpVal[i];

        }


        for (int i = 0; i < numBands; ++i) {
            averageSpectrum[i] /= fftHistory.size();
        }
    }
}

static void fillVarianceSpectrum(float* varianceSpectrum , int numBands, const FFTHistoryContainer& fftHistory, const float* averageSpectrum)
{
    for (FFTHistoryContainer::const_iterator it = fftHistory.cbegin(); it !=  fftHistory.cend() ; ++it) {

        const std::vector<float>& rsl = *it;

        for (int i = 0; i < rsl.size() ; ++i) {
            varianceSpectrum[i] += (rsl[i] - averageSpectrum[i] ) * (rsl[i] - averageSpectrum[i]);
        }
    }

    for (int i = 0; i < numBands; ++i) {
        varianceSpectrum[i] /= fftHistory.size();
    }
}

static float beatThreshold(float variance){
    return -15 * variance + 1.55;
}


void SoundManagerFmod::getBeat(float *spectrum, float *averageSpecturm, bool &isBass, bool &isLowM)
{
    FMOD_DSP_PARAMETER_FFT *dspFFT = nullptr;

    FMOD_RESULT rsl = mDsp->getParameterData(FMOD_DSP_FFT_SPECTRUMDATA , (void **) &dspFFT , 0,0,0);

    if(dspFFT)
    {
        int len = dspFFT->length  / 2;
        int chs  = dspFFT->numchannels;

        if( len > 0){
            int bands = mBeatDetector_bandLimits.size() / 2;

            for (int nb = 0; nb < bands; ++nb) {
                int bandBoundIdx = nb * 2;
                for (int idxFFT = mBeatDetector_bandLimits[bandBoundIdx]; idxFFT < mBeatDetector_bandLimits[bandBoundIdx + 1]; ++idxFFT) {
                    for (int c  = 0; c  < chs; ++c ) {
                        spectrum[nb] += dspFFT->spectrum[c][idxFFT];
                    }
                }
                spectrum[nb] /= (mBeatDetector_bandLimits[bandBoundIdx + 1]- mBeatDetector_bandLimits[bandBoundIdx]) * chs;

            }

            if (mFFTHistory_beatDetector.size() > 0) {
                 fillAverageSpectrum(averageSpecturm , bands , mFFTHistory_beatDetector);

                 std::vector<float> varianceSpectrum;
                 varianceSpectrum.resize(bands);

                 fillVarianceSpectrum(varianceSpectrum.data() , bands , mFFTHistory_beatDetector , averageSpecturm);
                 isBass = (spectrum[0] - .05 ) >beatThreshold(varianceSpectrum[0]) * averageSpecturm[0];
                 isLowM = (spectrum[1] - .005 ) >beatThreshold(varianceSpectrum[1]) * averageSpecturm[1];

            }

            std::vector<float> fftRsl;
            fftRsl.reserve(bands);
            for (int i = 0; i < bands; ++i) {
                fftRsl.push_back(spectrum[i]);
            }

            if(mFFTHistory_beatDetector.size() >= mFFTHistoryMaxSize){
                mFFTHistory_beatDetector.pop_back();
            }

            mFFTHistory_beatDetector.push_back(fftRsl);
        }

    }

}



