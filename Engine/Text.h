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

//フォントリスト
enum class Font
{
    Meiryo,
    Arial,
    MeiryoUI
};

//フォント名
namespace
{
    const WCHAR* FontList[]
    {
        L"メイリオ",
        L"Arial",
        L"Meiryo UI"
    };
}

//フォント設定
struct FontData
{
    Font font;                              //フォント名
    IDWriteFontCollection* fontCollection;  //フォントコレクション
    DWRITE_FONT_WEIGHT fontWeight;          //フォントの太さ
    DWRITE_FONT_STYLE fontStyle;            //フォントスタイル
    DWRITE_FONT_STRETCH fontStretch;        //フォントの幅
    FLOAT fontSize;                         //フォントサイズ
    WCHAR const* localeName;                //ロケール名
    DWRITE_TEXT_ALIGNMENT textAlignment;    //テキストの配置
    D2D1_COLOR_F Color;                     //テキストの色

    //デフォルト設定
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

    //フォントデータ
    FontData* Setting = new FontData();

    //string->wstring変換
    std::wstring StringToWString(std::string str);
public:
    //デフォルトコンストラクタを制限
    Text() = delete;

    //コンストラクタ
    Text(FontData* set) : Setting(set) {};

    /// <summary>コンストラクタ</summary>
    /// <param name="font">フォント名</param>
    /// <param name="fontcollection">フォントコレクション</param>
    /// <param name="fontweight">フォントの太さ</param>
    /// <param name="fontstyle">フォントスタイル</param>
    /// <param name="fontstretch">フォントの幅</param>
    /// <param name="fontsize">フォントサイズ</param>
    /// <param name="localename">ロケール名</param>
    /// <param name="textalignment">テキストの配置</param>
    /// <param name="color">フォントの色</param>
    Text(Font font,
        IDWriteFontCollection* fontcollection,
        DWRITE_FONT_WEIGHT fontweight,
        DWRITE_FONT_STYLE fontstyle,
        DWRITE_FONT_STRETCH fontstretch,
        FLOAT fontsize,
        WCHAR const* localename,
        DWRITE_TEXT_ALIGNMENT textalignment,
        D2D1_COLOR_F color);

    //フォント設定
    void SetFont(FontData* set);

    /// <summary>フォント設定</summary>
    /// <param name="font">フォント名</param>
    /// <param name="fontcollection">フォントコレクション</param>
    /// <param name="fontweight">フォントの太さ</param>
    /// <param name="fontstyle">フォントスタイル</param>
    /// <param name="fontstretch">フォントの幅</param>
    /// <param name="fontsize">フォントサイズ</param>
    /// <param name="localename">ロケール名</param>
    /// <param name="textalignment">テキストの配置</param>
    /// <param name="color">フォントの色</param>
    void SetFont(Font font,
        IDWriteFontCollection* fontcollection,
        DWRITE_FONT_WEIGHT fontweight,
        DWRITE_FONT_STYLE fontstyle,
        DWRITE_FONT_STRETCH fontstretch,
        FLOAT fontsize,
        WCHAR const* localename,
        DWRITE_TEXT_ALIGNMENT textalignment,
        D2D1_COLOR_F color);

    /// <summary>描画</summary>
    /// <param name="str">表示したい文字列</param>
    /// <param name="pos">描画位置</param>
    /// <param name="options">テキストの整形</param>
    void Draw(std::string str, XMFLOAT3 pos, D2D1_DRAW_TEXT_OPTIONS options = D2D1_DRAW_TEXT_OPTIONS_NONE);

    void Initialize();

    void Release();
};