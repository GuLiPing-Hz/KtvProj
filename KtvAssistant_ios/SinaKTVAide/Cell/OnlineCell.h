//
//  OnlineCell.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-29.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MessageInfoStruct.h"

@interface OnlineCell : UITableViewCell
{
    UIImageView     *imgHead;
    UILabel         *lblName;
    UILabel         *lblid;
    UIImageView     *imgsex;
    UILabel         *lbluserid;
    UIImageView     *separate;
    
    UIImageView     *moreView;
    UIButton        *sendGiftbtn;
    UIButton        *chatBtn;
    UIButton        *redBtn;
    UIImageView     *listBtn;
    UIImageView     *imginroom;
    UIImageView     *imgAngle;
}

@property (nonatomic,assign) OnlineInfo Info;
@property (nonatomic,strong) UIImageView *moreView;
@property (nonatomic, assign) BOOL isHigher;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier;

- (void)selectRow;
- (void)deSelectRow;

@end
