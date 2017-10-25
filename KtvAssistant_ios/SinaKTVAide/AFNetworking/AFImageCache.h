//
//  AFImageCache.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/21.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AFImageCache : NSCache

+ (AFImageCache *)af_sharedImageCache;

- (UIImage *)cachedImageForRequest:(NSURLRequest *)request;
- (void)cacheImage:(UIImage *)image
        forRequest:(NSURLRequest *)request;

@end
