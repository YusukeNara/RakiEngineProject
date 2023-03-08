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

	//�r�w�C�r�A���N���X
	class BehaviorNodeBase
	{
	public:
		BehaviorNodeBase() {}
		virtual ~BehaviorNodeBase() {}

		virtual void Init() = 0;

		virtual NODE_STATUS Run() = 0;

	};

	//�r�w�C�r�A�s����`�N���X
	class BehaviorActionNode : public BehaviorNodeBase
	{
	public:
		BehaviorActionNode() = default;
		~BehaviorActionNode() {};

		virtual void Init() override = 0;

		virtual NODE_STATUS Run() override = 0;
	};

	//�r�w�C�r�A����N���X
	class BehaviorJudgeNode : public BehaviorNodeBase
	{
	public:
		BehaviorJudgeNode() = default;
		~BehaviorJudgeNode() = default;

		virtual void Init() override = 0;

		virtual NODE_STATUS Run() override  = 0;
	};

	/// <summary>
	/// �r�w�C�r�A���s�N���X(AND)
	/// <para>�q�m�[�h�𖖔�������s����</para>
	/// <para>�ǂꂩ1�ł����s������I�������s�ƕԂ�</para>
	/// <para>�����܂Ő��������琬���ƕԂ�</para>
	/// <para>�ȒP�ɂ����Ύ��s���Ȃ���Ύq�m�[�h�S�����s</para>
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
	/// �r�w�C�r�A���s�N���X(OR)
	/// <para>�q�m�[�h�𖖔�������s����</para>
	/// <para>�ǂꂩ1�ł�����������I���������ƕԂ�</para>
	/// <para>�����܂Ŏ��s�����玸�s�ƕԂ�</para>
	/// <para>����͎q�m�[�h��1�������s����</para>
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
	/// �r�w�C�r�A���s�N���X
	/// <para>�q�m�[�h�����s����</para>
	/// <para>���������ۂɊ֌W�Ȃ��A���s���ȊO�̏�ԂɂȂ����炷���ɍĎ��s����</para>
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