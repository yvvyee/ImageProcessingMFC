#include "pch.h"
#include "CDib.h"

#define DIB_HEADER_MARKER ((WORD) ('M' << 8) | 'B')

CDib::CDib()
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
    m_pDib = NULL;
    m_pPtr = NULL;
    m_pRgbPtr = NULL;
}

CDib::CDib(const CDib& dib)
{
    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
    m_pDib = NULL;
    m_pPtr = NULL;
    m_pRgbPtr = NULL;

    if (dib.m_pDib != NULL)
    {
        m_pDib = new BYTE[dib.m_nDibSize];
        memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);
    }
    InitDib();
}

CDib::~CDib()
{
    Destroy();
}

LONG CDib::GetHeight() { 
    return m_nHeight; 
}

LONG CDib::GetWidth() { 
    return m_nWidth; 
}

WORD CDib::GetBitCount() { 
    return m_nBitCount; 
}

DWORD CDib::GetDibSize() { 
    return m_nDibSize; 
}

RGBBYTE** CDib::GetRGBPtr() {
    ASSERT(m_nBitCount == 24);
    ASSERT(m_pRgbPtr != NULL);
    return m_pRgbPtr;
}

BYTE** CDib::GetPtr() {
    ASSERT(m_nBitCount == 8);
    ASSERT(m_pPtr != NULL);
    return m_pPtr;
}

BOOL CDib::IsValid() { 
    return (m_pDib != NULL); 
}

BOOL CDib::CreateGrayImage(int nWidth, int nHeight, BYTE value)
{
    // 이미 객체가 있다면 삭제한다
    if (m_pDib) { Destroy(); }
    int nBitCount = 8;
    // 실제 영상 데이터의 크기 계산(바이트 단위)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // 메모리 공간 할당
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER)
        + sizeof(RGBQUAD) * (1 << nBitCount) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // 헤더의 필드 값 설정
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biWidth = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0;
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // 색상 팔레트를 그레이스케일로 초기화
    RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++)
    {
        pPal->rgbBlue = (BYTE)i;
        pPal->rgbGreen = (BYTE)i;
        pPal->rgbRed = (BYTE)i;
        pPal->rgbReserved = 0;
        pPal++;
    }

    // 멤버 변수 값 설정
    InitDib();

    // 픽셀 데이터 초기화
    BYTE* pData = (BYTE*)GetDibBitsAddr();
    memset(pData, value, dwSizeImage);
    return TRUE;
}

BOOL CDib::CreateRGBImage(int nWidth, int nHeight, COLORREF value)
{
    // 이미 객체가 있다면 삭제한다
    if (m_pDib) { Destroy(); }
    int nBitCount = 24;
    // 실제 영상 데이터의 크기 계산(바이트 단위)
    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);

    // 메모리 공간 할당
    m_pDib = new BYTE[sizeof(BITMAPINFOHEADER) + dwSizeImage];
    if (m_pDib == NULL) { return FALSE; }

    // 헤더의 필드 값 설정
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biWidth = nWidth;
    lpbmi->biHeight = nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;

    lpbmi->biXPelsPerMeter = 0;
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // 멤버 변수 값 설정
    InitDib();

    // 픽셀 데이터 초기화
    register int i, j;
    for (j = 0; j < m_nHeight; j++)
    {
        for (i = 0; i < m_nWidth; i++)
        {
            // RGBBYTE 클래스의 = 연산자 오버로딩 사용
            m_pRgbPtr[j][i] = value;
        }
    }
    return TRUE;
}

BOOL CDib::Load(LPCTSTR lpszFileName)
{
    CString filePath;
    filePath = lpszFileName;
    filePath.MakeUpper();

    if (filePath.Right(4) == ".BMP")
    {
        return LoadBMP(lpszFileName);
    }
    else
    {
        return FALSE;
    }
}

BOOL CDib::Save(LPCTSTR lpszFileName)
{
    CString filePath;
    filePath = lpszFileName;
    filePath.MakeUpper();

    if (filePath.Right(4) == ".BMP")
    {
        return SaveBMP(lpszFileName);
    }
    else
    {
        return FALSE;
    }
}

