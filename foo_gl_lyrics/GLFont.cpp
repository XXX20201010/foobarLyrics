#include "GLFont.h"

MyGLfont::MyGLfont()
{
    cl=RGB(255,255,255);
}

MyGLfont::~MyGLfont()
{
    if(hFont) DeleteObject(hFont);
}

//只有关闭光照和纹理才能正确显示颜色
void MyGLfont::SetTextColor(COLORREF textcolor)//字体颜色设置
{
    cl=textcolor;
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
    // 创建字体
    hFont = CreateFontIndirect(&lf);
}

//2D图像汉字只与屏幕相联系，与具体的变换矩阵无关，也就是说不能缩放旋转。
//x,y是2D图像汉字距屏幕左下角（注意不是左上角）的横向和纵向距离。
void MyGLfont::ShowText(int x, int y, LPCTSTR lpszText)
{
    // 保存原投影矩阵，将投影矩阵设为平行投影
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    glOrtho( 0, 640, 0, 480, -1, 1 );
    // 保存原模型变换矩阵，平移至( x, y )
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
        //把GDI位图复制到DIB
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

    //显示文字
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glColor3f(GetRValue(cl)/255.0,GetGValue(cl)/255.0,GetBValue(cl)/255.0);
    glRasterPos2i(x,y);
    glBitmap(size.cx, size.cy, 0.0, 2.0, size.cx+2.0f, 0.0, pBmpBits);
    delete []pBmpBits;//修改
    // 恢复投影矩阵和模型变换矩阵
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glColor3f(1.0,1.0,1.0);
}

void MyGLfont::Show2DText(char *str)
{
    char FTextList[255];
    GLYPHMETRICSFLOAT gmf[256];
    int m_iCount=strlen(str);    
    GLuint m_listbase;
    HDC hDC=wglGetCurrentDC();
    glPushMatrix();

    SelectObject(hDC,hFont);
    int  i=0; 
    int  j=0; 
    int ich,cch;
    m_listbase = glGenLists(256);


    //glColor4f(GetRValue(cl)/255.0,GetGValue(cl)/255.0,GetBValue(cl)/255.0, 1.0f);

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    while(i<m_iCount)
    {
        //int isUnicode = IS_TEXT_UNICODE_SIGNATURE | IS_TEXT_UNICODE_REVERSE_SIGNATURE | IS_TEXT_UNICODE_STATISTICS | IS_TEXT_UNICODE_ASCII16;
        //if (IsTextUnicode(str, strlen(str) + 1, &isUnicode)) {
            //ich = str[i];
            //i += 2;
            //wglUseFontOutlinesW(hDC,//字体轮廓设备联系DC 
            //    ich, //要转换为显示列表的第一个字符 
            //    1, //要转换为显示列表的字符数 
            //    m_listbase+j,//显示列表的基数 
            //    1.0f, //指定与实际轮廓的最大偏移量 
            //    0,//0.15f, //在Z轴负方向的值 
            //    WGL_FONT_POLYGONS, //指定显示列表线段或多边形 
            //    &gmf[j]);

            //FTextList[j]=j; 
            //j++; 
        //}

        /*else*/ if(IsDBCSLeadByte(str[i]))
        { 
            //判断是否为双字节 
            ich=str[i]; 
            ich=(ich<<8)+256; ////256为汉字内码“偏移量” 
            ich=ich+str[i+1]; 
            i++;i++; 
            wglUseFontOutlines(hDC,//字体轮廓设备联系DC 
                ich, //要转换为显示列表的第一个字符 
                1, //要转换为显示列表的字符数 
                m_listbase+j,//显示列表的基数 
                1.0f, //指定与实际轮廓的最大偏移量 
                0,//0.15f, //在Z轴负方向的值 
                WGL_FONT_POLYGONS, //指定显示列表线段或多边形 
                &gmf[j]); //接受字符的地址 
            FTextList[j]=j; 
            j++; 

        }
        else
        { 
            cch=str[i]; 
            i++; 
            wglUseFontOutlines(hDC, //字体轮廓设备联系DC 
                cch,//要转换为显示列表的第一个字符 
                1,//要转换为显示列表的字符数 
                m_listbase+j,//显示列表的基数 
                0.0f,//指定与实际轮廓的最大偏移量 
                0.0,//0.15f,//在Z轴负方向的值 
                WGL_FONT_POLYGONS, //指定显示列表线段或多边形 
                &gmf[j]);//接受字符的地址 
            FTextList[j]=j;
            j++;
        } 
    } 

    glPushAttrib(GL_LIST_BIT);
    {
        glListBase(m_listbase);
        glCallLists(m_iCount, GL_UNSIGNED_BYTE, &FTextList);
    }
    glPopAttrib();
    glPopMatrix();
    //glColor4f(1.0,1.0,1.0, 0.0f);
}
void MyGLfont::Show3DText(unsigned char *str)
{
    glPushMatrix();
    GLYPHMETRICSFLOAT pgmf[1];

    HDC hDC=wglGetCurrentDC();
    //设置当前字体 
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
