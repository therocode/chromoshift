#pragma once
#include "messages.hpp"
#include <SFML/Audio.hpp>

class AudioPlayer :
    public SoundMessageReceiver
{
    public:
        AudioPlayer(fea::MessageBus& bus);
        ~AudioPlayer();
        virtual void handleMessage(const SoundMessage& message) override;
    private:
        fea::MessageBus& mBus;
        sf::SoundBuffer mAdderBuf;
        sf::Sound mAdder;
        sf::SoundBuffer mSubberBuf;
        sf::Sound mSubber;
};
