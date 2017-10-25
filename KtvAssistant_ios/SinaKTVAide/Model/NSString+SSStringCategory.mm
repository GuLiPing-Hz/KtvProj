//
//  NSString+SSStringCategory.m
//  Vgirl
//
//  Created by Ming Jianhua on 12-2-3.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "NSString+SSStringCategory.h"

@implementation NSString (SSStringCategory)


- (NSString *)getImageName		//从一个图片全称种获取它的命名
{
	NSRange searchRange = [self rangeOfString:@"."];
	if (searchRange.location != NSNotFound)
	{
		return [self substringToIndex:searchRange.location];
	}
	else 
		return self;
}

- (NSString *)getImageType		//从一个图片全称获取它的类型
{
	NSRange searchRange = [self rangeOfString:@"."];
	if (searchRange.location != NSNotFound)
	{
		return [self substringFromIndex:searchRange.location + 1];
	}
	else 
		return @"png";
}

-(NSString *) stringByStrippingHTML //把一个http里面的标签去掉
{
    NSRange r; 
    NSString *s = [[self copy] autorelease]; 
    while ((r = [s rangeOfString:@"<[^>]*>" options:NSRegularExpressionSearch]).location != NSNotFound)
        
        s = [s stringByReplacingCharactersInRange:r withString:@""]; 
    
    s = [s stringByReplacingOccurrencesOfString:@"\n" withString:@""];
        
    return s; 
    
}

- (NSString *)clearWhiteSpaceAndNewLine{//去除一个String里面的空格跟换行
	return [[self componentsSeparatedByCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] 
			componentsJoinedByString:@""];
}

@end
