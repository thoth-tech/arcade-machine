#ifndef ARCADE_MACHINE_AUDIO_H
#define ARCADE_MACHINE_AUDIO_H

#include <string>

class Audio {
    public:
        Audio(){}

        void playMusic(int _currentMusic, float volume)
        {
            play_music(std::to_string(_currentMusic));
            set_music_volume(volume);
        }  

        void setVolume(float volume)
        {
            set_music_volume(volume);
        }  
};

#endif
