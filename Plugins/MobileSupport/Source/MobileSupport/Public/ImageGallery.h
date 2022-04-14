#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ImageGallery.generated.h"

DECLARE_DELEGATE_OneParam(FOnImageSelectedFromGallery, TArray<uint8>);

UINTERFACE(MinimalAPI)
class UImageGallery : public UInterface
{
	GENERATED_BODY()
};

class MOBILESUPPORT_API IImageGallery
{
	GENERATED_BODY()

public:
	virtual void OpenGallery() = 0;

	//delegate doesnt like being static...
	FOnImageSelectedFromGallery OnImageSelected;
};
