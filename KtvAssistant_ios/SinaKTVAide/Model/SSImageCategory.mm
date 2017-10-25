//
//  SSImageCategory.m
//  Sinashow
//
//  Created by Randy on 11-9-16.
//  Copyright 2011 新浪show. All rights reserved.
//

#import "SSImageCategory.h"

@implementation UIImage (SSImageCategory)

+ (UIImage *)imageWithName:(NSString *)aName
{
	return [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:aName ofType:@"png"]];
}

+ (UIImage *)imageWithName:(NSString *)aName 
					  type:(NSString *)aType
{
	return [UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:aName ofType:aType]];
}

+ (UIImage *)imageWithName:(NSString *)aName 
					  type:(NSString *)aType 
			  liftCapWidth:(NSInteger)aWidth 
			  topCapHeight:(NSInteger)aHeight
{
	UIImage *theImage = [UIImage imageWithName:aName type:aType];
	return	[theImage stretchableImageWithLeftCapWidth:aWidth topCapHeight:aHeight];
}

//定制的textField 背景 80＊82
+ (UIImage *)imageTextFieldBG
{
	return	[UIImage imageWithName:@"BG_textField" 
                             type:@"png" 
                     liftCapWidth:10 
                     topCapHeight:10];
}

//定制的Nav 上的右边 Image
+ (UIImage *)imageNavItemRightGray
{
	return	[UIImage imageWithName:@"Nav_item_right_gray"
							 type: @"png" 
					 liftCapWidth:10 
					 topCapHeight:0];
}

//定制的Nav 上的右边 Image
+ (UIImage *)imageNavItemRightRed
{
	return	[UIImage imageWithName:@"Nav_item_right_gray"
							 type: @"png" 
					 liftCapWidth:10 
					 topCapHeight:0];
}

//定制的lock Image
+ (UIImage *)imageLock
{
	return	[UIImage imageWithName:@"Icon_lock"
							 type: @"png"];
}

//定制的Image
+ (UIImage *)imageYellowMiddle
{
	return	[UIImage imageWithName:@"Btn_60_62"
							 type: @"png"
					 liftCapWidth:10
					 topCapHeight:0];
}

//定制的Image
+ (UIImage *)imageYellowMax
{
	return	[UIImage imageWithName:@"Btn_80_82"
							 type: @"png"
					 liftCapWidth:10
					 topCapHeight:0];
}

//定制的Image
+ (UIImage *)imageWitheMax
{
	return	[UIImage imageWithName:@"Btn_80_82_white"
							 type: @"png"
					 liftCapWidth:10
					 topCapHeight:0];
}

//定制的Image
+ (UIImage *)imageWitheBox40_41
{
	return	[UIImage imageWithName:@"BG_white_box40_41"
							 type: @"png"
					 liftCapWidth:10
					 topCapHeight:0];
}

//定制的Image 用于更多那些有输入框的背景
+ (UIImage *)imageWitheInput
{
	return	[UIImage imageWithName:@"BG_input_tableview"
							 type: @"png"
					 liftCapWidth:20
					 topCapHeight:20];
}

//定制的Image200 * 201
+ (UIImage *)imageWitheBox
{
	return	[UIImage imageWithName:@"BG_white_box"
							 type: @"png"
					 liftCapWidth:20
					 topCapHeight:20];
}

//定制的Image
+ (UIImage *)imageBGSendChatInput
{
	return	[UIImage imageWithName:@"BG_send_chat"
							 type: @"png"
					 liftCapWidth:15
					 topCapHeight:10];
}

//定制的Image
+ (UIImage *)imageWithRedMAX
{
	return	[UIImage imageWithName:@"BG_balance"
							 type: @"png"
					 liftCapWidth:20
					 topCapHeight:0];
}

//定制的Image
+ (UIImage *)imageWithInputMin
{
	return [UIImage imageWithName:@"BG_input60_62"
							 type:@"png"
					 liftCapWidth:10
					 topCapHeight:10];
}


//定制的Image
+ (UIImage *)imageWithBlueLineEdge
{
	return [UIImage imageWithName:@"BG_input"
							 type:@"png"
					 liftCapWidth:30
					 topCapHeight:30];
}

//定制的Image 更多的背景
+ (UIImage *)imageWithMoreBG
{
	return [UIImage imageWithName:@"BG_setting_1"
							 type:@"png"
					 liftCapWidth:30
					 topCapHeight:50];
}


//定制的Image
+ (UIImage *)imageWithWhiteBoxMax
{
	return [UIImage imageWithName:@"BG_setting_white" 
							 type:@"png"
					 liftCapWidth:20
					 topCapHeight:20];
}



@end