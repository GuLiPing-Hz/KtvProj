//
//  ChangeUserInfoViewController.h
//  SinaKTVAide

//  修改昵称 界面

//  Created by Li Pan on 13-12-18.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import "BaseViewController.h"

@interface ChangeUserInfoViewController : BaseViewController<UITextFieldDelegate>
@property (nonatomic,retain) UITextField *nickName;
@property (nonatomic,retain) UIView *nickView;
@property (nonatomic,retain) UILabel *nickNotice;
@end
