/**
 * @file    TestSpeedCalculator.h
 * @brief   This file is test file for SpeedCalculator class.
 */
#pragma once

#include "product.h"
#include "gtest/gtest.h"
#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

//  ./hrp2/arch/gcc/tool_stddef.h からコピー
typedef signed char			int8_t;		/* 符号付き8ビット整数 */
typedef unsigned char		uint8_t;	/* 符号無し8ビット整数 */

typedef signed short		int16_t;	/* 符号付き16ビット整数 */
typedef unsigned short		uint16_t;	/* 符号無し16ビット整数 */

typedef signed int			int32_t;	/* 符号付き32ビット整数 */
typedef unsigned int		uint32_t;	/* 符号無し32ビット整数 */

//typedef signed long long	int64_t;	/* 符号付き64ビット整数 */
typedef unsigned long long	uint64_t;	/* 符号無し64ビット整数 */

typedef int8_t				int_least8_t;	/* 8ビット以上の符号付き整数 */
typedef uint8_t				uint_least8_t;	/* 8ビット以上の符号無し整数 */

typedef long				intptr_t;	/* ポインタを格納できる符号付き整数 */
typedef unsigned long		uintptr_t;	/* ポインタを格納できる符号無し整数 */

//  ./hrp2/include/t_stddef.h よりコピーした
/*
 *  C言語/EC++でthrow()に対処するための定義
 */
#if !defined(__cplusplus) || defined(__embedded_cplusplus)
#define throw()
#endif

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ターゲット依存のデータ型のマクロのデフォルト定義
 */
#ifndef TOPPERS_bool
#define TOPPERS_bool	int				/* ブール型 */
#endif /* TOPPERS_bool */

#ifndef TOPPERS_size
#define TOPPERS_size	uintptr_t		/* サイズ型 */
#endif /* TOPPERS_size */

#ifndef TOPPERS_fp
struct TOPPERS_dummy_t { int TOPPERS_dummy_field; };
typedef	void			(*TOPPERS_fp_t)(struct TOPPERS_dummy_t);
#define TOPPERS_fp		TOPPERS_fp_t	/* プログラムの起動番地 */
#endif /* TOPPERS_fp */

/*
 *  共通データ型
 */
typedef TOPPERS_bool	bool_t;		/* 真偽値 */

typedef signed int		int_t;		/* 自然なサイズの符号付き整数 */
typedef unsigned int	uint_t;		/* 自然なサイズの符号無し整数 */

typedef signed long		long_t;		/* 長めのサイズの符号付き整数 */
typedef unsigned long	ulong_t;	/* 長めのサイズの符号無し整数 */

typedef int_t			FN;			/* 機能コード */
typedef	int_t			ER;			/* エラーコード */
typedef	int_t			ID;			/* オブジェクトのID番号 */
typedef	uint_t			ATR;		/* オブジェクトの属性 */
typedef	uint_t			STAT;		/* オブジェクトの状態 */
typedef	uint_t			MODE;		/* サービスコールの動作モード */
typedef	int_t			PRI;		/* 優先度 */
typedef	TOPPERS_size	SIZE;		/* メモリ領域のサイズ */

typedef	int_t			TMO;		/* タイムアウト指定 */
typedef	uint_t			RELTIM;		/* 相対時間 */
typedef	ulong_t			SYSTIM;		/* システム時刻 */
typedef	ulong_t			SYSUTM;		/* 性能評価用システム時刻 */

typedef	TOPPERS_fp		FP;			/* プログラムの起動番地 */

typedef	int_t			ER_BOOL;	/* エラーコードまたは真偽値 */
typedef	int_t			ER_ID;		/* エラーコードまたはID番号 */
typedef	int_t			ER_UINT;	/* エラーコードまたは符号無し整数 */

typedef uintptr_t		MB_T;		/* 管理領域を確保するためのデータ型 */

typedef	uint32_t		ACPTN;		/* アクセス許可パターン */
typedef	struct acvct {				/* アクセス許可ベクタ */
	ACPTN		acptn1;				/* 通常操作1のアクセス許可パターン */
	ACPTN		acptn2;				/* 通常操作2のアクセス許可パターン */
	ACPTN		acptn3;				/* 管理操作のアクセス許可パターン */
	ACPTN		acptn4;				/* 参照操作のアクセス許可パターン */
} ACVCT;

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  整数定数を作るマクロ
 */
#ifndef UINT_C
#define UINT_C(val)		(val ## U)		/* uint_t型の定数を作るマクロ */
#endif /* UINT_C */

#ifndef ULONG_C
#define ULONG_C(val)	(val ## UL)		/* ulong_t型の定数を作るマクロ */
#endif /* ULONG_C */

/*
 *  一般定数
 */
#ifndef NULL
#define NULL		0			/* 無効ポインタ */
#endif /* NULL */

#ifndef true
#define true		1			/* 真 */
#endif /* true */

#ifndef false
#define false		0			/* 偽 */
#endif /* false */

#define E_OK		0			/* 正常終了 */

/*
 *  メインエラーコード
 */
