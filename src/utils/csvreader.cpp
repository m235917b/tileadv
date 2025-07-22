#include <fstream>
#include <sstream>
#include <vector>

#include "utils/csvreader.hpp"
#include "model/tile.hpp"
#include "model/chunk.hpp"

Chunk csvToChunk(std::string filename)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;

    std::getline(file, line);
    std::stringstream ss(line);
    std::string value;
    std::getline(ss, value, ',');
    int sizex = std::stoi(value);
    std::getline(ss, value, ',');
    int sizey = std::stoi(value);

    Chunk chunk(sizex, sizey);

    int posx = 0;
    int posy = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        while (std::getline(ss, value, ',')) {
            Tile tile{ TileFactory::fromInt(std::stoi(value)) };
            chunk.setTile(posx, posy, tile);
            posx++;
        }
        posy++;
        posx = 0;
    }

    return chunk;
}