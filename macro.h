//*****************************************************************************
//!	@file	macro.h
//!	@brief	
//!	@note	マクロ定義
//!	@author	T.Suzuki
//*****************************************************************************
#pragma once

// フラグ立てる
#define FLAG_ENABLE( out, x )	( out |= 1 << x )				
// フラグ折る
#define FLAG_DISABLE( out, x )	( out &= ~( 1 << x ) )

//! 値を min, max の間でクランプ
#define CLAMP_VALUE( value, min, max ) ( value < min ) ? ( value = min ) : ( ( value > max ) ? ( value = max ) : ( value ) )
//! 値を min, max の間でループ
#define LOOP_VALUE( value, min, max )  ( value < min ) ? ( value = max ) : ( ( value > max ) ? ( value = min ) : ( value ) )

//! ポインタに対してデリートする
#define	SAFE_DELETE_CLASS( value ) ( value != NULL ) ? ( value->Deactivate(), delete value, value = NULL ) : ( value )
#define SAFE_DELETE( value )       ( value != NULL ) ? ( delete value,   value = NULL ) : ( value )
#define SAFE_DELETE_ARRAY( value ) ( value != NULL ) ? ( delete[] value, value = NULL ) : ( value )

//! ポインタに対してリリースする
#define SAFE_RELEASE( value )      ( value != NULL ) ? ( value->Release(), value = NULL ) : ( value )

//! 配列のサイズを取得
#define ARRAY_SIZE( array ) ( sizeof( array ) / sizeof( array[ 0 ] ) )

//******************************************************************************
//	End of file.
//******************************************************************************