void CDib::Draw(HDC hDC)
{
    Draw(hDC, 0, 0);
}

void CDib::Draw(HDC hDC, int x, int y)
{
    if (m_pDib == NULL) { return; }

    LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
    void* lpBits = (void*)GetDibBitsAddr();

    ::SetDIBitsToDevice(
        hDC,        // DC 객체
        x, y,       // 그려질 위치 좌표
        m_nWidth,   // 이미지 가로
        m_nHeight,  // 이미지 세로
        0, 0, 0,    // SrcX, SrcY, nStartScan
        m_nHeight,  // nNumScans
        lpBits,
        lpbi,
        DIB_RGB_COLORS);
}

void CDib::Draw(HDC hDC, int dx, int dy, int dw, int dh, DWORD dwRop)
{
    if (m_pDib == NULL) { return; }

    LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
    LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

    ::StretchDIBits(
        hDC,
        dx, dy,
        dw, dh,
        0, 0,
        m_nWidth,
        m_nHeight,
        lpDIBBits,
        lpbi,
        DIB_RGB_COLORS,
        SRCCOPY);
}

void CDib::Draw(HDC hDC, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop)
{
    if (m_pDib == NULL) { return; }

    LPBITMAPINFO lpbi = (LPBITMAPINFO)m_pDib;
    LPSTR lpDIBBits = (LPSTR)GetDibBitsAddr();

    ::StretchDIBits(
        hDC,
        dx, dy,
        dw, dh,
        sx, sy,
        sw, sh,
        lpDIBBits,
        lpbi,
        DIB_RGB_COLORS,
        SRCCOPY);
}

CDib& CDib::operator=(const CDib& dib)
{
    // 재귀 검사
    if (this == &dib) { return *this; }

    // dib가 비어있는 CDib 객체라면 자신도 비운다.
    if (dib.m_pDib == NULL)
    {
        Destroy();
        return *this;
    }

    // 현재 설정된 CDib 객체가 있다면 삭제
    if (m_pDib) { Destroy(); }

    // (BITMAPINFOHEADER + 픽셀 데이터) 공간 할당
    m_pDib = new BYTE[dib.m_nDibSize];

    // 내용 복사
    memcpy(m_pDib, dib.m_pDib, dib.m_nDibSize);

    // 멤버 변수 초기화
    InitDib();

    return *this;
}

BOOL CDib::Copy(CDib* pDib)
{
    // pDib가 비어있는 CDib 객체를 가리키고 있다면 자신도 비운다.
    if (!pDib->IsValid())
    {
        Destroy();
        return TRUE;
    }

    // 현재 설정된 CDib 객체가 있다면 삭제
    if (m_pDib) { Destroy(); }

    // DIB를 담을 메모리 공간 할당
    DWORD dwDibSize = pDib->GetDibSize();
    m_pDib = new BYTE[dwDibSize];
    if (m_pDib == NULL) { return FALSE; }

    // 복사
    BYTE* pbi = (BYTE*)pDib->GetBitmapInfoAddr();
    memcpy(m_pDib, pbi, dwDibSize);

    // 멤버 변수 초기화
    InitDib();
    return TRUE;
}

BOOL CDib::CopyToClipboard()
{
    int     dwSizeDib;
    HANDLE  hDib;

    if (!::OpenClipboard(NULL))	return FALSE;

    // DIB 전체를 전역 메모리 블럭에 복사

    dwSizeDib = GetDibSize();
    hDib = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, dwSizeDib);
    if (hDib == NULL)
    {
        ::CloseClipboard();
        return FALSE;
    }

    void* lpDib = ::GlobalLock((HGLOBAL)hDib);
    memcpy(lpDib, GetBitmapInfoAddr(), dwSizeDib);
    ::GlobalUnlock(hDib);

    // 클립보드에 데이터 입력

    ::EmptyClipboard();
    ::SetClipboardData(CF_DIB, hDib);
    ::CloseClipboard();

    return TRUE;
}

