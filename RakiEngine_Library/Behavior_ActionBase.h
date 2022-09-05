#pragma once

#include <string>

//アクション基底クラス

//実際どんな動きをするかを派生クラスで定義するべきだが
//それをどう実現したらいいかわからん...
class BehaviorActionBase
{
public:
	BehaviorActionBase() = default;
	~BehaviorActionBase() = default;

	//実行クラスの状態
	enum ACTION_STATE
	{
		STANDBY,
		RUNNING,	//実行中
		SUCCESS,	//成功
		FAILED,		//失敗
	};

	ACTION_STATE actionState = STANDBY;

	//実行関数
	virtual ACTION_STATE Run() = 0;

	virtual void Init() = 0;

	std::string actScriptName;
};