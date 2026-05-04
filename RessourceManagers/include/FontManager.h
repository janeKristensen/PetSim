#pragma once
#include "SFML//Graphics.hpp"

enum class FontName {
	TITLE
};

class FontManager
{
public:
	FontManager() {};
	void loadFont(FontName fontName, const std::string& filename)
	{
		sf::Font font;
		if (font.openFromFile(filename)) 
		{
			mFonts.insert({ fontName, font });
		}
	}
	const sf::Font& getFont(FontName fontName)
	{
		return mFonts.at(fontName);
	}

private:
	std::unordered_map<FontName, sf::Font> mFonts;
};