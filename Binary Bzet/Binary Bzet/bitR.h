#ifndef __BITR_H__
#define __BITR_H__

typedef size_t uint;
using namespace std;

class bitR{

public:
	bitR(){ m_size = 0; }
	~bitR(){}

	void bitR::add(uint start,uint end=0, uint step=0)
	{
		if(end==0)	end = start+1;
		if(end<=start || start<0)	return;
		m_start.push_back(start);
		m_end.push_back(end);
		m_step.push_back(step+1);

		uint temp = end-(((end-start)-1)%(step+1));
		if (temp > m_size)	m_size = temp;
	}

	bool bitR::at(uint index)
	{
		for(uint i=0; i<m_start.size(); i++)
			if((index >= m_start[i]) && (index < m_end[i]) && ((index-m_start[i]) % m_step[i] == 0))
				return true;
		return false;
	}

	uint bitR::size()
	{
		return m_size;
	}
private:
	uint m_size;
	std::vector<uint> m_start;
	std::vector<uint> m_end;
	std::vector<uint> m_step;
};

#endif
