#ifndef ARCADE_MACHINE_BUTTON_H
#define ARCADE_MACHINE_BUTTON_H

#include <string>
#include "splashkit.h"

/**
 * @brief Abstract Button Class
 * 
 * This abstract class is the base class for all derived classes
 * Contains three overloaded contructors and three virtual methods 
 */
class Button
{
public:
    /// This buttons ID
    int m_id;

    /// This buttons bitmap
    bitmap m_pic;

    /// This buttons sprite
    sprite m_btn;

    /// This buttons position in the window as point_2d in pixels (x,y)
    point_2d m_btnLocation;

    /// This buttons position in the window in pixels
    int m_x; 
    int m_y;

    /// This buttons centre point offset width in pixels
    int m_centreX;

    /// This buttons centre point offset height in pixels
    int m_centreY;
    std::string m_btnColor;

    /// This buttons hightlight bitmap
    bitmap m_btnHighlightPic;

    /// This buttons hightlight layer
    std::string m_btnHighlightText;

    /// This buttons image path
    std::string m_imagePath;

    /// Getters:
    auto id()       const -> const int&         { return m_id;          }
    auto pic()      const -> const bitmap&      { return m_pic;         }
    auto btn()      const -> const sprite&      { return m_btn;         }
    auto location() const -> const point_2d&    { return m_btnLocation; }
    auto x()        const -> const int&         { return m_x;           }
    auto y()        const -> const int&         { return m_y;           }
    auto centreX()  const -> const int&         { return m_centreX;     }
    auto centreY()  const -> const int&         { return m_centreY;     }
    auto color()    const -> const std::string& { return m_btnColor;    }

    /// Setters
    void setId(int id)      { this->m_id = id; }
    void setX(const int &x) { this->m_x  = x;  }
    void setY(const int &y) { this->m_y  = y;  }

    /**
    * @brief Enumeration of button types
    * 
    */
    enum Color
    {
        PLAY,
        EXIT,
        OPTS, 
        GAME,
        HOME
    };

    // Default Constructor
    Button() {}
    Button(Color c, float scale = 1);
    Button(Color c, float x, float y, int xCell, int yCell, float scale = 1);
    Button(Color c, std::string image, float scale = 1);

    // Destructor
    virtual ~Button() {}

    // Virtual fucntions
    virtual void btnImage(std::string image) = 0;
    virtual void drawButton() = 0;
    virtual std::string action(std::string keyinput = "") = 0;

    std::string btn_color(Color c);

};

#endif