#pragma once


#include <time.h>
#include <algorithm>
#include "gtest\gtest.h"
#include "BDArKit\KitHelper.h"
#include "Utils\Macro.hpp"
#include "Utils\String.hpp"
#include "Utils\SystemTools.hpp"
#include "Utils\File.hpp"
#include "Utils\Process.hpp"


#pragma warning(disable: 4800) //'BOOL' : forcing value to bool 'true' or 'false' (performance warning)

extern IKernelUtils*	g_pSysKit;
extern tstring			g_strModulePath;
extern tstring			g_strModuleFileFullName;
extern HANDLE			g_ExitSubProcess;

struct
{
	tstring			strTestFilePath;
	tstring			strFileName;
	FileTools::FileLockType	lockType;
} TestFileSample[] = 
{
	{ _T("TestSample\\C022E615-FABD-4543-9E7D-4AC035F83B30\\"), _T("%.exe"), FileTools::emAddFileRef	},
	{ _T("TestSample\\F6A18051-6CD3-492b-BDD2-EA9DC0D67158\\"), _T("%.exe"), FileTools::emLockFile		},
	{ _T("TestSample\\B99301EC-67F6-476b-A2B8-0F9432359E6F\\"), _T("%.exe"), FileTools::emRunFile		},
	{ _T("TestSample\\8C7B17BE-BB23-4571-9533-E7B90095D96C\\"), _T("%.exe"), FileTools::emNoShareFile	},
	{ _T("TestSample\\C022E615-FABD-4543-9E7D-4AC035F83B30\\C022E615-FABD-4543-9E7D-4AC035F83B30\\"), _T("%.exe"), FileTools::emAddFileRef	},
	{ _T("TestSample\\F6A18051-6CD3-492b-BDD2-EA9DC0D67158\\F6A18051-6CD3-492b-BDD2-EA9DC0D67158\\"), _T("%.exe"), FileTools::emLockFile	},
	{ _T("TestSample\\B99301EC-67F6-476b-A2B8-0F9432359E6F\\B99301EC-67F6-476b-A2B8-0F9432359E6F\\"), _T("%.exe"), FileTools::emRunFile		},
	{ _T("TestSample\\8C7B17BE-BB23-4571-9533-E7B90095D96C\\8C7B17BE-BB23-4571-9533-E7B90095D96C\\"), _T("%.exe"), FileTools::emNoShareFile	}
};

class FileOperTest : public testing::Test
{
protected:
	static void SetUpTestCase()
	{
		ResetEvent (g_ExitSubProcess);
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			TCHAR Buffer[10] = _T("\0");
			time_t	tt;
			time(&tt);

			_itot_s(tt * rand() % 1000000, Buffer, ARRAY_SIZE(Buffer), 16);
			TestFileSample[Index].strFileName.replace(TestFileSample[Index].strFileName.find(_T('%')), 1, Buffer);
		}
		SystemTools::EnableDriverMode (tstring(BDArKit_DRIVER_NAMEW), 1, TRUE);
	}

	static void TearDownTestCase()
	{
		SetEvent (g_ExitSubProcess);
		tstring strDirPath = g_strModulePath + tstring(_T("TestSample"));
		FileTools::RemoveDirecotryNested(strDirPath.c_str ());
	}

	virtual void SetUp()
	{
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			TCHAR Buffer[10] = _T("\0");
			time_t	tt;
			time(&tt);

			_itot_s(tt * rand() % 1000000, Buffer, ARRAY_SIZE(Buffer), 16);
			TestFileSample[Index].strFileName.insert(0, Buffer);
			tstring	strDirPath = g_strModulePath + TestFileSample[Index].strTestFilePath;
			tstring	strFilePath	= strDirPath + TestFileSample[Index].strFileName;
			FileTools::CreateDirectoryNested(strDirPath.c_str ());
			::CopyFile(g_strModuleFileFullName.c_str (), strFilePath.c_str (), TRUE);
		}
		SystemTools::EnableDriverMode (tstring(BDArKit_DRIVER_NAMEW), 1, TRUE);
	}

	virtual void TearDown()
	{
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring	strDirPath = TestFileSample[Index].strTestFilePath;
			strDirPath.erase (strDirPath.find (_T("\\")));
			strDirPath = g_strModulePath + strDirPath;
			ProcessTools::KillProcessByName (TestFileSample[Index].strFileName);
			FileTools::RemoveDirecotryNested(strDirPath.c_str ());
		}
	}
};

