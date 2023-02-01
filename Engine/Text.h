#pragma once
#include <d2d1.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

using namespace DirectX;

//�t�H���g���X�g
enum class Font
{
    Meiryo,
    Arial,
    MeiryoUI
};

//�t�H���g��
namespace
{
    const WCHAR* FontList[]
    {
        L"���C���I",
        L"Arial",
        L"Meiryo UI"
    };
}

//�t�H���g�ݒ�
struct FontData
{
    Font font;                              //�t�H���g��
    IDWriteFontCollection* fontCollection;  //�t�H���g�R���N�V����
    DWRITE_FONT_WEIGHT fontWeight;          //�t�H���g�̑���
    DWRITE_FONT_STYLE fontStyle;            //�t�H���g�X�^�C��
    DWRITE_FONT_STRETCH fontStretch;        //�t�H���g�̕�
    FLOAT fontSize;                         //�t�H���g�T�C�Y
    WCHAR const* localeName;                //���P�[����
    DWRITE_TEXT_ALIGNMENT textAlignment;    //�e�L�X�g�̔z�u
    D2D1_COLOR_F Color;                     //�e�L�X�g�̐F

    //�f�t�H���g�ݒ�
    FontData()
    {
        font = Font::Meiryo;
        fontCollection = nullptr;
        fontWeight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL;
        fontStyle = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
        fontStretch = DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL;
        fontSize = 20;
        localeName = L"ja-jp";
        textAlignment = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
        Color = D2D1::ColorF(D2D1::ColorF::White);
    }
};

class Text
{
    ID2D1Factory* pD2DFactory = nullptr;
    IDWriteFactory* pDWriteFactory = nullptr;
    IDWriteTextFormat* pTextFormat = nullptr;
    IDWriteTextLayout* pTextLayout = nullptr;
    ID2D1RenderTarget* pRT = nullptr;
    ID2D1SolidColorBrush* pSolidBrush = nullptr;
    IDXGISurface* pBackBuffer = nullptr;

    //�t�H���g�f�[�^
    FontData* Setting = new FontData();

    //string->wstring�ϊ�
    std::wstring StringToWString(std::string str);
public:
    //�f�t�H���g�R���X�g���N�^�𐧌�
    Text() = delete;

    //�R���X�g���N�^
    Text(FontData* set) : Setting(set) {};

    /// <summary>�R���X�g���N�^</summary>
    /// <param name="font">�t�H���g��</param>
    /// <param name="fontcollection">�t�H���g�R���N�V����</param>
    /// <param name="fontweight">�t�H���g�̑���</param>
    /// <param name="fontstyle">�t�H���g�X�^�C��</param>
    /// <param name="fontstretch">�t�H���g�̕�</param>
    /// <param name="fontsize">�t�H���g�T�C�Y</param>
    /// <param name="localename">���P�[����</param>
    /// <param name="textalignment">�e�L�X�g�̔z�u</param>
    /// <param name="color">�t�H���g�̐F</param>
    Text(Font font,
        IDWriteFontCollection* fontcollection,
        DWRITE_FONT_WEIGHT fontweight,
        DWRITE_FONT_STYLE fontstyle,
        DWRITE_FONT_STRETCH fontstretch,
        FLOAT fontsize,
        WCHAR const* localename,
        DWRITE_TEXT_ALIGNMENT textalignment,
        D2D1_COLOR_F color);

    //�t�H���g�ݒ�
    void SetFont(FontData* set);

    /// <summary>�t�H���g�ݒ�</summary>
    /// <param name="font">�t�H���g��</param>
    /// <param name="fontcollection">�t�H���g�R���N�V����</param>
    /// <param name="fontweight">�t�H���g�̑���</param>
    /// <param name="fontstyle">�t�H���g�X�^�C��</param>
    /// <param name="fontstretch">�t�H���g�̕�</param>
    /// <param name="fontsize">�t�H���g�T�C�Y</param>
    /// <param name="localename">���P�[����</param>
    /// <param name="textalignment">�e�L�X�g�̔z�u</param>
    /// <param name="color">�t�H���g�̐F</param>
    void SetFont(Font font,
        IDWriteFontCollection* fontcollection,
        DWRITE_FONT_WEIGHT fontweight,
        DWRITE_FONT_STYLE fontstyle,
        DWRITE_FONT_STRETCH fontstretch,
        FLOAT fontsize,
        WCHAR const* localename,
        DWRITE_TEXT_ALIGNMENT textalignment,
        D2D1_COLOR_F color);

    /// <summary>�`��</summary>
    /// <param name="str">�\��������������</param>
    /// <param name="pos">�`��ʒu</param>
    /// <param name="options">�e�L�X�g�̐��`</param>
    void Draw(std::string str, XMFLOAT3 pos, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE);

    void Initialize();

    void Release();
};