#pragma once
#include "CoreMinimal.h"
#include "ImageGallery.h"

#if PLATFORM_IOS
#import <UIKit/UIKit.h>
// UIImagePickerControllerDelegate to respond to user interactions
// UINavigationControllerDelegate because we want to present the photo library
// modally
@interface MyViewController : UIViewController <UIImagePickerControllerDelegate,
	UINavigationControllerDelegate>

@property(nonatomic, retain)
	UIImage* chosenImage;

+(void)runSelectPhoto;
@end

#endif

class MOBILESUPPORT_API IOSPhotoGallery : public IImageGallery 
{
public:
	virtual void OpenGallery() override;
};
