#pragma once
#include	<dinput.h>
#include	<wrl/client.h>

#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dxguid.lib")

using Microsoft::WRL::ComPtr;

class CDirectInput{
private:
	ComPtr<IDirectInput8>	m_dinput;
	ComPtr<IDirectInputDevice8>	m_dikeyboard;
	ComPtr<IDirectInputDevice8>	m_dimouse;

	char					m_keybuffer[256];		// キーボードバッファ
	char					m_oldkeybuffer[256];	// 前回の入力キーボードバッファ
	DIMOUSESTATE2			m_MouseState;			// マウスの状態
	DIMOUSESTATE2			m_MouseStateTrigger;	// マウスの状態
	POINT					m_MousePoint;			// マウス座標
	int						m_width;			// マウスのＸ座標最大
	int						m_height;			// マウスのＹ座標最大
	HWND					m_hwnd;
	CDirectInput() :m_dinput(nullptr), m_dikeyboard(nullptr), m_dimouse(nullptr) {
	}
public:

	CDirectInput(const CDirectInput&) = delete;
	CDirectInput& operator=(const CDirectInput&) = delete;
	CDirectInput(CDirectInput&&) = delete;
	CDirectInput& operator=(CDirectInput&&) = delete;

	static CDirectInput& GetInstance(){
		static CDirectInput Instance;
		return Instance;
	}

	~CDirectInput(){
		Exit();
	}

	//----------------------------------
	// DirectInput 初期処理
	//
	//		P1 : インスタンス値
	//		P2 : ウインドウハンドル値
	//
	//	戻り値
	//		true : 初期化成功
	//		false : 初期化失敗
	//----------------------------------
	bool Init(HINSTANCE hInst,HWND hwnd,int width,int height){
		HRESULT	hr;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_dinput, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// キーボードデバイス生成
		m_dinput->CreateDevice(GUID_SysKeyboard, &m_dikeyboard, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dikeyboard->SetDataFormat(&c_dfDIKeyboard);
		if(FAILED(hr)) {
			return false;
		}
		
		// 協調レベルの設定
		hr = m_dikeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		// マウスデバイス生成
		m_dinput->CreateDevice(GUID_SysMouse, &m_dimouse, NULL);
		if(FAILED(hr)) {
			return false;
		}

		// データフォーマットの設定
		hr = m_dimouse->SetDataFormat(&c_dfDIMouse2);
		if(FAILED(hr)) {
			return false;
		}
		
		// 協調レベルの設定
		hr = m_dimouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		if(FAILED(hr)) {
			return false;
		}

		// デバイスの設定
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL;							// 相対値モード
		m_dimouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);		// 軸モードの設定


		DIPROPRANGE diprg;
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwObj = DIJOFS_X;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = width - 1;

		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);		// Ｘ方向の範囲を指定
		diprg.diph.dwObj = DIJOFS_Y;
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = 0;
		diprg.lMax = height - 1;
		m_dimouse->SetProperty(DIPROP_RANGE, &diprg.diph);	// Ｙ方向の範囲を指定

		m_hwnd = hwnd;

		m_height = height;
		m_width  = width;

		return true;
	}

	//----------------------------------
	// マウス状態取得処理
	//----------------------------------
	void GetMouseState(){
		HRESULT	hr;

		DIMOUSESTATE2		mouseStateOld = m_MouseState;

		GetCursorPos(&m_MousePoint);
		ScreenToClient(m_hwnd, &m_MousePoint);

		// デバイスの認識
		hr = m_dimouse->Acquire();

		hr = m_dimouse->GetDeviceState(sizeof(m_MouseState),&m_MouseState);
		if (SUCCEEDED(hr)){
			for (int cnt = 0; cnt < 8; cnt++)
			{
				m_MouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ m_MouseState.rgbButtons[cnt]) & m_MouseState.rgbButtons[cnt]);
			}
		}
		else{
			if(hr == DIERR_INPUTLOST){
				// デバイスの認識
				hr = m_dimouse->Acquire();
			}
		}	
	}

	//----------------------------------
	// マウスＸ座標取得処理
	//----------------------------------
	int GetMousePosX() const{
		return m_MousePoint.x;
	}

	//----------------------------------
	// マウスＹ座標取得処理
	//----------------------------------
	int GetMousePosY() const{
		return m_MousePoint.y;
	}

	//----------------------------------
	// マウス左ボタンチェック
	//----------------------------------
	bool GetMouseLButtonCheck() const{
		if(m_MouseState.rgbButtons[0] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス右ボタンチェック
	//----------------------------------
	bool GetMouseRButtonCheck() const{
		if(m_MouseState.rgbButtons[1] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス中央ボタンチェック
	//----------------------------------
	bool GetMouseCButtonCheck() const{
		if(m_MouseState.rgbButtons[2] & 0x80){
			return true;
		}else{
			return false;	
		}
	}

	//----------------------------------
	// マウス左ボタンチェック(トリガー)
	//----------------------------------
	bool GetMouseLButtonTrigger() const {
		if (m_MouseStateTrigger.rgbButtons[0] & 0x80) {
			return true;
		}
		else {
			return false;
		}
	}

	//----------------------------------
	// マウス右ボタンチェック(トリガー)
	//----------------------------------
	bool GetMouseRButtonTrigger() const {
		if (m_MouseStateTrigger.rgbButtons[1] & 0x80) {
			return true;
		}
		else {
			return false;
		}
	}

	//----------------------------------
	// マウス中央ボタンチェック(トリガー)
	//----------------------------------
	bool GetMouseCButtonTrigger() const {
		if (m_MouseStateTrigger.rgbButtons[2] & 0x80) {
			return true;
		}
		else {
			return false;
		}
	}

	//----------------------------------
	// キーボードバッファ取得処理
	//----------------------------------
	void GetKeyBuffer(){
		HRESULT	hr;
		// デバイスの認識
		hr = m_dikeyboard->Acquire();
		// 前回の状態を保存
		memcpy(&m_oldkeybuffer,m_keybuffer,sizeof(m_keybuffer));
		hr = m_dikeyboard->GetDeviceState(sizeof(m_keybuffer),(LPVOID)&m_keybuffer);
		if(hr == DIERR_INPUTLOST){
			// デバイスの認識
			hr = m_dikeyboard->Acquire();
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool CheckKeyBuffer(int keyno){
		if(m_keybuffer[keyno] & 0x80){
			return true;
		}
		else{
			return false;
		}
	}

	//----------------------------------
	// キーが押されているかどうかをチェックする
	//		p1 :　チェックしたいキー番号(トリガー)
	//	戻り値
	//		true : 指定されたキーが押されている
	//----------------------------------
	bool CheckKeyBufferTrigger(int keyno){
		if(((m_keybuffer[keyno]^m_oldkeybuffer[keyno]) & m_keybuffer[keyno]) & 0x80){
			return true;
		}
		else{
			return false;
		}
	}

	//----------------------------------
	// DirectInput 終了処理
	//----------------------------------
	void Exit(){
	}	
};
