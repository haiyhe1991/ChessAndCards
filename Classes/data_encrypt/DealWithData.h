#ifndef __DEAL_WITH_DATA_H__
#define __DEAL_WITH_DATA_H__
class DealWithData 
{
public:
	DealWithData();
	~DealWithData();

	void decodeData();
	void deleteData();
private:
	void copyData(const char* pFileName);
};

#endif