BOOL CDib::PasteFromClipboard()
{
    HANDLE  hDib;
    DWORD   dwSize;
    void* lpDib;

    // CF_DIB 타입이 아니면 종료한다.
    if (!::IsClipboardFormatAvailable(CF_DIB)) return FALSE;

    // 클립 보드를 연다.
    if (!::OpenClipboard(NULL)) return FALSE;

    // 클립 보드 내용을 받아온다.
    hDib = ::GetClipboardData(CF_DIB);
    if (hDib == NULL)
    {
        ::CloseClipboard();
        return FALSE;
    }

    // 메모리 블럭의 크기는 DIB 전체 크기와 동일
    dwSize = (DWORD)::GlobalSize((HGLOBAL)hDib);
    lpDib = ::GlobalLock((HGLOBAL)hDib);

    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDib;
    LONG nWidth = lpbi->biWidth;
    LONG nHeight = lpbi->biHeight;
    WORD nBitCount = lpbi->biBitCount;
    DWORD dwSizeDib;

    DWORD dwSizeImage = nHeight * (DWORD)((nWidth * nBitCount / 8 + 3) & ~3);
    if (nBitCount == 8)
        dwSizeDib = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << nBitCount) + dwSizeImage;
    else
        dwSizeDib = sizeof(BITMAPINFOHEADER) + dwSizeImage;

    // 현재 설정된 CDib 객체가 있다면 삭제한다.
    if (m_pDib) Destroy();

    m_pDib = new BYTE[dwSizeDib];
    memcpy(m_pDib, lpDib, dwSizeDib);

    ::GlobalUnlock(hDib);

    ::CloseClipboard();

    // 멤버 변수 값 설정
    InitDib();

    return TRUE;
}

BOOL CDib::LoadBMP(LPCTSTR lpszFileName)
{
    CFile   file;
    UINT    nFileSize, nDibSize;

    BITMAPFILEHEADER    bmfh;
    LPBITMAPINFOHEADER  lpbmi;

    // 읽기 모드로 파일 열기
    if (!file.Open(lpszFileName, CFile::modeRead | CFile::shareDenyWrite))
        return FALSE;

    // BMP 파일의 크기 (바이트 단위)
    nFileSize = (UINT)file.GetLength();

    // BITMAPFILEHEADER 읽기
    if (file.Read((LPSTR)&bmfh, sizeof(BITMAPFILEHEADER))
        != sizeof(BITMAPFILEHEADER))
    {
        file.Close();
        return FALSE;
    }

    // BMP 파일임을 나타내는 "BM" 마커가 있는지 확인
    if (bmfh.bfType != DIB_HEADER_MARKER)
    {
        file.Close();
        return FALSE;
    }

    // Packed-DIB 메모리 블럭의 크기 계산
    nDibSize = nFileSize - sizeof(bmfh);

    // 현재 설정된 객체가 있다면 삭제한다.
    if (m_pDib) Destroy();

    // 위에서 계산한 만큼의 메모리 할당
    m_pDib = new BYTE[nDibSize];
    if (m_pDib == NULL)
    {
        file.Close();
        return FALSE;
    }

    // 파일로부터 Packed-DIB 크기만큼을 읽기
    if (file.Read(m_pDib, nDibSize) != nDibSize)
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // BITMAPINFOHEADER 버전을 확인한다. 
    lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    if (lpbmi->biSize != sizeof(BITMAPINFOHEADER))
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // 이제 m_pDib는 DIB를 담고있는 버퍼의 시작 주소를 가리키고 있다. 
    // CDib 클래스의 멤버 변수의 값을 설정한다.
    if (!InitDib())
    {
        delete[] m_pDib;
        m_pDib = NULL;
        file.Close();
        return FALSE;
    }

    // 파일 닫기
    file.Close();

    return TRUE;
}

