/*! 
* \file LyricsPlayer.h 
* \brief ��ʲ������ļ� 
*  
*���ҡ����������ء����Ÿ��
*
* \author  Excalibur, Email:c04n05@gmail.com
* \version 1.00.00  
* \date    2013-10-28
*/ 
#ifndef __LYRICS_PLAYER_H__
#define __LYRICS_PLAYER_H__

#include <Windows.h>
#include <string>
#include <vector>
#include "LrcDownloader.h"

using namespace std;

//����string���Ǹ�ʣ�����ģ����Ҫ��string��ʾ������ˢ�½���
typedef void (*LYC_CALLBACK)(const string&);

/// \brief ��ʲ������� 
///  
///��
class LyricsPlayer
{

public:
    LyricsPlayer(void);
    virtual ~LyricsPlayer(void);

    /// \brief ��ʼ���Ÿ��
    ///
    /// \param   void   
    /// \return  bool �Ƿ񲥷ųɹ�  
    /// \see ��  
    bool startDisplayLrc();

    /// \brief ֹͣ���Ÿ��
    ///
    /// \param   void   
    /// \return  bool �Ƿ�ֹͣ 
    /// \see ��  
    void stopDisplayLrc();

    void pauseDisplayLrc(bool isPause);

    void startPlayAnyTime(unsigned int tmPos);

    /// \brief ���ø���ļ�Ŀ¼
    ///
    /// \param strDir   ����ļ�Ŀ¼
    /// \return       void  
    /// \see ��  
    void setLrcDirectory(const string &strDir);

    /// \brief ���ø�ʻص�����
    ///
    ///�˻ص������Ĳ������ǵ�ǰ�ĸ��
    ///
    /// \param cb   �ص�����ָ��
    /// \return       void  
    /// \see ��  
    void setLrcCB(LYC_CALLBACK cb);

    /// \brief ��ʻص������Ƿ���Ч
    ///
    /// \param void
    /// \return   bool  �Ƿ���Ч
    /// \see ��  
    bool isLrcCBValid();

    /// \brief ���õ�ǰ���Ÿ���
    ///
    /// \param strSongName   ��������
    /// \param strAlbum      ר��
    /// \param strArtist     ������
    /// \return       void  
    /// \see ��
    bool setPlayingSong(const char *strSongName, const char *strAlbum, const char *strArtist, long long lStartTime);

    /// \brief ���õ�ǰ��������
    ///
    /// \param strSongName   ��������
    /// \return       void  
    /// \see ��
    bool setPlayingSong(const string &strSongName);

    /// \brief ��������ļ�
    ///
    /// \param fileName   ����ļ�·��
    /// \return       void  
    /// \see ��
    bool parseLrc(const string &fileName);

private:

    /// \brief ���ûص�����
    ///
    /// \param strLrc   ���и��
    /// \return       void  
    /// \see ��
    void callClientCb(const string &strLrc);

    /// \brief ��ȡһ�и��
    ///
    /// \param    lrcObj �ӳ�ʱ��͸��
    /// \return   bool   �Ƿ�ɹ�  
    /// \see ��
    bool getNextLrcLine(pair<unsigned int, string> &lrcObj, unsigned int &lastTimeStamp);

    
    unsigned int getSongIndex();

    /// \brief    ��ʶ�ʱ��ʾ�̺߳���
    ///
    /// \param    lpParameter �̺߳�������
    /// \return   DWORD       �߳��˳�״̬  
    /// \see ��
    static DWORD WINAPI delayFun(_In_  LPVOID lpParameter);

    /// \brief ���ظ���ļ�
    ///
    /// \param    autoDownload  
    /// \return   bool   ���ظ���Ƿ�ɹ�  
    /// \see ��
    bool loadLrcFile(bool autoDownload = false);

    /// \brief insert one line of lyrics
    void addLrcSentence(unsigned int timeStamp, string &lrc);

private:
    string m_lrcDir; ///< ����ļ�Ŀ¼
    vector<pair<unsigned int, string>> m_lrcVec; ///< �������(����Ϊ��λ)pair<timestamp, lyrics>
    static string m_info;       ///< �����Ϣ(��Ŀ�����֡�ר��������)
    LYC_CALLBACK m_cbFun;  ///< �ص�����������д�ظ��
    vector<pair<unsigned int, string>>::size_type m_curLrcLine; ///< ��ǰ�������
    unsigned int m_songIndex;

    typedef vector<pair<unsigned int, string>>::size_type LYCVEC_SIZE;

    BOOL m_isUTF8;  ///< lyrics file is UTF8 form
    string m_title; ///< ��������
    string m_artist; ///< ����(������)
    string m_album; ///< ר��

    static HANDLE m_thEvent; ///< thread stop event 
    static HANDLE m_freezeEvent; ///< thread stop event 
    HANDLE m_thLrc; ///< thread
    CRITICAL_SECTION m_cs;
    bool m_isPause;

    static long long m_tmOffset; //time offset for parse lyrics and download lyrics if required

    //used for pause play and resume play 
    long long m_tmStart;
    long long m_tmPause;
    long long m_tmDelay;

    LrcDownloader m_downloader;
};

#endif//__LYRICS_PLAYER_H__
