#ifndef ARCADE_MACHINE_AUDIO_H
#define ARCADE_MACHINE_AUDIO_H

#include <string>

class Audio {
    private:
        vector<music> m_music;
        int songId = 4;
    public:
        Audio()
        {
            getAllMusic();
        }

        void setSongId(int id)
        {
            if(this->songId != id){
                this->songId = id;
                stop_music();
                playMusic(id,100);
            }
        }

        void playMusic(int _currentMusic,float volume)
        {
            if(!music_playing()){
                play_music(std::to_string(_currentMusic));
            }
            
            set_music_volume(volume);
        }  
        void set_song(float song);
        void setVolume(float volume)
        {
            set_music_volume(volume);
        }

        void playNextSong()
        {
            if (this->songId != (this->m_music.size() - 1))
                play_music(this->m_music[this->songId + 1]);
            else
                play_music(this->m_music[0]);
        }
        int getCurrentMusic(){
            return songId;
        }
        void getAllMusic()
        {
            m_music.push_back(music_named("music_mainmenu"));
            m_music.push_back(music_named("1"));
            m_music.push_back(music_named("2"));
            m_music.push_back(music_named("3"));
        }
};

#endif
