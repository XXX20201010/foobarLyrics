#include "GLFont.h"

MyGLfont::MyGLfont()
{
    cl=RGB(255,255,255);
    m_red = 0.0f;
    m_green = 0.0f;
    m_blue = 0.0f;
}

MyGLfont::~MyGLfont()
{
    if(hFont) DeleteObject(hFont);
}

//ֻ�йرչ��պ����������ȷ��ʾ��ɫ
void MyGLfont::SetTextColor(COLORREF textcolor)//������ɫ����
{
    cl=textcolor;
    m_red = GetRValue(cl) / 255.0;
    m_green = GetGValue(cl) / 255.0;
    m_blue = GetBValue(cl) / 255.0;
}

void MyGLfont::MyCreateFont(char *facename, int height, int weight, bool italic,bool underline,bool strikeout)
{
    //LOGFONTA lf;
    LOGFONT lf;
    lf.lfHeight = height;
    lf.lfWidth = 0;
    lf.lfEscapement = 0;
    lf.lfOrientation = 0;
    lf.lfWeight = weight;
    lf.lfItalic = italic;
    lf.lfUnderline = underline;
    lf.lfStrikeOut = strikeout;
    lf.lfCharSet = DEFAULT_CHARSET;
    lf.lfOutPrecision = OUT_TT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = /*PROOF_QUALITY*/ANTIALIASED_QUALITY;
    lf.lfPitchAndFamily = VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN;
    strcpy(lf.lfFaceName,facename);
    // ��������
    hFont = CreateFontIndirect(&lf);
}

//2Dͼ����ֻ����Ļ����ϵ�������ı任�����޹أ�Ҳ����˵����������ת��
//x,y��2Dͼ���־���Ļ���½ǣ�ע�ⲻ�����Ͻǣ��ĺ����������롣
void MyGLfont::ShowText(int x, int y, LPCTSTR lpszText)
{
    // ����ԭͶӰ���󣬽�ͶӰ������Ϊƽ��ͶӰ
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, 640, 0, 480, -1, 1 );
    // ����ԭģ�ͱ任����ƽ����( x, y )
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    RECT rect;
    GetClientRect(GetActiveWindow(),&rect);
    glTranslatef((float)x,(float)y,0);
    HBITMAP hbitmap;
    BITMAP bm;
    SIZE size;
    UCHAR* pBmpBits;
    HFONT hOldFont;
    HDC hdc = wglGetCurrentDC();
    hOldFont = (HFONT)SelectObject(hdc, hFont);
    ::GetTextExtentPoint32(hdc, lpszText, strlen(lpszText), &size);
    hbitmap = CreateBitmap(size.cx, size.cy,1, 1, NULL);
    HDC hMemDC = ::CreateCompatibleDC(hdc);
    if(hMemDC)
    {
        HBITMAP hPrevBmp = (HBITMAP)SelectObject(hMemDC,hbitmap);
        HFONT hPrevFont = (HFONT)SelectObject(hMemDC, hFont);
        SetBkColor(hMemDC, RGB(0, 0, 0));
        ::SetTextColor(hMemDC,RGB(255,255,255));
        SetBkMode(hMemDC, OPAQUE);
        TextOut(hMemDC, 0, 0, lpszText, strlen(lpszText));
        //��GDIλͼ���Ƶ�DIB
        SelectObject(hdc,hbitmap);
        GetObject(hbitmap, sizeof(bm), &bm);
        size.cx = (bm.bmWidth + 31) & (~31);
        size.cy = bm.bmHeight;
        int bufsize = size.cy * (((bm.bmWidth + 31) & (~31)) /8);
        pBmpBits = new UCHAR[bufsize];
        memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
        struct
        {
            BITMAPINFOHEADER bih;
            RGBQUAD col[2];
        }
        bic;
        BITMAPINFO *binf = (BITMAPINFO *)&bic;
        binf->bmiHeader.biSize = sizeof(binf->bmiHeader);
        binf->bmiHeader.biWidth = bm.bmWidth;
        binf->bmiHeader.biHeight = bm.bmHeight;
        binf->bmiHeader.biPlanes = 1;
        binf->bmiHeader.biBitCount = 1;
        binf->bmiHeader.biCompression = BI_RGB;
        binf->bmiHeader.biSizeImage = bufsize;
        binf->bmiHeader.biXPelsPerMeter = 1;
        binf->bmiHeader.biYPelsPerMeter = 1;
        binf->bmiHeader.biClrUsed = 0;
        binf->bmiHeader.biClrImportant = 0;
        ::GetDIBits(hdc, hbitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);
        SelectObject(hMemDC,hPrevBmp);
    }
    ::DeleteDC(hMemDC);
    DeleteObject( hbitmap );
    SelectObject(hdc, hOldFont);

    //��ʾ����
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glColor3f(GetRValue(cl)/255.0,GetGValue(cl)/255.0,GetBValue(cl)/255.0);
    glRasterPos2i(x,y);
    glBitmap(size.cx, size.cy, 0.0, 2.0, size.cx+2.0f, 0.0, pBmpBits);
    delete []pBmpBits;//�޸�
    // �ָ�ͶӰ�����ģ�ͱ任����
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glColor3f(1.0,1.0,1.0);
}

