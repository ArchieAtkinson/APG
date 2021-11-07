#include <cmath>
#include <iostream>
#include <string>

#include "../external/raylib/src/raylib.h"
#include "../external/raylib/src/raymath.h"

#include "../external/argparse/include/argparse/argparse.hpp"

#include "../inc/album_info.hpp"
#include "../inc/display.hpp"

int screenWidth = 1080;
int screenHeight = 1920;

const int posterWidth = 3508;
const int posterHeight = 4961;

const int border = int(posterWidth * 0.05);
const int line_thickness = static_cast<int>(static_cast<float>(border) * 0.05f);
const int cover_name_gap = border * 1.1f;
const int num_tracks_per_col = 10;

RenderTexture viewport;
Texture2D create_album_cover_texture(std::string filepath)
{
    const char *filepath_c = filepath.c_str();
    auto img = LoadImage(filepath_c);
    int img_size = posterWidth - (border * 2);
    ImageResize(&img, img_size, img_size);
    auto texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}


int main(int argc, char const *argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------

    argparse::ArgumentParser program("APG");
    program.add_argument("artwork");
    program.add_argument("info");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error &err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    auto cover_art_path = program.get<std::string>("artwork");
    auto info_file_path = program.get<std::string>("info");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "APG");
    MaximizeWindow();
    SetTargetFPS(1);

    viewport = LoadRenderTexture(posterWidth, posterHeight);

    Font font_black = LoadFontEx("resources/Urbanist-Black.ttf", 256, 0, 0);
    Font font_bold = LoadFontEx("resources/Urbanist-Bold.ttf", 256, 0, 0);

    auto cover_art_tex = create_album_cover_texture(cover_art_path);

    auto album_info = AlbumInfo(info_file_path);

    std::cout << album_info.track_list << std::endl;

    while (!WindowShouldClose()) // Detect window close button or ESC ey
    {
        BeginTextureMode(viewport);

        DrawRectangle(0, 0, posterWidth, posterHeight, RAYWHITE);

        DrawTexture(cover_art_tex, border, border, WHITE);

        TextLine album_name(album_info.album_name, font_black, posterWidth * 0.075f);
        album_name.set_reference_point({border, border + cover_art_tex.height});
        album_name.set_offset({0, album_name.size * 0.1f});
        album_name.draw();

        TextLine artist_name(album_info.artist_name, font_bold, posterWidth * 0.035f);
        artist_name.set_reference_point(album_name.position);
        artist_name.set_offset({0, album_name.size});
        artist_name.draw();

        Vector2 line_start{border, artist_name.position.y + artist_name.size * 1.1};
        Vector2 line_end{posterWidth - border, line_start.y};
        DrawLineEx(line_start, line_end, line_thickness, BLACK);

        TextLine album_date(album_info.album_date, font_bold, posterWidth * 0.025f);
        album_date.set_reference_point({posterWidth - border, line_end.y});
        album_date.set_offset({ -album_date.get_length(), -(artist_name.size * 0.1 + album_date.size)});
        album_date.draw();

        TextLine album_duration(album_info.album_duration, font_bold, posterWidth * 0.025f);
        album_duration.set_reference_point(album_date.set_reference_point());
        album_duration.set_offset({ -album_duration.get_length(), artist_name.size * 0.1});
        album_duration.draw();

        double space_for_track_list = (posterHeight - border) - (line_start.y + artist_name.size * 0.1f);
        int track_font_size = static_cast<int>(round(space_for_track_list / static_cast<double>(num_tracks_per_col)));
        Vector2 track_base_pos{border, line_start.y + artist_name.size * 0.1f};
        Vector2 track_offset_pos{0, 0};
        for (auto i = 0; i < album_info.track_names.size(); i++)
        {
            const char *track_cstr = album_info.track_names[i].c_str();
            Vector2 track_pos = Vector2Add(track_base_pos, track_offset_pos);
            DrawTextEx(font_bold, track_cstr, track_pos, track_font_size, 1.0f, BLACK);
            track_offset_pos.y += track_font_size;
            if (i == (num_tracks_per_col - 1))
            {
                track_offset_pos.y = 0;
                track_offset_pos.x = posterWidth / 2.0f;
            }
        }

        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        float display_height = static_cast<float>(GetScreenHeight());
        float display_width = static_cast<float>(GetScreenHeight()) / 1.414f;

        DrawTexturePro(viewport.texture, {0, 0, posterWidth, -posterHeight},
                       {(GetScreenWidth() - display_width) / 2, 0, display_width, display_height}, {0, 0}, 0.0f, WHITE);
        DrawRectangleLinesEx({(GetScreenWidth() - display_width) / 2, 0, display_width, display_height}, 3, BLACK);

        EndDrawing();

        //----------------------------------------------------------------------------------
    }

#ifndef DEBUG
    Image viewport_img = LoadImageFromTexture(viewport.texture);
    ImageFlipVertical(&viewport_img);
    const char *output_file_name =
        TextFormat("%s - %s.png", album_info.album_name.c_str(), album_info.artist_name.c_str());
    ExportImage(viewport_img, output_file_name);
    UnloadImage(viewport_img);
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}