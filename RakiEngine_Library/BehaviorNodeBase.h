#pragma once
#include <vector>
#include <memory>


namespace bTreesys
{
	enum NODE_STATUS
	{
		STATE_WAIT,
		STATE_RUN,
		STATE_SUCCESS,
		STATE_FAILED,
	};

	//ビヘイビア基底クラス
	class BehaviorNodeBase
	{
	public:
		BehaviorNodeBase() {}
		virtual ~BehaviorNodeBase() {}

		virtual void Init() = 0;

		virtual NODE_STATUS Run() = 0;

	};

	//ビヘイビア行動定義クラス
	class BehaviorActionNode : public BehaviorNodeBase
	{
	public:
		BehaviorActionNode() = default;
		~BehaviorActionNode() {};

		virtual void Init() override = 0;

		virtual NODE_STATUS Run() override = 0;
	};

	//ビヘイビア判定クラス
	class BehaviorJudgeNode : public BehaviorNodeBase
	{
	public:
		BehaviorJudgeNode() = default;
		~BehaviorJudgeNode() = default;

		virtual void Init() override = 0;

		virtual NODE_STATUS Run() override  = 0;
	};

	/// <summary>
	/// ビヘイビア実行クラス(AND)
	/// <para>子ノードを末尾から実行する</para>
	/// <para>どれか1つでも失敗したら終了し失敗と返す</para>
	/// <para>末尾まで成功したら成功と返す</para>
	/// <para>簡単にいえば失敗しなければ子ノード全部実行</para>
	/// </summary>
	class BehaviorSequenceNode : public BehaviorNodeBase
	{
	public:
		BehaviorSequenceNode() = default;
		~BehaviorSequenceNode() = default;

		virtual void Init() override;

		virtual NODE_STATUS Run() override;

		void AddChild(const std::shared_ptr<BehaviorNodeBase> &node) {
			childs.push_back(node);
		}
	private:
		int activeArrayNum = 0;
		std::vector<std::shared_ptr<BehaviorNodeBase>> childs;
	};

	/// <summary>
	/// ビヘイビア実行クラス(OR)
	/// <para>子ノードを末尾から実行する</para>
	/// <para>どれか1つでも成功したら終了し成功と返す</para>
	/// <para>末尾まで失敗したら失敗と返す</para>
	/// <para>これは子ノードを1つだけ実行する</para>
	/// </summary>
	class BehaviorSelectorNode : public BehaviorNodeBase
	{
	public:
		BehaviorSelectorNode() = default;
		~BehaviorSelectorNode() = default;

		virtual void Init() override;

		virtual NODE_STATUS Run() override;

		void AddChild(const std::shared_ptr<bTreesys::BehaviorNodeBase> &node) {
			childs.push_back(node);
		}

	private:
		int activeArrayNum = 0;
		std::vector<std::shared_ptr<BehaviorNodeBase>> childs;
	};

	/// <summary>
	/// ビヘイビア実行クラス
	/// <para>子ノードを実行する</para>
	/// <para>ただし成否に関係なく、実行中以外の状態になったらすぐに再実行する</para>
	/// </summary>
	class BehaviorRepeatNode : public BehaviorNodeBase
	{
	public:
		BehaviorRepeatNode() = default;
		~BehaviorRepeatNode() = default;

		virtual void Init();

		virtual NODE_STATUS Run();

		void SetChild(const std::shared_ptr<BehaviorNodeBase> &node) {
			child = node;
		}

	private:
		std::shared_ptr<BehaviorNodeBase>	child;
	};


}