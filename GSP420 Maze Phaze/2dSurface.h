#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>
#include <string>

class Surface
{
public:
	Surface(void);
	virtual ~Surface(void);

	bool loadSurface(LPDIRECT3DDEVICE9 device, std::string filename);

	void render(LPDIRECT3DDEVICE9 pDevice);
	void setPosition(int x, int y);
	void setSize(int percent);

	//set src and dest rect are for manual use, instead of setposition and set size
	void setSrcRect(int left, int top, int height, int width);
	void setDestRect(int left, int top, int height, int width);

	IDirect3DSurface9* getSurface() { return surface; }
	RECT getSrcRect() { return srcRect; }

private:
	IDirect3DSurface9* surface;

	RECT srcRect; //source location in the loaded surface
	RECT destRect;//destination location on the screen
	int width;
	int height;
	int imageScale;
};
