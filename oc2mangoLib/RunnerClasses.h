//
//  ORunner.h
//  MangoFix
//
//  Created by Jiang on 2020/4/26.
//  Copyright © 2020 yongpengliang. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
@interface ORCodeCheck : NSObject
@property (nonatomic, assign)NSInteger lineNum;
@property (nonatomic, assign)NSInteger columnStart;
@property (nonatomic, assign)NSInteger length;
@property (nonatomic, copy)NSString *filename;
@end

// MARK: - Base
typedef enum{
    TypeVoid = 0x00,
    TypeUChar = 0x01,
    TypeUInt,
    TypeUShort,
    TypeULong,
    TypeULongLong,
    TypeBOOL,
    TypeChar,
    TypeShort,
    TypeInt,
    TypeLong,
    TypeLongLong,
    TypeFloat,
    TypeDouble,
    TypeBaseMask = 0x0F,
    TypeUnion = 0x10,
    TypeStruct = 0x20,
    TypeSEL = 0x30,
    TypeClass = 0x40,
    TypeObject = 0x50,
    TypeBlock = 0x60,
    TypeId = 0x70,
    TypeEnum = 0x80,
    TypeUnKnown = 0xF0
}TypeKind;
enum{
    AttributeConst = 1,
    AttributeStatic = 1 << 1,
    AttributeVolatile = 1 << 2,
    AttributeStrong = 1 << 3,
    AttributeBlock = 1 << 4,
    AttributeWeak = 1 << 5,
    AttributeExtern = 1 << 6,
    AttributeNonnull = 1 << 7,
    AttributeNullable = 1 << 8,
    AttributeBridge = 1 << 9
};


@interface ORTypeSpecial: NSObject
@property (nonatomic, assign) TypeKind type;
@property (nonatomic, nullable, copy) NSString * name;
+ (instancetype)specialWithType:(TypeKind)type name:(NSString *)name;
@end


@interface ORVariable :NSObject
@property (nonatomic, assign) NSInteger ptCount;
@property (nonatomic, nullable, copy) NSString * varname;
+ (instancetype)copyFromVar:(ORVariable *)var;
@end

@interface ORTypeVarPair: NSObject
@property (nonatomic, strong)ORTypeSpecial *type;
@property (nonatomic, strong)ORVariable *var;
@end

@interface ORFuncVariable: ORVariable
@property(nonatomic,strong) NSMutableArray <ORTypeVarPair *> *pairs;
@end

@interface ORFuncDeclare: NSObject
@property(nonatomic,strong) ORTypeVarPair *returnType;
@property(nonatomic,strong) ORFuncVariable *funVar;
@property(nonatomic,assign) BOOL isBlockDeclare;
- (void)becomeNormalFuncDeclare;
@end

// MARK: - Expression
@interface ORExpression: ORCodeCheck

@end

typedef enum {
    OCValueVariable, // value: NSString
    OCValueSelf, // value: nil
    OCValueSuper, // value: nil
    OCValueSelector, // value: sel NSString
    OCValueProtocol, // value: String
    OCValueDictionary, // value: Exp Array
    OCValueArray, // value: Exp Array
    OCValueNSNumber, // value: Exp
    OCValueString, // value: NSString
    OCValueCString, // value: NSString
    OCValueInt, // value: NSString
    OCValueDouble, // value: NSString
    OCValueNil, //  value: nil
    OCValueNULL, //  value: nil
    OCValueBOOL, //  value: @"YES" @"NO"
    
    //Class
    OCValueMethodCall,
    OCValueFuncCall,
    OCValueBlock,
    OCValueCollectionGetValue // array[0] , dict[@"key"]
}OC_VALUE_TYPE;

@interface ORValueExpression: ORExpression
@property (nonatomic, assign)OC_VALUE_TYPE value_type;
@property (nonatomic,strong)id value;
@end

@interface ORMethodCall: ORValueExpression
@property (nonatomic, strong)ORValueExpression * caller;
@property (nonatomic, assign)BOOL isDot;
@property (nonatomic, strong)NSMutableArray *names;
@property (nonatomic, strong)NSMutableArray <ORExpression *> *values;
- (NSString *)selectorName;
@end

@interface ORCFuncCall: ORValueExpression
@property (nonatomic, strong)ORValueExpression *caller;
@property (nonatomic, strong)NSMutableArray <ORExpression *>*expressions;
@end

@interface ORScopeImp: ORValueExpression
@property(nonatomic,strong) NSMutableArray* statements;
- (void)addStatements:(id)statements;
- (void)copyFromImp:(ORScopeImp *)imp;
@end

@interface ORBlockImp: ORScopeImp
@property(nonatomic,strong) ORFuncDeclare *declare;
@end

@interface ORSubscriptExpression: ORValueExpression
@property (nonatomic, strong)ORValueExpression * caller;
@property (nonatomic, strong)ORExpression * keyExp;
@end

typedef enum {
    AssignOperatorAssign,
    AssignOperatorAssignAnd,
    AssignOperatorAssignOr,
    AssignOperatorAssignXor,
    AssignOperatorAssignAdd,
    AssignOperatorAssignSub,
    AssignOperatorAssignDiv,
    AssignOperatorAssignMuti,
    AssignOperatorAssignMod,
    AssignOperatorAssignShiftLeft,
    AssignOperatorAssignShiftRight,
}AssignOperatorType;

@interface ORAssignExpression: ORExpression
@property (nonatomic,strong)ORValueExpression * value;
@property (nonatomic,assign)AssignOperatorType assignType;
@property (nonatomic,strong)ORExpression * expression;
@end

