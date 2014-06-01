#include "audioplayer.hpp"

AudioPlayer::AudioPlayer(fea::MessageBus& bus) :
    mBus(bus)
{
    mBus.addSubscriber<SoundMessage>(*this);
    mBus.addSubscriber<SongPlayingMessage>(*this);

    mAdderBuf.loadFromFile("audio/adder.ogg");
    mSubberBuf.loadFromFile("audio/subber.ogg");

    mAdder.setBuffer(mAdderBuf);
    mSubber.setBuffer(mSubberBuf);

    mSong.openFromFile("audio/song.ogg");
    mSong.setLoop(true);
}

AudioPlayer::~AudioPlayer()
{
    mBus.removeSubscriber<SoundMessage>(*this);
    mBus.removeSubscriber<SongPlayingMessage>(*this);
}

void AudioPlayer::handleMessage(const SoundMessage& message)
{
    Sound sound = std::get<0>(message.mData);

    switch(sound)
    {
        case ADDER:
            mAdder.play();
            break;
        case SUBBER:
            mSubber.play();
            break;
    }
}

void AudioPlayer::handleMessage(const SongPlayingMessage& message)
{
    bool playing = std::get<0>(message.mData);

    if(playing)
    {
        mSong.stop();
        mSong.play();
    }
    else
        mSong.stop();
}
