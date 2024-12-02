




#TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#
#
PRJNAMETOOT = SoundVisualizer
DEFINES += "SOUNDVISUALIZER_API="
DEFINES += "SOUNDVISUALIZER_API(...)="
#
DEFINES += "UCLASS()=SOUNDVISUALIZER_API"
DEFINES += "UCLASS(...)=SOUNDVISUALIZER_API"




INCLUDEPATH += ../Intermediate/Build/Win64/UnrealEditor/Inc/$$PRJNAMETOOT/UHT
#INCLUDEPATH += $$PRJNAMETOOT $$PRJNAMETOOT/Public $$PRJNAMETOOT/Private #don't use this,
# we should follow UE project struct to include files, start from prj.Build.cs folder
#
#  The Thirdparty libs
INCLUDEPATH += ../ThirdpartyLibs/FMOD/inc
#
#
include(defs.pri)
include(inc.pri)
#
#

DISTFILES += \
    SoundVisualizer.Target.cs \
    SoundVisualizer/SoundVisualizer.Build.cs \
    SoundVisualizerEditor.Target.cs

HEADERS += \
    SoundVisualizer/Fmod/SoundManagerFmod.h \
    SoundVisualizer/Public/FmodAudioManager.h \
    SoundVisualizer/Public/TootGameInstance.h \
    SoundVisualizer/Public/TootGameMode.h \
    SoundVisualizer/SoundVisualizer.h

SOURCES += \
    SoundVisualizer/Fmod/SoundManagerFmod.cpp \
    SoundVisualizer/Private/FmodAudioManager.cpp \
    SoundVisualizer/Private/TootGameInstance.cpp \
    SoundVisualizer/Private/TootGameMode.cpp \
    SoundVisualizer/SoundVisualizer.cpp







