#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "raylib.h"
#include "raymath.h"

const int screenWidth = 2880;
const int screenHeight = int(screenWidth * 1.414f);
const int border = int(screenWidth * 0.05);

class AlbumInfo {
public:
  std::string album_name;
  std::string artist_name;
  std::string album_date;
  std::string album_length_mins;
  std::vector<std::string> track_names;

  AlbumInfo(const char* file_path) {
    std::ifstream file;
    file.open(file_path);
    if (!file) {
      std::cout << "Unable to open file";
      return;
    }
    std::string line;
    int line_count = 0;
    while (std::getline(file, line)) {
        switch (line_count) {
          case 0: {
            this->artist_name = line;
            break;
          }
          case 1: {
            this->album_name = line;
            break;
          }
          case 2: {
            this->album_date = line;
            break;
          }
          case 3: {
            this->album_length_mins = line;
            break;
          }
          default: {
            track_names.push_back(std::to_string(line_count - 3) + ". " + line);
            break;
          }
        }
        line_count++;
      }
      file.close();
  }
};

int main() {
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(screenWidth, screenHeight, "APG");

  Font font_black = LoadFontEx("resources/Urbanist-Black.ttf", 256, 0, 0);
  Font font_bold = LoadFontEx("resources/Urbanist-Bold.ttf", 256, 0, 0);
  Font font_reg = LoadFontEx("resources/Urbanist-SemiBold.ttf", 256, 0, 0);

  SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

  auto cover_art_img = LoadImage("data/folklore.jpg");
  int img_size = screenWidth - (border * 2);
  ImageResize(&cover_art_img, img_size, img_size);
  auto cover_art_tex = LoadTextureFromImage(cover_art_img);

  auto album_info = AlbumInfo("data/folklore.txt");

  

  while (!WindowShouldClose())  // Detect window close button or ESC key
  {
    // Draw5
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawTexture(cover_art_tex, border, border, WHITE);

    const char* album_name = album_info.album_name.c_str();
    int album_font_size = int(screenWidth * 0.075f);
    Vector2 album_pos{border, (1.1 * border) + img_size};
    DrawTextEx(font_black, album_name, album_pos, album_font_size , 1.0f, BLACK);

    const char* artist_name = album_info.artist_name.c_str();
    int artist_font_size = int(screenWidth * 0.035f);
    Vector2 artist_pos{border, album_pos.y + album_font_size};
    DrawTextEx(font_bold, artist_name, artist_pos, artist_font_size, 1.0f, BLACK);

    const char* album_date = album_info.album_date.c_str();
    int date_font_size = int(screenWidth * 0.030f);
    Vector2 date_pos{screenWidth - border - date_font_size * 5, album_pos.y + album_font_size*1.1};
    DrawTextEx(font_bold, album_date, date_pos, date_font_size, 1.0f, BLACK);

    Vector2 line_start{border, artist_pos.y + artist_font_size + 10};
    Vector2 line_end{screenWidth - border, line_start.y};
    DrawLineEx(line_start, line_end, 5.0f, BLACK);


    int track_font_size = int(screenWidth * 0.025f);
    Vector2 track_base_pos{border, line_start.y + 10};
    Vector2 track_offset_pos{0, 0};
    for (auto i = 0; i < album_info.track_names.size(); i++) {
      const char* track_cstr = album_info.track_names[i].c_str();
      Vector2 track_pos = Vector2Add(track_base_pos, track_offset_pos);
      DrawTextEx(font_reg, track_cstr, track_pos, track_font_size, 1.0f, BLACK);
      track_offset_pos.y += track_font_size;
      if (track_pos.y > (screenHeight - border - track_font_size)){
        track_offset_pos.y = 0;
        track_offset_pos.x = screenWidth/2.0f;
      }
    }
      EndDrawing();

      
      //----------------------------------------------------------------------------------
  }
  TakeScreenshot("test.png");
  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow();  // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}