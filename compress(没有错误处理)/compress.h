#include <abprec.h>
#include <iostream>
using namespace std;
//����ʹ��ϰ�� 
void MyPackFile(const String& filename,const String& direname);				//���ļ�filename���д��Ŀ¼direname�� 
void MyPackDirectory(const String& indirectory,const String& outdirectory); //��Ŀ¼indirectory���������Ŀ¼outdirectory��
void MyUnPackDirectory(const String& buffer,const String& outdirectory);    //���ļ�buffer(�������ļ�Ҳ������Ŀ¼)���������Ŀ¼outdirectory�� 
//���׵��õݹ� 
void PackFile(const String& filename,FileOutputStream& outbuffer);//����ļ�filenameд���ļ���outbuffer
void PackDirectory(const String& indirectory,FileOutputStream& outbuffer); //���Ŀ¼indirectoryд���ļ���outbuffer
void UnPackFile(const String& filename,FileInputStream& inbuffer,struct MyFileData& myData);//��ѹ�ļ���inbufferд���ļ�filename ----
void UnPackDirectory(const String& directoryname,FileInputStream& inbuffer,struct MyFileData& myData); //��ѹ�ļ���inbufferд��Ŀ¼directoryname
//�Զ����ļ��ṹ 
struct MyFileData
{
	//�ļ���Ŀ¼��Ҫ���� 
	FileAttributes attrs;
	int64 creationTime;
	int64 lastAccessTime;
	int64 lastModifiedTime;
	// 
	int64 length;//Ŀ¼û���ļ���С������Ҫ�� 
	int64 filenumber;//-1:�ļ���0����Ŀ¼��>=1:�ļ����� 
	int64 namelen;//û��namelen,�Ĳ������� 
	tchar_t name[128];
};

