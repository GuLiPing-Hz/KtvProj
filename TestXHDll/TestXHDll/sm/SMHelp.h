#ifndef SMHELP__H__
#define SMHELP__H__

#ifdef SMDLL_EXPORTS
#define DLL_CLASS __declspec(dllexport)
#else
#define DLL_CLASS __declspec(dllimport)
#endif

typedef void (*DEALMESSAGE) (int cmd,int wParam,int lParam);

class DLL_CLASS CSMHelp
{
public:
	//ע��ص�����
	static void InitSMHelp(DEALMESSAGE pDealMessage=NULL);
	//(player.exe ���ڵ�Ŀ¼,"player.exe")
	CSMHelp(const char* exeDir,const char* exeName);
	~CSMHelp();
	//�رղ�����
	bool PostClosePlayer();
	//�л�����,�ڶ����������ڻ������û���mdm�ļ���һ��ֻҪ��ΪNULL
	bool PostSwitchSong(const char* file,const char* mdm,int info);
	//����ͼƬ
	bool PostSingerPic(const char* file);
	//��������
	bool PostControlResume();
	//��ͣ����
	bool PostControlPause();
	//�л�����ģʽ
	bool PostPlayGrade(unsigned int eType);
	//ԭ�鳪�л�
	bool PostPlayAccompany();
	//������Ϣ
	bool PostSongInfo(const char* singer,const char* song);
	//֧�ּ�����
	bool PostSongStartEnd(int start,int end);
	//������˷�����
	bool PostMicValue(int info);
	//��������
	bool PostSetVolume(int info);
	//bool PostGetVolume();
	//��������
	bool PostAddTone();
	//������
	bool PostSubTone();
	//��ȡ����
	bool PostGetTone();
	//����
	bool PostMuteOpen();
	//ȡ������
	bool PostMuteClose();
	//���͹���,�Լ���Ļ��Ϣ����������μ�processcommunicate.h PMessage�ṹ
	bool PostMessage(int type,unsigned int mode,unsigned int fsize,unsigned int argb,const char* message);
	//��
	bool PostFireWarning();
	//ȡ����
	bool PostNoFireWarning();
	//���׸������
	bool PostNextSong(const char* name);
	//����
	bool PostSound(unsigned int eInt);
	//�Ƿ�����
	bool PostOnline(int eInt);
	//������Ƶ��Ϣ����Я������,��������μ�processcommunicate.h VideoInfo�ṹ
	bool PostVideoInfo(int operation,int type,unsigned int width,unsigned int height
		,unsigned int bytes_pixel,unsigned int size,bool first,const char* name);
	//�����ݳ���������Ϣ����������μ�processcommunicate.h RankingInfo�ṹ
	//pPlayerScore:PlayerScore* ָ��;nPlayer: ����������
	bool PostRankingList(void* pPlayerScore,int nPlayer);
	//֪ͨ���������ŵ��Ƿ��������û��ݳ��ĸ���,�Ƿ��ǹ������Ƭ
	bool PostFromOther(bool bOther,bool bAdvertisement);
	//�����ǹ��ʵʱ���⣬����������,��������μ�processcommunicate.h OtherGrade�ṹ
	bool PostOhterGrade(unsigned int cur_time,float cur_pitch,int sentence_index,float realtime_score
		,float accumulate_score,double cur_db,float sentence_score,bool sentence_switch);
	//�������Ž��ȣ�0-100
	bool PostProgress(int progress);
	//ѡ��������࣬������Ĭ�ϻ��ڵ�ǰĿ¼��ȡrandomlist.csv�ļ�����ȡ���������Ϣ���뱣֤��������ͬ
	bool PostCategory(const char* category);
	//�������������������˼true����ͣ���������Ϊfalse���Լ������¹���
	bool PostSelectState(bool bStop);
private:
	bool KillProcessFromName(const char* pProcessName);
public:
	static DEALMESSAGE m_pDealMessage;
};

#endif//SMHELP__H__