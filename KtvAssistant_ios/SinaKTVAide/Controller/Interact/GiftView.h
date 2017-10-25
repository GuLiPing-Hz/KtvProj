//
//  GiftView.h
//  SinaKTVAide
//
//  Created by cxp on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CustomListBox.h"
#import <UIKit/UINavigationController.h>
#import "GiftScrollView.h"

@interface GiftView : UIView<CustomListBoxDelegate, GiftScrollDelegate>
{
    UIView  *giftView;
    UILabel *num;
    UIButton *numbtn;
    UILabel *lblwho;
    UIButton *whobtn;
    UIButton *sendbtn;
    UIButton *chargebtn;
    UIButton *normalbtn;
    UIButton *luckylbtn;
    UIButton *explbtn;
    NSInteger giftNum;
    GiftScrollView *view1;
    GiftScrollView *view2;
    GiftScrollView *view3;
    NSInteger giftid;
    UILabel *lblprice;
    UIImageView *seplucky;
    UIImageView *sepnormal;
    UIImageView *sepexp;
    int idx;
    long mygold;
    NSInteger giftprice;
    NSMutableArray *userName;
    NSMutableArray *useridx;
}
@property (nonatomic,retain) CustomListBox  *listBox;
@property (nonatomic,retain) CustomListBox  *peoplelistBox;
@property (strong, nonatomic) UINavigationController *naviMain;
-(void)addSendTarge:(NSString *)name index:(NSInteger)index;
-(void)showMessage;
@end
