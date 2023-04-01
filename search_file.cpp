#include<iostream>
#include<cstring>
#include<regex>
#include<windows.h>
using namespace std;
int search_dir(string dir,string regexp,bool directory=false)
{
	int count=0;
	string full_name,trunc_name;
	HANDLE hFind;
	WIN32_FIND_DATA findData;
	char dirNew[1024];
	strcpy(dirNew, dir.c_str());
	strcat(dirNew, "\\*.*");
	hFind=FindFirstFile(dirNew, &findData);
	do
	{
		if((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0&&strcmp(findData.cFileName,".")!=0&&strcmp(findData.cFileName,"..")!=0)
		{
			strcpy(dirNew,dir.c_str());
			strcat(dirNew,"\\");
			strcat(dirNew,findData.cFileName);
			count+=search_dir(dirNew,regexp);
		}
		else if(strcmp(findData.cFileName,".")!=0&&strcmp(findData.cFileName,"..")!=0)
		{
			full_name=(dir+"\\"+findData.cFileName);
			if(directory)
			{
				if(regex_match(string(full_name),regex(regexp)))
				{
					cout<<full_name<<endl;
					count++;
				}
			}
			else
			{
				if(regex_match(string(findData.cFileName),regex(regexp)))
				{
					cout<<full_name<<endl;
					count++;
				}
			}
		}
	}
	while(FindNextFile(hFind,&findData));
	FindClose(hFind);
	return count;
}
int main(int argc,char** argv)
{
	int ret;
	if(argc==1)
	{
		cout<<"usage:\n\t"<<argv[0]<<" <regular_expression> <-d|-f> [directory]\nsearch files\nuse -d to search in the path(including filename)";
		cout<<"\nuse -f to search in the filename";
		exit(0);
	}
	if(argc==2)
	{
		cout<<"error:not enough parameters(should be 2 or 3, got 1)\n";
		cout<<"usage:\n\t"<<argv[0]<<" <regular_expression> <-d|-f> [directory]\nsearch files\nuse -d to search in the path(including filename)";
		cout<<"\nuse -f to search in the filename";
		exit(0);
	}
	string reg=argv[1],mode=argv[2];
	bool dir=false;
	if(argc==3)
	{
		if(mode!="-d"&&mode!="-f")
			cout<<"warning:invalid mode \""<<mode<<"\", defaulting to -f\n";
		if(mode=="-d")
			dir=true;
		ret=search_dir(".",reg,dir);
	}
	if(argc>=4)
	{
		if(argc>4)
			cout<<"warning:extra parameters, discarding\n";
		if(mode!="-d"&&mode!="-f")
			cout<<"warning:invalid mode \""<<mode<<"\", defaulting to -f\n";
		if(mode=="-d")
			dir=true;
		ret=search_dir(string(argv[3]),reg,dir);
	}
	return ret;
}
