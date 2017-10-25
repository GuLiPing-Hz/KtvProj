//
//  ExpressionView.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-8-11.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//
#define MaxImgNum 126

#import "ExpressionView.h"

@implementation ExpressionView

- (id)initWithFrame:(CGRect)frame index:(NSInteger) index
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        for (NSInteger i =0; i<3; i++) {
            for (NSInteger j = 0 ; j< 7; j++) {
                UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake(6 + (_MainScreen_Width-12)/7 * j, 5 + 54 * i, (_MainScreen_Width-12)/7, 54)];
                if ((i== 2 && j==6) || (index * 20 + i * 7 + j + 1) == (MaxImgNum + 1))
                {
                    [btn setImage:[UIImage imageNamed:@"chat_backspace.png"] forState:UIControlStateNormal];
                    [btn setImage:[UIImage imageNamed:@"chat_backspace.png"] forState:UIControlStateHighlighted];
                    btn.tag = -1;
                }
                else
                {
                    [btn setImage:[UIImage imageNamed:[NSString stringWithFormat:@"%d.png", index * 20 + i * 7 + j + 1]] forState:UIControlStateNormal];
                    //[btn setImage:[UIImage imageNamed:[NSString stringWithFormat:@"%d.png", index * 20 + i * 7 + j + 1]] forState:UIControlStateHighlighted];
                    btn.tag = index * 20 + i * 7 + j + 1;
                }
                [btn addTarget:self action:@selector(btnClick:) forControlEvents:UIControlEventTouchUpInside];
                [self addSubview:btn];
                if (index * 20 + i * 7 + j + 1 > MaxImgNum) {
                    return self;
                }
            }
        }
    }
    return self;
}


-(void)btnClick:(id)sender
{
    UIButton *btn =(UIButton *)sender;
    [_delegate ExpressionClick:btn.tag];
}

@end
