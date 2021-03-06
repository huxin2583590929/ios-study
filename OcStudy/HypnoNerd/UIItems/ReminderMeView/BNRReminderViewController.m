//
//  BNRReminderViewController.m
//  HypnoNerd
//
//  Created by HuXin on 2021/7/7.
//

#import "BNRReminderViewController.h"
#import <Masonry/Masonry.h>
#import <UserNotifications/UserNotifications.h>

@interface BNRReminderViewController () <UNUserNotificationCenterDelegate>

@property (nonatomic, weak)IBOutlet UIDatePicker *datePinncker;
@property (weak, nonatomic) IBOutlet UIButton *remindLabel;

@end

@implementation BNRReminderViewController

- (instancetype)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        UITabBarItem *tbi = self.tabBarItem;
        tbi.title = @"Reminder";
        UINavigationItem *navItem = self.navigationItem;
        navItem.title = @"timeReminer";
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.datePinncker mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.right.equalTo(self.view).insets(UIEdgeInsetsMake(80, 50, 0, 50));
    }];
    
    [self.remindLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.bottom.equalTo(self.view).insets(UIEdgeInsetsMake(0, 60, 120, 60));
        make.top.equalTo(self.datePinncker.mas_bottom).mas_offset(40);
    }];
    
}

- (IBAction)addReminder:(id)sender {
    NSDate *date = self.datePinncker.date;
    NSLog(@"Setting a reminder for %@",date);
    
    UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];
    center.delegate = self;
    UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
    content.title = [NSString localizedUserNotificationStringForKey:@"Hello" arguments:nil];
    content.body = [NSString stringWithFormat:@"%@",date];
    content.sound = [UNNotificationSound defaultSound];
    
    UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:60 repeats:NO];
    
    UNNotificationRequest *request = [UNNotificationRequest requestWithIdentifier:@"OXNotification" content:content trigger:trigger];
    
    [center addNotificationRequest:request withCompletionHandler:^(NSError *_Nullable error) {
        NSLog(@"??????????????????");
    }];
}

- (void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler {
    
    NSDictionary * userInfo = notification.request.content.userInfo;
    UNNotificationRequest *request = notification.request; // ?????????????????????
    UNNotificationContent *content = request.content; // ???????????????????????????
    NSNumber *badge = content.badge;  // ?????????????????????
    NSString *body = content.body;    // ???????????????
    UNNotificationSound *sound = content.sound;  // ?????????????????????
    NSString *subtitle = content.subtitle;  // ????????????????????????
    NSString *title = content.title;  // ?????????????????????
    
    if([notification.request.trigger isKindOfClass:[UNPushNotificationTrigger class]]) {
        NSLog(@"iOS10 ????????????????????????:%@", body);
        
    } else {
        // ?????????????????????
        NSLog(@"iOS10 ????????????????????????:{\\\\nbody:%@???\\\\ntitle:%@,\\\\nsubtitle:%@,\\\\nbadge???%@???\\\\nsound???%@???\\\\nuserInfo???%@\\\\n}",body,title,subtitle,badge,sound,userInfo);
    }
    completionHandler(UNNotificationPresentationOptionBadge|UNNotificationPresentationOptionSound|UNNotificationPresentationOptionAlert); // ?????????????????????????????????????????????????????????Badge???Sound???Alert????????????????????????
}

- (void)setTitle:(NSString *)title {
    self.navigationItem.title = @"timeReminder";
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
