#pragma once

#include "CoreMinimal.h"
#include "ImageGallery.h"

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

#define DECLARE_JAVA_METHOD(name) static jmethodID name;

#define INIT_JAVA_METHOD(name, signature) \
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true)) { \
	name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID,	#name, signature, false); \
		check(name != NULL); \
	} else { \
		check(0); \
	}
#endif

class MOBILESUPPORT_API AndroidPhotoGallery : public IImageGallery
{
#if PLATFORM_ANDROID

public:
	static struct _JNIEnv* ENV;

	DECLARE_JAVA_METHOD(AndroidThunkJava_OpenGallery);

	JNIEXPORT void Java_com_epicgames_ue4_GameActivity_onImageSelected(JNIEnv* jni, jclass clazz, jbyteArray imageBytes)
	{
		AsyncTask(ENamedThreads::GameThread, [=]()
			{
				if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
				{
					jbyte* dataPtr = Env->GetByteArrayElements(imageBytes, NULL);
					jsize len = Env->GetArrayLength(imageBytes);
					TArray<uint8> result((uint8*)dataPtr, (int32)len);
					OnImageSelected.ExecuteIfBound(result);
				}
			});
	}
#endif

	AndroidPhotoGallery();
public:
	virtual void OpenGallery() override;
};
