// UIKit+AFNetworking.h
//
// Copyright (c) 2011–2016 Alamofire Software Foundation ( http://alamofire.org/ )
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#if TARGET_OS_IOS || TARGET_OS_TV
#import <UIKit/UIKit.h>

#ifndef _UIKIT_BJLAFNETWORKING_
    #define _UIKIT_BJLAFNETWORKING_

#if TARGET_OS_IOS
    #import "BJLAFAutoPurgingImageCache.h"
    #import "BJLAFImageDownloader.h"
    #import "BJLAFNetworkActivityIndicatorManager.h"
    #import "UIRefreshControl+BJLAFNetworking.h"
#endif

    #import "UIActivityIndicatorView+BJLAFNetworking.h"
    #import "UIButton+BJLAFNetworking.h"
    #import "UIImageView+BJLAFNetworking.h"
    #import "UIProgressView+BJLAFNetworking.h"
#endif /* _UIKIT_BJLAFNETWORKING_ */
#endif
