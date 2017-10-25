//
//  TopicInfo.h
//  SinaKTVAide
//
//  Created by apple on 13-10-28.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TopicInfo : NSObject

@property (nonatomic, assign) long topicId;
@property (nonatomic, strong) NSString *topicTitle;
@property (nonatomic, strong) NSString *topicUrl;
@property (nonatomic, strong) NSString *pinyin;   //只有返回的是歌手列表时该字段才有值
@property (nonatomic, assign) int artistHotVal;     //只有返回的是歌手列表时该字段才有值
@property (nonatomic,strong) NSString* topicImgIcon;// 主题图标 url

+(TopicInfo *)initWithDictionary:(NSDictionary *)data;

@end