@interface ORDeclareExpression: ORExpression
@property (nonatomic,strong)ORTypeVarPair *pair;
@property (nonatomic,strong, nullable)ORExpression * expression;
@end

typedef enum {
    UnaryOperatorIncrementSuffix,
    UnaryOperatorDecrementSuffix,
    UnaryOperatorIncrementPrefix,
    UnaryOperatorDecrementPrefix,
    UnaryOperatorNot,
    UnaryOperatorNegative,
    UnaryOperatorBiteNot,
    UnaryOperatorSizeOf,
    UnaryOperatorAdressPoint,
    UnaryOperatorAdressValue
}UnaryOperatorType;
@interface ORUnaryExpression: ORExpression
@property (nonatomic,strong)ORExpression * value;
@property (nonatomic,assign)UnaryOperatorType operatorType;
@end

typedef enum {
    BinaryOperatorAdd,
    BinaryOperatorSub,
    BinaryOperatorDiv,
    BinaryOperatorMulti,
    BinaryOperatorMod,
    BinaryOperatorShiftLeft,
    BinaryOperatorShiftRight,
    BinaryOperatorAnd,
    BinaryOperatorOr,
    BinaryOperatorXor,
    BinaryOperatorLT,
    BinaryOperatorGT,
    BinaryOperatorLE,
    BinaryOperatorGE,
    BinaryOperatorNotEqual,
    BinaryOperatorEqual,
    BinaryOperatorLOGIC_AND,
    BinaryOperatorLOGIC_OR
}BinaryOperatorType;

@interface ORBinaryExpression: ORExpression
@property (nonatomic,strong)ORExpression * left;
@property (nonatomic,assign)BinaryOperatorType operatorType;
@property (nonatomic,strong)ORExpression * right;
@end

@interface ORTernaryExpression: ORExpression
@property (nonatomic,strong)ORExpression * expression;
@property (nonatomic,strong)NSMutableArray <ORExpression *>*values;
@end
// MARK: - Statement
@interface ORStatement:NSObject
@property (nonatomic, strong)ORBlockImp *funcImp;
@end

@interface ORIfStatement : ORStatement
@property (nonatomic,strong)ORExpression * condition;
@property (nonatomic,strong, nullable)ORIfStatement * last;
@end

@interface ORWhileStatement : ORStatement
@property (nonatomic,strong)ORExpression * condition;
@end

@interface ORDoWhileStatement : ORStatement
@property (nonatomic,strong)ORExpression * condition;
@end

@interface ORCaseStatement : ORStatement
@property (nonatomic,strong)ORExpression * value;
@end

@interface ORSwitchStatement : ORStatement
@property (nonatomic,strong)ORExpression * value;
@property (nonatomic,strong)NSMutableArray <ORCaseStatement *>*cases;
@end

@interface ORForStatement : ORStatement
@property (nonatomic,strong)NSMutableArray <ORExpression *>*varExpressions;
@property (nonatomic,strong)ORExpression * condition;
@property (nonatomic,strong)NSMutableArray <ORExpression *>* expressions;
@end

@interface ORForInStatement : ORStatement
@property (nonatomic,strong)ORDeclareExpression * expression;
@property (nonatomic,strong)ORExpression * value;
@end

@interface ORReturnStatement : ORStatement
@property (nonatomic,strong)ORExpression * expression;
@end

@interface ORBreakStatement : ORStatement

@end

@interface ORContinueStatement : ORStatement

@end

// MARK: - Class
typedef NS_ENUM(NSUInteger, MFPropertyModifier) {
    MFPropertyModifierMemStrong = 0x00,
    MFPropertyModifierMemWeak = 0x01,
    MFPropertyModifierMemCopy = 0x2,
    MFPropertyModifierMemAssign = 0x03,
    MFPropertyModifierMemMask = 0x0F,
    
    MFPropertyModifierAtomic = 0x00,
    MFPropertyModifierNonatomic =  0x10,
    MFPropertyModifierAtomicMask = 0xF0,
};
@interface ORPropertyDeclare: ORCodeCheck
@property(nonatomic,strong) NSMutableArray *keywords;
@property(nonatomic,strong) ORTypeVarPair * var;
@property(nonatomic,assign) MFPropertyModifier modifier;
@end

@interface ORMethodDeclare: ORCodeCheck
@property(nonatomic,assign) BOOL isClassMethod;
@property(nonatomic,strong) ORTypeVarPair * returnType;
@property(nonatomic,strong) NSMutableArray *methodNames;
@property(nonatomic,strong) NSMutableArray <ORTypeVarPair *>*parameterTypes;
@property(nonatomic,strong) NSMutableArray *parameterNames;
- (NSString *)selectorName;
@end

@interface ORMethodImplementation: NSObject
@property (nonatomic,strong) ORMethodDeclare * declare;
@property (nonatomic,strong) ORBlockImp *imp;
@end

@interface ORClass: NSObject
@property (nonatomic,copy)NSString *className;
@property (nonatomic,copy)NSString *superClassName;
@property (nonatomic,strong)NSMutableArray <NSString *>*protocols;
@property (nonatomic,strong)NSMutableArray <ORPropertyDeclare *>*properties;
@property (nonatomic,strong)NSMutableArray <ORTypeVarPair *>*privateVariables;
@property (nonatomic,strong)NSMutableArray <ORMethodImplementation *>*methods;
+ (instancetype)classWithClassName:(NSString *)className;
@end
NS_ASSUME_NONNULL_END
