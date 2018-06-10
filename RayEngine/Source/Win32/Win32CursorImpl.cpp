#include "..\..\Include\Win32\Win32CursorImpl.h"
#include "Win32Bitmap.h"

#if defined (RE_PLATFORM_WINDOWS)
namespace RayEngine
{
	namespace System
	{
		Win32CursorImpl::Win32CursorImpl()
			: m_Hcursor(0),
			m_Shared(false)
		{
		}

		Win32CursorImpl::~Win32CursorImpl()
		{
			if (!m_Shared)
			{
				if (m_Hcursor != 0)
				{
					DestroyCursor(m_Hcursor);
					m_Hcursor = 0;
				}
			}
		}

		bool Win32CursorImpl::LoadFromFile(const Tchar* const filename, const Tchar* filepath, int32 x, int32 y)
		{
			//Destroy old cursor if method is called multiple times
			if (m_Hcursor != 0)
			{
				DestroyCursor(m_Hcursor);
				m_Hcursor = 0;
			}

			//Create bitmap
			Win32Bitmap bitmap(filename, filepath, 24, 24);
			if (!bitmap.IsValid())
				return false;

			ICONINFO cInfo = {};
			cInfo.fIcon = false;
			cInfo.xHotspot = x;
			cInfo.yHotspot = y;
			cInfo.hbmMask = bitmap.GetMask();
			cInfo.hbmColor = bitmap.GetBitmap();

			//Create cursor handle
			SetLastError(0);
			HCURSOR hCursor = static_cast<HCURSOR>(CreateIconIndirect(&cInfo));
			if (hCursor == 0)
			{
				int32 err = GetLastError();
				return false;
			}

			m_Hcursor = hCursor;
			return true;
		}

		bool Win32CursorImpl::LoadFromOS(STANDARD_CURSOR cursor)
		{
			Tchar* winCursor = nullptr;
			switch (cursor)
			{
			case STANDARD_CURSOR_ARROW_HELP:
				winCursor = IDC_HELP;
				break;
			case STANDARD_CURSOR_ARROW_WAIT:
				winCursor = IDC_APPSTARTING;
				break;
			case STANDARD_CURSOR_CROSS:
				winCursor = IDC_CROSS;
				break;
			case STANDARD_CURSOR_CLICK:
				winCursor = IDC_HAND;
				break;
			case STANDARD_CURSOR_WAIT:
				winCursor = IDC_WAIT;
				break;
			case STANDARD_CURSOR_TEXTMARKER:
				winCursor = IDC_IBEAM;
				break;
			case STANDARD_CURSOR_DENIED:
				winCursor = IDC_NO;
				break;
			case STANDARD_CURSOR_CLICK_PERSON:
				winCursor = IDC_PERSON;
				break;
			case STANDARD_CURSOR_CLICK_LOCATION:
				winCursor = IDC_PIN;
				break;
			case STANDARD_CURSOR_MOVE:
				winCursor = IDC_SIZEALL;
				break;
			case STANDARD_CURSOR_SIZE_NS:
				winCursor = IDC_SIZENS;
				break;
			case STANDARD_CURSOR_SIZE_NE_SW:
				winCursor = IDC_SIZENESW;
				break;
			case STANDARD_CURSOR_SIZE_WE:
				winCursor = IDC_SIZEWE;
				break;
			case STANDARD_CURSOR_SIZE_NW_SE:
				winCursor = IDC_SIZENWSE;
				break;
			case STANDARD_CURSOR_STANDARD:
			default:
				winCursor = IDC_ARROW;
				break;
			}

			//Get OS-Cursor
			SetLastError(0);
			HCURSOR hCursor = static_cast<HCURSOR>(LoadImage(0, winCursor, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED | LR_DEFAULTCOLOR));
			if (hCursor == 0)
			{
				int32 error = GetLastError();
				return false;
			}

			//Set shared
			m_Shared = true;

			//The Cursor is OS-Owned cannot copy since animated cursor wont work
			m_Hcursor = hCursor;
			return (m_Hcursor != 0);
		}

		bool Win32CursorImpl::IsShared() const
		{
			return m_Shared;
		}

		CursorImpl* Win32CursorImpl::Copy()
		{
			Win32CursorImpl* instance = new Win32CursorImpl();
			if (m_Shared)
				instance->m_Hcursor = m_Hcursor;
			else
				instance->m_Hcursor = static_cast<HCURSOR>(CopyImage(static_cast<HANDLE>(m_Hcursor), IMAGE_CURSOR, 0, 0, 0));
				
			instance->m_Shared = m_Shared;
			return instance;
		}

		CursorImpl* Win32CursorImpl::Move()
		{
			Win32CursorImpl* instance = new Win32CursorImpl();
			instance->m_Hcursor = m_Hcursor;
			instance->m_Shared = m_Shared;

			m_Hcursor = 0;
			m_Shared = false;

			return instance;
		}

		HCURSOR Win32CursorImpl::GetHCURSOR() const
		{
			return m_Hcursor;
		}
	}
}

#endif