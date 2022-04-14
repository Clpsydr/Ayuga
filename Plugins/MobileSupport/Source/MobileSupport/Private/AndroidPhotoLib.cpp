#include "AndroidPhotoLib.h"

AndroidPhotoGallery::AndroidPhotoGallery()
{
	#if PLATFORM_ANDROID
		//ImageSelectedFromGalleryProxy = OnImageSelected;  //might need to revert delegate to original name status
		INIT_JAVA_METHOD(AndroidThunkJava_OpenGallery, "()V");
	#endif
}

void AndroidPhotoGallery::OpenGallery()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_OpenGallery);

#endif
}