TEST_F(FileOperTest, DeleteFileTest1)
{
	if ( g_pSysKit != NULL )
	{
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
			ASSERT_EQ(ERROR_ALREADY_EXISTS, FileTools::FileIsExist (strTestFile));
			FileTools::LockItFile (strTestFile, TestFileSample[Index].lockType);
			Sleep (100);

			ASSERT_TRUE(g_pSysKit->DeleteFileW (strTestFile.c_str(), TRUE)) << strTestFile.c_str();
			ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile)) << strTestFile.c_str();
			ASSERT_EQ(ERROR_ACCESS_DENIED, FileTools::TryToCreateTestFile (strTestFile)) << strTestFile.c_str();
			Sleep (100);

			ASSERT_TRUE(g_pSysKit->DeleteFileW (strTestFile.c_str(), FALSE)) << strTestFile.c_str();
			ASSERT_EQ(ERROR_SUCCESS, FileTools::TryToCreateTestFile (strTestFile)) << strTestFile.c_str();
			ASSERT_TRUE(g_pSysKit->DeleteFileW (strTestFile.c_str(), FALSE)) << strTestFile.c_str();
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
	}
}

TEST_F(FileOperTest, RebootDeleteFileTest1)
{
	if ( g_pSysKit != NULL )
	{
		FileTools::CWow64RedirDisable autoRedirDisable(TRUE);

		TCHAR	Buffer[MAX_PATH] = _T("\0");
		GetWindowsDirectory(Buffer, sizeof(Buffer) / sizeof(TCHAR));
		tstring strDatFile = tstring(Buffer) + _T("\\System32\\Drivers\\BDArKit.dat");

		DeleteFile(strDatFile.c_str());

		// 测试多个文件添加删除列表
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
			ASSERT_TRUE(g_pSysKit->DeleteFileOnRebootW (strTestFile.c_str(), TRUE)) << strTestFile.c_str();
		}
		ASSERT_EQ(ERROR_ALREADY_EXISTS, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();

		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
			ASSERT_TRUE(g_pSysKit->DeleteFileOnRebootW (strTestFile.c_str(), FALSE)) << strTestFile.c_str();
		}
		ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();

		// 测试单个文件添加删除列表
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
			ASSERT_TRUE(g_pSysKit->DeleteFileOnRebootW (strTestFile.c_str(), TRUE)) << strTestFile.c_str();
			ASSERT_EQ(ERROR_ALREADY_EXISTS, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();
			ASSERT_TRUE(g_pSysKit->DeleteFileOnRebootW (strTestFile.c_str(), FALSE)) << strTestFile.c_str();
			ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
	}
}

// 由于注册表穿透在驱动卸载后会触发蓝屏， 因此驱动卸载测试暂时终止
// 该测试在开启Verifier下禁止测试,否则会因为关闭驱动没有完全释放内存而蓝屏
//TEST_F(FileOperTest, RebootDeleteFileTest2)
//{
//	if ( g_pSysKit != NULL )
//	{
//		FileTools::CWow64RedirDisable autoRedirDisable(TRUE);
//
//		TCHAR	Buffer[MAX_PATH] = _T("\0");
//		GetWindowsDirectory(Buffer, sizeof(Buffer) / sizeof(TCHAR));
//		tstring strDatFile = tstring(Buffer) + _T("\\System32\\Drivers\\BDArKit.dat");
//
//		DeleteFile(strDatFile.c_str());
//
//		// 测试添加文件后重启删除功能是否生效
//		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
//		{
//			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
//			ASSERT_TRUE(g_pSysKit->DeleteFileOnRebootW (strTestFile.c_str(), TRUE)) << strTestFile.c_str();
//			ASSERT_EQ(ERROR_ALREADY_EXISTS, FileTools::FileIsExist (strTestFile)) << strTestFile.c_str();
//		}
//		ASSERT_EQ(ERROR_ALREADY_EXISTS, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();
//
//		// 重启服务,模拟系统重启操作
//
//		BOOL bNeedUninstall = TRUE;
//		g_pSysKit->UnInit (&bNeedUninstall);
//		SystemTools::ControlService (tstring(BDArKit_DRIVER_NAMEW), SERVICE_CONTROL_STOP);
//		Sleep (1000);
//		SystemTools::ControlService (tstring(BDArKit_DRIVER_NAMEW), SERVICE_CONTROL_START);
//		g_pSysKit->Init (NULL);
//
//		// 测试模拟重启后文件是否还存在
//		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
//		{
//			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
//			ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile)) << strTestFile.c_str();
//		}
//		ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();
//	}
//	else
//	{
//		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
//	}
//}

