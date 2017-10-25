//
//  CommenTools.m
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-11-4.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "CommenTools.h"

@implementation CommenTools


+ (UIImage *)resizeImage:(UIImage *)image withSize:(CGSize)size
{
    UIGraphicsBeginImageContextWithOptions(size, NO, 0.0);
    CGRect imageRect = CGRectMake(0.0, 0.0, size.width, size.height);
    [image drawInRect:imageRect];
    image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}

+ (NSString*)encodeAsURIComponent:(NSString *)str
{
    NSString *res = [str stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    //    res = [res stringByReplacingOccurrencesOfString:@"&" withString:@"%26"];
    return res;
}

+ (NSString *)decodeFromURLComponent:(NSString *)str
{
    return [str stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
}



+ (NSString*) serializeDeviceToken:(NSData*) deviceToken
{
    NSMutableString *str = [NSMutableString stringWithCapacity:64];
    int length = [deviceToken length];
    char *bytes = malloc(sizeof(char) * length);
    
    [deviceToken getBytes:bytes length:length];
    
    for (int i = 0; i < length; i++)
    {
        [str appendFormat:@"%02.2hhX", bytes[i]];
        //        if ((i + 1) % 4 == 0)
        //            [str appendFormat:@" "];
    }
    free(bytes);
    
    return str;
}

+ (NSString *)newUUID
{
    CFUUIDRef theUUID = CFUUIDCreate(NULL);
    CFStringRef string = CFUUIDCreateString(NULL, theUUID);
    CFRelease(theUUID);
    return (__bridge NSString *)string;
}

+ (NSInteger)getDecodedStringLength:(NSString *)str
{
    NSStringEncoding enc = CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000);
    return [str lengthOfBytesUsingEncoding:enc];
}

/**
 * 根据错误代码(ErrorCode)显示错误对话框
 * @param _errorCode  错误描述代码（例：5001）
 *
 */

+ (void)showAlertViewWithTitle:(NSString *)_errorCode
{
    NSString *message = [NSString stringWithFormat:@""];
    if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForNoRoom]) {
        message = @"房间已关闭,请退出房间";//@"房间不存在，请稍后重试！";//50001
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForOrderOverage]) {
        message = @"已经点了很多喽，先唱几首吧";//@"点播歌曲超过99首，请稍后重试！";//50002
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForSocketError]) {
        message = @"Socket错误，请稍后重试！";//50003
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForSongError]) {
        message = @"歌曲不存在，请稍后重试！";//50004
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForDataError]) {
        message = @"数据接收错误，请稍后重试！";//50005
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomClosed]) {
        message = @"房间已关闭,请退出房间";//50007
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForRoomSafeguard]) {
        message = @"房间维护,请退出房间";//50008
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForParamPostError]) {
        message = @"参数传递错误，请稍后重试！";//10001
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForSecretError]) {
        message = @"Secret错误，请稍后重试！";//10002
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForSongPlaying]) {
        message = @"歌曲正在演唱，无法删除/置顶";//50009
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForLogin]) {
        message = @"很抱歉，该包厢您无法进入。请重试，或联系服务人员";//90000
        
    }else if ([_errorCode isEqualToString:kPKtvAssistantErrorCodeForSongExisted]){
        message = @"歌曲已点，无法重复点歌";//50006
        
    }else{
        message = _errorCode;
    }
    
    UIAlertView *alert =[[UIAlertView alloc] initWithTitle:@""
                                                   message:message
                                                  delegate:self
                                         cancelButtonTitle:@"确定"
                                         otherButtonTitles:nil];
    [alert show];
}

/**
 * 根据错误Msg显示错误对话框
 * @param _errorMessage  错误描述信息（例：服务器繁忙，请稍后）
 *
 */
+ (void)showAlertViewWithErrorMessage:(NSString *)_errorMessage
{
    if (_errorMessage == nil) {
        return;
    }
    
    UIAlertView *alert =[[UIAlertView alloc] initWithTitle:@""
                                                   message:_errorMessage
                                                  delegate:self
                                         cancelButtonTitle:@"确定"
                                         otherButtonTitles:nil];
    [alert show];
}

/**
 * 根据错误Msg显示错误对话框
 * @param _errorMessage  错误描述信息（例：服务器繁忙，请稍后）
 *
 */
+ (void)showAlertViewWithMessage:(NSString *)_message
{
    
    UIAlertView *alert =[[UIAlertView alloc] initWithTitle:@""
                                                   message:_message
                                                  delegate:self
                                         cancelButtonTitle:@"确定"
                                         otherButtonTitles:nil];
    [alert show];
}



static NSDate *lastDate = nil;

+ (void)showNetworkDisconnectedAlertView
{
    if (lastDate == nil)
        lastDate = [[NSDate alloc] initWithTimeIntervalSince1970:0];
    
    NSTimeInterval interval = [[NSDate date] timeIntervalSinceDate:lastDate];
    if (interval > 1)
    {
        [self showAlertViewWithTitle:@"无法获取信息，请检查您的网络连接"];
        lastDate = [NSDate date];
    }
    
}


