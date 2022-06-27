#pragma once


/*所有handler实例之间传递的消息父类*/
class ZinxMessage{
public:
	ZinxMessage() {}
	virtual ~ZinxMessage() {}
};



//处理这handler抽象类，需要重写处理信息方法和获取下一个处理者方法
//他的核心逻辑为责任链
class AZinxHandler
{
public:
	AZinxHandler();
	virtual ~AZinxHandler();
	
	//负责 调用执行当前环节的处理 
	//并执行下一阶段的处理函数，直到没有下一环节
	void Handle(ZinxMessage* _inputMsg);
	
protected:
	//子类需要重写的类
	// 负责 数据的处理 由继承的子类去具体实现
	virtual ZinxMessage* Internel_Handle(ZinxMessage* _inputMsg) = 0;

	//_next_input 是 internel_handle的返回值
	//获取下一个处理环节函数
	virtual AZinxHandler* GetNextHandler(ZinxMessage* _next_input) = 0;
	
	
	


};

