#pragma once
#pragma comment(lib, "urlmon.lib")

#include <urlmon.h>

class CBindStatusCallback : public IBindStatusCallback
{
	// Inherited via IBindStatusCallback
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppvObject) override
	{
		if (!ppvObject)
			return E_INVALIDARG;

		if (IID_IUnknown == riid)
			*ppvObject = static_cast<IBindStatusCallback*>(this);
		else if (IID_IBindStatusCallback == riid)
			*ppvObject = static_cast<IBindStatusCallback*>(this);
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}

		reinterpret_cast<IUnknown*>(*ppvObject)->AddRef();
		return S_OK;
	}
	virtual ULONG __stdcall AddRef(void) override
	{
		return 1;
	}
	virtual ULONG __stdcall Release(void) override
	{
		return 1;
	}
	virtual HRESULT __stdcall OnStartBinding(DWORD dwReserved, IBinding * pib) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT __stdcall GetPriority(LONG * pnPriority) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT __stdcall OnLowResource(DWORD reserved) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT __stdcall OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT __stdcall OnStopBinding(HRESULT hresult, LPCWSTR szError) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT __stdcall GetBindInfo(DWORD * grfBINDF, BINDINFO * pbindinfo) override
	{
		*grfBINDF = BINDF_GETNEWESTVERSION;

		DWORD cbSize = pbindinfo->cbSize;
		memset(pbindinfo, 0, cbSize);
		pbindinfo->cbSize = cbSize;
		pbindinfo->dwCodePage = CP_UTF8;

		return S_OK;
	}
	virtual HRESULT __stdcall OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC * pformatetc, STGMEDIUM * pstgmed) override
	{
		return E_NOTIMPL;
	}
	virtual HRESULT __stdcall OnObjectAvailable(REFIID riid, IUnknown * punk) override
	{
		return E_NOTIMPL;
	}
};