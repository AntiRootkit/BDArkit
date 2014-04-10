#pragma once


namespace Utility
{

}

namespace OSInfo
{
#define IDR_DRIVER_32                   10100
#define IDR_DRIVER_64                   10101

	BOOL Is32Bit();
	BOOL Is64Bit();
}

namespace File
{
	class CWow64RedirDisable
	{
	public:
		CWow64RedirDisable(bool bAutoDisable = false);
		~CWow64RedirDisable();

		bool DisableRedirection();
		bool Revert();

	private:
		PVOID m_OldValue;
	};

	static FILETIME fileTimeFromLargeInteger(const LARGE_INTEGER& llValue)
	{
		FILETIME ft = { llValue.LowPart, llValue.HighPart };
		return ft;
	}
}