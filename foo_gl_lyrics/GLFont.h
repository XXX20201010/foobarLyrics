//GLFont.h�ļ�

#ifndef __GL_FONT_H__
#define __GL_FONT_H__

#include <windows.h>
#include "gl/gl.h"
#pragma warning(disable:4244)
//OpenGL������ʾ��
class MyGLfont
{
    HFONT hFont;
    //operator HFONT( ) const;
    COLORREF cl;
public:
    MyGLfont();
    virtual ~MyGLfont();
    void SetTextColor(COLORREF textcolor);//������ɫ����
    void SetLrcFont(HFONT f);//why can't be HFONT&  ?
    void MyCreateFont(char *facename, int height, int weight, bool italic,bool underline,bool strikeout);
    void ShowText(int x, int y, LPCTSTR lpszText);//��ʾͼ��2D����
    void Show2DGbkText(char *str);//��ʾͼ��2D����
    void Show3DText(unsigned char *str);//��ʾͼ��3D����

private:
    float m_red;
    float m_green;
    float m_blue;
};

#endif
