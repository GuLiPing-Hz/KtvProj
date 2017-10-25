//
//  UIButton+AFNetworking.m
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import "UIButton+AFNetworking.h"
#import "AFImageCache.h"
#import <objc/runtime.h>

@implementation UIButton (AFNetworking)

static char kAFImageRequestOperationObjectKey;

+ (NSOperationQueue *)af_sharedImageRequestOperationQueue {
    static NSOperationQueue *_af_imageRequestOperationQueue = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _af_imageRequestOperationQueue = [[NSOperationQueue alloc] init];
        [_af_imageRequestOperationQueue setMaxConcurrentOperationCount:NSOperationQueueDefaultMaxConcurrentOperationCount];
    });
    
    return _af_imageRequestOperationQueue;
}

//get af_imageRequestOperation
- (AFHTTPRequestOperation *)af_imageRequestOperation {
    return (AFHTTPRequestOperation *)objc_getAssociatedObject(self, &kAFImageRequestOperationObjectKey);
}

//set af_imageRequestOperation
- (void)setAf_imageRequestOperation:(AFImageRequestOperation *)imageRequestOperation
{
    objc_setAssociatedObject(self, &kAFImageRequestOperationObjectKey, imageRequestOperation, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (void)setImageWithURL:(NSURL *)url {
    [self setImageWithURL:url placeholderImage:nil];
}

- (void)setImageWithURL:(NSURL *)url
       placeholderImage:(UIImage *)placeholderImage
{
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
    [request addValue:@"image/*" forHTTPHeaderField:@"Accept"];
    
    [self setImageWithURLRequest:request placeholderImage:placeholderImage success:nil failure:nil];
}

#pragma mark - function
- (void)setImageWithURLRequest:(NSURLRequest *)urlRequest
              placeholderImage:(UIImage *)placeholderImage
                       success:(void (^)(NSURLRequest *request, NSHTTPURLResponse *response, UIImage *image))success
                       failure:(void (^)(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error))failure
{
    [self cancelImageRequestOperation];
    
    UIImage *cachedImage = [[AFImageCache af_sharedImageCache] cachedImageForRequest:urlRequest];
    if (cachedImage) {
        if (success) {
            success(nil, nil, cachedImage);
        } else {
            [self setImage:cachedImage forState:UIControlStateNormal];
        }
        
        self.af_imageRequestOperation = nil;
    } else {
        [self setImage:placeholderImage forState:UIControlStateNormal];
        
        AFImageRequestOperation *requestOperation = [[AFImageRequestOperation alloc] initWithRequest:urlRequest];
        [requestOperation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
            if ([urlRequest isEqual:[self.af_imageRequestOperation request]]) {
                if (success) {
                    success(operation.request, operation.response, responseObject);
                } else if (responseObject) {
                    [self setImage:responseObject forState:UIControlStateNormal];
                    //self.image = responseObject;
                }
                
                if (self.af_imageRequestOperation == operation) {
                    self.af_imageRequestOperation = nil;
                }
            }
            
            [[AFImageCache af_sharedImageCache] cacheImage:responseObject forRequest:urlRequest];
        } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
            if ([urlRequest isEqual:[self.af_imageRequestOperation request]]) {
                if (failure) {
                    failure(operation.request, operation.response, error);
                }
                
                if (self.af_imageRequestOperation == operation) {
                    self.af_imageRequestOperation = nil;
                }
            }
        }];
        
        self.af_imageRequestOperation = requestOperation;
        
        [[[self class] af_sharedImageRequestOperationQueue] addOperation:self.af_imageRequestOperation];
    }
}

- (void)cancelImageRequestOperation {
    [self.af_imageRequestOperation cancel];
    self.af_imageRequestOperation = nil;
}

@end
