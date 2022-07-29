//
// Module Name:	TStatechart.hpp
//
// Description:	Template class for statechart engine.
//
// Copyright (c) 1994-2005 Grant D. Schultz.	All rights reserved.
//
//		
//
//		PN, 5. Nov 2005:
//
//      a description of this Framework you can find under
//
//		www.codeproject.com/samples/Statechart.asp
//
//		you need not understand this code, actually implementing a fsm is much simple
//
//     PN. changed classes into structs and removed Tamplate in order to let run on 8051 cpu.
//     unint23 -> uint8 etc.
//
//
#if !defined(__TSTATECHART_HPP__)
#define __TSTATECHART_HPP__

//#include <vector>
//#include <algorithm>
//#pragma warning(pop)
//#include "stdtype.h"
#include<stdint.h>
#include<stdbool.h>


typedef  uint8_t uStInt;
typedef  int8_t  stInt;

#define maxDepth 12


typedef struct
{
	stInt	m_vi32Ancestry[maxDepth];
	stInt  m_vi32AncestrySize;

	stInt				m_stIntHistoryReturnState;
} xInternalState;


typedef void(*t_fvoid)(void);

typedef uStInt (*t_uStIntVoid) (void);

typedef struct
{
	stInt	m_i32StateName;
	stInt	m_i32ParentStateName;
	stInt	m_i32DefaultChildToEnter;
	stInt   m_keepHistory;

	t_uStIntVoid m_pfu32EventChecker;
	t_fvoid  m_pfDefaultStateEntry; 
	t_fvoid  m_pfEnteringState;
	t_fvoid m_pfLeavingState;
} xStateType;



typedef struct 
{


		uStInt u32LastStateExited;
		uStInt		m_u32NumStates;

		xStateType	*m_pxaUserStateDefns;

		uStInt				m_u32CurrentState;
		xInternalState		*m_xaStateData;


		uStInt       m_vu32exitingStates[maxDepth];

		stInt				m_i32ExitingStatesIndex;
		
} TStatechart;




#define BEGIN_EVENT_HANDLER( engine_name, state_and_flags)	\
{																		\
	uStInt	u32LastStateExited_##engine_name;							\
	uStInt	u32State_and_flags_##engine_name = state_and_flags;			\
		beginEventAction( engine_name,											\
		(state_and_flags),& u32LastStateExited_##engine_name)



#define END_EVENT_HANDLER( engine_name)						\
		endEventAction( engine_name, u32State_and_flags_##engine_name,			\
			u32LastStateExited_##engine_name);							\
}



void createTStatechart(TStatechart* t, xStateType* const xaStates,
	const uStInt u32NS, const uStInt u32InitialState);



void destructTStatechart(TStatechart* t);

bool ProcessEvent(TStatechart* t);

// bool inState(TStatechart* t, const stInt u32State);



void beginEventAction(TStatechart* t, uStInt u32DestStateAndFlags,
	uStInt* u32LastStateExited);

void endEventAction(TStatechart* t, uStInt u32DestStateAndFlags,
	uStInt u32LastStateExited);

void resetHistoryReturns(TStatechart* t);


#endif	
