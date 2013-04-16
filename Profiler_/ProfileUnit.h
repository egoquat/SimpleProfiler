#pragma once

#include <WTypes.h>

#define MAX_PROFILESAMPLENAME	( 64)
#define MAX_PROFILERESULT		( 128)

class CProfileUnit  
{
public:
	CProfileUnit();
	CProfileUnit( unsigned int uikey, char *szProfileUnitName );
	~CProfileUnit();
	void Clear( void);

public:
	unsigned int key;

protected:
	char m_lpszName[MAX_PROFILESAMPLENAME];

	/// @name ��� ����
	LONGLONG m_llStartTick;			///< �ѹ� üũ�ϴ� ������ ����
	LONGLONG m_llSumTime;			///< �� ������ ���� �� ��
	LONGLONG m_llSumTime_GrandTotal;	///< ��������� ����ġ �� ��

	/// @name �����ϴ� %
	float m_fLatestPercentage;
	float m_fMaxPercentage;
	float m_fMinPercentage;
	float m_fTotalPercentage;

	float m_fAve_ms;			// �� �����ӵ��� ȣ��ð����
	float m_fAve_ms_GrandTotal;

	unsigned long m_ulAve_tick;
	unsigned long m_ulAve_tick_GrandTotal;

	/// @name ȣ�� ȸ��
	unsigned long m_ulCountInFrame;		///< �� ������ ���� ȣ��� ȸ��
	unsigned long m_ulCountInFrame_GrandTotal;	// ��������� �� ȣ��� ȸ��

	unsigned long m_ulLatestCount;		///< �ֱ��� ȣ�� ȸ��

private:
	void Init( unsigned int uikey, char *lpszName);

public:
	void BeginUnit_( unsigned int uiKey, char *lpszName, LONGLONG llTick );
	void EndUnit_( LONGLONG llTick);	

	BOOL CheckName( char *lpszName);
	char* GetName( void)	{ return ( m_lpszName); }

	void GenerateResult( LONGLONG llTotalTime, LONGLONG llGrandTotalTime, LONGLONG llTickPerMS = 1 );
	void GetResult_Percentage( float *pfLatestPercentage, float *pfMaxPercentage, float *pfMinPercentage, float *pfTotalPercentage);
	void GetResult_StatisticAve( float *pfAve_ms, 
							unsigned long *pulAve_tick, 
							float *pfAve_ms_GrandTotal,
							unsigned long *pulAve_tick_GrandTotal );

	void GetResultText( char *lpszText );
	void GetResultText_Average( char *lpszText );
	void GetResultText_Average_Total( char *lpszText );
};
