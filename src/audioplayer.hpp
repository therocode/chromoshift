#pragma once
#include "messages.hpp"
#include <SFML/Audio.hpp>

class AudioPlayer :
    public SoundMessageReceiver,
    public SongPlayingMessageReceiver
{
    public:
        AudioPlayer(fea::MessageBus& bus);
        ~AudioPlayer();
        virtual void handleMessage(const SoundMessage& message) override;
        virtual void handleMessage(const SongPlayingMessage& message) override;
    private:
        fea::MessageBus& mBus;
        sf::SoundBuffer mAdderBuf;
        sf::Sound mAdder;
        sf::SoundBuffer mSubberBuf;
        sf::Sound mSubber;
        sf::SoundBuffer mDieBuf;
        sf::Sound mDie;
        sf::Music mSong;
};
