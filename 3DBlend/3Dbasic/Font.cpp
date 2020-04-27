#include "Font.h"



Font::Font()
{
	m_font = 0;
}
Font::Font(const Font &)
{
}
bool Font::InitFont(ID3D11Device * dev, char * fileName, WCHAR * textureFileName)
{
	bool result;
	result = LoadFontData(fileName);
	if (!result)
		return false;

	// Load the texture that has the font 
	result = LoadTexture(dev, textureFileName);
	if (!result)
		return false;

	return true;
}

void Font::ShutDown()
{
}

ID3D11ShaderResourceView * Font::GetTexture()
{
	return nullptr;
}

void Font::BuildVertexArr(void *, char *, float, float)
{
}


Font::~Font()
{
}

bool Font::LoadFontData(char * fileName)
{
	std::ifstream fin;
	int i;
	char temp;

	m_font = new FontType[95];
	if (!m_font)
	{
		return false;
	}
	// read in the font size and spacing between chars
	fin.open(fileName);
	if (fin.fail())
	{
		return false;
	}

	//Read in the 95 used ascii characters for text
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].size;
	}
	fin.close();
	return true;
}

void Font::ReleaseFontData()
{

}

bool Font::LoadTexture(ID3D11Device * dev, WCHAR * wchar)
{
	return false;
}

void Font::ReleaseTexture()
{
}
