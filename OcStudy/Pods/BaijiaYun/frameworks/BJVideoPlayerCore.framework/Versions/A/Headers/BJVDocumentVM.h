//
//  BJVDocumentVM.h
//  BJVideoPlayerCore
//
//  Created by 辛亚鹏 on 2021/7/5.
//  Copyright © 2021 BaijiaYun. All rights reserved.
//

#import "BJVBaseVM.h"
#import "BJVUser.h"

#import <BJLiveBase/BJLDocument.h>
#import <BJLiveBase/BJLSlideshowUI.h>
#import <BJLiveBase/BJLSlidePage.h>
#import <BJLiveBase/BJLBlackboardUI.h>
#import <BJLiveBase/BJLWritingBoardUI.h>

#import "BJVWindowUpdateModel.h"
#import "BJVWritingBoard.h"

NS_ASSUME_NONNULL_BEGIN

/**
 ### 课件管理、显示
 #discussion BJLDocument：课件，教室内默认至少有一个白板课件，老师可能会上传多个课件，每个课件有一页或多页，每个课件内的 pageIndex 单独计算、从 0 开始
 #discussion BJLSlidePage：幻灯片，将所有课件拆散、组成成幻灯片序列，课件的每一页对应一个幻灯片，所有幻灯片 slidePageIndex 整体计算、从 0 开始
 #discussion 参考 `loadAllDocuments`
 */
@interface BJVDocumentVM : BJVBaseVM

#pragma mark - document management

/**
 翻页课件
 #param documentID 课件 ID
 #param pageIndex 目标页在课件中的序号
 #return BJLError:
 BJLErrorCode_invalidArguments  错误参数
 */
- (nullable BJLError *)requestTurnToDocumentID:(NSString *)documentID
                                     pageIndex:(NSInteger)pageIndex;

/**
 添加课件
 #discussion 添加成功将调用 `BJLDocumentVM` 的 `didAddDocument:`
 #param document 课件
 #return BJLError:
 BJLErrorCode_invalidArguments  错误参数
 */
- (nullable BJLError *)addDocument:(BJLDocument *)document;

/**
 删除课件
 #discussion 删除成功将调用 `BJLDocumentVM` 的 `didDeleteDocument:`
 #param documentID 课件 ID
 #return BJLError:
 BJLErrorCode_invalidArguments  错误参数
 */
- (nullable BJLError *)deleteDocumentWithID:(NSString *)documentID;

/**
 添加白板
 #discussion 最多同时存在 10 页白板
 #return BJLError
 */
- (nullable BJLError *)addWhiteboardPage;

/**
 删除白板
 #param pageIndex 白板页码，使用 BJLSlidePage 的 `slidePageIndex`
 #return BJLError
 */
- (nullable BJLError *)deleteWhiteboardPageWithIndex:(NSInteger)pageIndex;


/**
 获取文档备注信息
 #param documentID 文档ID
 #param completion 返回 remarkInfo 备注信息
 #return task
 */
- (nullable NSURLSessionDataTask *)requestCompleteRemarkInfoWithDocumentID:(NSString *)documentID
                                                                completion:(void (^)(NSDictionary * _Nullable remarkInfo, BJLError * _Nullable error))completion;

#pragma mark - document show

/** 所有课件 */
@property (nonatomic, readonly, copy, nullable) NSArray<BJLDocument *> *allDocuments;


/** 设置单实例文档下的白板数据，进教室前设置，未设置使用默认白板数据 */
@property (nonatomic, nullable) BJLWhiteboard *whiteboard;

/**
 `allDocuments` 被覆盖更新
 #discussion 覆盖更新才调用，增量更新不调用
 #param allDocuments 所有课件
 */
- (BJLObservable)allDocumentsDidOverwrite:(nullable NSArray<BJLDocument *> *)allDocuments;

/**
 加载所有课件
 #discussion 连接教室后、掉线重新连接后自动调用加载
 #discussion 加载成功后更新 `allDocuments`、调用 `allDocumentsDidOverwrite:`
 */
- (void)loadAllDocuments;

/**
 添加课件通知
 #discussion 同时更新 `allDocuments`
 #param document 课件
 */
- (BJLObservable)didAddDocument:(BJLDocument *)document;

/**
 删除课件通知
 #discussion 同时更新 `allDocuments`
 #param document 课件
 */
