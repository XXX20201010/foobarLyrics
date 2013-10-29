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
    bool stopDisplayLrc();

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

    bool setCurrentSong(const string &strSongName, const string &strAlbum, const string &strSinger);

    /// \brief ���õ�ǰ��������
    ///
    /// \param strSongName   ��������
    /// \return       void  
    /// \see ��
    bool setCurrentSong(const string &strSongName);

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
    bool getNextLrcLine(pair<unsigned int, string> &lrcObj);

    static DWORD WINAPI delayFun(_In_  LPVOID lpParameter);

private:
    string m_strDir; ///< ����ļ�Ŀ¼
    vector<pair<unsigned int, string>> m_lycVec; ///< �������(����Ϊ��λ)pair<timestamp, lyrics>
    HANDLE m_timerQueue; ///< ��ʱ������
    static string m_info;       ///< �����Ϣ(��Ŀ�����֡�ר��������)
    LYC_CALLBACK m_cbFun;  ///< �ص�����������д�ظ��
    vector<pair<unsigned int, string>>::size_type m_curLyc; ///< ��ǰ�������
};

#endif//__LYRICS_PLAYER_H__
