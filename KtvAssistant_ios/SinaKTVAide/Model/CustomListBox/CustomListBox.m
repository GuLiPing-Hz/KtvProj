//
//  CustomListBox.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-25.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "CustomListBox.h"

@implementation CustomListBox

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.backgroundColor = [UIColor clearColor];
        self.hidden = YES;
        showType = 0;
        UIView *backView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height - 10)];
        backView.backgroundColor = UIColorFromRGB(0x000000);
        backView.alpha = 0.6 ;
        [self addSubview: backView];
        
        UIImageView *imgBottom = [[UIImageView alloc] initWithFrame:CGRectMake(self.frame.size.width /2 - 8, self.frame.size.height - 10, 16, 10)];
        [imgBottom setImage:[UIImage imageNamed:@"droplist_bg_arrow.png"]];
        imgBottom.backgroundColor = [UIColor clearColor];
        [self addSubview:imgBottom];
        
        _tableView = [[ UITableView alloc] initWithFrame:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height - 10) style:UITableViewStylePlain];
        _tableView.backgroundColor = [UIColor clearColor];
        _tableView.dataSource = self;
        _tableView.delegate = self;
        _tableView.tableFooterView = [[UIView alloc]init];
        _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        [self addSubview:_tableView];
    }
    return self;
}

-(void)setData:(NSArray *)_keys values:(NSArray *)_values
{
    key = _keys;
    value = _values;
    [_tableView reloadData];
}

#pragma mark table delegate

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return  value.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if (cell == nil) {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.textLabel.font = [UIFont systemFontOfSize:12];
        cell.textLabel.textAlignment = UITextAlignmentCenter;
        cell.backgroundColor = [UIColor clearColor];
        cell.textLabel.backgroundColor = [UIColor clearColor];
        cell.textLabel.textColor = [UIColor whiteColor];
        [cell bringSubviewToFront:cell.textLabel];
        cell.selectedBackgroundView = [[UIView alloc] init];
        cell.selectedBackgroundView.backgroundColor = [UIColor blackColor];
        cell.selectedBackgroundView.alpha = 0.4f;
    }
    cell.textLabel.text = [key objectAtIndex:indexPath.row];
    
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [_delegate ListBoxClick:self key:[key objectAtIndex:indexPath.row] value:[value objectAtIndex:indexPath.row]];
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    [self ShowOrHide];
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return  20;
}

-(void)ShowOrHide
{
    if (showType == 0)
    {
        showType = -1;
        self.hidden = NO;
        self.alpha = 0;
        [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            self.alpha = 1;
        } completion:^(BOOL finished){
            showType = 1;
        }];
    }
    else if(showType == 1)
    {
        showType = -1;
        self.alpha = 1;
        [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
            self.alpha = 0;
        } completion:^(BOOL finished){
            self.hidden = YES;
            showType = 0;
        }];
    }

}


-(void)Hide
{
    if (showType != 1) {
        return;
    }
    showType = -1;
    self.alpha = 1;
    [UIView animateWithDuration:0.5 delay:0 options:UIViewAnimationOptionCurveEaseInOut animations:^{
        self.alpha = 0;
    } completion:^(BOOL finished){
        self.hidden = YES;
        showType = 0;
    }];
}


@end
