//
//  ExchangeTableView.m
//  SinaKTVAide
//
//  Created by 陈 鹏 on 14-7-24.
//  Copyright (c) 2014年 Zhang Chen. All rights reserved.
//

#import "ExchangeTableView.h"
#import "ExchangeLogCell.h"
#import "ExchangeLog.h"
#import "PKtvAssistantAPI.h"

@implementation ExchangeTableView

- (id)initWithFrame:(CGRect)frame type:(NSInteger)type
{
    self = [super initWithFrame:frame];
    if (self) {
        tableType = type;
        _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height) style:UITableViewStylePlain];
        _tableView.dataSource = self;
        _tableView.delegate = self;
        _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
        _tableView.tableFooterView = [[UIView alloc] init];//多余的cell屏蔽掉
        [self addSubview:_tableView];
        isLoad = false;
        index = 1;
        _data  = [[NSMutableArray alloc] init];
        [self loadData];
    }
    return self;
}

-(void)loadData
{
    if (isLoad) {
        return;
    }
    isLoad = true;
    NSString *strUrl;
    long uid=[UserSessionManager getInstance].currentUser.uid;
    strUrl = [PKtvAssistantAPI getGetGiftLogUrl:uid type:tableType page:index size:5];
    NSURL *url =[[NSURL alloc] initWithString:strUrl];
    NSURLRequest *request = [[NSURLRequest alloc] initWithURL:url];
    [AFHTTPRequestOperation addAcceptableContentTypes:[NSSet setWithObjects:@"text/plain", nil]];
    AFHTTPRequestOperation * operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
    [operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        NSDictionary *html = [PCommonUtil NSDictionaryFromJSONData:responseObject];
        NSString *status = [html objectForKey:@"status"];
        if ([status compare:@"1"] == NSOrderedSame) {
            NSDictionary *result = [html objectForKey:@"result"];
            NSArray *dataList = [result objectForKey:@"ktvgiftloglist"];
            
            for (NSDictionary *dic in dataList) {
                ExchangeLog *info = [ExchangeLog initWithDictionary:dic];
                [_data addObject:info];
            }
            [_tableView reloadData];
            index++;
        }
        else
        {
            
        }
        isLoad = NO;
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        isLoad = NO;
    }];
    [operation start];
}

//切换tab时候重新加载table
-(void)reloadTable
{
    index = 1;
    _data  = [[NSMutableArray alloc] init];
    [self loadData];
}

#pragma mark tableviewdelegate

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _data.count;
}

-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    ExchangeLog *log = (ExchangeLog *)[_data objectAtIndex:indexPath.row];
    ExchangeLogCell *cell =[tableView dequeueReusableCellWithIdentifier:@"cell" ];
    if (cell == nil) {
        cell = [[ExchangeLogCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell" cellType:tableType];
        cell.backgroundView = nil;
        cell.backgroundColor = [UIColor clearColor];
        cell.selectionStyle = UITableViewCellSelectionStyleNone;
    }
    cell.model = log;
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return  120;
}

-(void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if (scrollView == _tableView) {
        if (_tableView.contentOffset.y + _tableView.frame.size.height > (_tableView.contentSize.height + 60)) {
            [self loadData];
        }
        
    }
}



@end