- (BJLObservable)didDeleteDocument:(BJLDocument *)document;

/**
 添加白板通知
 #discussion 同时更新 `allDocuments`
 #param pageIndex 白板页码
 */
- (BJLObservable)didAddWhiteboardPage:(NSInteger)pageIndex;

/**
 删除白板通知
 #discussion 同时更新 `allDocuments`
 #param pageIndex 白板页码
 */
- (BJLObservable)didDeleteWhiteboardPageWithIndex:(NSInteger)pageIndex;

/** 课件总页数，未知页数的文档认为只有一页 */
@property (nonatomic, readonly) NSInteger totalPageCount;

/** 课件当前页信息 */
@property (nonatomic, readonly, nullable) BJLSlidePage *currentSlidePage;

/** 是否禁止学生翻页 */
@property (nonatomic, readonly) BOOL forbidStudentChangePPT;

/**
 通过 documentID 获取 document
 #param documentID 课件 ID
 */
- (nullable BJLDocument *)documentWithID:(NSString *)documentID;

/**
 通过 documentID、pageIndex 获取 slide page
 #param documentID 课件 ID
 #param pageIndex 目标页在课件中的序号
 */
- (nullable BJLSlidePage *)slidePageWithDocumentID:(NSString *)documentID
                                         pageIndex:(NSInteger)pageIndex;

/**
 通过 documentID、pageIndex 获取 slide page
 #param documentID 课件 ID
 #param pageIndex 目标页在课件中的序号
 #param step 具体动画步数
 */
- (nullable BJLSlidePage *)slidePageWithDocumentID:(NSString *)documentID
                                         pageIndex:(NSInteger)pageIndex
                                              step:(NSInteger)step;

/**
 指定 documentID，获取对应课件的所有页面
 #param documentID 课件 ID
 */
- (NSArray<BJLSlidePage *> *)slidePagesWithDocumentID:(NSString *)documentID;

/** 获取 slide pages */
- (NSArray<BJLSlidePage *> *)allSlidePages;

#pragma mark - interactive class 专业版小班课 API

/** 黑板视图控制器 */
@property (nonatomic, readonly) UIViewController<BJLBlackboardUI> *blackboardViewController;
/** 黑板页数 */
//@property (nonatomic, readonly) NSInteger blackboardContentPages;

/** 设置黑板视图（blackboardViewController）的默认背景图 */
//@property (nonatomic) UIImage *blackboardImage;

/**
 指定文档 ID 创建对应的视图控制器
 #param documentID 文档 ID，通过 BJLDocument 的 documentID 获得
 #return 对应文档的视图控制器
 */
- (UIViewController<BJLSlideshowUI> *)documentViewControllerWithID:(NSString *)documentID;

/**
 指定文档 ID 创建对应的预览课件
 #param documentID 文档 ID，通过 BJLDocument 的 documentID 获得
 #return 对应文档的视图控制器，翻页/画笔等操作都不会同步到远端，仅供本地预览
 */
- (UIViewController<BJLSlideshowUI> *)previewDocumentViewControllerWithID:(NSString *)documentID;

/**
 更新文档显示信息
 #param documentID 文档 ID
 #param displayInfo 文档显示信息
 */
- (nullable BJLError *)updateDocumentWithID:(NSString *)documentID displayInfo:(BJLDocumentDisplayInfo *)displayInfo;

/**
 更新文档显示信息的通知
 #param document 更新后的文档信息
 */
- (BJLObservable)didUpdateDocument:(BJLDocument *)document;

/**
 更新文档窗口
 #param documentID 文档 ID
 #param action 更新类型，参考 BJVWindowsUpdateModel 的 BJVWindowsUpdateAction
 #param displayInfos 教室内所有文档窗口显示信息
 #return 调用错误, 参考 BJLErrorCode
 */
- (nullable BJLError *)updateDocumentWindowWithID:(NSString *)documentID
                                           action:(NSString *)action
                                     displayInfos:(NSArray<BJVWindowDisplayInfo *> *)displayInfos;
- (nullable BJLError *)updateDocumentWindowWithID:(NSString *)documentID
                                           action:(NSString *)action
                                     displayInfos:(NSArray<BJVWindowDisplayInfo *> *)displayInfos
                                       nextAction:(nullable NSString *)nextAction;

/**
 文档窗口更新通知
 #param updateModel 更新信息
 #param shouldReset 是否重置
 */
