#ifndef ARCADE_MACHINE_AUDIO_H
#define ARCADE_MACHINE_AUDIO_H

#include <string>

class Audio {
    private:
        vector<music> m_music;
        int songId = 4;
        float volume = 5;
    public:
        Audio()
        {
        }

        void setSongId(int id)
        {
            if(this->songId != id){
                this->songId = id;
                stop_music();
                playMusic(id);
            }
        }
        void playMusic(int _currentMusic)
        {
            if(!music_playing()){
                play_music(std::to_string(_currentMusic));
                set_music_volume(this->volume/10);
            }
        }  
        void set_volume_for_machine(){
            set_music_volume(this->volume/10);
        }
        void setVolume(float volume)
        {
            this->volume = volume;
        }
        float getVolume(){
            return this->volume;
        }
        int getCurrentMusic(){
            return songId;
        }
        
};

#endif
