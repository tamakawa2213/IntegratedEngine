#include "Text.h"
#include "CallDef.h"
#include "Direct3D.h"
#include "Sprite.h"

Text::Text()
	: m_layerBuffer(nullptr), m_shaderResourceView(nullptr), pConstantBuffer_(nullptr), pVertexBuffer_(nullptr), pIndexBuffer_(nullptr), pSampler_(nullptr),
    stringList("")
{
}

Text::~Text()
{
    SAFE_RELEASE(m_layerBuffer);
    SAFE_RELEASE(m_shaderResourceView);
    SAFE_RELEASE(pConstantBuffer_);
    SAFE_RELEASE(pVertexBuffer_);
    SAFE_RELEASE(pIndexBuffer_);
    SAFE_RELEASE(pSampler_);
}

HRESULT Text::InitChar(LPCWSTR c)
{
    HRESULT hr;
    // フォントハンドルの設定
    //LPCSTR font = "ＭＳ ゴシック";
    LOGFONT lf =
    {
        (LONG)fontSize, 0, 0, 0,
        FW_NORMAL, 0, 0, 0,
        SHIFTJIS_CHARSET,
        OUT_TT_ONLY_PRECIS,
        CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY,
        DEFAULT_PITCH | FF_MODERN,
        NULL,
    };

    // フォントハンドルを生成
    HFONT hFont = CreateFontIndirectW(&lf);

    // 現在のウィンドウに適用
    HDC hdc = GetDC(NULL);
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, hFont));
    // デバイスにフォントを持たせないとGetGlyphOutlineW関数はエラーとなる。

    // 出力する文字(一文字だけ)
    UINT code = static_cast<UINT>(*c);

    // 17階調のグレーのグリフビットマップ
    const UINT gradFlag = GGO_GRAY4_BITMAP;

    // ビットマップの設定
    GLYPHMETRICS gm;
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };

    // フォントビットマップを取得
    DWORD size = GetGlyphOutlineW(hdc, code, gradFlag, &gm, 0, NULL, &mat);
    BYTE* pMono = new BYTE[size];
    GetGlyphOutlineW(hdc, code, gradFlag, &gm, size, pMono, &mat);

    // フォントの幅と高さ
    INT fontWidth = gm.gmCellIncX;
    INT fontHeight = tm.tmHeight;

    // レンダーターゲットの設定
    D3D11_TEXTURE2D_DESC rtDesc;
    ZeroMemory(&rtDesc, sizeof(rtDesc));
    rtDesc.Width = fontWidth;
    rtDesc.Height = fontHeight;
    rtDesc.MipLevels = 1;
    rtDesc.ArraySize = 1;
    rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    rtDesc.SampleDesc.Count = 1;
    rtDesc.SampleDesc.Quality = 0;
    rtDesc.Usage = D3D11_USAGE_DYNAMIC;
    rtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    rtDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    rtDesc.MiscFlags = 0;

    // フォント用テクスチャを作成
    hr = Direct3D::pDevice->CreateTexture2D(&rtDesc, nullptr, &m_layerBuffer);
    if (FAILED(hr))
    {
        return hr;
    }
    assert(m_layerBuffer != 0);

    // フォント用テクスチャリソースにテクスチャ情報をコピー
    D3D11_MAPPED_SUBRESOURCE mappedSubrsrc;
    hr = Direct3D::pContext->Map(m_layerBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubrsrc);
    if (FAILED(hr))
    {
        return hr;
    }

    {
        BYTE* pBits = static_cast<BYTE*>(mappedSubrsrc.pData);
        INT iOfs_x = gm.gmptGlyphOrigin.x;
        INT iOfs_y = tm.tmAscent - gm.gmptGlyphOrigin.y;
        INT iBmp_w = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
        INT iBmp_h = gm.gmBlackBoxY;
        INT Level = 17;
        INT x, y;
        DWORD Alpha, Color;
        int mem = (int)mappedSubrsrc.RowPitch * (int)tm.tmHeight;
        memset(pBits, 0, mem);
        for (y = iOfs_y; y < iOfs_y + iBmp_h; y++)
        {
            for (x = iOfs_x; x < iOfs_x + iBmp_w; x++)
            {
                Alpha = (255 * pMono[x - iOfs_x + iBmp_w * (y - iOfs_y)]) / (Level - 1);
                Color = 0x00ffffff | (Alpha << 24);
                memcpy(static_cast<BYTE*>(pBits) + mappedSubrsrc.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
            }
        }
    }

    Direct3D::pContext->Unmap(m_layerBuffer, 0);
    // フォント情報の書き込み
    // iOfs_x, iOfs_y : 書き出し位置(左上)
    // iBmp_w, iBmp_h : フォントビットマップの幅高
    // Level : α値の段階 (GGO_GRAY4_BITMAPなので17段階)

    // メモリ解放
    delete[] pMono;

    // シェーダリソースビューの設定
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    //srvDesc.Format = rtDesc.Format;
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    //srvDesc.Texture2D.MipLevels = rtDesc.MipLevels;
    srvDesc.Texture2D.MipLevels = 1;

    // シェーダリソースビューを作成
    hr = Direct3D::pDevice->CreateShaderResourceView(m_layerBuffer, &srvDesc, &m_shaderResourceView);

    return hr;
}

