//
//  BannerInfo.h
//  SinaKTVAide
//
//  Created by sp on 13-12-10.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BannerSongParameter.h"

@interface BannerInfo : NSObject

@property (nonatomic, assign) long bID;                                     //
@property (nonatomic, assign) int type;                                     //
@property (nonatomic, strong) NSString *imagePhoto;                         //
@property (nonatomic, strong) NSString *title;                              //
@property (nonatomic, strong) BannerSongParameter *songParameter;           //

+(BannerSongParameter *)initWithDefaultValue;
+(BannerSongParameter *)initWithDictionary:(NSDictionary *)data;

-(void) log;

@end
