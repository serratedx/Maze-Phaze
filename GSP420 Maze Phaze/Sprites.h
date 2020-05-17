#pragma once
#include <iostream>
#include <string.h>
#include <d3d9.h>
#include <d3dx9tex.h>


class Sprite
{
private:
	LPD3DXSPRITE spriteobj;
public:
	Sprite();
	~Sprite();

	void spriteInit(LPDIRECT3DDEVICE9 device);
	LPDIRECT3DTEXTURE9 LoadTexture(std::string filename, D3DCOLOR transcolor = D3DCOLOR_XRGB(0,0,0), LPDIRECT3DDEVICE9 device = NULL);
	void Sprite_Draw_Frame(LPDIRECT3DTEXTURE9 texture, int destx, int desty, int framenum, int framew, int frameh, int columns);
	void Sprite_Animate(int &frame, int startframe, int endframe, int direction, int &starttime, int delay);
	LPD3DXSPRITE getSpriteObj();
};