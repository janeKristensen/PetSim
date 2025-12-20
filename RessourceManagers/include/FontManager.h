#pragma once
#include "SFML//Graphics.hpp"

enum class FontName {
	TITLE
};

class FontManager
{
public:
	static FontManager* getInstance();
	void loadFont(FontName fontName, const std::string& filename)
	{
		sf::Font font;
		font.openFromFile(filename);
		mFonts.insert({fontName, font});
	}
	const sf::Font& getFont(FontName fontName)
	{
		return mFonts[fontName];
	}

protected:
	FontManager() {};
	static FontManager* mInstance;

private:
	std::unordered_map<FontName, sf::Font> mFonts;
};