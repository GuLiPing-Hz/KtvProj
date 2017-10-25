//
//  Header.h
//  SinaKTVAide
//
//  Created by 顾利平 on 15/5/18.
//  Copyright (c) 2015年 tiange. All rights reserved.
//

#ifndef SongOperationProtocol_H
#define SongOperationProtocol_H

#import "SongsBookModel.h"
#import "TopSongCellModel.h"
#import "KTVOrderedSongInfo.h"

@protocol SongOperation <NSObject>

@optional

-(void)onOrder:(SongsBookModel*) info view:(id)cell point:(CGPoint)point;
-(void)onTop:(SongsBookModel*) info;
-(void)onDelete:(SongsBookModel*) info;
-(void)onCollect:(SongsBookModel*) info;

-(void)onTopOrder:(TopSongCellModel*) info view:(id)cell point:(CGPoint)point;
-(void)onTopTop:(TopSongCellModel*) info;
-(void)onTopDelete:(TopSongCellModel*) info;
-(void)onTopCollect:(TopSongCellModel*) info;

-(void)onOrderedTop:(KTVOrderedSongInfo*)info;
-(void)onOrderedDelete:(KTVOrderedSongInfo*)info;
-(void)onOrderedCollect:(KTVOrderedSongInfo*)info;
@end

#endif//SongOperationProtocol_H
