//
//  BannerSongParameter.h
//  SinaKTVAide
//
//  Created by sp on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface BannerSongParameter : NSObject

@property (nonatomic, assign) int modelID;
@property (nonatomic, assign) int topicID;
@property (nonatomic, assign) long artistID;


+(BannerSongParameter *)initWithDefaultValue;
+(BannerSongParameter *)initWithDictionary:(NSDictionary *)data;

-(void) log;

@end
