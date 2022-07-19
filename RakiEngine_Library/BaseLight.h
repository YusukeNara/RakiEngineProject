#pragma once

//光源のプログラムは本当に継承で正しいのか確認すること！
//
class BaseLight
{
	//光源初期化
	virtual void Init() = 0;
	//光源データアップロード
	virtual void Upload() = 0;


};

