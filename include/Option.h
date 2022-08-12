#ifndef ARCADE_MACHINE_OPTION_H
#define ARCADE_MACHINE_OPTION_H

// Options class

using namespace std;
using std::vector;

class Audio {
    public:
        Audio(){}

        void playMusic(int _currentMusic,float volume)
        {
            play_music(to_string(_currentMusic));
            set_music_volume(volume);
        }  

        void setVolume(float volume)
        {
            set_music_volume(volume);
        }  
};

class Option {
    private: 
        int m_displayStyle=1;
        int _selector=1;
        bool m_isSelected=false;
        int m_currentMusic=1;
        int m_insideSeletor=1;
        float m_volume=40;
        bool m_isOptionOpen=true;

    public:
        Option(){}

        float getVolume()
        { 
            return m_volume / 100;
        }
        
        void volumeControl()
        {
            if(_selector == 2 && m_isSelected && key_typed(RIGHT_KEY) && m_volume < 100 && m_insideSeletor == 1)
                m_volume += 20;

            if(_selector ==2 && m_isSelected && key_typed(LEFT_KEY) && m_volume > 0 && m_insideSeletor == 1)
                m_volume -= 20;
        }

        void setCurrentMusic()
        {
            if(_selector == 2 && m_isSelected && m_volume < 100 && m_insideSeletor == 2)
            {
                if(key_typed(LEFT_KEY) && m_currentMusic > 1)
                    m_currentMusic = m_currentMusic-1;
                if(key_typed(RIGHT_KEY) && m_currentMusic < 3)
                    m_currentMusic = m_currentMusic+1;
            }
        }

        int getCurrentMusic()
        {
            return m_currentMusic;
        }

        void changeDisplay()
        {
            if(_selector == 3 && m_isSelected)
            {
                switch (m_displayStyle) {
                    case 1:
                        if (key_typed(RIGHT_KEY))
                            m_displayStyle = 2;
                        else if (key_typed(DOWN_KEY))
                            m_displayStyle = 3;
                    break;
                    case 2:
                        if (key_typed(DOWN_KEY))
                            m_displayStyle = 4;
                        else if(key_typed(LEFT_KEY))
                            m_displayStyle = 1;
                    break;
                    case 3:
                        if (key_typed(UP_KEY))
                            m_displayStyle = 1;
                        else if (key_typed(RIGHT_KEY))
                            m_displayStyle = 4;
                    break;
                }
            }
        }
        bool isChangeMusic()
        {
            if(_selector==2&&m_isSelected&&m_volume<100&&m_insideSeletor==2&&(key_typed(LEFT_KEY)||key_typed(RIGHT_KEY))){return true;}
            else return false;
        }
        bool isChangeVoLume()
        {
            if(_selector==2&&m_isSelected&&m_volume<100&&m_insideSeletor==1&&(key_typed(LEFT_KEY)||key_typed(RIGHT_KEY))){return true;}
            else return false;
        }
        
        void changeSelector()
        {
            if(key_typed(P_KEY))
            {
                if(m_isSelected==false)
                {
                    m_isSelected=true;
                }
                else if(m_isSelected==true)
                {
                    m_isSelected=false;
                }
                
            }                    

            if(key_typed(DOWN_KEY)&&_selector<4 && !m_isSelected)
            {
                _selector=_selector+1;
            }

            if(key_typed(UP_KEY)&&_selector>1 && !m_isSelected)
            {
                _selector=_selector-1;
            }

            if(_selector==2&&m_isSelected==true&&m_insideSeletor==1)
            {
                if(key_typed(DOWN_KEY))
                {
                    m_insideSeletor=2;
                }
            }

            if(_selector==2&&m_isSelected==true&&m_insideSeletor==2)
            {
                if(key_typed(UP_KEY))
                {
                    m_insideSeletor=1;
                }
            }             
        }

