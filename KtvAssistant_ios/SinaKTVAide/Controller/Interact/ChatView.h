//
//  PublicChatView.h
//  chat_rebot
//
//  Created by test123 on 14-3-26.
//  Copyright (c) 2014年 Lipeng. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Message.h"

#define LRSPACE 11
#define kSpace 5

#define kW 35
#define kH 13

#define GetW(rect)      (rect.size.width)
#define GetH(rect)      (rect.size.height)

#define GetX(rect)      (rect.origin.x)
#define GetY(rect)      (rect.origin.y)

#define GetXW(rect)     ((rect.origin.x)+(rect.size.width))
#define GetYH(rect)     ((rect.origin.y)+(rect.size.height))

@protocol ChatDelegate <NSObject>

- (void)chatViewOperateWithIndex:(NSUInteger)index idx:(NSInteger)idx name:(NSString *)name;

@end

@interface ChatView : UIView <UITableViewDataSource, UITableViewDelegate>
{
    UITableView* tableview;
            
    NSDictionary* colourBars;
    NSMutableArray* message;
    
    CGFloat max_Height;
    
    UIColor* nc;
    UIColor* ic;
    UIFont * tf;
}
@property (nonatomic, assign) NSInteger adapterWidth;
@property (nonatomic, assign) NSInteger roomidx;
@property (nonatomic, assign) id<ChatDelegate> delegate;

- (id)initWithFrame:(CGRect)frame roomid:(NSInteger)roomid;

- (void)clearMessage;

- (void)pushMessage:(BaseMessage *)msg;

- (CGRect)createContent:(NSString *)content rect:(CGRect)rect font:(UIFont *)font  tc:(UIColor *)tc cell:(UIView *)cell;

- (CGRect)createLbl:(NSString *)title rect:(CGRect)rect font:(UIFont *)font tc:(UIColor *)tc cell:(UIView *)cell flag:(BOOL)flag idx:(int)idx;

- (CGRect)createIcon:(int)vip level:(int)level rect:(CGRect)rect cell:(UIView *)cell source:(int)source identity:(int)identity;

- (CGRect)createGift:(int)idx rect:(CGRect)rect cell:(UIView *)cell;

- (CGFloat)creategiftstarinfo:(NSString *)name cell:(UIView *)cell;

@end
