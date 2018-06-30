#include "..\..\Include\System\Clipboard.h"

#if defined(RE_PLATFORM_ANDROID)
#include "AndroidAppState.h"
#include <jni.h>
#include <unistd.h>
#include <android/looper.h>

namespace RayEngine
{
	/////////////////////////////////////////////////////////////
	int GetClipboardStringCallback(int fd, int events, void* data)
	{
		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);

		JavaVM* vm = activity->vm;
		JNIEnv* env = nullptr;
		vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
		
		if (env != nullptr)
		{
			jclass jContext = env->FindClass("android/content/Context");
			jfieldID jContext_string_CLIPBOARD_SERVICE = env->GetStaticFieldID(jContext, "CLIPBOARD_SERVICE", "Ljava/lang/String;");

			jclass jActivity = env->GetObjectClass(activity->clazz);
			jmethodID jActivity_getSystemService = env->GetMethodID(jActivity, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

			jclass jClipDataItem = env->FindClass("android/content/ClipData$Item");
			jmethodID jClipDataItem_getText = env->GetMethodID(jClipDataItem, "getText", "()Ljava/lang/CharSequence;");

			jclass jCharSequence = env->FindClass("java/lang/CharSequence");
			jmethodID jCharSequence_toString = env->GetMethodID(jCharSequence, "toString", "()Ljava/lang/String;");

			jclass jClipData = env->FindClass("android/content/ClipData");
			jmethodID jClipData_getItemAt = env->GetMethodID(jClipData, "getItemAt", "(I)Landroid/content/ClipData$Item;");

			jclass jClipboardManager = env->FindClass("android/content/ClipboardManager");
			jmethodID jClipboardManager_getPrimaryClip = env->GetMethodID(jClipboardManager, "getPrimaryClip", "()Landroid/content/ClipData;");
			jmethodID jClipboardManager_hasPrimaryClip = env->GetMethodID(jClipboardManager, "hasPrimaryClip", "()Z");


			jobject clipboardService = env->GetStaticObjectField(jContext, jContext_string_CLIPBOARD_SERVICE);
			jobject clipboard = env->CallObjectMethod(GetNativeActivity()->clazz, jActivity_getSystemService, clipboardService);
			if (env->CallBooleanMethod(clipboard, jClipboardManager_hasPrimaryClip) == JNI_TRUE)
			{
				jobject primaryClip = env->CallObjectMethod(clipboard, jClipboardManager_getPrimaryClip);
				
				jint index = 0;
				jobject item = env->CallObjectMethod(primaryClip, jClipData_getItemAt, index);
				jobject itemCharSequence = env->CallObjectMethod(item, jClipDataItem_getText);
				jstring textString = reinterpret_cast<jstring>(env->CallObjectMethod(itemCharSequence, jCharSequence_toString));

				jboolean isCopy;
				const char* cString = env->GetStringUTFChars(textString, &isCopy);
			}

			env->DeleteLocalRef(jContext);
			env->DeleteLocalRef(jActivity);
			env->DeleteLocalRef(jClipDataItem);
			env->DeleteLocalRef(jClipData);
			env->DeleteLocalRef(jClipboardManager);

			env->DeleteLocalRef(clipboardService);
			env->DeleteLocalRef(clipboard);
		}

		return 0;
	}



	/////////////////////////////////////////////////////////////
	std::string Clipboard::GetString()
	{
		ANativeActivity* activity = GetNativeActivity();
		AndroidAppState* state = reinterpret_cast<AndroidAppState*>(activity->instance);
		ALooper_acquire(state->Looper);
		ALooper* mainLooper = state->Looper;

		int32 looperPipe[2];
		if (pipe(looperPipe) == 0)
		{
			ALooper_addFd(mainLooper, looperPipe[0], 0, ALOOPER_EVENT_INPUT, GetClipboardStringCallback, nullptr);
			
			int32 msg = 1;
			write(looperPipe[1], &msg, sizeof(int32));
		}

		ALooper_release(state->Looper);
		return std::string();
	}



	/////////////////////////////////////////////////////////////
	void Clipboard::SetString(const std::string& string)
	{
		if (AndroidGetSDKVersion() < 12)
			return;

		//jclass jClipData = env->FindClass("android/content/ClipData");
		//jmethodID jClipData_newPlainText = env->GetStaticMethodID(jClipData, "newPlainText",
		//	"(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Landroid/content/ClipData;");
	}
}

#endif