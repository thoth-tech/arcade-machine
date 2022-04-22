using namespace std;
using std::vector;
class Audio{
    public:
        Audio(){
        };
        void playMusic(int _currentMusic,float volume)
        {
            play_music(to_string(_currentMusic));
            set_music_volume(volume);
        }  
        void setVolume(float volume){
            set_music_volume(volume);
        }  
};
class Option{
    private: 

        bool _isGameRunning=false;
        bool _isChosen=false;
        int _displayStyle=1;
        int _selector=1;
        bool _isSelected=false;
        int _currentMusic=1;
        int _insideSeletor=1;
        float _volume=40;
        bool _isOptionOpen=false;
    
    public:
        Option(){

        };

        float getVolumn(){return _volume/100;}
        
        void VolumeControl(){
             if(_selector==2&&_isSelected&&key_typed(RIGHT_KEY)&&_volume<100&&_insideSeletor==1)
            {
                _volume=_volume+20;
            }
        if(_selector==2&&_isSelected&&key_typed(LEFT_KEY)&&_volume>0&&_insideSeletor==1)
        {
            
            _volume=_volume-20;
            
        }
        }
        void setCurrentMusic(){
            if(_selector==2&&_isSelected&&_volume<100&&_insideSeletor==2)
            {
            if(key_typed(LEFT_KEY)&&_currentMusic>1){
                _currentMusic=_currentMusic-1;
            }
            if(key_typed(RIGHT_KEY)&&_currentMusic<3){
                _currentMusic=_currentMusic+1;
            }
            }
        }
        int getCurrentMusic()
        {
            return _currentMusic;
        }
        void ChangeDisPlay(){
            if(_selector==3&&_isSelected){
                if(_displayStyle==1){
                    if(key_typed(RIGHT_KEY)){_displayStyle=2;}
                    if(key_typed(DOWN_KEY)){_displayStyle=3;}
                }
                if(_displayStyle==2){
                    if(key_typed(DOWN_KEY)){_displayStyle=4;}
                    if(key_typed(LEFT_KEY)){_displayStyle=1;}
                }
                if(_displayStyle==3){
                    if(key_typed(UP_KEY)){_displayStyle=1;}
                    if(key_typed(RIGHT_KEY)){_displayStyle=4;}
                }
                if(_displayStyle==4){
                    if(key_typed(UP_KEY)){_displayStyle=2;}
                    if(key_typed(LEFT_KEY)){_displayStyle=3;}
                }
            }
        }
        bool isChangeMusic(){
            if(_selector==2&&_isSelected&&_volume<100&&_insideSeletor==2&&(key_typed(LEFT_KEY)||key_typed(RIGHT_KEY))){return true;}
            else return false;
        }
        bool isChangeVoLume(){
            if(_selector==2&&_isSelected&&_volume<100&&_insideSeletor==1&&(key_typed(LEFT_KEY)||key_typed(RIGHT_KEY))){return true;}
            else return false;
        }
        
        void changeSelector()
        {
            if(key_typed(P_KEY)){
                if(_isSelected==false){
                    _isSelected=true;
                    
                }
                else if(_isSelected==true){
                    _isSelected=false;

                }
                
            }                        
            if(key_typed(DOWN_KEY)&&_selector<4 && !_isSelected){
                _selector=_selector+1;
                
            }
            if(key_typed(UP_KEY)&&_selector>1 && !_isSelected){
                _selector=_selector-1;
                
            }
            if(_selector==2&&_isSelected==true&&_insideSeletor==1)
            {
                if(key_typed(DOWN_KEY)){
                    _insideSeletor=2;
                };
            }
            if(_selector==2&&_isSelected==true&&_insideSeletor==2)
            {
                if(key_typed(UP_KEY)){
                    _insideSeletor=1;
                };
            }             
        }
        void updateOption(){
            if(_isOptionOpen){
            changeSelector();
            ChangeDisPlay();
            setCurrentMusic();
            VolumeControl();
            drawIntinialHub();}
            // if(!_isOptionOpen&&key_typed(O_KEY)){_isOptionOpen=true;}
            // if(_isOptionOpen&&key_typed(O_KEY)){_isOptionOpen=false;}
            if(key_typed(O_KEY)){_isOptionOpen=!_isOptionOpen;}
        }
        void drawIntinialHub(){
            
            draw_bitmap("back_ground",bitmap_width("backCurrentGame"),0);
            if(_selector==1){
                draw_bitmap("backCurrentGame",0,0);
                draw_bitmap("sound_notSelected",0,bitmap_height("backCurrentGame"));
                // draw_bitmap();
                draw_bitmap("backMenu_notSelected",0,3*bitmap_height("backCurrentGame"));
                draw_text("RESET DEFAULT",color_red(),400,100);
                draw_text("yes",color_red(),300,300);
                draw_text("no",color_red(),700,300);
            }
            if(_selector==2)
            {
                
                draw_bitmap("changeSound",0,bitmap_height("backCurrentGame"));
                draw_bitmap("backGame_notSelected",0,0);
                // draw_bitmap();
                draw_bitmap("backMenu_notSelected",0,3*bitmap_height("backCurrentGame"));
                
                fill_rectangle(color_red(),bitmap_width("backCurrentGame")+200,200,screen_width()-bitmap_width("backCurrentGame")-400,50);
                double a=screen_width()-bitmap_width("backCurrentGame")-400;
                double b=a/100*_volume;
                fill_rectangle(color_yellow(),bitmap_width("backCurrentGame")+200,200,b,50);
                draw_text(to_string(screen_width()-bitmap_width("backCurrentGame")),color_red(),300,300);
                if (_insideSeletor==1){
                    draw_text("CHANGE SOUND",color_white(),bitmap_width("backCurrentGame")+100,200);
                    
                }
                if(_insideSeletor==2){
                    draw_text("CHANGE MUSIC",color_white(),bitmap_width("backCurrentGame")+100,500);
                    draw_text(to_string(_currentMusic),color_white(),bitmap_width("backCurrentGame")+250,500);
                }
                

            }
            if(_selector==3){
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
                if(_displayStyle==1){fill_rectangle(color_red(),first_column_x,first_row_y,rec_width,rec_height);}
                if(_displayStyle==2){fill_rectangle(color_red(),secong_column_x,first_row_y,rec_width,rec_height);}
                if(_displayStyle==3){fill_rectangle(color_red(),first_column_x,second_row_y,rec_width,rec_height);}
                if(_displayStyle==4){fill_rectangle(color_red(),secong_column_x,second_row_y,rec_width,rec_height);}
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

