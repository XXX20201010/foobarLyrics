#include "LyricsPlayer.h"
#include <cassert>
#include <fstream>

string LyricsPlayer::m_info;

LyricsPlayer::LyricsPlayer(void) : m_cbFun(NULL)
{
}

LyricsPlayer::~LyricsPlayer(void)
{
}

void LyricsPlayer::setLrcDirectory(const string &strDir)
{
    m_lrcDir = strDir;
}

bool LyricsPlayer::setPlayingSong(const char *strSongName, const char *strAlbum, const char *strArtist)
{
    m_title = strSongName;
    m_artist = strArtist;
    m_album = strAlbum;

    //��ո����Ϣ
    m_lycVec.clear();
    m_info.clear();

    //���ظ���ļ�
    if (!loadLrcFile()) {
        string strErr = "û���ҵ����";
        callClientCb(strErr);

        return false;
    }
    
    //��ʼ���Ÿ��
    return startDisplayLrc();
}

void LyricsPlayer::setLrcCB(LYC_CALLBACK cb)
{
    m_cbFun = cb;
}

bool LyricsPlayer::isLrcCBValid()
{
    if (m_cbFun != NULL) {
        return true;
    }
    return false;
}

bool LyricsPlayer::parseLrc(const string &fileName)
{
    ifstream file(fileName);
    if (!file.is_open()) {
        return false;
    }

    int lastTimeStamp = 0;
    string strLrcLin;
    while (file.peek() != EOF) {
        string line;
        getline(file, line);

        if (line.length() < 1) {
            continue;
        }

        string::size_type pos = line.find("[ti:");//��Ŀ
        if (pos != string::npos) {
            m_info += " ��Ŀ : ";
            m_info += line.substr(pos + 4, line.find_last_not_of("]") - pos - 3);
            continue;
        }
        
        pos = line.find("[ar:");//����
        if (pos != string::npos) {
            m_info += " ���� : ";
            m_info += line.substr(pos + 4, line.find_last_not_of("]") - pos - 3);
            continue;
        }
        
        pos = line.find("[al:");//ר��
        if (pos != string::npos) {
            m_info += " ר�� : ";
            m_info += line.substr(pos + 4, line.find_last_not_of("]") - pos - 3);
            continue;
        }

        pos = line.find("[by:");//����ṩ
        if (pos != string::npos) {
            m_info += " ��� : ";
            m_info += line.substr(pos + 4, line.find_last_not_of("]") - pos - 3);
            continue;
        }

        pos = line.find("[id:");//??
        if (pos != string::npos) {
            m_info += " id : ";
            m_info += line.substr(pos + 4, line.find_last_not_of("]") - pos - 3);
            continue;
        }

        //����ʱ��
        pos = line.find_first_of("[");
        string::size_type timeEndPos = line.find_first_of("]");
        string strTime = line.substr(pos + 1, timeEndPos - pos - 1);
        char time[2];
        memcpy(time, strTime.c_str(), 2); //��÷�����
        int mins = atoi(time);
        memcpy(time, strTime.c_str() + 3, 2); //�������
        int secs = atoi(time);
        memcpy(time, strTime.c_str() + 6, 2); //��ú�����
        int ms = atoi(time);

        unsigned int timeStamp = ms + mins * 60 * 1000 + secs * 1000; //�������
        
        strLrcLin = line.substr(timeEndPos + 1);
        pair<unsigned int, string> lrcElem = make_pair<unsigned int, string>(timeStamp, strLrcLin);
        m_lycVec.push_back(lrcElem);
    }

    return true;
}

bool LyricsPlayer::startDisplayLrc()
{
    HANDLE threadHandle;
    DWORD thID;

    //��ʼ����ʲ���λ��
    m_curLyc = 0;

    //�����߳̿�ʼ���Ÿ��
    threadHandle = CreateThread(NULL, 0, delayFun, this, 0, &thID);

    if (threadHandle == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}


DWORD WINAPI LyricsPlayer::delayFun(_In_  LPVOID lpParameter)
{
    LyricsPlayer *player = (LyricsPlayer*)lpParameter;
    
    if (player == NULL || !player->isLrcCBValid()) {
        return -1;
    }

    player->callClientCb(m_info);
    pair<unsigned int, string> lrcObj;
    unsigned int lastTimeStamp = 0;
    unsigned int delay;
    while (player->getNextLrcLine(lrcObj)) {
        
        delay = lrcObj.first - lastTimeStamp;
        lastTimeStamp = lrcObj.first;
        Sleep(delay);
        player->callClientCb(lrcObj.second);
    }

    return 0;
}


bool LyricsPlayer::getNextLrcLine(pair<unsigned int, string> &lrcObj)
{
    if (m_curLyc >= m_lycVec.size()) {
        return false;
    }

    lrcObj = m_lycVec[m_curLyc];
    m_curLyc++;
    return true;
}

void LyricsPlayer::callClientCb(const string &strLrc)
{
    m_cbFun(strLrc);
}

bool LyricsPlayer::loadLrcFile()
{
    //����ļ������������Lyrics show panel��ͬ���� : ������ - ��������.lrc
    string lrcName = m_lrcDir + m_artist + " - " + m_title + ".lrc";
    
    //todo �� �ж��ļ��Ƿ���ڣ��粻������Ҫ�������粢����
    
    return parseLrc(lrcName);
}
