//
//  YYClassInfo.h
//  YYModel <https://github.com/ibireme/YYModel>
//
//  Created by ibireme on 15/5/9.
//  Copyright (c) 2015 ibireme.
//
//  This source code is licensed under the MIT-style license found in the
//  LICENSE file in the root directory of this source tree.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Type encoding's type.
 */
typedef NS_OPTIONS(NSUInteger, BJLYYEncodingType) {
    BJLYYEncodingTypeMask       = 0xFF, ///< mask of type value
    BJLYYEncodingTypeUnknown    = 0, ///< unknown
    BJLYYEncodingTypeVoid       = 1, ///< void
    BJLYYEncodingTypeBool       = 2, ///< bool
    BJLYYEncodingTypeInt8       = 3, ///< char / BOOL
    BJLYYEncodingTypeUInt8      = 4, ///< unsigned char
    BJLYYEncodingTypeInt16      = 5, ///< short
    BJLYYEncodingTypeUInt16     = 6, ///< unsigned short
    BJLYYEncodingTypeInt32      = 7, ///< int
    BJLYYEncodingTypeUInt32     = 8, ///< unsigned int
    BJLYYEncodingTypeInt64      = 9, ///< long long
    BJLYYEncodingTypeUInt64     = 10, ///< unsigned long long
    BJLYYEncodingTypeFloat      = 11, ///< float
    BJLYYEncodingTypeDouble     = 12, ///< double
    BJLYYEncodingTypeLongDouble = 13, ///< long double
    BJLYYEncodingTypeObject     = 14, ///< id
    BJLYYEncodingTypeClass      = 15, ///< Class
    BJLYYEncodingTypeSEL        = 16, ///< SEL
    BJLYYEncodingTypeBlock      = 17, ///< block
    BJLYYEncodingTypePointer    = 18, ///< void*
    BJLYYEncodingTypeStruct     = 19, ///< struct
    BJLYYEncodingTypeUnion      = 20, ///< union
    BJLYYEncodingTypeCString    = 21, ///< char*
    BJLYYEncodingTypeCArray     = 22, ///< char[10] (for example)
    
    BJLYYEncodingTypeQualifierMask   = 0xFF00,   ///< mask of qualifier
    BJLYYEncodingTypeQualifierConst  = 1 << 8,  ///< const
    BJLYYEncodingTypeQualifierIn     = 1 << 9,  ///< in
    BJLYYEncodingTypeQualifierInout  = 1 << 10, ///< inout
    BJLYYEncodingTypeQualifierOut    = 1 << 11, ///< out
    BJLYYEncodingTypeQualifierBycopy = 1 << 12, ///< bycopy
    BJLYYEncodingTypeQualifierByref  = 1 << 13, ///< byref
    BJLYYEncodingTypeQualifierOneway = 1 << 14, ///< oneway
    
    BJLYYEncodingTypePropertyMask         = 0xFF0000, ///< mask of property
    BJLYYEncodingTypePropertyReadonly     = 1 << 16, ///< readonly
    BJLYYEncodingTypePropertyCopy         = 1 << 17, ///< copy
    BJLYYEncodingTypePropertyRetain       = 1 << 18, ///< retain
    BJLYYEncodingTypePropertyNonatomic    = 1 << 19, ///< nonatomic
    BJLYYEncodingTypePropertyWeak         = 1 << 20, ///< weak
    BJLYYEncodingTypePropertyCustomGetter = 1 << 21, ///< getter=
    BJLYYEncodingTypePropertyCustomSetter = 1 << 22, ///< setter=
    BJLYYEncodingTypePropertyDynamic      = 1 << 23, ///< @dynamic
};

/**
 Get the type from a Type-Encoding string.
 
 @discussion See also:
 https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtTypeEncodings.html
 https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtPropertyIntrospection.html
 
 @param typeEncoding  A Type-Encoding string.
 @return The encoding type.
 */
BJLYYEncodingType BJLYYEncodingGetType(const char *typeEncoding);


/**
 Property information.
 */
@interface BJLYYClassPropertyInfo: NSObject
@property (nonatomic, assign, readonly) objc_property_t property; ///< property's opaque struct
@property (nonatomic, strong, readonly) NSString *name;           ///< property's name
@property (nonatomic, assign, readonly) BJLYYEncodingType type;      ///< property's type
@property (nonatomic, strong, readonly) NSString *typeEncoding;   ///< property's encoding value
@property (nonatomic, strong, readonly) NSString *ivarName;       ///< property's ivar name
@property (nullable, nonatomic, assign, readonly) Class cls;      ///< may be nil
@property (nullable, nonatomic, strong, readonly) NSArray<NSString *> *protocols; ///< may nil
@property (nonatomic, assign, readonly) SEL getter;               ///< getter (nonnull)
@property (nonatomic, assign, readonly) SEL setter;               ///< setter (nonnull)

/**
 Creates and returns a property info object.
 
 @param property property opaque struct
 @return A new object, or nil if an error occurs.
 */
- (instancetype)initWithProperty:(objc_property_t)property;
@end


/**
 Class information for a class.
 */
@interface BJLYYClassInfo: NSObject
@property (nonatomic, assign, readonly) Class cls; ///< class object
@property (nullable, nonatomic, assign, readonly) Class superCls; ///< super class object
@property (nullable, nonatomic, assign, readonly) Class metaCls;  ///< class's meta class object
@property (nonatomic, readonly) BOOL isMeta; ///< whether this class is meta class
@property (nonatomic, strong, readonly) NSString *name; ///< class name
@property (nullable, nonatomic, strong, readonly) BJLYYClassInfo *superClassInfo; ///< super class's class info
@property (nullable, nonatomic, strong, readonly) NSDictionary<NSString *, BJLYYClassPropertyInfo *> *propertyInfos; ///< properties

/**
 If the class is changed (for example: you add a method to this class with
 'class_addMethod()'), you should call this method to refresh the class info cache.
 
 After called this method, `needUpdate` will returns `YES`, and you should call 
 'classInfoWithClass' or 'classInfoWithClassName' to get the updated class info.
 */
- (void)setNeedUpdate;

/**
 If this method returns `YES`, you should stop using this instance and call
 `classInfoWithClass` or `classInfoWithClassName` to get the updated class info.
 
 @return Whether this class info need update.
 */
- (BOOL)needUpdate;

/**
 Get the class info of a specified Class.
 
 @discussion This method will cache the class info and super-class info
 at the first access to the Class. This method is thread-safe.
 
 @param cls A class.
 @return A class info, or nil if an error occurs.
 */
+ (nullable instancetype)classInfoWithClass:(Class)cls;

/**
 Get the class info of a specified Class.
 
 @discussion This method will cache the class info and super-class info
 at the first access to the Class. This method is thread-safe.
 
 @param className A class name.
 @return A class info, or nil if an error occurs.
 */
+ (nullable instancetype)classInfoWithClassName:(NSString *)className;

@end

NS_ASSUME_NONNULL_END
