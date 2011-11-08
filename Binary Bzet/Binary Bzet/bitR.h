#ifndef __BITR_H__
#define __BITR_H__

typedef unsigned int u32;

class bitR
{
public:
	bitR(){ m_size = 0; }
	~bitR(){}
	void bitR::add(u32 start,u32 end=0, u32 step=0)
	{
		if(end==0)	end = start+1;
		if(end > m_size)	m_size = end;
		if(end<=start || start<0)	exit(1);
		m_start.push_back(start);
		m_end.push_back(end);
		m_step.push_back(step+1);
	}
	bool bitR::at(u32 index)
	{
		for(u32 i=0; i<m_start.size(); i++)
			if((index >= m_start[i]) && (index < m_end[i]) && ((index-m_start[i]) % m_step[i] == 0))
				return true;
		return false;
	}
	u32 bitR::size()
	{
		return m_size;
	}
private:
	u32 m_size;
	std::vector<u32> m_start;
	std::vector<u32> m_end;
	std::vector<u32> m_step;
};
#endif