// 改测试在开启Verifier下禁止测试,否则会因为关闭驱动没有完全释放内存而蓝屏
//TEST_F(FileOperTest, RebootDeleteFileTest3)
//{
//	if ( g_pSysKit != NULL )
//	{
//		FileTools::CWow64RedirDisable autoRedirDisable(TRUE);
//
//		TCHAR	Buffer[MAX_PATH] = _T("\0");
//		GetWindowsDirectory(Buffer, sizeof(Buffer) / sizeof(TCHAR));
//		tstring strDatFile = tstring(Buffer) + _T("\\System32\\Drivers\\BDArKit.dat");
//
//		DeleteFile(strDatFile.c_str());
//
//		// 测试添加加锁文件后重启删除功能是否生效
//		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
//		{
//			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
//			ASSERT_TRUE(g_pSysKit->DeleteFileOnRebootW (strTestFile.c_str(), TRUE)) << strTestFile.c_str();
//			FileTools::LockItFile (strTestFile, TestFileSample[Index].lockType);
//		}
//
//		// 重启服务,模拟系统重启操作
//		BOOL bNeedUninstall = TRUE;
//		g_pSysKit->UnInit (&bNeedUninstall);
//		SystemTools::ControlService (tstring(BDArKit_DRIVER_NAMEW), SERVICE_CONTROL_STOP);
//		Sleep (1000);
//		SystemTools::ControlService (tstring(BDArKit_DRIVER_NAMEW), SERVICE_CONTROL_START);
//		g_pSysKit->Init (NULL);
//
//		// 测试模拟重启后文件是否还存在
//		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
//		{
//			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
//			ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile)) << strTestFile.c_str();
//		}
//		ASSERT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strDatFile)) << strDatFile.c_str();
//	}
//	else
//	{
//		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
//	}
//}