void MyGLfont::Show2DGbkText(char *str)
{
    float lrcWidth = 0.0f;//the whole length of this lyrics
    char FTextList[255];
    GLYPHMETRICSFLOAT gmf[256];
    int m_iCount=strlen(str);    
    GLuint m_listbase;
    HDC hDC=wglGetCurrentDC();
    glPushMatrix();

    SelectObject(hDC,hFont);
    int  i=0; 
    int  j=0; 
    DWORD ich,cch;
    m_listbase = glGenLists(256);

    glColor4f(m_red, m_green, m_blue, 1.0f);
    while(i < m_iCount) {
       if(IsDBCSLeadByte(str[i])) { 
            //�ж��Ƿ�Ϊ˫�ֽ� 
            ich=str[i]; 

            ich=(ich<<8);

            //see Issue #1
            //GB2312     0xB0-0xF7(176-247)      0xA0-0xFE(160-254)
            //GBK        0x81-0xFE(129-254)      0x40-0xFE( 64-254)
            if ((unsigned char)(str[i + 1]) >= 0xA0) {
                ich += 256;//256 is the offset of GB2312, actually i'm not sure why this bug exist!
            }

            ich=ich+str[i+1]; 
            i++;i++; 
            wglUseFontOutlines(hDC,//���������豸��ϵDC 
                ich, //Ҫת��Ϊ��ʾ�б�ĵ�һ���ַ� 
                1, //Ҫת��Ϊ��ʾ�б���ַ��� 
                m_listbase+j,//��ʾ�б�Ļ��� 
                0.0f, //ָ����ʵ�����������ƫ���� 
                0.0f,//0.15f ��Z�Ḻ�����ֵ 
                WGL_FONT_POLYGONS, //ָ����ʾ�б��߶λ����� 
                &gmf[j]); //�����ַ��ĵ�ַ 
        } else { 
            cch=str[i]; 
            i++; 
            wglUseFontOutlines(hDC, //���������豸��ϵDC 
                cch,//Ҫת��Ϊ��ʾ�б�ĵ�һ���ַ� 
                1,//Ҫת��Ϊ��ʾ�б���ַ��� 
                m_listbase+j,//��ʾ�б�Ļ��� 
                0.0f,//ָ����ʵ�����������ƫ���� 
                0.0f,//��Z�Ḻ�����ֵ 
                WGL_FONT_POLYGONS, //ָ����ʾ�б��߶λ����� 
                &gmf[j]);//�����ַ��ĵ�ַ 
        } 

       //count the length of this word
       lrcWidth += gmf[j].gmfCellIncX;
       FTextList[j]=j; 
       j++; 
    } 

    //set the Z coordinate for the font display in the middle of the front and back
    GLfloat fontZCoor = (m_front + m_back) / 2;

    //the width of the area in opengl space
    GLfloat areaWidth = m_width / m_front * fontZCoor;

    glLoadIdentity(); 
    if ((lrcWidth - areaWidth) > 0.000001f) {

        //the length of the lyrics is longer than lyrics display window
        //we just set the lyrics align left
        glTranslatef(-(areaWidth) / 2, 0.0f, -fontZCoor);
    } else {

        //set the lyrics align center
        glTranslatef(-(lrcWidth / 2), 0.0f, -fontZCoor);
    }

    glPushAttrib(GL_LIST_BIT);
    {
        glListBase(m_listbase);
        glCallLists(m_iCount, GL_UNSIGNED_BYTE, &FTextList);
    }
    glPopAttrib();
    glPopMatrix();
}

void MyGLfont::SetArea(float width, float height, float front, float back)
{
    m_width = width;
    m_height = height;
    m_front = front;
    m_back = back;
}

void MyGLfont::Show3DText(unsigned char *str)
{
    glPushMatrix();
    GLYPHMETRICSFLOAT pgmf[1];

    HDC hDC=wglGetCurrentDC();
    //���õ�ǰ���� 
    SelectObject(wglGetCurrentDC(),hFont); 

    DWORD dwChar;
    int ListNum;
    for(size_t i=0;i<strlen((char *)str);i++)
    {
        if(IsDBCSLeadByte(str[i]))
        {
            dwChar=(DWORD)((str[i]<<8)|str[i+1]);
            i++;
        }
        else 
            dwChar=str[i];
        ListNum=glGenLists(1);
        glColor3f(GetRValue(cl)/255.0,GetGValue(cl)/255.0,GetBValue(cl)/255.0);
        wglUseFontOutlines(hDC,dwChar,1,ListNum,0.0,0.1f,WGL_FONT_POLYGONS,pgmf);
        glCallList(ListNum);
        glDeleteLists(ListNum,1);
    }
    glPopMatrix();
    glColor3f(1.0,1.0,1.0);
}


void MyGLfont::SetLrcFont(HFONT f)
{
    hFont = f;
}
