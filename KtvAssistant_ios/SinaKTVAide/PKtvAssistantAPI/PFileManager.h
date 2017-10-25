//
//  PFileManager.h
//  miglab_mobile
//
//  Created by pig on 13-7-1.
//  Copyright (c) 2013年 pig. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface PFileManager : NSObject

-(NSString *)getCacheHomeDirectory;
-(NSString *)createPath:(NSString *)tpath;
-(long long)getLocalFileSize:(NSString *)filepath;
//计算文件夹下文件的总大小
-(long long)getFileSizeForDir:(NSString *)dir;

@end