#define E_SYS		(-5)		/* システムエラー */
#define E_NOSPT		(-9)		/* 未サポート機能 */
#define E_RSFN		(-10)		/* 予約機能コード */
#define E_RSATR		(-11)		/* 予約属性 */
#define E_PAR		(-17)		/* パラメータエラー */
#define E_ID		(-18)		/* 不正ID番号 */
#define E_CTX		(-25)		/* コンテキストエラー */
#define E_MACV		(-26)		/* メモリアクセス違反 */
#define E_OACV		(-27)		/* オブジェクトアクセス違反 */
#define E_ILUSE		(-28)		/* サービスコール不正使用 */
#define E_NOMEM		(-33)		/* メモリ不足 */
#define E_NOID		(-34)		/* ID番号不足 */
#define E_NORES		(-35)		/* 資源不足 */
#define E_OBJ		(-41)		/* オブジェクト状態エラー */
#define E_NOEXS		(-42)		/* オブジェクト未生成 */
#define E_QOVR		(-43)		/* キューイングオーバーフロー */
#define E_RLWAI		(-49)		/* 待ち状態の強制解除 */
#define E_TMOUT		(-50)		/* ポーリング失敗またはタイムアウト */
#define E_DLT		(-51)		/* 待ちオブジェクトの削除 */
#define E_CLS		(-52)		/* 待ちオブジェクトの状態変化 */
#define E_WBLK		(-57)		/* ノンブロッキング受付け */
#define E_BOVR		(-58)		/* バッファオーバーフロー */

/*
 *  オブジェクト属性
 */
#define TA_NULL		UINT_C(0)	/* オブジェクト属性を指定しない */

/*
 *  タイムアウト指定
 */
#define TMO_POL		0			/* ポーリング */
#define TMO_FEVR	(-1)		/* 永久待ち */
#define TMO_NBLK	(-2)		/* ノンブロッキング */

/*
 *  アクセス許可パターン
 */
#define TACP_KERNEL		(0U)	/* カーネルドメインだけにアクセスを許可 */
#define TACP_SHARED		(~0U)	/* すべてのドメインからアクセスを許可 */

/*
 *  型に関する情報を取り出すためのマクロ
 *
 *  以下のALIGN_TYPEの定義は，アラインメントの単位が2の巾乗であることを
 *  仮定している．
 */
#ifndef offsetof
#define	offsetof(structure, field)	((uintptr_t) &(((structure *) 0)->field))
#endif /* offsetof */

#ifndef alignof
#define alignof(type)	offsetof(struct { char field1; type field2; }, field2)
#endif /* alignof */

#ifndef ALIGN_TYPE				/* アラインメントのチェック */
#define ALIGN_TYPE(addr, type) \
						((((uintptr_t)(addr)) & (alignof(type) - 1)) == 0U)
#endif /* ALIGN_TYPE */

/*
 *  assertマクロ
 */
#undef assert
#ifndef NDEBUG
#define assert(exp)		((void)((exp) ? 0 : (TOPPERS_assert_fail(#exp, \
							__FILE__, __LINE__), TOPPERS_assert_abort(), 0)))
#else /* NDEBUG */
#define assert(exp)		((void) 0)
#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif

/*
 *  エラーコード生成・分解マクロ
 *
 *  以下のマクロ定義は，符号付き整数が2の補数で表現されており，右シフト
 *  演算子（>>）が算術シフトされることを仮定している．そうでない開発環
 *  境では，target_stddef.hでこれらの定義を上書きしなければならない．
 *
 *  MERCDマクロの定義は，メインエラーコードが負であることを仮定している．
 *  そうでない場合，2つの定義が一致しなくなる．
 */
#ifndef ERCD
#define ERCD(mercd, sercd) \
				((ER)((((uint_t)(sercd)) << 8) | (((uint_t)(mercd)) & 0xffU)))
#endif /* ERCD */

#ifndef MERCD
#ifdef INT8_MAX
#define MERCD(ercd)		((ER)((int8_t)(ercd)))
#else /* INT8_MAX */
#define MERCD(ercd)		((ER)(((uint_t) ercd) | ~0xffU))
#endif /* INT8_MAX */
#endif /* MERCD */

#ifndef SERCD
#define SERCD(ercd)		((ER)((ercd) >> 8))
#endif /* SERCD */

/*
 *  アクセス許可パターン生成マクロ
 */
#define TACP(domid)		(1U << ((domid) - 1))	/* domidだけにアクセスを許可 */

/*
 *  相対時間（RELTIM）に指定できる最大値
 */
#if UINT_MAX < LONG_MAX
#define TMAX_RELTIM		((RELTIM) UINT_MAX)
#else /* UINT_MAX < LONG_MAX */
#define TMAX_RELTIM		((RELTIM) LONG_MAX)
#endif /* UINT_MAX < LONG_MAX */

//  ./hrp2/sdk/common/ev3api/src/ev3api_sensor.h からコピー

/**
 * \~English
 * [TODO: sync with jp version]
 * \brief Enumeration type for supported sensor ports
 *
 * \~Japanese
 * \brief センサポートを表す番号
 */
