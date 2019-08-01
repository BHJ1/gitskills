#include"compress.h"
using namespace std;
void pathConvert_Single2Double(string& s){
	string::size_type pos = 0;
	while ((pos = s.find('\\', pos)) != string::npos){
		s.insert(pos, "\\");
		pos = pos + 2;
	}
}
int main(void)
{
	int flag=-1;
	cout<<"打包文件输入：1；解包文件输入：2；推出：0\n"; 
	cin>>flag;
	while(flag!=0) 
	{
			if(flag==1)
		{
			string fileordire;
			string dire;
			cout<<"请输入需要打包文件或者目录的路径\n";
			cin>>fileordire;
			cout<<"请输入需存放目录的路径\n";
			cin>>dire; 
			pathConvert_Single2Double(fileordire);
			pathConvert_Single2Double(dire);
			String in=String(fileordire.c_str(),fileordire.size());
			String out=String(dire.c_str(),dire.size());
			if(SystemFile::isFile(in))
				MyPackFile(in,out);
			else
				MyPackDirectory(in,out);		
		}
		else
		{
			string fileordire;
			string dire;
			cout<<"请输入需要解包文件\n";
			cin>>fileordire;
			cout<<"请输入需存放目录的路径\n";
			cin>>dire; 
			pathConvert_Single2Double(fileordire);
			pathConvert_Single2Double(dire);
			String in=String(fileordire.c_str(),fileordire.size());
			String out=String(dire.c_str(),dire.size());
			MyUnPackDirectory(in,out);
		}
		cout<<"打包文件输入：1；解包文件输入：2；推出：0\n"; 
		cin>>flag;
	}
	
	return 0;
} 
