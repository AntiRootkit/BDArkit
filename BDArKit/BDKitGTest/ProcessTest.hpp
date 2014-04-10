#pragma once

#include <time.h>
#include <algorithm>
#include "gtest\gtest.h"
#include "BDArKit\KitHelper.h"
#include "Utils\Macro.hpp"
#include "Utils\String.hpp"
#include "Utils\SystemTools.hpp"
#include "Utils\Process.hpp"


#pragma warning(disable: 4800) //'BOOL' : forcing value to bool 'true' or 'false' (performance warning)


extern IKernelUtils*	g_pSysKit;
extern tstring			g_strModulePath;
extern tstring			g_strModuleFileFullName;
extern HANDLE			g_ExitSubProcess;
extern LPCTSTR			g_TestRunCommand;

class ProcessOperTest : public testing::Test
{
protected:
	static void SetUpTestCase() {
		ResetEvent (g_ExitSubProcess);	
	}

	static void TearDownTestCase() {
		SetEvent (g_ExitSubProcess);	
	}

	virtual void SetUp() {
		TCHAR Buffer[10] = _T("\0");
		time_t	tt;
		time(&tt);

		ResetEvent (g_ExitSubProcess);
		_itot_s(tt * rand() % 1000000, Buffer, ARRAY_SIZE(Buffer), 16);
		tstring strSamplePath = g_strModulePath;
		strSamplePath.append (_T("TestSample\\"));
		FileTools::CreateDirectoryNested(strSamplePath.c_str ());
		m_SampleProcessName = tstring(Buffer) + tstring(_T(".exe"));
		m_SampleFile = strSamplePath +m_SampleProcessName;
		::CopyFile(g_strModuleFileFullName.c_str (), m_SampleFile.c_str (), TRUE);
		ProcessTools::RunWithProcess (m_SampleFile, tstring(g_TestRunCommand));
	}

	virtual void TearDown() {
		SetEvent (g_ExitSubProcess);
		tstring	strDirPath  = m_SampleFile;
		tstring lastPath	= strDirPath.substr(strDirPath.find_first_not_of (g_strModulePath));
		lastPath.erase (lastPath.find (_T("\\")));
		strDirPath = g_strModulePath + lastPath;
		ProcessTools::KillProcessByName (m_SampleProcessName);
		FileTools::RemoveDirecotryNested(strDirPath.c_str ());
	}

public:
	static tstring m_SampleFile;
	static tstring m_SampleProcessName;
};
tstring ProcessOperTest::m_SampleFile;
tstring ProcessOperTest::m_SampleProcessName;


TEST_F(ProcessOperTest, OpenProcessTest1)
{
	if (g_pSysKit != NULL) {
		DWORD dwID = ProcessTools::GetProcessIDByName (ProcessOperTest::m_SampleProcessName);
		if (dwID != -1) {
			HANDLE hProcess = g_pSysKit->OpenProcess (PROCESS_ALL_ACCESS, FALSE, dwID);
			ASSERT_NE(NULL, (DWORD)hProcess) << ProcessOperTest::m_SampleProcessName.c_str ();
			ASSERT_EQ(ERROR_SUCCESS, GetLastError()) << ProcessOperTest::m_SampleProcessName.c_str ();
			if (hProcess != NULL) {
				g_pSysKit->CloseHandle (hProcess);	
			}
		}
		else {
			ASSERT_TRUE(FALSE) << "SampleProcess(%s) Run Failed, Please Check it!!" << ProcessOperTest::m_SampleProcessName.c_str ();
		}
	}
	else
	{
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";
	}
}

TEST_F(ProcessOperTest, KillProcessTest1)
{
	if (g_pSysKit != NULL) {
		DWORD dwID = ProcessTools::GetProcessIDByName (ProcessOperTest::m_SampleProcessName);
		if (dwID != -1) {
			ASSERT_TRUE(g_pSysKit->TerminateProcessId (dwID, ERROR_SUCCESS)) << ProcessOperTest::m_SampleProcessName.c_str ();
			ASSERT_EQ(ERROR_SUCCESS, GetLastError()) << ProcessOperTest::m_SampleProcessName.c_str ();
		}
		else {
			ASSERT_TRUE(FALSE) << "SampleProcess(%s) Run Failed, Please Check it!!" << ProcessOperTest::m_SampleProcessName.c_str ();
		}
	}
	else {
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";
	}
}

TEST_F(ProcessOperTest, KillProcessTest2)
{
	if (g_pSysKit != NULL) {
		DWORD dwID = ProcessTools::GetProcessIDByName (ProcessOperTest::m_SampleProcessName);
		if (dwID != -1) {
			HANDLE hProcess = g_pSysKit->OpenProcess (PROCESS_ALL_ACCESS, FALSE, dwID);
			ASSERT_NE(NULL, (DWORD)hProcess) << ProcessOperTest::m_SampleProcessName.c_str ();
			ASSERT_EQ(ERROR_SUCCESS, GetLastError()) << ProcessOperTest::m_SampleProcessName.c_str ();

			ASSERT_TRUE(g_pSysKit->TerminateProcess (hProcess, ERROR_SUCCESS)) << ProcessOperTest::m_SampleProcessName.c_str ();
			ASSERT_EQ(ERROR_SUCCESS, GetLastError()) << ProcessOperTest::m_SampleProcessName.c_str ();

			ASSERT_TRUE(g_pSysKit->CloseHandle (hProcess));
		}
		else {
			ASSERT_TRUE(FALSE) << "SampleProcess(%s) Run Failed, Please Check it!!" << ProcessOperTest::m_SampleProcessName.c_str ();
		}
	}
	else {
		ASSERT_TRUE(FALSE) << "BDArKit Initialize Failed, Please Check it!!";
	}
}