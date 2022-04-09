/*
 * BJYFFOptions.h
 *
 * Copyright (c) 2013-2015 Bilibili
 * Copyright (c) 2013-2015 Zhang Rui <bbcallen@gmail.com>
 *
 * This file is part of ijkPlayer.
 *
 * ijkPlayer is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * ijkPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ijkPlayer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#import <Foundation/Foundation.h>

typedef enum BJYFFOptionCategory {
    kBJYFFOptionCategoryFormat = 1,
    kBJYFFOptionCategoryCodec  = 2,
    kBJYFFOptionCategorySws    = 3,
    kBJYFFOptionCategoryPlayer = 4,
    kBJYFFOptionCategorySwr    = 5,
} BJYFFOptionCategory;

// for codec option 'skip_loop_filter' and 'skip_frame'
typedef enum BJYAVDiscard {
    /* We leave some space between them for extensions (drop some
     * keyframes for intra-only or drop just some bidir frames). */
    BJY_AVDISCARD_NONE    =-16, ///< discard nothing
    BJY_AVDISCARD_DEFAULT =  0, ///< discard useless packets like 0 size packets in avi
    BJY_AVDISCARD_NONREF  =  8, ///< discard all non reference
    BJY_AVDISCARD_BIDIR   = 16, ///< discard all bidirectional frames
    BJY_AVDISCARD_NONKEY  = 32, ///< discard all frames except keyframes
    BJY_AVDISCARD_ALL     = 48, ///< discard all
} BJYAVDiscard;

struct IjkMediaPlayer;

@interface BJYFFOptions : NSObject

+(BJYFFOptions *)optionsByDefault;

-(void)applyTo:(struct IjkMediaPlayer *)mediaPlayer;

- (void)setOptionValue:(NSString *)value
                forKey:(NSString *)key
            ofCategory:(BJYFFOptionCategory)category;

- (void)setOptionIntValue:(int64_t)value
                   forKey:(NSString *)key
               ofCategory:(BJYFFOptionCategory)category;


-(void)setFormatOptionValue:       (NSString *)value forKey:(NSString *)key;
-(void)setCodecOptionValue:        (NSString *)value forKey:(NSString *)key;
-(void)setSwsOptionValue:          (NSString *)value forKey:(NSString *)key;
-(void)setPlayerOptionValue:       (NSString *)value forKey:(NSString *)key;

-(void)setFormatOptionIntValue:    (int64_t)value forKey:(NSString *)key;
-(void)setCodecOptionIntValue:     (int64_t)value forKey:(NSString *)key;
-(void)setSwsOptionIntValue:       (int64_t)value forKey:(NSString *)key;
-(void)setPlayerOptionIntValue:    (int64_t)value forKey:(NSString *)key;

@end
