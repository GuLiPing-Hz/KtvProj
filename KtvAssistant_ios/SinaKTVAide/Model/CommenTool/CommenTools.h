//
//  CommenTools.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SongInfo.h"

#define IS_RETINA ([UIScreen instancesRespondToSelector:@selector(currentMode)] ? CGSizeEqualToSize(CGSizeMake(640, 960), [[UIScreen mainScreen] currentMode].size) : NO)

@interface CommenTools : NSObject


+ (UIImage *) resizeImage:(UIImage *)image withSize:(CGSize)size;

+ (NSString *) encodeAsURIComponent:(NSString *)str;
+ (NSString *) decodeFromURLComponent:(NSString *)str;
+ (NSString*) serializeDeviceToken:(NSData*) deviceToken;
+ (NSString *)newUUID;
+ (NSInteger)getDecodedStringLength:(NSString *)str;

/**
 * 根据错误代码(ErrorCode)显示错误对话框
 * @param _errorCode  错误描述代码（例：5001）
 *
 */
+ (void)showAlertViewWithTitle:(NSString *)_errorCode;
+ (void)showNetworkDisconnectedAlertView;

/**
 * 根据错误Msg显示错误对话框
 * @param _errorMessage  错误描述信息（例：服务器繁忙，请稍后）
 *
 */
+ (void)showAlertViewWithErrorMessage:(NSString *)_errorMessage;
+ (void)showAlertViewWithMessage:(NSString *)_message;

+ (NSString *)getSystemVersion;
+ (NSString *)getBuildVersion;
+ (NSString *)getHomeDirectory;

//将歌曲放倒本地预订歌曲列表内
+ (void)addSongToPreVodSongListWithSongInfo:(SongInfo *)info;
//删除本地预点歌曲
+ (void)deleteSongInPreVodSongListWithSongInfo:(SongInfo *)info;
//获取本地预点歌曲信息
+ (NSMutableArray *)getPreVodSongListData;


/**
 *  图片尺寸地址转换,函数内部会自动检测屏幕分辨率，若是视网膜屏，取出的地址是_scale自动+1后的分辨率地址，
 *  但当_scale = 3 时，返回的还是640*640分辨率的地址
 *  @param _originalURL 原始图地址
 *  @param _scale  转变的尺寸类型；(0:获取的是100*100的图片地址)、(1:获取的是200*200的图片地址)、(2:获取的是320*320的图片地址)、(3:获取的是640*640的图片地址)
 *
 *  @return return value description
 */
+ (NSString *) GetURLWithResolutionScaleTransfered:(NSString *)_originalURL scale:(NSInteger)_scale;


@end