+ (NSString *)getSystemVersion
{
    return [[UIDevice currentDevice] systemVersion];
}
+ (NSString *)getBuildVersion
{
    return [[NSBundle mainBundle] objectForInfoDictionaryKey: (NSString *)kCFBundleVersionKey];
}

+(NSString *)getHomeDirectory
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    return documentsDirectory;
}

+ (void)addSongToPreVodSongListWithSongInfo:(SongInfo *)info{
    NSString *localSettingPath = [[CommenTools getHomeDirectory] stringByAppendingPathComponent:PRE_VOD_SONGLIST_PATH];
    NSMutableArray *localInfo;
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:localSettingPath])
    {
        localInfo = [NSMutableArray arrayWithContentsOfFile:localSettingPath];
    }
    else
    {
        [[NSFileManager defaultManager] createFileAtPath:localSettingPath contents:nil attributes:0];
        localInfo = [NSMutableArray array];
    }
   // NSDictionary *dict = @{@"haozi":@"xiao"};
    NSMutableDictionary *dictionary = [[NSMutableDictionary alloc] init];
    [dictionary setObject:[NSString stringWithFormat:@"%ld",info.songId] forKey:@"songid"];
    [dictionary setObject:info.songName forKey:@"songname"];
    [dictionary setObject:info.songArtist forKey:@"songartist"];
    [dictionary setObject:[NSString stringWithFormat:@"%d",info.songIsScore] forKey:@"songisscore"];
    [dictionary setObject:[NSString stringWithFormat:@"%d",info.isOrdered] forKey:@"isordered"];
    PLog(@"dictionary: %@",dictionary);
    [localInfo addObject:dictionary];
    
    //NSData *data = [NSKeyedArchiver archivedDataWithRootObject:localInfo];//序列化
    
    [localInfo writeToFile:localSettingPath atomically:YES];


}
+ (void)deleteSongInPreVodSongListWithSongInfo:(SongInfo *)info{
    NSString *localSettingPath = [[CommenTools getHomeDirectory] stringByAppendingPathComponent:PRE_VOD_SONGLIST_PATH];

    NSMutableArray *localInfo;
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:localSettingPath])
    {
        localInfo = [NSMutableArray arrayWithContentsOfFile:localSettingPath];
    }
    else
    {
        [[NSFileManager defaultManager] createFileAtPath:localSettingPath contents:nil attributes:nil];
        localInfo = [[NSMutableArray alloc] init];
    }
    for (int i = 0; i < [localInfo count]; i++) {
        NSMutableDictionary *dic = [localInfo objectAtIndex:i];
        long dicSongId = [[dic objectForKey:@"songid"] longValue];
        if (dicSongId == info.songId) {
            [localInfo removeObjectAtIndex:i];
            break;
        }
        
    }
    [localInfo writeToFile:localSettingPath atomically:YES];

}
+ (NSMutableArray *)getPreVodSongListData{
    NSString *localSettingPath = [[CommenTools getHomeDirectory] stringByAppendingPathComponent:PRE_VOD_SONGLIST_PATH];
    
    NSMutableArray *localInfo;
    
    if ([[NSFileManager defaultManager] fileExistsAtPath:localSettingPath])
    {
        localInfo = [NSMutableArray arrayWithContentsOfFile:localSettingPath];
        return localInfo;
        
    }
    return nil;
}


/**
 *  图片尺寸地址转换,函数内部会自动检测屏幕分辨率，若是视网膜屏，取出的地址是_scale自动+1后的分辨率地址，
 *  但当_scale = 3 时，返回的还是640*640分辨率的地址
 *  @param _originalURL 原始图地址
 *  @param _scale  转变的尺寸类型；(0:获取的是100*100的图片地址)、(1:获取的是200*200的图片地址)、(2:获取的是320*320的图片地址)、(3:获取的是640*640的图片地址)
 *
 *  @return return value description
 */
+ (NSString *) GetURLWithResolutionScaleTransfered:(NSString *)_originalURL scale:(NSInteger)_scale
{
    if (!_originalURL || [_originalURL isEqualToString:@""]){
        return nil;
    }
    NSString *_url = _originalURL;
    NSArray *target_size = [[NSArray alloc] initWithObjects:@"_100_100.jpg", @"_200_200.jpg", @"_320_320.jpg", @"_640_640.jpg", nil];
    for (int i = 0; i < target_size.count; i++){
        _url = [_url stringByReplacingOccurrencesOfString:[target_size objectAtIndex:i] withString:@".jpg"];
    }
    
    if (IS_RETINA || iPhone5){
        if(_scale < 3 ){
            _scale++;
        }
    }
    
    _url = [_url stringByReplacingOccurrencesOfString:@".jpg" withString:[target_size objectAtIndex:_scale]];
    //NSLog(@"url:%@",_url);
    return _url;
    //return [NSURL URLWithString:_url];
}



@end
