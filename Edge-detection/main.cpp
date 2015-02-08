#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

const int WinWidth = 700, WinHeight = 700;
float edgeThreshold = 0.1f;
const sf::Color edgeColor = sf::Color::Black, clearColor = sf::Color::White;
std::string ImagePath = "img.png";

//Returns value 0.0 being the darkest, 1.0 being the brightest
inline float GetBrightness(const sf::Color &c) { return (0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b) / 255.0; }
inline bool GoodCoords(const sf::Image &img, int x, int y) { return x >= 0 and y >= 0 and x < img.getSize().x and y < img.getSize().y; }

bool IsEdge(const sf::Image &img, int x, int y)
{
    float bright = GetBrightness(img.getPixel(x,y));
    if(GoodCoords(img, x+1, y  ) and bright - GetBrightness(img.getPixel(x+1, y  )) > edgeThreshold)   return true;
    if(GoodCoords(img, x+1, y+1) and bright - GetBrightness(img.getPixel(x+1, y+1)) > edgeThreshold)   return true;
    if(GoodCoords(img, x-1, y-1) and bright - GetBrightness(img.getPixel(x-1, y-1)) > edgeThreshold)   return true;
    if(GoodCoords(img, x-1, y  ) and bright - GetBrightness(img.getPixel(x-1, y  )) > edgeThreshold)   return true;
    if(GoodCoords(img, x-1, y+1) and bright - GetBrightness(img.getPixel(x-1, y+1)) > edgeThreshold)   return true;
    if(GoodCoords(img, x  , y-1) and bright - GetBrightness(img.getPixel(x  , y-1))   > edgeThreshold) return true;
    if(GoodCoords(img, x  , y+1) and bright - GetBrightness(img.getPixel(x  , y+1))   > edgeThreshold) return true;
    if(GoodCoords(img, x+1, y-1) and bright - GetBrightness(img.getPixel(x+1, y-1)) > edgeThreshold)   return true;
    if(GoodCoords(img, x+1, y  ) and bright - GetBrightness(img.getPixel(x+1, y  )) > edgeThreshold)   return true;
    if(GoodCoords(img, x+1, y+1) and bright - GetBrightness(img.getPixel(x+1, y+1)) > edgeThreshold)   return true;
    return false;
}

sf::Image* GetEdgesImage(const sf::Image &img)
{
    sf::Image *result = new sf::Image();
    result->create(img.getSize().x, img.getSize().y);
    for(int i = 0; i < img.getSize().x; ++i)
    {
        for(int j = 0; j < img.getSize().y; ++j)
        {
            if(IsEdge(img, i, j)) result->setPixel(i, j, edgeColor);
            else result->setPixel(i, j, sf::Color::White);
        }
    }
    return result;
}

sf::Image* GetBlendedImage(const sf::Image &img, const sf::Image &edgeImage)
{
    sf::Image *result = new sf::Image();
    result->create(img.getSize().x, img.getSize().y);
    for(int i = 0; i < img.getSize().x; ++i)
    {
        for(int j = 0; j < img.getSize().y; ++j)
        {
            sf::Color imgColor = img.getPixel(i,j);
            if(edgeImage.getPixel(i,j) == edgeColor)
            {
                result->setPixel(i, j, edgeColor);
            }
            else result->setPixel(i, j, imgColor);
        }
    }
    return result;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "SFML works!");

    sf::Image img, *edgesImg, *blendedImg;
    img.loadFromFile(ImagePath);
    sf::Texture tex, edgesTex, blendedTex;
    tex.loadFromImage(img);
    sf::Sprite sp(tex);
    sp.setTexture(tex);

    while (window.isOpen())
    {
        edgesImg = GetEdgesImage(img);
        edgesTex.loadFromImage(*edgesImg);
        sf::Sprite edgeSprite(edgesTex);
        edgeSprite.setPosition(img.getSize().x, 0.0f);

        blendedImg = GetBlendedImage(img, *edgesImg);
        blendedTex.loadFromImage(*blendedImg);
        sf::Sprite blendedSprite(blendedTex);
        blendedSprite.setPosition(img.getSize().x, img.getSize().y);

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(clearColor);
        window.draw(sp);
        window.draw(edgeSprite);
        window.draw(blendedSprite);
        window.display();

        std::cout << "Introduce the threshold [0.0, 1.0] = "; std::cin >> edgeThreshold; std::cout << std::endl;
    }
    return 0;
}
