#ifndef ALBUM_INFO_HPP
#define ALBUM_INFO_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class AlbumInfo
{
public:
    std::string album_name;
    std::string artist_name;
    std::string album_date;
    std::string album_duration;
    std::vector<std::string> track_names;
    std::string track_list;

    AlbumInfo(std::string filepath)
    {
        std::ifstream file(filepath);
        
        if (!file)
        {
            std::cout << "Unable able to open Album Info File" << std::endl;
            return;
        }
        
        std::string line;
        int line_count = 0;
        while (std::getline(file, line))
        {
            switch (line_count)
            {
            case 0:
            {
                this->artist_name = line;
                break;
            }
            case 1:
            {
                this->album_name = line;
                break;
            }
            case 2:
            {
                this->album_date = line;
                break;
            }
            case 3:
            {
                this->album_duration = line;
                break;
            }
            default:
            {
                track_names.push_back(std::to_string(line_count - 3) + ". " + line);
                track_list += std::to_string(line_count - 3) + ". " + line + "\n";
                break;
            }
            }
            line_count++;
        }

        file.close();
    }
};

#endif