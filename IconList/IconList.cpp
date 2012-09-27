// IconList.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <shlobj.h>
#include <shlwapi.h>

namespace
{
	HRESULT GetIconList(std::vector<std::wstring>& nameList)
	{
		CComPtr<IShellFolder> pDesktop;
		CComPtr<IEnumIDList> pIconList;
		
		ITEMIDLIST itemList = {0};
		LPITEMIDLIST pItemlist = &itemList;
		HRESULT hr = S_OK;

		hr = SHGetDesktopFolder(&pDesktop);
		if(SUCCEEDED(hr))
		{
			hr = pDesktop->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &pIconList);
			if(SUCCEEDED(hr))
			{
				ULONG count = 0;
				while((hr = pIconList->Next(1, &pItemlist, &count)) == S_OK)
				{
					STRRET info = {0};
					hr = pDesktop->GetDisplayNameOf(pItemlist, SHGDN_INFOLDER, &info);
					if(FAILED(hr))
					{
						return hr;
					}
					nameList.push_back(std::wstring(info.pOleStr));
					CoTaskMemFree(info.pOleStr);
				}
			}
		}
		return hr;
	}

	void PrintIconName(const std::vector<std::wstring>& nameList)
	{
		setlocale(LC_ALL, "");
		std::copy(
					nameList.cbegin(),
					nameList.cend(),
					std::ostream_iterator<std::wstring, wchar_t>(std::wcout, L"\n")
				 );
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(SUCCEEDED(::CoInitialize(NULL)))
	{
		std::vector<std::wstring> nameList;
		HRESULT hr = GetIconList(nameList);
		if(SUCCEEDED(hr))
		{
			PrintIconName(nameList);
		}
		::CoUninitialize();
	}
	return 0;
}