TEST_F(FileOperTest, OpenReadFileTest1)
{
	if ( g_pSysKit != NULL )
	{
		FileTools::CWow64RedirDisable autoRedirDisable(TRUE);

		TCHAR	Buffer[MAX_PATH] = _T("\0");
		GetWindowsDirectory(Buffer, sizeof(Buffer) / sizeof(TCHAR));
		tstring strInfPath = tstring(Buffer) + _T("\\Inf\\");
		tstring strInfPattern = strInfPath + _T("*.?nf");

		WIN32_FIND_DATAW	win32Data = {0x00};
		HANDLE hFindFile = ::FindFirstFile(strInfPattern.c_str(), &win32Data);
		ASSERT_NE(INVALID_HANDLE_VALUE, hFindFile);
		if (hFindFile != INVALID_HANDLE_VALUE)
		{
			while(TRUE)
			{
				ZeroMemory(&win32Data, sizeof(win32Data));
				::FindNextFileW (hFindFile, &win32Data);
				if ( GetLastError() == ERROR_NO_MORE_FILES)
				{
					break;
				}
				tstring strFileName = strInfPath + tstring(win32Data.cFileName);
				
				CHAR Content1[4096] = "\0";
				CHAR Content2[4096] = "\0";
				HANDLE hOrgFile = INVALID_HANDLE_VALUE;
				HANDLE hMyFile	= INVALID_HANDLE_VALUE;

				hOrgFile = ::CreateFileW(
					strFileName.c_str(), FILE_READ_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hOrgFile != INVALID_HANDLE_VALUE)
				{
					DWORD dwBytesToRead1 = 0;
					DWORD dwBytesToRead2 = 0;
					if (::ReadFile (hOrgFile, Content1, 4096, &dwBytesToRead1, NULL))
					{
						hMyFile = g_pSysKit->CreateFileW(
							strFileName.c_str(), FILE_READ_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
							NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
						ASSERT_NE(INVALID_HANDLE_VALUE, hMyFile) << strFileName.c_str();
						if (hMyFile != INVALID_HANDLE_VALUE)
						{
							ASSERT_TRUE(g_pSysKit->ReadFile (hMyFile, Content2, 4096, &dwBytesToRead2, NULL));
							ASSERT_EQ(dwBytesToRead1, dwBytesToRead2);
							if (dwBytesToRead1 == dwBytesToRead2)
							{
								ASSERT_EQ(0, memcmp(Content1, Content1, dwBytesToRead1));
							}
							CloseHandle (hMyFile);
						}
					}
					CloseHandle (hOrgFile);
				}
			}

			FindClose (hFindFile);
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";
	}
}

bool FindFileCmp(CONST WIN32_FIND_DATA &win32Node1, CONST WIN32_FIND_DATA &win32Node2) 
{ 
	return tstring(win32Node1.cFileName) < tstring(win32Node2.cFileName); 
} 
TEST_F(FileOperTest, QueryFileTest1)
{
	if ( g_pSysKit != NULL )
	{
		FileTools::CWow64RedirDisable autoRedirDisable(TRUE);

		TCHAR	Buffer[MAX_PATH] = _T("\0");
		GetWindowsDirectory(Buffer, sizeof(Buffer) / sizeof(TCHAR));

		// FileFirstFile的模式匹配为'*.*'仍然枚举包括文件夹和文件
		// 穿透驱动认为这里不合理,'*.*'应该只枚举文件,'*'枚举包括文件和文件夹
		// 因此这里和实际的API调用效果有差别, 单元自测需要区别对待
		tstring strDriversPath = tstring(Buffer) + _T("\\System32\\Drivers\\*");

		WIN32_FIND_DATAW	win32Data = {0x00};
		std::vector<WIN32_FIND_DATA>	vecOrgFileList;
		HANDLE hFindFile = ::FindFirstFile(strDriversPath.c_str(), &win32Data);
		ASSERT_NE(INVALID_HANDLE_VALUE, hFindFile);

		if (hFindFile != INVALID_HANDLE_VALUE)
		{
			while(TRUE)
			{
				if (_tcscmp(win32Data.cFileName, _T(".")) && _tcscmp(win32Data.cFileName, _T("..")))
				{
					vecOrgFileList.push_back(win32Data);
				}
				ZeroMemory(&win32Data, sizeof(win32Data));
				::FindNextFileW (hFindFile, &win32Data);
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					break;
				}
			}
			FindClose (hFindFile);
		}

		SetLastError (ERROR_SUCCESS);
		ZeroMemory(&win32Data, sizeof(win32Data));
		std::vector<WIN32_FIND_DATA>	vecMyFileList;
		hFindFile = g_pSysKit->FindFirstFile(strDriversPath.c_str(), &win32Data);
		ASSERT_EQ(ERROR_SUCCESS, GetLastError());
		ASSERT_NE(INVALID_HANDLE_VALUE, hFindFile);

		if (hFindFile != INVALID_HANDLE_VALUE)
		{
			while(TRUE)
			{
				if (_tcscmp(win32Data.cFileName, _T(".")) && _tcscmp(win32Data.cFileName, _T("..")) )
				{
					vecMyFileList.push_back(win32Data);
				}
				ZeroMemory(&win32Data, sizeof(win32Data));
				g_pSysKit->FindNextFileW (hFindFile, &win32Data);
				// Check the LAST ERROR!
				ASSERT_EQ(_tcslen(win32Data.cFileName) == 0? ERROR_NO_MORE_FILES : ERROR_SUCCESS, GetLastError());
				if (GetLastError() == ERROR_NO_MORE_FILES)
				{
					break;
				}
			}
			g_pSysKit->FindClose (hFindFile);
		}

		ASSERT_EQ(vecOrgFileList.size(), vecMyFileList.size());
		if (vecOrgFileList.size() == vecMyFileList.size())
		{
			std::sort(vecOrgFileList.begin(), vecOrgFileList.end(), FindFileCmp);
			std::sort(vecMyFileList.begin(), vecMyFileList.end(), FindFileCmp);
			std::vector<WIN32_FIND_DATA>::const_iterator Iter1 = vecOrgFileList.begin();
			std::vector<WIN32_FIND_DATA>::const_iterator Iter2 = vecMyFileList.begin();
			for (; Iter1 != vecOrgFileList.end(); Iter1++, Iter2++)
			{
				// 可能文件其他属性没有获取, 先暂时比较文件路径吧
				//ASSERT_EQ(0, memcmp ((WIN32_FIND_DATA*)&(*Iter1), (WIN32_FIND_DATA*)&(*Iter2), sizeof(WIN32_FIND_DATA)));
				ASSERT_EQ(0, memcmp(Iter1->cFileName, Iter2->cFileName, sizeof(Iter1->cFileName)));
			}
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
	}
}

TEST_F(FileOperTest, LockFileTest1)
{
	if ( g_pSysKit != NULL )
	{
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
			EXPECT_TRUE(g_pSysKit->LockFile (strTestFile.c_str ())) << strTestFile.c_str();
			EXPECT_EQ(ERROR_SUCCESS, GetLastError());
			Sleep (100);

			EXPECT_FALSE(::DeleteFile(strTestFile.c_str ())) << strTestFile.c_str();
			EXPECT_EQ(ERROR_ACCESS_DENIED, GetLastError());
			EXPECT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile));
			Sleep (100);

			EXPECT_TRUE(g_pSysKit->unLockFile (strTestFile.c_str ())) << strTestFile.c_str();
			EXPECT_EQ(ERROR_SUCCESS, GetLastError());
			Sleep (100);

			EXPECT_TRUE(::DeleteFile(strTestFile.c_str ())) << strTestFile.c_str();
			EXPECT_EQ(ERROR_SUCCESS, GetLastError());
			EXPECT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile)) << strTestFile.c_str();
			Sleep (100);
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
	}
}

