#include "AndroidAppState.h"

#if defined(RE_PLATFORM_ANDROID)

namespace RayEngine
{
	AndroidAppState::AndroidAppState()
		: m_Activity(nullptr),
		m_Configuration(nullptr),
		m_HasFocus(false)
	{
	}

	AndroidAppState::~AndroidAppState()
	{
		if (m_Configuration != nullptr)
			AConfiguration_delete(m_Configuration);
	}

	void AndroidAppState::SetActivity(ANativeActivity* activity)
	{
		//Set activity
		m_Activity = activity;

		//Delete old configuration
		if (m_Configuration != nullptr)
			AConfiguration_delete(m_Configuration);

		//Create a new config and get from activity's assetmanager
		m_Configuration = AConfiguration_new();
		if (m_Configuration == nullptr)
		{
			LOGE("'AConfiguration_new' returned nullptr");
			return;
		}
		
		if (m_Activity != nullptr)
		{
			AConfiguration_fromAssetManager(m_Configuration, m_Activity->assetManager);
		}
		else
		{
			LOGE("'activity' was nullptr");
		}



		return;
	}

	AConfiguration* AndroidAppState::GetConfiguration() const
	{
		return m_Configuration;
	}

	ANativeActivity* AndroidAppState::GetActivity() const
	{
		return m_Activity;
	}

	void AndroidAppState::OnFocusChanged(RayEngine::int32 hasFocus)
	{
		m_HasFocus = (hasFocus != 0);
	}

	AndroidEventManager& AndroidAppState::GetEvents()
	{
		return m_Events;
	}

	AndroidWindowManager& AndroidAppState::GetWindow()
	{
		return m_Window;
	}

	bool AndroidAppState::HasFocus() const
	{
		return m_HasFocus;
	}
}

#endif