#pragma once

#include "..\Defines.h"
#include "..\Types.h"

namespace RayEngine
{
	namespace System
	{
		enum STANDARD_ICON
		{
			STANDARD_ICON_UNKNOWN = 0,
			STANDARD_ICON_APP = 1,
			STANDARD_ICON_ERROR = 2,
			STANDARD_ICON_WARNING = 3,
		};

		class RE_API IconImpl
		{
		public:
			IconImpl(const IconImpl& other) = delete;
			IconImpl(IconImpl&& other) = delete;
			IconImpl& operator=(const IconImpl& other) = delete;
			IconImpl& operator=(IconImpl&& other) = delete;

			IconImpl() {}
			virtual ~IconImpl() {}

			//Load a image to use as icon from file - See TextureLoader to see available formats - returns true if succeeded
			virtual bool LoadFromFile(const Tchar* const filename, const Tchar* filePath) = 0;
			//Load a standard icon from operating system
			virtual bool LoadFromOS(STANDARD_ICON icon) = 0;

			//Returns a new instance as a copy of the implementation - the old object is still valid
			virtual IconImpl* Copy() = 0;
			//Returns a new instance that contains all the old implementation data - the old object is invalidated
			virtual IconImpl* Move() = 0;
		};

		class RE_API Icon
		{
		public:
			Icon();
			Icon(const Icon& other);
			Icon(Icon&& other);
			~Icon();

			//Load a image to use as icon from file - returns true if succeded
			bool LoadFromFile(const Tchar* const filename, const Tchar* filePath);
			//Load a standard icon from operating system
			bool LoadFromOS(STANDARD_ICON icon);

			//Get implementation interface
			const IconImpl* GetImplementation() const;

			//Asignment operators
			Icon& operator=(const Icon& other);
			Icon& operator=(Icon&& other);

		private:
			IconImpl* m_Impl;
		};
	}
}