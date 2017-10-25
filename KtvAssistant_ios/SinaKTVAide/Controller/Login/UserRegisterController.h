//
//  UserRegisterController.h
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-11-3.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"

@interface UserRegisterController : BaseViewController <UITableViewDataSource,UITableViewDelegate,UITextFieldDelegate,UIGestureRecognizerDelegate>
{
    BOOL isShow;
    NSInteger space;
}

@property(nonatomic,retain) UITextField *txtName;
@property(nonatomic,retain) UITextField *txtPwd;
@property(nonatomic,retain) UITableView *tableRegister;
@property(nonatomic,retain) UIButton    *btnRegister;
@property(nonatomic,assign) NSInteger   sex; // 1男 0女
@property(nonatomic,retain) UIImageView *imgWomen;
@property(nonatomic,retain) UIImageView *imgMen;

@end
