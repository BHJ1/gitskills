#include"compress.h"
using namespace std;
void MyPackFile(const String& filename,const String& direname)
{
	Directory directory(direname);
	File file(filename);
	File* subfile=directory.createFile(String("Pack_")+file.getName());//打包文件在前面加“Pack_” 区别
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
	//获取结构判断属性 
	File infile(buffer);
	FileInputStream myinputStream(infile);
	struct MyFileData myData;
	myinputStream.read((unsigned char*)&myData,sizeof(myData));
	if(myData.filenumber==-1)			//解压文件 
	{
		Directory outdire(outdirectory);
		File* subfile=outdire.createFile(String(myData.name,myData.namelen));       //根据获取到myData创建文件 
		UnPackFile((*subfile).getFullName(),myinputStream,myData);
	}
	else								//解压目录 
	{
		Directory outdire(outdirectory);
		Directory subdir=outdire.createSubDirectory(String(myData.name,myData.namelen));//创建目录 
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
	//将文件结构信息（mydata）写入outbuffer 
	outbuffer.write((const unsigned char*)&mydata,sizeof(mydata)); 
	//设置缓存 
	unsigned char buff[1024];						
	myinputStream.read(buff,(int)mydata.length);//文件大小 ，可能会爆内存 
	outbuffer.write(buff,mydata.length);
	myinputStream.close();
} 
void PackDirectory(const String& indirectory,FileOutputStream& outbuffer)
{
	struct MyFileData mydata;
	Directory indire(indirectory);
	//设置mydata 
	mydata.attrs=indire.getFileAttrs();
	mydata.creationTime=indire.getCreationTime();
	mydata.lastAccessTime=indire.getLastAccess();
	mydata.lastModifiedTime=indire.getLastModified();
	//mydata.length=indire.getLength();//目录没有getLength 
	mydata.namelen=indire.getName().getLength();
	for(int i=0;i<mydata.namelen;i++)
		mydata.name[i]=indire.getName().getCStr()[i];
	vector<String> subs;
	indire.list(subs);
	mydata.filenumber=subs.size();
	//保存结构信息入outbuffer
	outbuffer.write((const unsigned char*)&mydata,sizeof(mydata));
	//循环查找打包文件（目录） 
	for(String& s:subs)
	{
		if(SystemFile::isFile(s))
			PackFile(s,outbuffer);//打包文件 
		else
			PackDirectory(s,outbuffer);//打包目录 
	}
}
void UnPackFile(const String& filename,FileInputStream& inbuffer,struct MyFileData& myData)//通过myData已经确认这是个文件，不能再读，再读就是文件内容 
{
	File outfile(filename);
	//set atrr 
	outfile.setLastModified(myData.lastModifiedTime);
	outfile.setCreationTime(myData.creationTime);
	outfile.setLastAccess(myData.lastAccessTime);
	outfile.setFileAttrs(myData.attrs); 
	//read from inbuffer
	unsigned char buff[1024];
	inbuffer.read(buff,(int)myData.length);//文件大小 
	//write in filename
	FileOutputStream myoutStream(outfile,APPEND_MODE);
	myoutStream.write(buff,(int)myData.length);
	myoutStream.close();
}
void UnPackDirectory(const String& directoryname,FileInputStream& inbuffer,struct MyFileData& myData)//通过myData已经确认这是个目录，不能再读，再读就是文件内容
{
	Directory directory(directoryname);
	//设置属性
	directory.setLastModified(myData.lastModifiedTime);
	directory.setCreationTime(myData.creationTime);
	directory.setLastAccess(myData.lastAccessTime);
	directory.setFileAttrs(myData.attrs); 
	// 	
	int64 filenumber=myData.filenumber;
	for(int i=0;i<filenumber;i++)
	{
		inbuffer.read((unsigned char*)&myData,sizeof(myData));//myData变为子文件结构
		int64 filenumber=myData.filenumber;
		if(filenumber==-1)//子文件
		{
			File* subfile=directory.createFile(String(myData.name,myData.namelen));//创建子文件
			UnPackFile((*subfile).getFullName(),inbuffer,myData);
		} 
		else              //子目录 
		{
			Directory subDirectory=directory.createSubDirectory(String(myData.name,myData.namelen));//创建子目录
			UnPackDirectory(subDirectory.getFullName(),inbuffer,myData);
		} 		
	} 
}
