//*****************************************************************************
//!	@file	macro.h
//!	@brief	
//!	@note	�}�N����`
//!	@author	T.Suzuki
//*****************************************************************************
#pragma once

// �t���O���Ă�
#define FLAG_ENABLE( out, x )	( out |= 1 << x )				
// �t���O�܂�
#define FLAG_DISABLE( out, x )	( out &= ~( 1 << x ) )

//! �l�� min, max �̊ԂŃN�����v
#define CLAMP_VALUE( value, min, max ) ( value < min ) ? ( value = min ) : ( ( value > max ) ? ( value = max ) : ( value ) )
//! �l�� min, max �̊ԂŃ��[�v
#define LOOP_VALUE( value, min, max )  ( value < min ) ? ( value = max ) : ( ( value > max ) ? ( value = min ) : ( value ) )

//! �|�C���^�ɑ΂��ăf���[�g����
#define	SAFE_DELETE_CLASS( value ) ( value != NULL ) ? ( value->Deactivate(), delete value, value = NULL ) : ( value )
#define SAFE_DELETE( value )       ( value != NULL ) ? ( delete value,   value = NULL ) : ( value )
#define SAFE_DELETE_ARRAY( value ) ( value != NULL ) ? ( delete[] value, value = NULL ) : ( value )

//! �|�C���^�ɑ΂��ă����[�X����
#define SAFE_RELEASE( value )      ( value != NULL ) ? ( value->Release(), value = NULL ) : ( value )

//! �z��̃T�C�Y���擾
#define ARRAY_SIZE( array ) ( sizeof( array ) / sizeof( array[ 0 ] ) )

//******************************************************************************
//	End of file.
//******************************************************************************