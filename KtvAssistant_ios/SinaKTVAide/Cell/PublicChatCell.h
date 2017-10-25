//
//  PublicChatCell.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-23.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MessageInfoStruct.h"
#import "User.h"
#import "UserSessionManager.h"
#import "com_ios_NetUtil.h"
#import "CoreTextView.h"

@protocol ChatCellHBProtocol <NSObject>

@required
-(void)onHBClick:(AllInfo)info;

@optional
-(void)onUserHeadClick:(CGPoint)point view:(id)view name:(NSString*)name idx:(int)idx;

@end

@interface PublicChatCell : UITableViewCell
{
    UIImageView     *imgBack;
    UIImageView     *imgHead;
    UIImageView     *imgSpeaker;
    UIImageView     *gift;
    UILabel         *lblTime;
    UILabel         *lblName;
    UILabel         *lblroomname;
    UIView          *talkContentView;
    UIView          *textContainer;
    UILabel         *lblRedtype;
    UILabel         *lblget;
    UILabel         *lblmark;
    UILabel         *lblsys;
    UIImageView     *grayimgBack;
    UIImageView     *imgbroad;
    UIButton        *hbBtn;
}

@property (nonatomic,assign) AllInfo Info;
@property (nonatomic,retain) CoreTextView *textView;
@property (nonatomic,assign) NSInteger y;
@property (nonatomic,assign) BOOL isShow;
@property(nonatomic,retain) id<ChatCellHBProtocol> mHBDelegate;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier;

+ (CGFloat)CountRowHeight:(AllInfo)Info;

@end