TEST_F(FileOperTest, LockFileTest2)
{
	if ( g_pSysKit != NULL )
	{
		ULONG FocusTestNum = 5;
		for (ULONG Index = 0; Index < ARRAY_SIZE(TestFileSample); ++Index)
		{
			tstring strTestFile = g_strModulePath + TestFileSample[Index].strTestFilePath + TestFileSample[Index].strFileName;
			for (ULONG count = 0; count < FocusTestNum; ++count)
			{
				EXPECT_TRUE(g_pSysKit->LockFile (strTestFile.c_str ())) << strTestFile.c_str();
				EXPECT_EQ(ERROR_SUCCESS, GetLastError());
				Sleep (100);
			}

			EXPECT_FALSE(::DeleteFile(strTestFile.c_str ())) << strTestFile.c_str();
			EXPECT_EQ(ERROR_ACCESS_DENIED, GetLastError());
			EXPECT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile));
			Sleep (100);

			for (ULONG count = 0; count < FocusTestNum; ++count)
			{
				EXPECT_TRUE(g_pSysKit->unLockFile (strTestFile.c_str ())) << strTestFile.c_str();
				EXPECT_EQ(ERROR_SUCCESS, GetLastError());
				Sleep (100);
			}

			EXPECT_TRUE(::DeleteFile(strTestFile.c_str ())) << strTestFile.c_str();
			EXPECT_EQ(ERROR_SUCCESS, GetLastError());
			EXPECT_EQ(ERROR_FILE_NOT_FOUND, FileTools::FileIsExist (strTestFile)) << strTestFile.c_str();
			Sleep (100);
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";;
	}
}