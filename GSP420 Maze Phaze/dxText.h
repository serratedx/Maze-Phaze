#include <d3d9.h>
#include <d3dx9tex.h> //needed for text
#include <string>

class dxText
{
public:
	dxText(void); //Constructor
	~dxText(void); //deconstructor

	bool init(DWORD size, LPDIRECT3DDEVICE9 device); //initializes our interface
	void drawText(std::string text, int x, int y, int width, int height, DWORD color); //draw a text string


private:
	ID3DXFont *g_font; //font
};


