#include "../catch.hpp"

#include "geometrize/bitmap/bitmap.h"
#include "geometrize/bitmap/rgba.h"
#include "geometrize/core.h"

geometrize::rgba computeScanlinesColor(const geometrize::rgba targetColor, const geometrize::rgba currentColor, const int alpha)
{
    geometrize::Bitmap target(10, 10, targetColor);
    geometrize::Bitmap current(10, 10, currentColor);
    std::vector<geometrize::Scanline> lines;
    for(int y = 0; y < 10; y++) {
        lines.push_back(geometrize::Scanline(y, 0, 10, 0));
    }
    return geometrize::core::computeColor(target, current, lines, alpha);
}

TEST_CASE("Test computation of scanline color for whole image", "[core]")
{
    const geometrize::rgba expected{23, 0, 0, 128};
    const geometrize::rgba result{computeScanlinesColor(geometrize::rgba{12, 12, 12, 0}, geometrize::rgba{0, 212, 62, 0}, 128)};
    REQUIRE(expected == result);


}

TEST_CASE("Test drawing of scanlines", "[core]")
{
    geometrize::Bitmap image(50, 50, geometrize::rgba{10, 20, 30, 128});
    std::vector<geometrize::Scanline> lines;
    for(int y = 0; y < 50; y++) {
        lines.push_back(geometrize::Scanline(y, 0, 50, 255));
    }

    // TODO check drawn lines
}

TEST_CASE("Test copying of scanlines", "[core]")
{
    const geometrize::rgba color{128, 64, 32, 200};
    const unsigned int width{10};
    const unsigned int height{10};

    geometrize::Bitmap source{width, height, color};
    geometrize::Bitmap destination(width, height, geometrize::rgba{0, 0, 0, 0});

    std::vector<geometrize::Scanline> scanlines;
    for(int y = 3; y < 7; y++) {
        scanlines.push_back(geometrize::Scanline(y, 3, 7, 255));
    }

    geometrize::core::copyLines(destination, source, scanlines);

    for(const geometrize::Scanline& line : scanlines) {
        const int y{line.y};
        for(int x = line.x1; x < line.x2; x++) {
            REQUIRE(destination.getPixel(x, y) == color);
        }
    }
}

TEST_CASE("Test calculation of average color", "[core]")
{
    SECTION("Simple image")
    {
        const geometrize::rgba color{10, 50, 90, 255};
        const geometrize::Bitmap image{50, 50, color};
        REQUIRE(color == geometrize::core::getAverageImageColor(image));
    }
}