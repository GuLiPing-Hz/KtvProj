//
//  KTVInputSecretController.h
//  SinaKTVAide
//
//  Created by Zhang Chen on 13-10-29.
//  Copyright (c) 2013年 Zhang Chen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseViewController.h"

@interface KTVInputSecretController : BaseViewController<UITextFieldDelegate>
{
    NSString   *loginKeyWord;
}
@property (nonatomic, strong) UITextField *field;

@end
