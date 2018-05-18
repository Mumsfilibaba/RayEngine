#include "..\..\Include\Win32\Win32IconImpl.h"
#include "Win32Bitmap.h"

#if defined (RE_PLATFORM_WINDOWS)
namespace RayEngine
{
	namespace System
	{
		Win32IconImpl::Win32IconImpl()
			: m_Hicon(0)
		{
		}

		Win32IconImpl::~Win32IconImpl()
		{
			if (m_Hicon != 0)
			{
				DestroyIcon(m_Hicon);
				m_Hicon = 0;
			}
		}

		bool Win32IconImpl::LoadFromFile(const Tchar* filename, const Tchar* filepath)
		{
			//Delete old icon if function should be called multiple times
			if (m_Hicon != 0)
			{
				DestroyIcon(m_Hicon);
				m_Hicon = 0;
			}

			Win32Bitmap bitmap(filename, filepath, 32, 32);
			if (!bitmap.IsValid())
				return false;

			//Create desc
			ICONINFO iInfo = {};
			iInfo.fIcon = true;
			iInfo.hbmColor = bitmap.GetBitmap();
			iInfo.hbmMask = bitmap.GetMask();

			//Create icon handle
			SetLastError(0);
			HICON hIcon = CreateIconIndirect(&iInfo);			
			if (hIcon == 0)
			{
				int32 err = GetLastError();
				return false;
			}

			m_Hicon = hIcon;
			return true;
		}

		bool Win32IconImpl::LoadFromOS(STANDARD_ICON icon)
		{
			//Delete old icon if function should be called multiple times
			if (m_Hicon != 0)
			{
				DestroyIcon(m_Hicon);
				m_Hicon = 0;
			}

			Tchar* winIcon = 0;
			if (icon == STANDARD_ICON_APP)
				winIcon = IDI_APPLICATION;
			else if (icon == STANDARD_ICON_WARNING)
				winIcon = IDI_WARNING;
			else if (icon == STANDARD_ICON_ERROR)
				winIcon = IDI_ERROR;

			//Get icon from windows
			SetLastError(0);
			HICON hIcon = static_cast<HICON>(LoadImage(0, winIcon, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED | LR_DEFAULTCOLOR));
			if (hIcon == 0)
			{
				int32 error = GetLastError();
				return false;
			}

			//Duplicate icon so that we are free to use how ever we want
			m_Hicon = static_cast<HICON>(CopyImage(static_cast<HANDLE>(hIcon), IMAGE_ICON, 0, 0, 0));
			return (m_Hicon != 0);
		}

		IconImpl* Win32IconImpl::Copy()
		{
			Win32IconImpl* instance = new Win32IconImpl();
			instance->m_Hicon = static_cast<HICON>(CopyImage(static_cast<HANDLE>(m_Hicon), IMAGE_ICON, 0, 0, 0));
			return instance;
		}

		IconImpl* Win32IconImpl::Move()
		{
			Win32IconImpl* instance = new Win32IconImpl();
			instance->m_Hicon = m_Hicon;
			m_Hicon = 0;
			return instance;
		}

		HICON Win32IconImpl::GetHICON() const
		{
			return m_Hicon;
		}
	}
}

#endif