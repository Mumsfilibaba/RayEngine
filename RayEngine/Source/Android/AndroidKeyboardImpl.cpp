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

#include "RayEngine.h"

#if defined(RE_PLATFORM_ANDROID)
#include "..\..\Include\System\\Keyboard.h"
#include "AndroidAppState.h"

namespace RayEngine
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::IsPressed(KEY keyCode)
	{
		//Not implemented on android
		return false;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::ShowVirtualKeyboardVisible()
	{
		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);

		if (!state->HasFocus)
			return false;

		JavaVM* vm = activity->vm;
		JNIEnv* env = nullptr;
		
		vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
		if (env == nullptr)
		{
			JavaVMAttachArgs attachArgs = {};
			attachArgs.group = 0;
			attachArgs.name = "C++ Thread";
			attachArgs.version = JNI_VERSION_1_6;

			if (vm->AttachCurrentThread(&env, reinterpret_cast<void*>(&attachArgs)) == JNI_ERR)
				return false;
		}

		jclass jWindow = env->FindClass("android/view/Window");
		jmethodID JWindow_GetDecorView = env->GetMethodID(jWindow, "getDecorView", "()Landroid/view/View;");
		env->DeleteLocalRef(jWindow);

		jclass jActivity = env->GetObjectClass(activity->clazz);
		jmethodID jActivity_getSystemService = env->GetMethodID(jActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
		jmethodID jActivity_getWindow = env->GetMethodID(jActivity, "getWindow", "()Landroid/view/Window;");
		env->DeleteLocalRef(jActivity);

		jclass jInputMethodManager = env->FindClass("android/view/inputmethod/InputMethodManager");
		jmethodID jInputMethodManager_showSoftInput = env->GetMethodID(jInputMethodManager, "showSoftInput", "(Landroid/view/View;I)Z");
		env->DeleteLocalRef(jInputMethodManager);

		jclass jContext = env->FindClass("android/content/Context");
		jfieldID jContext_string_INPUT_METHOD_SERVICE = env->GetStaticFieldID(jContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
		jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(jContext, jContext_string_INPUT_METHOD_SERVICE);
		env->DeleteLocalRef(jContext);


		jobject inputMethodManager = env->CallObjectMethod(activity->clazz, jActivity_getSystemService, INPUT_METHOD_SERVICE);
		jobject window = env->CallObjectMethod(activity->clazz, jActivity_getWindow);
		jobject decorView = env->CallObjectMethod(window, JWindow_GetDecorView);

		jint flags = 0;
		if (env->CallBooleanMethod(inputMethodManager, jInputMethodManager_showSoftInput, decorView, flags) == JNI_FALSE)
			return false;

		env->DeleteLocalRef(inputMethodManager);
		env->DeleteLocalRef(window);
		env->DeleteLocalRef(decorView);

		vm->DetachCurrentThread();
		return true;
	}



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Keyboard::HideVirtualKeyboardVisible()
	{
		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);

		if (!state->HasFocus)
			return false;

		JavaVM* vm = activity->vm;
		JNIEnv* env = nullptr;

		vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
		if (env == nullptr)
		{
			JavaVMAttachArgs attachArgs = {};
			attachArgs.group = 0;
			attachArgs.name = "C++ Thread";
			attachArgs.version = JNI_VERSION_1_6;

			if (vm->AttachCurrentThread(&env, reinterpret_cast<void*>(&attachArgs)) == JNI_ERR)
				return false;
		}

		jclass jView = env->FindClass("android/view/View");
		jmethodID jView_getWindowToken = env->GetMethodID(jView, "getWindowToken", "()Landroid/os/IBinder;");
		env->DeleteLocalRef(jView);

		jclass jWindow = env->FindClass("android/view/Window");
		jmethodID JWindow_GetDecorView = env->GetMethodID(jWindow, "getDecorView", "()Landroid/view/View;");
		env->DeleteLocalRef(jWindow);

		jclass jActivity = env->GetObjectClass(activity->clazz);
		jmethodID jActivity_getSystemService = env->GetMethodID(jActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
		jmethodID jActivity_getWindow = env->GetMethodID(jActivity, "getWindow", "()Landroid/view/Window;");
		env->DeleteLocalRef(jActivity);

		jclass jInputMethodManager = env->FindClass("android/view/inputmethod/InputMethodManager");
		jmethodID jInputMethodManager_hideSoftInputFromWindow = env->GetMethodID(jInputMethodManager, "hideSoftInputFromWindow", "(Landroid/os/IBinder;I)Z");
		env->DeleteLocalRef(jInputMethodManager);

		jclass jContext = env->FindClass("android/content/Context");
		jfieldID jContext_string_INPUT_METHOD_SERVICE = env->GetStaticFieldID(jContext, "INPUT_METHOD_SERVICE", "Ljava/lang/String;");
		jobject INPUT_METHOD_SERVICE = env->GetStaticObjectField(jContext, jContext_string_INPUT_METHOD_SERVICE);
		env->DeleteLocalRef(jContext);


		jobject inputMethodManager = env->CallObjectMethod(activity->clazz, jActivity_getSystemService, INPUT_METHOD_SERVICE);
		jobject window = env->CallObjectMethod(activity->clazz, jActivity_getWindow);
		jobject decorView = env->CallObjectMethod(window, JWindow_GetDecorView);
		jobject binder = env->CallObjectMethod(decorView, jView_getWindowToken);

		jint flags = 0;
		if (env->CallBooleanMethod(inputMethodManager, jInputMethodManager_hideSoftInputFromWindow, binder, flags) == JNI_FALSE)
			return false;

		env->DeleteLocalRef(binder);
		env->DeleteLocalRef(inputMethodManager);
		env->DeleteLocalRef(window);
		env->DeleteLocalRef(decorView);

		vm->DetachCurrentThread();
		return true;
	}
}

#endif