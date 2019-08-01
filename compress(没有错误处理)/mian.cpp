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
	cout<<"����ļ����룺1������ļ����룺2���Ƴ���0\n"; 
	cin>>flag;
	while(flag!=0) 
	{
			if(flag==1)
		{
			string fileordire;
			string dire;
			cout<<"��������Ҫ����ļ�����Ŀ¼��·��\n";
			cin>>fileordire;
			cout<<"����������Ŀ¼��·��\n";
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
			cout<<"��������Ҫ����ļ�\n";
			cin>>fileordire;
			cout<<"����������Ŀ¼��·��\n";
			cin>>dire; 
			pathConvert_Single2Double(fileordire);
			pathConvert_Single2Double(dire);
			String in=String(fileordire.c_str(),fileordire.size());
			String out=String(dire.c_str(),dire.size());
			MyUnPackDirectory(in,out);
		}
		cout<<"����ļ����룺1������ļ����룺2���Ƴ���0\n"; 
		cin>>flag;
	}
	
	return 0;
} 
