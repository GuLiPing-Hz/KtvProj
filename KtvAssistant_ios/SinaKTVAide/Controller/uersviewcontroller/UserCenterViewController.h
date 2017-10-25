//
//  UserCenterViewController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-16.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"
#import "ZBarReaderViewController.h"

@interface UserCenterViewController :BaseViewController<UIImagePickerControllerDelegate,UINavigationControllerDelegate,UIActionSheetDelegate>

@property (nonatomic,retain) UILabel                    *lblName;
@property (nonatomic,retain) UILabel                    *lblMoney;
@property (nonatomic,retain) UIImageView                *imgHead;
@property (nonatomic,retain) UIButton                   *btnHead;
@property (nonatomic,retain) UIButton                   *btnGift;
@property (nonatomic,retain) UILabel                    *lblGift;
@property (nonatomic,retain) UIButton                   *btnGold;
@property (nonatomic,strong) UIImageView               *egoHeadImage;
@property (nonatomic,retain) UIActionSheet              *changeHeadPhotoSheet;
@property (nonatomic,retain) NSString                   *keyWord;
@property (nonatomic,strong) UIImage                    *changeHeadImage;
@property (nonatomic,strong) NSMutableArray             *tbView;


@end
