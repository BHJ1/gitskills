#include"compress.h"
using namespace std;
void MyPackFile(const String& filename,const String& direname)
{
	Directory directory(direname);
	File file(filename);
	File* subfile=directory.createFile(String("Pack_")+file.getName());//����ļ���ǰ��ӡ�Pack_�� ����
	//Opens the file if it exists and seeks to the end of the file, or creates a new file.
	FileOutputStream myoutStream(*subfile,APPEND_MODE);
	PackFile(filename,myoutStream);
	myoutStream.close();
}
void MyPackDirectory(const String& indirectory,const String& outdirectory)
{
	Directory indire(indirectory);
	Directory outdire(outdirectory);
	File* subfile=outdire.createFile(String("Pack_")+indire.getName());
	FileOutputStream myoutStream(*subfile,APPEND_MODE);
	PackDirectory(indirectory,myoutStream);
	myoutStream.close();
}
void MyUnPackDirectory(const String& buffer,const String& outdirectory)
{
	//��ȡ�ṹ�ж����� 
	File infile(buffer);
	FileInputStream myinputStream(infile);
	struct MyFileData myData;
	myinputStream.read((unsigned char*)&myData,sizeof(myData));
	if(myData.filenumber==-1)			//��ѹ�ļ� 
	{
		Directory outdire(outdirectory);
		File* subfile=outdire.createFile(String(myData.name,myData.namelen));       //���ݻ�ȡ��myData�����ļ� 
		UnPackFile((*subfile).getFullName(),myinputStream,myData);
	}
	else								//��ѹĿ¼ 
	{
		Directory outdire(outdirectory);
		Directory subdir=outdire.createSubDirectory(String(myData.name,myData.namelen));//����Ŀ¼ 
		UnPackDirectory(subdir.getFullName(),myinputStream,myData);
	}
	myinputStream.close();
} 
void PackFile(const String& filename,FileOutputStream& outbuffer)
{
	struct MyFileData mydata;
	File infile(filename);
	FileInputStream myinputStream(infile);
	//set mydata 
	mydata.attrs=infile.getFileAttrs();
	mydata.creationTime=infile.getCreationTime();
	mydata.lastAccessTime=infile.getLastAccess();
	mydata.lastModifiedTime=infile.getLastModified();
	mydata.length=infile.getLength();
	mydata.filenumber=-1;
	mydata.namelen=infile.getName().getLength();
	for(int i=0;i<mydata.namelen;i++)
		mydata.name[i]=infile.getName().getCStr()[i];
	//���ļ��ṹ��Ϣ��mydata��д��outbuffer 
	outbuffer.write((const unsigned char*)&mydata,sizeof(mydata)); 
	//���û��� 
	unsigned char buff[1024];						
	myinputStream.read(buff,(int)mydata.length);//�ļ���С �����ܻᱬ�ڴ� 
	outbuffer.write(buff,mydata.length);
	myinputStream.close();
} 
void PackDirectory(const String& indirectory,FileOutputStream& outbuffer)
{
	struct MyFileData mydata;
	Directory indire(indirectory);
	//����mydata 
	mydata.attrs=indire.getFileAttrs();
	mydata.creationTime=indire.getCreationTime();
	mydata.lastAccessTime=indire.getLastAccess();
	mydata.lastModifiedTime=indire.getLastModified();
	//mydata.length=indire.getLength();//Ŀ¼û��getLength 
	mydata.namelen=indire.getName().getLength();
	for(int i=0;i<mydata.namelen;i++)
		mydata.name[i]=indire.getName().getCStr()[i];
	vector<String> subs;
	indire.list(subs);
	mydata.filenumber=subs.size();
	//����ṹ��Ϣ��outbuffer
	outbuffer.write((const unsigned char*)&mydata,sizeof(mydata));
	//ѭ�����Ҵ���ļ���Ŀ¼�� 
	for(String& s:subs)
	{
		if(SystemFile::isFile(s))
			PackFile(s,outbuffer);//����ļ� 
		else
			PackDirectory(s,outbuffer);//���Ŀ¼ 
	}
}
void UnPackFile(const String& filename,FileInputStream& inbuffer,struct MyFileData& myData)//ͨ��myData�Ѿ�ȷ�����Ǹ��ļ��������ٶ����ٶ������ļ����� 
{
	File outfile(filename);
	//set atrr 
	outfile.setLastModified(myData.lastModifiedTime);
	outfile.setCreationTime(myData.creationTime);
	outfile.setLastAccess(myData.lastAccessTime);
	outfile.setFileAttrs(myData.attrs); 
	//read from inbuffer
	unsigned char buff[1024];
	inbuffer.read(buff,(int)myData.length);//�ļ���С 
	//write in filename
	FileOutputStream myoutStream(outfile,APPEND_MODE);
	myoutStream.write(buff,(int)myData.length);
	myoutStream.close();
}
void UnPackDirectory(const String& directoryname,FileInputStream& inbuffer,struct MyFileData& myData)//ͨ��myData�Ѿ�ȷ�����Ǹ�Ŀ¼�������ٶ����ٶ������ļ�����
{
	Directory directory(directoryname);
	//��������
	directory.setLastModified(myData.lastModifiedTime);
	directory.setCreationTime(myData.creationTime);
	directory.setLastAccess(myData.lastAccessTime);
	directory.setFileAttrs(myData.attrs); 
	// 	
	int64 filenumber=myData.filenumber;
	for(int i=0;i<filenumber;i++)
	{
		inbuffer.read((unsigned char*)&myData,sizeof(myData));//myData��Ϊ���ļ��ṹ
		int64 filenumber=myData.filenumber;
		if(filenumber==-1)//���ļ�
		{
			File* subfile=directory.createFile(String(myData.name,myData.namelen));//�������ļ�
			UnPackFile((*subfile).getFullName(),inbuffer,myData);
		} 
		else              //��Ŀ¼ 
		{
			Directory subDirectory=directory.createSubDirectory(String(myData.name,myData.namelen));//������Ŀ¼
			UnPackDirectory(subDirectory.getFullName(),inbuffer,myData);
		} 		
	} 
}
