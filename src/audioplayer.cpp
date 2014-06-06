#include "audioplayer.hpp"

AudioPlayer::AudioPlayer(fea::MessageBus& bus) :
    mBus(bus)
{
    mBus.addSubscriber<SoundMessage>(*this);
    mBus.addSubscriber<SongPlayingMessage>(*this);

    fea::AudioFile file;

    file.open("audio/adder.ogg");
    mAdderSample.loadSampleData(file);
    file.open("audio/subber.ogg");
    mSubberSample.loadSampleData(file);
    file.open("audio/die.ogg");
    mDieSample.loadSampleData(file);

    mAdder.setSample(mAdderSample);
    mSubber.setSample(mSubberSample);
    mDie.setSample(mDieSample);

    mSong.openFile("audio/song.ogg");
    mSong.setLooping(true);
}

AudioPlayer::~AudioPlayer()
{
    mBus.removeSubscriber<SoundMessage>(*this);
    mBus.removeSubscriber<SongPlayingMessage>(*this);
}

void AudioPlayer::update()
{
    mAudioPlayer.update();
}

void AudioPlayer::handleMessage(const SoundMessage& message)
{
    Sound sound = std::get<0>(message.mData);

    switch(sound)
    {
        case ADDER:
            mAdderHandle = mAudioPlayer.play(mAdder);
            break;
        case SUBBER:
            mSubberHandle = mAudioPlayer.play(mSubber);
            break;
        case DIE:
            mDieHandle = mAudioPlayer.play(mDie);
            break;
    }
}


void AudioPlayer::handleMessage(const SongPlayingMessage& message)
{
    bool playing = std::get<0>(message.mData);

    if(playing)
    {
        mAudioPlayer.stop(mSongHandle);
        mAudioPlayer.stop(mAdderHandle);
        mAudioPlayer.stop(mSubberHandle);
        mAudioPlayer.stop(mDieHandle);
        mSongHandle = mAudioPlayer.play(mSong);
    }
    else
        mAudioPlayer.stop(mSongHandle);
}
