#include "audioplayer.hpp"

AudioPlayer::AudioPlayer(fea::MessageBus& bus) :
    mBus(bus)
{
    mBus.addSubscriber<SoundMessage>(*this);

    mAdderBuf.loadFromFile("audio/adder.ogg");
    mSubberBuf.loadFromFile("audio/subber.ogg");

    mAdder.setBuffer(mAdderBuf);
    mSubber.setBuffer(mSubberBuf);
}

AudioPlayer::~AudioPlayer()
{
    mBus.removeSubscriber<SoundMessage>(*this);
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
