#pragma once

#include "window_helper.h"
#include <windows.h> 
#include <dwmapi.h>
#include <stdio.h>
#include <GL/gl.h> 
#include <GL/glu.h> 
#include "GLFont.h"
#include "LyricsPlayer.h"
#include "SakuraParticle.h"

//You have to preinstall Windows SDK or else you can't get the OpenGL32.lib and glu32.lib
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "Dwmapi.lib")

// This class implements our window. 
// It uses a helper class from window_helper.h that emulates
// ATL/WTL conventions. The custom helper class is used to
// allow this tutorial to be compiled when ATL/WTL is not
// available, for example on Visual C++ 2008 Express Edition.
// The message_filter is used to process keyboard shortcuts.
// To be notified about playback status changes, we need a play
// callback. Those callbacks ar registered and unregistered in
// foobar2000 0.9. Since all callback methods are guaranteed to
// be called in the context of the main thread, we can derive
// our window class from play_callback and register 'this'.
class CLyricsWindow :
	public CSimpleWindowImpl<CLyricsWindow>,
	private message_filter,
	private play_callback
{
public:
	typedef CSimpleWindowImpl<CLyricsWindow> super;

	static void ShowWindow();
	static void HideWindow();

	// Dispatches window messages to the appropriate handler functions.
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT & lResult);

	// Message handler functions.
	// The function signatures are intended to be compatible with the MSG_WM_* macros in WTL.
	LRESULT OnCreate(LPCREATESTRUCT pCreateStruct);
	void OnDestroy();
	void OnClose();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//void OnLButtonDown(UINT nFlags, CPoint point);
	void OnContextMenu(HWND hWnd, CPoint point);
	void OnSetFocus(HWND hWndOld);
	void OnPaint(HDC hdc);
	void OnPrintClient(HDC hdc, UINT uFlags);

	// Helper to handle double buffering when appropriate.
	void PaintContent(PAINTSTRUCT &ps);

	// Real drawing is done here.
	void Draw(HDC hdc, CRect rcPaint);

	// helpers methods
	HWND Create(HWND hWndParent);
	inline void RedrawWindow() {::RedrawWindow(m_hWnd, 0, 0, RDW_INVALIDATE);}

private:
	// This is a singleton class.
	CLyricsWindow() {}
	~CLyricsWindow() {}

	static CLyricsWindow g_instance;

	void set_selection(metadb_handle_list_cref p_items);

	// message_filter methods
	virtual bool pretranslate_message(MSG * p_msg);

	// play_callback methods (the ones we're interested in)
	virtual void on_playback_new_track(metadb_handle_ptr p_track);
	virtual void on_playback_stop(play_control::t_stop_reason reason);
	virtual void on_playback_dynamic_info_track(const file_info & p_info);

	// play_callback methods (the rest)
	virtual void on_playback_starting(play_control::t_track_command p_command, bool p_paused) {}
	virtual void on_playback_seek(double p_time);
	virtual void on_playback_pause(bool p_state);
	virtual void on_playback_edited(metadb_handle_ptr p_track) {}
	virtual void on_playback_dynamic_info(const file_info & p_info) {}
	virtual void on_playback_time(double p_time) {}
	virtual void on_volume_change(float p_new_val) {}

    /// \brief ��ʾ��ʵĻص�����
    ///
    /// \param    strLrc ���
    /// \return   void
    /// \see �ο��ļ� LyricsPlayer.h 
    static void displayLrcCallback(const string &strLrc);

    /// \brief ��������
    ///
    /// \param    void
    /// \return   GLvoid 
    /// \see ��
    GLvoid drawScene();

    /// \brief ���贰�ڴ�С
    ///
    /// \param    width  ���
    /// \param    height �߶�
    /// \return   GLvoid 
    /// \see ��
    GLvoid resizeWindow(GLsizei width, GLsizei height);

    /// \brief ��ʼ��OpenGL
    ///
    /// \param    width  ���
    /// \param    height �߶�
    /// \return   GLvoid 
    /// \see ��
    GLvoid initializeGL(GLsizei width, GLsizei height); 

    /// \brief �������ظ�ʽ
    ///
    /// \param    hdc  ����DC���
    /// \return   BOOL 
    /// \see ��
    BOOL bSetupPixelFormat(HDC hdc);

    //create lrc directory in foobar install path
    void createLRCDir(const char* strDir);

private:
	CFont m_font;

	// This is used to notify other components of the selection
	// in our window. In this overly simplistic case, our selection
	// will be empty, when playback is stopped. Otherwise it will
	// contain the playing track.
	ui_selection_holder::ptr m_selection;

    LyricsPlayer m_lrcPlayer; ///< ��ʲ�����
    static HWND m_wnd;///< �����ھ��
    static string m_strLrc; ///< ���
    HDC m_hDC; ///< �����ڵ�DC 
    HGLRC m_hRC; ///< OpenGL context
    MyGLfont m_lrcFont;  ///< �������

    POINT m_dragStart; // for lyrics window drag&drop

    SakuraParticle *m_sakuraParticles;
};
