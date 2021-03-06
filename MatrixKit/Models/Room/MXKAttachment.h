/*
 Copyright 2015 OpenMarket Ltd

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#import <Foundation/Foundation.h>
#import <MatrixSDK/MatrixSDK.h>

extern NSString *const kMXKAttachmentErrorDomain;

/**
 List attachment types
 */
typedef enum : NSUInteger {
    MXKAttachmentTypeUndefined,
    MXKAttachmentTypeImage,
    MXKAttachmentTypeAudio,
    MXKAttachmentTypeVideo,
    MXKAttachmentTypeLocation,
    MXKAttachmentTypeFile
    
} MXKAttachmentType;

/**
 `MXKAttachment` represents a room attachment.
 */
@interface MXKAttachment : NSObject

/**
 The related matrix event.
 */
@property (nonatomic, readonly) MXEvent *event;

/**
 The attachment type.
 */
@property (nonatomic, readonly) MXKAttachmentType type;

/**
 The attachment information retrieved from event content.
 In case of image, the thumbnail information are not defined anymore, 'thumbnailURL' and 'thumbnailInfo' are then nil.
 */
@property (nonatomic, readonly) NSString *contentURL;
@property (nonatomic, readonly) NSDictionary *contentInfo;

// The URL of a 'standard size' thumbnail
@property (nonatomic, readonly) NSString *thumbnailURL;

@property (nonatomic) NSDictionary *thumbnailInfo;

@property (nonatomic, readonly) NSString *thumbnailMimeType;

/**
 The original file name retrieved from the event body (if any).
 */
@property (nonatomic, readonly) NSString *originalFileName;

/**
 The actual attachment url
 */
@property (nonatomic, readonly) NSString *actualURL;

/**
 The thumbnail orientation (relevant in case of image).
 */
@property (nonatomic, readonly) UIImageOrientation thumbnailOrientation;

/**
 The cache file path of the attachment.
 */
@property (nonatomic, readonly) NSString *cacheFilePath;

/**
 Local url used to store a preview of the attachment.
 */
@property (nonatomic) NSString *previewURL;

/**
 True if the attachment is encrypted
 The encryption status of the thumbnail is not covered by this
 property: it is possible for the thumbnail to be encrypted
 whether this peoperty is true or false.
 */
@property (nonatomic, readonly) BOOL isEncrypted;

/**
 */
- (instancetype)initWithEvent:(MXEvent*)mxEvent andMatrixSession:(MXSession*)mxSession;
- (void)destroy;

/**
 Gets the thumbnail for this attachment if it is in the memory or disk cache,
 otherwise return nil
 */
- (UIImage *)getCachedThumbnail;

/**
 For image attachments, gets a UIImage for the full-res image
 */
- (void)getImage:(void (^)(UIImage *))onSuccess failure:(void (^)(NSError *error))onFailure;

/**
 Decrypt the attachment data into memory and provide it as an NSData
 */
- (void)getAttachmentData:(void (^)(NSData *))onSuccess failure:(void (^)(NSError *error))onFailure;

/**
 Decrypts the attachment to a newly created temporary file.
 If the isEncrypted property is YES, this method (or getImage) should be used to
 obtain the full decrypted attachment. The behaviour of this method is undefined
 if isEncrypted is NO.
 It is the caller's responsibility to delete the temporary file once it is no longer
 needed.
 */
- (void)decryptToTempFile:(void (^)(NSString *))onSuccess failure:(void (^)(NSError *error))onFailure;

/**
 Gets the thumbnails for this attachment, downloading it or loading it from disk cache
 if necessary
 */
- (void)getThumbnail:(void (^)(UIImage *))onSuccess failure:(void (^)(NSError *error))onFailure;

/**
 * Gets a thumbnail URL for a given size of thumbnail, if possible,
 * or if only one size of thumbnail is available, return the URL for that.
 * Don't forget to multiply by the pixel density of your display
 */
- (NSString *)getThumbnailUrlForSize:(CGSize)size;

/**
 Download the attachment data if it is not already cached.
 
 @param onAttachmentReady block called when attachment is available at 'cacheFilePath'.
 @param onFailure the block called on failure.
 */
- (void)prepare:(void (^)())onAttachmentReady failure:(void (^)(NSError *error))onFailure;

/**
 Save the attachment in user's photo library. This operation is available only for images and video.
 
 @param onSuccess
 @param onFailure
 */
- (void)save:(void (^)())onSuccess failure:(void (^)(NSError *error))onFailure;

/**
 Copy the attachment data in general pasteboard.
 
 @param onSuccess
 @param onFailure
 */
- (void)copy:(void (^)())onSuccess failure:(void (^)(NSError *error))onFailure;

/**
 Prepare the attachment data to share it. The original name of the attachment (if any) is used
 to name the prepared file.
 
 The developer must call 'onShareEnd' when share operation is ended in order to release potential
 resources allocated here.
 
 @param onReadyToShare the block called when attachment is ready to share at the provided file URL.
 @param onFailure the block called on failure.
 */
- (void)prepareShare:(void (^)(NSURL *fileURL))onReadyToShare failure:(void (^)(NSError *error))onFailure;
- (void)onShareEnded;

@end
