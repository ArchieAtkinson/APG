#include <cmath>
#include <iostream>
#include <string>

#include "../external/raylib/src/raylib.h"
#include "../external/raylib/src/raymath.h"

#include "../external/argparse/include/argparse/argparse.hpp"

#include "../inc/album_info.h"
#include "../inc/display.h"
#include "../inc/root_dir.h"

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
    program.add_argument("artwork").default_value(APG::ROOT_DIR + "/example_data/raceforspace.png");
    program.add_argument("info").default_value(APG::ROOT_DIR + "/example_data/raceforspace.txt");

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

    std::string fontpath_black = APG::ROOT_DIR + "/resources/Urbanist-Black.ttf";
    std::string fontpath_bold = APG::ROOT_DIR + "/resources/Urbanist-Bold.ttf";
    Font font_black = LoadFontEx(fontpath_black.c_str(), 256, 0, 0);
    Font font_bold = LoadFontEx(fontpath_bold.c_str(), 256, 0, 0);

    auto cover_art_tex = create_album_cover_texture(cover_art_path);
    auto album_info = AlbumInfo(info_file_path);

    while (!WindowShouldClose()) // Detect window close button or ESC ey
    {
        BeginTextureMode(viewport);

        DrawRectangle(0, 0, posterWidth, posterHeight, RAYWHITE);

        DrawTexture(cover_art_tex, border, border, WHITE);

        TextLine album_name(album_info.album_name, font_black, posterWidth * 0.075f);
        album_name.set_reference_point({border, static_cast<float>(border) + cover_art_tex.height});
        album_name.set_offset({0, album_name.size * 0.1f});
        album_name.draw();

        TextLine artist_name(album_info.artist_name, font_bold, posterWidth * 0.035f);
        artist_name.set_reference_point(album_name.get_position());
        artist_name.set_offset({0, album_name.size});
        artist_name.draw();

        Vector2 line_start{border, artist_name.get_position().y + artist_name.size * 1.1f};
        Vector2 line_end{posterWidth - border, line_start.y};
        DrawLineEx(line_start, line_end, line_thickness, BLACK);

        TextLine album_date(album_info.album_date, font_bold, posterWidth * 0.025f);
        album_date.set_reference_point({posterWidth - border, line_end.y});
        album_date.set_offset({-album_date.get_length(), -(artist_name.size * 0.1f + album_date.size)});
        album_date.draw();

        TextLine album_duration(album_info.album_duration, font_bold, posterWidth * 0.025f);
        album_duration.set_reference_point(album_date.get_reference_point());
        album_duration.set_offset({-album_duration.get_length(), artist_name.size * 0.1f});
        album_duration.draw();

        int  lines_per_side = 10;
        if (album_info.track_names.size() < lines_per_side){
            lines_per_side = album_info.track_names.size();
        }
        std::vector<std::string> left_tracks_list(album_info.track_names.begin(), album_info.track_names.begin() + lines_per_side);
        std::vector<std::string> right_tracks_list(album_info.track_names.begin() + lines_per_side, album_info.track_names.end());

        std::string left_tracks;
        for (auto tracks: left_tracks_list){
            left_tracks += (tracks + "\n ");
            std::cout << tracks << std::endl;
        }

        std::string right_tracks;
        for (auto tracks: right_tracks_list){
            right_tracks += (tracks + "\n ");
        }

        float space_for_track_list = (posterHeight - border) - (line_start.y + artist_name.size * 0.1f);
        int track_font_size = static_cast<int>(round(space_for_track_list / static_cast<double>(num_tracks_per_col)));

        float line_length = line_end.x - line_start.x;
        TextBox left_track_TB(left_tracks, font_bold, track_font_size, {line_length, space_for_track_list});
        left_track_TB.set_reference_point({line_start});
        left_track_TB.set_offset({0, artist_name.size * 0.1f});
        left_track_TB.draw();


        // float space_for_track_list = (posterHeight - border) - (line_start.y + artist_name.size * 0.1f);
        // Vector2 track_base_pos{border, line_start.y + artist_name.size * 0.1f};
        // int track_font_size = static_cast<int>(round(space_for_track_list /
        // static_cast<double>(num_tracks_per_col))); Vector2 track_base_pos{border, line_start.y + artist_name.size
        // * 0.1f}; Vector2 track_offset_pos{0, 0}; for (auto i = 0; i < album_info.track_names.size(); i++)
        // {
        //     const char *track_cstr = album_info.track_names[i].c_str();
        //     Vector2 track_pos = Vector2Add(track_base_pos, track_offset_pos);
        //     DrawTextEx(font_bold, track_cstr, track_pos, track_font_size, 1.0f, BLACK);
        //     track_offset_pos.y += track_font_size;
        //     if (i == (num_tracks_per_col - 1))
        //     {
        //         track_offset_pos.y = 0;
        //         track_offset_pos.x = posterWidth / 2.0f;
        //     }
        // }

        EndTextureMode();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        float display_height = static_cast<float>(GetScreenHeight());
        float display_width = static_cast<float>(GetScreenHeight()) / 1.414f;

        DrawTexturePro(viewport.texture, {0, 0, posterWidth, -posterHeight},
                       {(GetScreenWidth() - display_width) / 2, 0, display_width, display_height}, {0, 0}, 0.0f, WHITE);
        DrawRectangleLinesEx({(GetScreenWidth() - display_width) / 2, 0, display_width, display_height}, 3, BLACK);

        std::string placeholder =
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et "
            "dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip "
            "ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu "
            "fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
            "mollit anim id est laborum.";

        auto textbox = TextBox(placeholder, font_black, 30.0f, {500.0f, 5000.0f});
        textbox.draw();
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