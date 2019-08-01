#include <abprec.h>
#include <iostream>
using namespace std;
//符合使用习惯 
void MyPackFile(const String& filename,const String& direname);				//把文件filename打包写在目录direname里 
void MyPackDirectory(const String& indirectory,const String& outdirectory); //把目录indirectory打包，放在目录outdirectory里
void MyUnPackDirectory(const String& buffer,const String& outdirectory);    //把文件buffer(可能是文件也可能是目录)解包，放在目录outdirectory里 
//容易调用递归 
void PackFile(const String& filename,FileOutputStream& outbuffer);//打包文件filename写入文件流outbuffer
void PackDirectory(const String& indirectory,FileOutputStream& outbuffer); //打包目录indirectory写入文件流outbuffer
void UnPackFile(const String& filename,FileInputStream& inbuffer,struct MyFileData& myData);//解压文件流inbuffer写入文件filename ----
void UnPackDirectory(const String& directoryname,FileInputStream& inbuffer,struct MyFileData& myData); //解压文件流inbuffer写入目录directoryname
//自定义文件结构 
struct MyFileData
{
	//文件，目录都要设置 
	FileAttributes attrs;
	int64 creationTime;
	int64 lastAccessTime;
	int64 lastModifiedTime;
	// 
	int64 length;//目录没有文件大小（不需要） 
	int64 filenumber;//-1:文件，0：空目录，>=1:文件个数 
	int64 namelen;//没有namelen,的不出名字 
	tchar_t name[128];
};