        void updateOption()
        {
            if(m_isOptionOpen)
            {
                changeSelector();
                changeDisplay();
                setCurrentMusic();
                volumeControl();
                drawIntinialHub();
            }

            // if(!_isOptionOpen&&key_typed(O_KEY)){_isOptionOpen=true;}
            // if(_isOptionOpen&&key_typed(O_KEY)){_isOptionOpen=false;}

            if(key_typed(O_KEY))
            {
                m_isOptionOpen=!m_isOptionOpen;
            }
        }

        void drawIntinialHub()
        {
            draw_bitmap("back_ground", bitmap_width("backCurrentGame"), 0);

            if(_selector==1)
            {
                draw_bitmap("backCurrentGame", 0, 0);
                draw_bitmap("sound_notSelected", 0, bitmap_height("backCurrentGame"));
                // draw_bitmap();
                draw_bitmap("backMenu_notSelected", 0, 3*bitmap_height("backCurrentGame"));
                draw_text("RESET DEFAULT", color_red(), 400, 100);
                draw_text("yes", color_red(), 300, 300);
                draw_text("no", color_red(), 700, 300);
            }

            if(_selector==2)
            {
                draw_bitmap("changeSound",0,bitmap_height("backCurrentGame"));
                draw_bitmap("backGame_notSelected",0,0);
                // draw_bitmap();
                draw_bitmap("backMenu_notSelected",0,3*bitmap_height("backCurrentGame"));
                
                fill_rectangle(color_red(),bitmap_width("backCurrentGame")+200,200,screen_width()-bitmap_width("backCurrentGame")-400,50);
                double a=screen_width()-bitmap_width("backCurrentGame")-400;
                double b=a/100*m_volume;
                fill_rectangle(color_yellow(),bitmap_width("backCurrentGame")+200,200,b,50);
                draw_text(to_string(screen_width()-bitmap_width("backCurrentGame")),color_red(),300,300);

                if (m_insideSeletor==1)
                {
                    draw_text("CHANGE SOUND",color_white(),bitmap_width("backCurrentGame")+100,200);
                }

                if(m_insideSeletor==2)
                {
                    draw_text("CHANGE MUSIC",color_white(),bitmap_width("backCurrentGame")+100,500);
                    draw_text(to_string(m_currentMusic),color_white(),bitmap_width("backCurrentGame")+250,500);
                }
            }

            if(_selector==3)
            {
                double x=screen_width();
                double y=screen_height();
                double a=bitmap_width("backCurrentGame");
                double rec_width=(x-a-450)/2;
                double rec_height=(y-450)/2;
                double first_column_x=150+a;
                double secong_column_x=a+300+rec_width;
                double first_row_y=150;
                double second_row_y=300+rec_height;
                fill_rectangle(color_white(),first_column_x,first_row_y,rec_width,rec_height);
                fill_rectangle(color_white(),secong_column_x,first_row_y,rec_width,rec_height);
                fill_rectangle(color_white(),first_column_x,second_row_y,rec_width,rec_height);
                fill_rectangle(color_white(),secong_column_x,second_row_y,rec_width,rec_height);
                if(m_displayStyle==1){fill_rectangle(color_red(),first_column_x,first_row_y,rec_width,rec_height);}
                if(m_displayStyle==2){fill_rectangle(color_red(),secong_column_x,first_row_y,rec_width,rec_height);}
                if(m_displayStyle==3){fill_rectangle(color_red(),first_column_x,second_row_y,rec_width,rec_height);}
                if(m_displayStyle==4){fill_rectangle(color_red(),secong_column_x,second_row_y,rec_width,rec_height);}
            }

            if(_selector==4)
            {
                draw_bitmap("backMenu",0,3*bitmap_height("backCurrentGame"));
                draw_bitmap("sound_notSelected",0,bitmap_height("backCurrentGame"));
                // draw_bitmap();
                draw_bitmap("backGame_notSelected",0,0);
            }
        }
};

#endif