typedef enum {
    EV3_PORT_1 = 0,               //!< \~English Port 1                             \~Japanese ポート1
    EV3_PORT_2 = 1,           //!< \~English Port 2                 \~Japanese ポート2
    EV3_PORT_3 = 2,               //!< \~English Port 3                         \~Japanese ポート3
    EV3_PORT_4 = 3,       //!< \~English Port 4                             \~Japanese ポート4
    TNUM_SENSOR_PORT = 4, //!< \~English Number of sensor ports \~Japanese センサポートの数
} sensor_port_t;

/**
 * \~English
 * \brief Enumeration type for supported sensor types
 *
 * \~Japanese
 * \brief サポートするセンサタイプ
 */
typedef enum {
    NONE_SENSOR = 0,     //!< \~English Not connected                                     \~Japanese センサ未接続
    ULTRASONIC_SENSOR,   //!< \~English Ultrasonic sensor                                 \~Japanese 超音波センサ
    GYRO_SENSOR,             //!< \~English Gyroscope sensor                              \~Japanese ジャイロセンサ
    TOUCH_SENSOR,            //!< \~English Touch sensor                                          \~Japanese タッチセンサ
    COLOR_SENSOR,            //!< \~English Color sensor                                          \~Japanese カラーセンサ
        HT_NXT_ACCEL_SENSOR, //!< \~English HiTechnic NXT acceleration sensor \~Japanese 加速度センサ（HiTechnic社製）
        NXT_TEMP_SENSOR,     //!< \~English NXT temperature sensor            \~Japanese NXT温度センサ
    TNUM_SENSOR_TYPE     //!< \~English Number of sensor types                    \~Japanese センサタイプの数
} sensor_type_t;

/**
 * \~English
 * \brief Enumeration type for colors that can be detected by color sensor
 *
 * \~Japanese
 * \brief カラーセンサで識別できるカラーの番号
 */
typedef enum {
    COLOR_NONE   = 0, //!< \~English None                         \~Japanese 無色
    COLOR_BLACK  = 1, //!< \~English Black                        \~Japanese 黒
    COLOR_BLUE   = 2, //!< \~English Blue                         \~Japanese 青
    COLOR_GREEN  = 3, //!< \~English Green                \~Japanese 緑
    COLOR_YELLOW = 4, //!< \~English Yellow               \~Japanese 黄
    COLOR_RED    = 5, //!< \~English Red                          \~Japanese 赤
    COLOR_WHITE  = 6, //!< \~English White                    \~Japanese 白
    COLOR_BROWN  = 7, //!< \~English Brown                        \~Japanese 茶
    TNUM_COLOR            //!< \~English Number of colors \~Japanese 識別できるカラーの数
} colorid_t;

/**
 * \~English
 * \brief Structure for an RGB raw value
 *
 * \~Japanese
 * \brief RGB Raw値を格納する構造体
 */
typedef struct {
    uint16_t r; //!< \~English Red value   \~Japanese 赤
    uint16_t g; //!< \~English Green value \~Japanese 緑
    uint16_t b; //!< \~English Blue value  \~Japanese 青
} rgb_raw_t;

// ../hrp2/sdk/common/ev3api/src/ev3api_motor.h からコピー
/**
 * \~English
 * [TODO: sync with jp version]
 * \defgroup ev3motor Motor
 * \brief    Definitions of API for controlling motors.
 *
 * \~Japanese
 * \defgroup ev3motor サーボモータ
 * \brief    モータ制御に関するAPI．
 *
 * @{
 */

#pragma once

/**
 * \~English
 * [TODO: sync with jp version]
 * \brief ID for supported motor ports
 *
 * \~Japanese
 * \brief モータポートを表す番号
 */
typedef enum {
    EV3_PORT_A = 0,     //!< \~English Port A                            \~Japanese ポートA
    EV3_PORT_B = 1,             //!< \~English Port B                            \~Japanese ポートB
    EV3_PORT_C = 2,             //!< \~English Port C                            \~Japanese ポートC
    EV3_PORT_D = 3,     //!< \~English Port D                        \~Japanese ポートD
    TNUM_MOTOR_PORT = 4 //!< \~English Number of motor ports \~Japanese モータポートの数
} motor_port_t;

/**
 * \~English
 * [TODO: sync with jp version]
 * \brief Enumeration type for supported motor types
 *
 * \~Japanese
 * \brief サポートするモータタイプ
 */
typedef enum {
    NONE_MOTOR = 0,        //!< \~English Not connected         \~Japanese モータ未接続
    MEDIUM_MOTOR,          //!< \~English Medium servo motor    \~Japanese サーボモータM
    LARGE_MOTOR,           //!< \~English Large servo motor     \~Japanese サーボモータL
    UNREGULATED_MOTOR, //!< \~English Unregulated motor     \~Japanese 未調整モータ
    TNUM_MOTOR_TYPE    //!< \~English Number of motor types \~Japanese モータタイプの数
} motor_type_t;
