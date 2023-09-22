#pragma once

#include <Windows.h>
#include <exception>
#include <stdio.h>
#include <string>

class com_exception : public std::exception
{
private:
	HRESULT mResult;
	std::string mDescription;

public:
	com_exception(HRESULT hr) : mResult{ hr }, mDescription{ }
	{
	}
	com_exception(HRESULT hr, std::string msg) : mResult{ hr }, mDescription{ msg }
	{
	}

	virtual const char* what() const override
	{
		static char buffer[512]{};
		
		if (mDescription.size() == 0)
		{
			sprintf_s(buffer, "Failure with HRESULT : %x", mResult);
		}
		else
		{
			sprintf_s(buffer, "Failure with HRESULT : %x\n%s", mResult, mDescription.c_str());
		}

		return buffer;
	}
};

inline void ThrowIfFailed(HRESULT hr, std::string msg ="")
{
	if (FAILED(hr))
	{
		throw com_exception(hr, msg);
	}
}