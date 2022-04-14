#include "FlowerGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "AndroidPhotoLib.h"
#include "IOSPhotoLib.h"
#include "ImageGallery.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"

AFlowerGameHUD::AFlowerGameHUD()
{
#if PLATFORM_IOS
	PhotoLibrary = new IOSPhotoGallery();
#elif PLATFORM_ANDROID
	PhotoLibrary = new AndroidPhotoGallery();		//cant find this?
#endif

}

void AFlowerGameHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AFlowerGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UUserWidget* AFlowerGameHUD::ShowWidget(const EWidgetEnum EWidgetId, const int32 ZOrder)
{
	HideWidget();
	TSubclassOf<UUserWidget>* ClassPtr = WidgetClasses.Find(EWidgetId);
	if (ClassPtr && *ClassPtr)
	{
		CreateWidgetByClass(*ClassPtr, ZOrder);
	}

	return CurrentWidget;
}

void AFlowerGameHUD::HideWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}

UUserWidget* AFlowerGameHUD::GetCurrentWidget() const
{
	return CurrentWidget;
}

UUserWidget* AFlowerGameHUD::CreateWidgetByClass(const TSubclassOf<UUserWidget> WidgetClass, const int32 ZOrder)
{
	CurrentWidget = CreateWidget(GetWorld(), WidgetClass);
	CurrentWidget->AddToViewport(ZOrder);
	return CurrentWidget;
}

void AFlowerGameHUD::OpenGallery()
{
	if (PhotoLibrary)
	{
		PhotoLibrary->OnImageSelected.BindUObject(this, &ThisClass::OnPhotoSelected);
		PhotoLibrary->OpenGallery();
	}
}

void AFlowerGameHUD::OnPhotoSelected(TArray<uint8> PhotoBytes)
{
	int32 len = PhotoBytes.Num();
	AsyncTask(ENamedThreads::GameThread, [=]()
		{
			UTexture2D* ImportedTexture = LoadImageFromBytes(PhotoBytes);
			BP_OnPhotoSelected(ImportedTexture);
		});
}

class UTexture2D* AFlowerGameHUD::LoadImageFromBytes(const TArray<uint8>& ImageBytes)
{
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(ImageBytes.GetData(), ImageBytes.Num());
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(ImageBytes.GetData(), ImageBytes.Num()))
	{
		TArray<uint8> UncompressedBGRA;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			FName UniqueName = MakeUniqueObjectName(nullptr, UTexture2D::StaticClass());
			UTexture2D* NewTexture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(),
				ImageWrapper->GetHeight(),
				EPixelFormat::PF_B8G8R8A8, UniqueName);
			if (!NewTexture) return nullptr;
			if (UTexture2D* ImageTexture = Cast<UTexture2D>(NewTexture))
			{
				void* TextureData = ImageTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);//locking the data its multithreaded
				FMemory::Memmove(TextureData, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
				//FMemory::Memzero((void *)PixelData.GetData(), PixelData.Num() * sizeof(uint8));
				ImageTexture->PlatformData->Mips[0].BulkData.Unlock();
				ImageTexture->UpdateResource();
			}
			return NewTexture;
		}
	}
	return nullptr;
}