HRESULT Text::InitChar(std::string c)
{
    wchar_t file[CHAR_MAX];
    size_t ret;
    mbstowcs_s(&ret, file, c.c_str(), c.length());
    return InitChar(file);
}

void Text::Initialize(std::string text)
{
    stringList = text;
    // 頂点情報
    VERTEX vertices[] =
    {
        { XMVectorSet(-1.0f,  1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },	// 四角形の頂点（左上）
        { XMVectorSet(1.0f,  1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) },	// 四角形の頂点（右上）
        { XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) },	// 四角形の頂点（右下）
        { XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f),XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) },	// 四角形の頂点（左下）
    };

    // 頂点データ用バッファの設定
    D3D11_BUFFER_DESC bd_vertex;
    ZeroMemory(&bd_vertex, sizeof(bd_vertex));
    bd_vertex.ByteWidth = sizeof(vertices);
    bd_vertex.Usage = D3D11_USAGE_DEFAULT;
    bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd_vertex.CPUAccessFlags = 0;
    bd_vertex.MiscFlags = 0;
    bd_vertex.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA data_vertex;
    data_vertex.pSysMem = vertices;
    Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

    //インデックス情報
    int index[] = { 0,1,2, 0,2,3 };

    // インデックスバッファを生成する
    D3D11_BUFFER_DESC   bd;
    bd.ByteWidth = sizeof(index);
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = index;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);

    // 定数情報の追加
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(CONSTANT_BUFFER);
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

    // 定数バッファを作成
    Direct3D::pDevice->CreateBuffer(&bufferDesc, nullptr, &pConstantBuffer_);

    //コンスタントバッファ作成
    D3D11_BUFFER_DESC cb;
    cb.ByteWidth = sizeof(CONSTANT_BUFFER);
    cb.Usage = D3D11_USAGE_DYNAMIC;
    cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb.MiscFlags = 0;
    cb.StructureByteStride = 0;
    Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);

    // サンプラの設定
    D3D11_SAMPLER_DESC samplerDesc;
    ZeroMemory(&samplerDesc, sizeof(samplerDesc));
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    //samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    //samplerDesc.BorderColor[0] = 1.0f;		// ホワイト
    //samplerDesc.BorderColor[1] = 1.0f;		// ..
    //samplerDesc.BorderColor[2] = 1.0f;		// ..
    //samplerDesc.BorderColor[3] = 1.0f;		// ..
    //samplerDesc.MinLOD = 0;
    //samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // サンプラステートを作成
    Direct3D::pDevice->CreateSamplerState(&samplerDesc, &pSampler_);

    // フォントテキスト初期化
    for (int i = 0; i < stringList.length(); i++)
    {
        TCHAR b[32];
        wchar_t file[CHAR_MAX];
        size_t ret;
        mbstowcs_s(&ret, file, stringList.c_str(), stringList.length());
        lstrcpy(b, file);
        InitChar(&b[i]);
    }
}

void Text::Draw()
{
    Direct3D::SetShader(SHADER_2D);
    CONSTANT_BUFFER cb;

    cb.matW = XMMatrixTranspose(XMMatrixScaling(1.0f / Direct3D::scrWidth, 1.0f / Direct3D::scrHeight, 1.0f));

    D3D11_MAPPED_SUBRESOURCE pdata;
    Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
    memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));		// データを値を送る

    Direct3D::pContext->PSSetSamplers(0, 1, &pSampler_);
    Direct3D::pContext->PSSetShaderResources(0, 1, &m_shaderResourceView);

    Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

    //頂点バッファ
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;
    Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

    //インデックスバッファをセット
    stride = sizeof(int);
    Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);
    Direct3D::pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //コンスタントバッファ
    Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
    Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用
    Direct3D::pContext->DrawIndexed(6, 0, 0);
}
