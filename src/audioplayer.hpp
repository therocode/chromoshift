#pragma once
#include "messages.hpp"
#include <fea/audio.hpp>

class AudioPlayer :
    public SoundMessageReceiver,
    public SongPlayingMessageReceiver
{
    public:
        AudioPlayer(fea::MessageBus& bus);
        ~AudioPlayer();
        void update();
        void handleMessage(const SoundMessage& message) override;
        void handleMessage(const SongPlayingMessage& message) override;
    private:
        fea::MessageBus& mBus;
        fea::AudioPlayer mAudioPlayer;
        fea::AudioSample mAdderSample;
        fea::Audio mAdder;
        fea::AudioSample mSubberSample;
        fea::Audio mSubber;
        fea::AudioSample mDieSample;
        fea::Audio mDie;
        fea::AudioFileStream mSong;
        
        fea::AudioHandle mAdderHandle;
        fea::AudioHandle mSubberHandle;
        fea::AudioHandle mDieHandle;
        fea::AudioHandle mSongHandle;
};
