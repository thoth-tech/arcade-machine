#ifndef ARCADE_MACHINE_AUDIO_H
#define ARCADE_MACHINE_AUDIO_H

#include <string>

class Audio {
    private:
        vector<music> m_music;
        int songId;
    public:
        Audio()
        {
            getAllMusic();
        }

        void setSongId(int id)
        {
            this->songId = id;
        }

        void playMusic(float volume)
        {
            play_music(m_music[0]);
            set_music_volume(volume);
            setSongId(0);
        }

        void playMusic(std::string song, float volume)
        {
            play_music(song);
            set_music_volume(volume);
            music thisSong = music_named(song);

            for (int i = 0; i < this->m_music.size(); i++)
            {
                if (thisSong == this->m_music[i])
                {
                    write_line("same song");
                    setSongId(i);
                }
            }
        }

        void playMusic(int _currentMusic, float volume)
        {
            play_music(std::to_string(_currentMusic));
            set_music_volume(volume);
        }

        void playNextSong()
        {
            if (this->songId != (this->m_music.size() - 1))
                play_music(this->m_music[this->songId + 1]);
            else
                play_music(this->m_music[0]);
        }

        void setVolume(float volume)
        {
            set_music_volume(volume);
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
