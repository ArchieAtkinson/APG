#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <iostream>
#include <string>

#include "../external/raylib/src/raylib.h"
#include "../external/raylib/src/raymath.h"

class Element
{
  public:
    virtual void draw() = 0;
    void set_reference_point(Vector2 reference_point){
        m_reference_point = reference_point;
        position = Vector2Add(m_reference_point, m_offset);
    }
    void set_offset(Vector2 offset){
        m_offset = offset;
        position = Vector2Add(m_reference_point, m_offset);
    }
    Vector2 get_offset(){
        return m_offset;
    }
    Vector2 set_reference_point(){
        return m_reference_point;
    }
  public:
    Vector2 position = {0, 0};

  protected:
    Vector2 m_reference_point = {0, 0};
    Vector2 m_offset = {0, 0};

};

class TextLine : public Element
{
  public:
    TextLine(std::string text, Font font, float size) : text(text), size(size), font(font){}
    void draw(){
        DrawTextEx(font, text.c_str(), position, size, 1.0f, color);
    }
    float get_length(){
        return MeasureTextEx(font, text.c_str(), size, 1.5f).x;
    }
  public:
    std::string text = "Place Holder";
    Font font = GetFontDefault();  
    float size = 1.0f;
    Color color = BLACK;
};


#endif //!DISPLAY_HPP