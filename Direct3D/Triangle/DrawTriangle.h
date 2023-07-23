#pragma once
#include "D3DFramework.h"
class DrawTriangle :public D3DFramework
{
	struct VERTEX
	{
		FLOAT x, y, z;
		FLOAT Color[4];
	};

	Microsoft::WRL::ComPtr<ID3D11InputLayout> mspInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mspVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> mspVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> mspPixelShader;

public:
	void Initizlize(HINSTANCE hInstance, int widht = 800, int heigth = 600) override;
	void Destory() override;
	
protected:
	void Render() override;

private:
	void IniTriangle();
	void InitPipeline();
};