BOOL CDib::SaveBMP(LPCTSTR lpszFileName)
{
    CFile	file;
    BITMAPFILEHEADER    bmfh;

    // DIB 객체를 포함하고 있지 않으면 FALSE를 반환.
    if (m_pDib == NULL)
        return FALSE;

    // 쓰기 모드로 파일 열기
    if (!file.Open(lpszFileName, CFile::modeCreate | CFile::modeWrite |
        CFile::shareDenyWrite))
        return FALSE;

    // BITMAPFILEHEADER 멤버 변수 설정
    bmfh.bfType = DIB_HEADER_MARKER;	// 항상 "BM"
    bmfh.bfSize = (DWORD)sizeof(BITMAPFILEHEADER) + m_nDibSize;
    bmfh.bfReserved1 = 0;		// 항상 0
    bmfh.bfReserved2 = 0;		// 항상 0
    bmfh.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER)
        + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPaletteNums());

    // BITMAPFILEHEADER 쓰기 
    file.Write((LPSTR)&bmfh, sizeof(bmfh));

    // DIB 구조(BITMAPINFOHEADER + 색상 테이블 + 픽셀 데이터) 쓰기
    file.Write(m_pDib, m_nDibSize);

    // 파일 닫기
    file.Close();

    return TRUE;
}

LPBITMAPINFO CDib::GetBitmapInfoAddr() {
    return (LPBITMAPINFO)m_pDib;
}

int CDib::GetPaletteNums()
{
    switch (m_nBitCount)
    {
    case 1: return 2;
    case 2: return 16;
    case 8: return 256;
    default: return 0;
    }
}

LPVOID CDib::GetDibBitsAddr()
{
    LPBITMAPINFOHEADER lpbmi;
    LPVOID lpDibBits;

    lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpDibBits = (LPVOID)((BYTE*)m_pDib + lpbmi->biSize
        + sizeof(RGBQUAD) * GetPaletteNums());

    return lpDibBits;
}

BOOL CDib::InitDib()
{
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;

    // 이미지의 가로크기, 세로크기, 픽셀 당 비트수, DIB 크기에 대한 멤버변수 설정
    m_nWidth = lpbmi->biWidth;
    m_nHeight = lpbmi->biHeight;
    m_nBitCount = lpbmi->biBitCount;

    DWORD dwSizeImage = m_nHeight *
        (DWORD)((m_nWidth * m_nBitCount / 8 + 3) & ~3);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * GetPaletteNums()
        + dwSizeImage;

    // m_pPtr 또는 m_pRgbPtr 멤버 변수 설정
    if (!AllocPtr()) { return FALSE; }
    return TRUE;
}

BOOL CDib::AllocPtr()
{
    if (m_nBitCount == 8)        // 그레이스케일 이미지
    {
        if (m_pPtr) FreePtr();
        m_pPtr = new BYTE * [m_nHeight];
        if (!m_pPtr) return FALSE;

        int nRWidth = ((m_nWidth * m_nBitCount / 8 + 3) & ~3);
        BYTE* pData = (BYTE*)GetDibBitsAddr();
        for (int i = 0; i < m_nHeight; i++)
            m_pPtr[i] = (BYTE*)(pData + (m_nHeight - i - 1) * nRWidth);
    }
    else if (m_nBitCount == 24)  // 트루칼라 이미지
    {
        if (m_pRgbPtr) FreePtr();
        m_pRgbPtr = new RGBBYTE * [m_nHeight];
        if (!m_pRgbPtr) return FALSE;

        int nRWidth = ((m_nWidth * m_nBitCount / 8 + 3) & ~3);
        BYTE* pData = (BYTE*)GetDibBitsAddr();
        for (int i = 0; i < m_nHeight; i++)
            m_pRgbPtr[i] = (RGBBYTE*)(pData + (m_nHeight - i - 1) * nRWidth);
    }
    else
        return FALSE;

    return TRUE;
}

void CDib::FreePtr()
{
    if (m_pPtr != NULL)
    {
        delete[] m_pPtr;
        m_pPtr = NULL;
    }

    if (m_pRgbPtr != NULL)
    {
        delete[] m_pRgbPtr;
        m_pRgbPtr = NULL;
    }
}

void CDib::Destroy()
{
    if (m_pDib)
    {
        delete[] m_pDib;
        m_pDib = NULL;
    }

    FreePtr();

    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
}