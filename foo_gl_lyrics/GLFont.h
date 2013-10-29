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
    COLORREF cl;
public:
    MyGLfont();
    virtual ~MyGLfont();
    void SetTextColor(COLORREF textcolor);//������ɫ����
    void MyCreateFont(char *facename, int height, int weight, bool italic,bool underline,bool strikeout);
    void ShowText(int x, int y, LPCTSTR lpszText);//��ʾͼ��2D����
    void Show2DText(char *str);//��ʾͼ��2D����
    void Show3DText(unsigned char *str);//��ʾͼ��3D����
};

#endif
