#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	"CDirectxGraphics.h"

CDirectXGraphics* GetDX11Obj();
ID3D11Device* GetDX11Device();
ID3D11DeviceContext*	GetDX11DeviceContext();

void DX11LightInit(DirectX::XMFLOAT4 lightpos);
void DX11LightUninit();
void DX11LightUpdate(DirectX::XMFLOAT4 lpos);

bool DX11Init(HWND hwnd, int width, int height, bool fullscreen);
void DX11Uninit();
void DX11BeforeRender(float ClearColor[]);
void DX11AfterRender();

int DX11GetScreenWidth();
int DX11GetScreenHeight();

void TurnOffZbuffer();
void TurnOnZbuffer();

void TurnOnAlphablend();

void TurnOffAlphablend();

void TurnWire();

void TurnSolid();

