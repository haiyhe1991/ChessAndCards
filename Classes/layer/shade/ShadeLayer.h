#ifndef __SHADE_LAYER_H__
#define __SHADE_LAYER_H__
#include "chessAndCards.h"
//һ���Ե��ڵ���
class ShadeLayer :public Layer
{
public:
	virtual bool init();
	CREATE_FUNC(ShadeLayer);
	~ShadeLayer();

	//virtual void onEnter();							//��ʼ������

	//virtual list<LoadItem> getResourceList();			//ȡ����Դ�����б�

	//virtual void releaseResource();					//�ͷ���Դ
//
//private:
//	MainLayout*				mainLayout;		//�����ı�������		

};

//���س���
class RecvingLayer : public ShadeLayer
{
public:
	virtual bool init();
	CREATE_FUNC(RecvingLayer);

	~RecvingLayer();										//��������

private:
	Node*					layout;					//���������Դ
};



#endif