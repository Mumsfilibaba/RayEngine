/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "..\..\Include\System\Clipboard.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidAppState.h"
#include <jni.h>
#include <unistd.h>
#include <android/looper.h>

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetClipboardStringCallback(int fd, int events, void* data)
	{
		if (AndroidGetSDKVersion() < 12)
			return 0;

		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);

		JavaVM* vm = activity->vm;
		JNIEnv* env = nullptr;
		vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
		
		if (env != nullptr)
		{
			jclass jContext = env->FindClass("android/content/Context");
			jfieldID jContext_string_CLIPBOARD_SERVICE = env->GetStaticFieldID(jContext, "CLIPBOARD_SERVICE", "Ljava/lang/String;");
			jobject CLIPBOARD_SERVICE = env->GetStaticObjectField(jContext, jContext_string_CLIPBOARD_SERVICE);
			env->DeleteLocalRef(jContext);

			jclass jActivity = env->GetObjectClass(activity->clazz);
			jmethodID jActivity_getSystemService = env->GetMethodID(jActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
			env->DeleteLocalRef(jActivity);

			jclass jClipDataItem = env->FindClass("android/content/ClipData$Item");
			jmethodID jClipDataItem_getText = env->GetMethodID(jClipDataItem, "getText", "()Ljava/lang/CharSequence;");
			env->DeleteLocalRef(jClipDataItem);

			jclass jCharSequence = env->FindClass("java/lang/CharSequence");
			jmethodID jCharSequence_toString = env->GetMethodID(jCharSequence, "toString", "()Ljava/lang/String;");
			env->DeleteLocalRef(jCharSequence);

			jclass jClipData = env->FindClass("android/content/ClipData");
			jmethodID jClipData_getItemAt = env->GetMethodID(jClipData, "getItemAt", "(I)Landroid/content/ClipData$Item;");
			env->DeleteLocalRef(jClipData);

			jclass jClipboardManager = env->FindClass("android/content/ClipboardManager");
			jmethodID jClipboardManager_getPrimaryClip = env->GetMethodID(jClipboardManager, "getPrimaryClip", "()Landroid/content/ClipData;");
			jmethodID jClipboardManager_hasPrimaryClip = env->GetMethodID(jClipboardManager, "hasPrimaryClip", "()Z");
			env->DeleteLocalRef(jClipboardManager);


			jobject clipboard = env->CallObjectMethod(GetNativeActivity()->clazz, jActivity_getSystemService, CLIPBOARD_SERVICE);
			if (env->CallBooleanMethod(clipboard, jClipboardManager_hasPrimaryClip) == JNI_TRUE)
			{
				jobject primaryClip = env->CallObjectMethod(clipboard, jClipboardManager_getPrimaryClip);
				
				jint index = 0;
				jobject item = env->CallObjectMethod(primaryClip, jClipData_getItemAt, index);
				jobject itemCharSequence = env->CallObjectMethod(item, jClipDataItem_getText);
				jstring textString = reinterpret_cast<jstring>(env->CallObjectMethod(itemCharSequence, jCharSequence_toString));

				jboolean isCopy;
				const char* cString = env->GetStringUTFChars(textString, &isCopy);
				int32 length = env->GetStringLength(textString);

				*reinterpret_cast<char**>(data) = reinterpret_cast<char*>(malloc(length));
				strcpy(*reinterpret_cast<char**>(data), cString);

				env->ReleaseStringUTFChars(textString, cString);

				env->DeleteLocalRef(item);
				env->DeleteLocalRef(itemCharSequence);
				env->DeleteLocalRef(textString);
			}

			env->DeleteLocalRef(CLIPBOARD_SERVICE);
			env->DeleteLocalRef(clipboard);
		}

		return 0;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::string Clipboard::GetString()
	{
		std::string result;

		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
		ALooper_acquire(state->Looper);
		ALooper* mainLooper = state->Looper;

		int32 looperPipe[2];
		if (pipe(looperPipe) == 0)
		{
			char* nativeString = nullptr;
			ALooper_addFd(mainLooper, looperPipe[0], 0, ALOOPER_EVENT_INPUT, GetClipboardStringCallback, &nativeString);
			
			int32 msg = 1;
			write(looperPipe[1], &msg, sizeof(int32));

			while (nativeString == nullptr);

			result = nativeString;
			free(reinterpret_cast<void*>(nativeString));
		}

		ALooper_release(state->Looper);
		return result;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int SetClipboardStringCallback(int fd, int events, void* data)
	{
		if (AndroidGetSDKVersion() < 12)
			return 0;

		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);

		JavaVM* vm = activity->vm;
		JNIEnv* env = nullptr;
		vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

		if (env != nullptr)
		{
			jclass jContext = env->FindClass("android/content/Context");
			jfieldID jContext_string_CLIPBOARD_SERVICE = env->GetStaticFieldID(jContext, "CLIPBOARD_SERVICE", "Ljava/lang/String;");
			jobject CLIPBOARD_SERVICE = env->GetStaticObjectField(jContext, jContext_string_CLIPBOARD_SERVICE);
			env->DeleteLocalRef(jContext);

			jclass jActivity = env->GetObjectClass(activity->clazz);
			jmethodID jActivity_getSystemService = env->GetMethodID(jActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
			env->DeleteLocalRef(jActivity);

			jclass jClipData = env->FindClass("android/content/ClipData");
			jmethodID jClipData_newPlainText = env->GetStaticMethodID(jClipData, "newPlainText", 
				"(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;");

			jclass jClipboardManager = env->FindClass("android/content/ClipboardManager");
			jmethodID jClipboardManager_setPrimaryClip = env->GetMethodID(jClipboardManager, "setPrimaryClip", "(Landroid/content/ClipData;)V");
			env->DeleteLocalRef(jClipboardManager);


			jobject clipboard = env->CallObjectMethod(GetNativeActivity()->clazz, jActivity_getSystemService, CLIPBOARD_SERVICE);
			jstring label = env->NewStringUTF("NativeLabel");
			jstring text = env->NewStringUTF(*reinterpret_cast<const char**>(data));
			jobject clipdata = env->CallStaticObjectMethod(jClipData, jClipData_newPlainText, label, text);
			env->CallVoidMethod(clipboard, jClipboardManager_setPrimaryClip, clipdata);

			*reinterpret_cast<const char**>(data) = nullptr;

			env->DeleteLocalRef(jClipData);
			env->DeleteLocalRef(label);
			env->DeleteLocalRef(text);
			env->DeleteLocalRef(clipdata);
			env->DeleteLocalRef(CLIPBOARD_SERVICE);
			env->DeleteLocalRef(clipboard);
		}

		return 0;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Clipboard::SetString(const std::string& string)
	{
		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
		ALooper_acquire(state->Looper);
		ALooper* mainLooper = state->Looper;

		int32 looperPipe[2];
		if (pipe(looperPipe) == 0)
		{
			char* str = const_cast<char*>(string.c_str());
			ALooper_addFd(mainLooper, looperPipe[0], 0, ALOOPER_EVENT_INPUT, SetClipboardStringCallback, &str);

			int32 msg = 1;
			write(looperPipe[1], &msg, sizeof(int32));

			while (str != nullptr);
		}
	}
}

#endif