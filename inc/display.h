#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <assert.h>

#include "../external/raylib/src/raylib.h"
#include "../external/raylib/src/raymath.h"

class Element
{
  public:
    virtual void draw() = 0;
    Vector2 get_reference_point()
    {
        return m_reference_point;
    }

    void set_reference_point(Vector2 reference_point)
    {
        m_reference_point = reference_point;
        m_position = Vector2Add(m_reference_point, m_offset);
    }

    Vector2 get_offset()
    {
        return m_offset;
    }

    void set_offset(Vector2 offset)
    {
        m_offset = offset;
        m_position = Vector2Add(m_reference_point, m_offset);
    }

    Vector2 get_position(){
        return m_position;
    }

    void set_position(Vector2 position){
        m_position = position;
        m_reference_point = position;
        m_offset = {0, 0};
    }

  public:
    

  protected:
    Vector2 m_reference_point = {0, 0};
    Vector2 m_offset = {0, 0};
    Vector2 m_position = {0, 0};
};

class TextLine : public Element
{
  public:
    TextLine(std::string text, Font font, float size) : text(text), size(size), font(font)
    {
    }
    void draw()
    {
        DrawTextEx(font, text.c_str(), m_position, size, 1.0f, color);
    }
    float get_length()
    {
        return MeasureTextEx(font, text.c_str(), size, 1.0f).x;
    }

  public:
    std::string text = "Place Holder";
    Font font = GetFontDefault();
    float size = 1.0f;
    Color color = BLACK;
    
  private:
};

class TextBox : public Element
{
  public:
    TextBox(std::string text, Font font, float size, Vector2 box) : text(text), size(size), font(font), m_box(box)
    {
        word_list = split(text, ' ');;
    }
    void draw()
    {
        BeginScissorMode(m_position.x, m_position.y, m_box.x, m_box.y);
        int current_line_length = 0;
        int current_line_number = 0;
        std::string wrapped_line;
        for(auto word: word_list){
            float length = MeasureTextEx(font, word.c_str(), size, 1.0f).x;
            if (current_line_length + length >= m_box.x){
                DrawTextEx(font, wrapped_line.c_str(),  {m_position.x, m_position.y + current_line_number * size}, size, 1.0f, color);
                current_line_number++;
                wrapped_line = "";
                current_line_length = 0;
            }
            current_line_length += length;
            wrapped_line += (word + " ");
            if (word.find("\n") != std::string::npos){
                DrawTextEx(font, wrapped_line.c_str(), {m_position.x, m_position.y + current_line_number * size}, size, 1.0f, color);
                current_line_length = 0;
                wrapped_line = "";
                current_line_number++;
            }
        }
        DrawTextEx(font, wrapped_line.c_str(), {m_position.x, m_position.y + current_line_number * size}, size, 1.0f, color);
        
        EndScissorMode();
    }
  public:
    std::string text = "Placeholder";
    Font font = GetFontDefault();
    float size = 1.0f;
    Color color = BLACK;
  private:
    template <typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::istringstream iss(s);
        std::string item;
        while (std::getline(iss, item, delim)) {
            *result++ = item;
        }
    }
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }
  private:
    Vector2 m_box;
    float m_spacing;
    bool wrap = true;
    std::vector<std::string> word_list;
};

#endif //! DISPLAY_HPP