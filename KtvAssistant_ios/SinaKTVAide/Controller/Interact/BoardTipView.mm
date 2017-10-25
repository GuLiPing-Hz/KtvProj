//
//  BoardTipView.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-9-4.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "BoardTipView.h"
#import "NSMutableAttributedString+Weibo.h"

@implementation BoardTipView

- (id)initWithInfo:(NSMutableDictionary *)dic
{
    self = [super init];
    if (self) {
        self.backgroundColor = [UIColor whiteColor];
        NSInteger height = 32;
        CGSize size;
        
        UIImageView *imgSeperate= [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, _MainScreen_Width, 4)];
        imgSeperate.backgroundColor = UIColorFromRGB(0xff9b25);
        [self addSubview:imgSeperate];
        
        imgSpearker = [[UIImageView alloc] initWithFrame:CGRectMake(11,7,44,44)];
        imgSpearker.image = [UIImage imageNamed:@"chatList_icon_speaker.png"];
        imgSpearker.layer.cornerRadius = 22;
        imgSpearker.layer.masksToBounds = YES;
        imgSpearker.backgroundColor = [UIColor clearColor];
        [self addSubview:imgSpearker];
        
        lblName = [[UILabel alloc] initWithFrame:CGRectMake(58, 7, 160, 14)];
        lblName.textColor = UIColorFromRGB(0xe4397d);
        lblName.font = [UIFont systemFontOfSize:14];
        lblName.backgroundColor = [UIColor clearColor];
        [self addSubview:lblName];
        
        lbltime = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, 100, 10)];
        lbltime.font = [UIFont systemFontOfSize:10];
        lbltime.textColor = UIColorFromRGB(0x666666);
        lbltime.backgroundColor = [UIColor clearColor];
        [self addSubview:lbltime];
        
        /*lblContent = [[UILabel alloc] init];
        lblContent.font = [UIFont systemFontOfSize:14];
        lblContent.textColor = [UIColor blackColor];
        lblContent.backgroundColor = [UIColor clearColor];
        [self addSubview:lblContent];*/
        
        _textView = [[CoreTextView alloc] init];
        _textView.backgroundColor = [UIColor clearColor];
        [self addSubview:_textView];
        
        lblFrom = [[UILabel alloc] init];
        lblFrom.font = [UIFont systemFontOfSize:11];
        lblFrom.textColor = UIColorFromRGB(0x8a8e97);
        lblFrom.backgroundColor = [UIColor clearColor];
        [self addSubview:lblFrom];
        
        lblName.text = [dic objectForKey:@"name"];
        
        NSDate *date=[NSDate dateWithTimeIntervalSince1970:[[dic objectForKey:@"time"] intValue]];
        NSDateFormatter *format=[[NSDateFormatter alloc]init];
        [format setDateFormat:@"hh:mm"];
        NSString *nsdate=[format stringFromDate:date];
        lbltime.text = nsdate;
        [lbltime sizeToFit];
        lbltime.frame = CGRectMake(_MainScreen_Width - lbltime.frame.size.width - 11, 11, lbltime.frame.size.width, 10);
        
        //富文本显示内容
        //取最长的一行
        NSArray *newarray;
        NSArray *array = [[dic objectForKey:@"content"] componentsSeparatedByString:@"\n"];
        newarray = [array sortedArrayUsingComparator:^NSComparisonResult(id obj1, id obj2) {
            NSUInteger len0 = [(NSString *)obj1 length];
            NSUInteger len1 = [(NSString *)obj2 length];
            return len0 > len1 ? NSOrderedAscending : NSOrderedDescending;
        }];
        NSString *sortedmsg = [newarray objectAtIndex:0];
        for (int i = 1; i < newarray.count; i++) {
            NSString *string = [newarray objectAtIndex:i];
            sortedmsg = [sortedmsg stringByAppendingString:@"\n"];
            sortedmsg = [sortedmsg stringByAppendingString:string];
        }

        NSMutableAttributedString *newText = [NSMutableAttributedString weiboAttributedStringWithString:[dic objectForKey:@"content"] type:0];
        
        NSMutableAttributedString *broadText = [NSMutableAttributedString weiboAttributedStringWithString:[newarray objectAtIndex:0] type:0];
        
        //_textView.autoresizingMask = UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleBottomMargin;
        _textView.backgroundColor = [UIColor clearColor];
     
        NSInteger textWidth = [CoreTextView adjustSizeWidthWithAttributedString:broadText maxHeight:30].width;
        _textView.attributedString = newText;
        if (textWidth > 200)
            textWidth = 200;
        
        NSInteger textHegiht = [CoreTextView adjustSizeWithAttributedString:newText maxWidth:textWidth].height;
        _textView.frame = CGRectMake(58, 27, textWidth, textHegiht);

        height += size.height + 10 + textHegiht;
        
        if ([[dic objectForKey:@"roomid"] intValue])
        {
            lblFrom.text = [NSString stringWithFormat:@"来自于  -  %@", [dic objectForKey:@"roomname"]];
            size = [lblFrom.text sizeWithFont:[UIFont systemFontOfSize:11] constrainedToSize:CGSizeMake(MAXFLOAT, 11)];
            lblFrom.frame = CGRectMake(_MainScreen_Width - size.width - 11, height, size.width, size.height);
            height += 11 + 8;
        }
        if (IOS7_OR_LATER)
            self.frame = CGRectMake(0, 64, _MainScreen_Width, height);
        else
            self.frame = CGRectMake(0, 44, _MainScreen_Width, height);
            
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