- (BJLObservable)didUpdateDocumentWindowWithModel:(BJVWindowUpdateModel *)updateModel shouldReset:(BOOL)shouldReset;

/** 当前ppt课件品质 YES: 原图，NO: 流畅，默认为 NO */
@property (nonatomic, assign, readonly) BOOL pptQualityIsOriginal;

/**
 切换课件品质
 #discussion 仅对专业小班课静态PPT有效
 #param isOriginal YES: 原图，NO: 流畅
 */
- (nullable NSError *)pptQualityChange:(BOOL)isOriginal;

#pragma mark - PPT 授权

/** 学生：是否有 h5 ppt 操作权限  老师：当前对学生的h5 ppt的授权状态  */
@property (nonatomic, readonly) BOOL authorizedH5PPT;

/** 课件授权的学生 */
@property (nonatomic, readonly, nullable) NSArray<NSString *> *authorizedPPTUserNumbers;

/**
 授权课件
 #param authorized authorized
 #param userNumber userNumber
 #return BJLErrorCode_invalidUserRole 错误角色，仅老师或助教能授权给学生
 */
- (nullable BJLError *)updateStudentPPTAuthorized:(BOOL)authorized userNumber:(NSString *)userNumber;

#pragma mark - 动态课件事件

/** 动态课件是否在播放视频 */
@property (nonatomic, readonly) BOOL webPPTPlayingVideo;

#pragma mark - 动效 PPT 可交互

/**
 给大班课所有学生授权操作h5课件权限
 #param authorized 是否可以操作课件
 */
- (nullable BJLError *)updateAllStudentH5PPTAuthorized:(BOOL)authorized;

#pragma mark - 小黑板

/** 获取当前登录用户所在group是否有更多在线用户未加载 */
@property (nonatomic, readonly) BOOL hasMoreWritingBoardUsers; // NON-KVO

/** 所有参加作答小黑板的用户 */
@property (nonatomic, readonly, copy, nullable) NSArray<BJVUser *> *allWritingBoardParticipatedUsers;

/** 所有已提交小黑板的用户 */
@property (nonatomic, readonly, copy, nullable) NSArray<BJVUser *> *allWritingBoardSubmitedUsers;

/**
 根据 writingBoard 创建对应的小黑板控制器
 #param writingBoard 小黑板信息
 #return 对应小黑板的视图控制器
 */
- (UIViewController<BJLWritingBoardUI> *)writingBoardViewControllerWithWritingBoard:(BJVWritingBoard *)writingBoard;

/** 设置小黑板视图（writingboardViewController）的默认背景图 */
@property (nonatomic) UIImage *writingBoardImage;


/**
 发布小黑板的通知
 #discussion 直播间内所有用户不区分角色都会收到此信令
 #param writingBoard 小黑板信息, 此时 writingBoard 的 `submitedUsers` 和 `participatedUsers` 为空,此处不做统计
 */
- (BJLObservable)didPublishWritingBoard:(BJVWritingBoard *)writingBoard;


/**
 提交小黑板作答内容的通知
 #param writingBoard 小黑板信息, 此时 writingBoard 的 `submitedUsers` 和 `participatedUsers` 为空,此处不做统计
 */
- (BJLObservable)didSubmitWritingBoard:(BJVWritingBoard *)writingBoard;

/**
 参与作答小黑板的通知
 #param writingBoard 小黑板信息, 此时 writingBoard 的 `submitedUsers` 和 `participatedUsers` 为空,此处不做统计
 */
- (BJLObservable)didParticipateWritingBoard:(BJVWritingBoard *)writingBoard;

/**
 拉取小黑板状态的通知
 #param writingBoard 小黑板信息
 此时 writingBoard 的 `submitedUsers` 和 `participatedUsers` 为空,此处不做统计, 建议使用 `documentVM.allWritingBoardParticipatedUsers` 和 `documentVM,allWritingBoardSubmitedUsers`
 */
- (BJLObservable)didPullWritingBoard:(nullable BJVWritingBoard *)writingBoard;

/**
 小黑板窗口更新通知
 #param updateModel 更新信息
 #param shouldReset 是否重置
 */
//- (BJLObservable)didUpdateWritingBoradWindowWithModel:(BJVWindowUpdateModel *)updateModel shouldReset:(BOOL)shouldReset;

@end


NS_ASSUME_NONNULL_END
