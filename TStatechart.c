
#include <string.h>
#include <stdlib.h>
#include "TStatechart.h"

const uStInt	uStIntNoMatch = 0x80,
uStIntHandlingDone = 0x40;



/*
void verifyStateNames(TStatechart* t)
{
	for (uStInt u=0; u!=t->m_u32NumStates; u++)
	{
		assert(t->m_pxaUserStateDefns[u].m_i32StateName == static_cast<stInt>(u));

		assert(t->m_pxaUserStateDefns[u].m_i32StateName !=
			t->m_pxaUserStateDefns[u].m_i32ParentStateName);

		assert(t->m_pxaUserStateDefns[u].m_i32ParentStateName <
			(stInt)t->m_u32NumStates);

		assert(t->m_pxaUserStateDefns[u].m_i32DefaultChildToEnter <
			(stInt)t->m_u32NumStates);

		assert(t->m_pxaUserStateDefns[u].m_i32StateName !=
			t->m_pxaUserStateDefns[u].m_i32DefaultChildToEnter);
	}
}
*/



void generateAncestries(TStatechart* t)
{
	uStInt		u32StateBeingProcessed,
				u32CurrentAncestor;
	int i1;

	for (u32StateBeingProcessed=0;
		u32StateBeingProcessed < t->m_u32NumStates;
			u32StateBeingProcessed++)
		{
			t->m_xaStateData[u32StateBeingProcessed].m_vi32AncestrySize = 0;
			for (i1= maxDepth-1; i1> 0; --i1)
			{
				t->m_xaStateData[u32StateBeingProcessed].m_vi32Ancestry[i1] = 
					t->m_xaStateData[u32StateBeingProcessed].m_vi32Ancestry[i1-1];
			}
			t->m_xaStateData[u32StateBeingProcessed].m_vi32Ancestry[0] =
					u32StateBeingProcessed;
			++ t->m_xaStateData[u32StateBeingProcessed].m_vi32AncestrySize;
			u32CurrentAncestor = u32StateBeingProcessed;
			while (-1 != t->m_pxaUserStateDefns[u32CurrentAncestor].
				m_i32ParentStateName)
			{
				for (i1= maxDepth-1; i1> 0; --i1)
				{
					t->m_xaStateData[u32StateBeingProcessed].m_vi32Ancestry[i1] = 
						t->m_xaStateData[u32StateBeingProcessed].m_vi32Ancestry[i1-1];
				}
				t->m_xaStateData[u32StateBeingProcessed].m_vi32Ancestry[0] =
					t->m_pxaUserStateDefns[u32CurrentAncestor].m_i32ParentStateName;
				++ t->m_xaStateData[u32StateBeingProcessed].m_vi32AncestrySize;

				u32CurrentAncestor = t->m_pxaUserStateDefns[u32CurrentAncestor].
					m_i32ParentStateName;
		}
	}
}



void resetHistoryReturns(TStatechart* t)
{
	uStInt		u32StateBeingProcessed;

	for (u32StateBeingProcessed=0;
		u32StateBeingProcessed<t->m_u32NumStates;
			u32StateBeingProcessed++)
	{
		t->m_xaStateData[u32StateBeingProcessed].m_stIntHistoryReturnState = -1;
	}
	
}



  
void setInitialState(TStatechart* t, uStInt u32InitialState)
{
	stInt	i32ChildToEnter;
	if (0 != t->m_pxaUserStateDefns[u32InitialState].m_pfDefaultStateEntry)
	{
		t->m_pxaUserStateDefns[u32InitialState].m_pfDefaultStateEntry();
//		xStateType xs;
//		xs = t->m_pxaUserStateDefns[u32InitialState];
//		xs.m_pfDefaultStateEntry();
	}

	if (0 != t->m_pxaUserStateDefns[u32InitialState].m_pfEnteringState)
	{
		t->m_pxaUserStateDefns[u32InitialState].m_pfEnteringState();
//		xStateType xs;
//		xs = t->m_pxaUserStateDefns[u32InitialState];
//		xs.m_pfEnteringState();
	}	

	t->m_u32CurrentState = u32InitialState;

	i32ChildToEnter = t->m_pxaUserStateDefns[u32InitialState].
		m_i32DefaultChildToEnter;		

	while (-1 != i32ChildToEnter)
	{
//		xStateType xs;
		if (0 != t->m_pxaUserStateDefns[i32ChildToEnter].
			m_pfDefaultStateEntry)
		{
			t->m_pxaUserStateDefns[i32ChildToEnter].m_pfDefaultStateEntry();
//			xs = t->m_pxaUserStateDefns[i32ChildToEnter];
//			xs.m_pfDefaultStateEntry();
		} 
		if (0 != t->m_pxaUserStateDefns[i32ChildToEnter].m_pfEnteringState)
		{
			t->m_pxaUserStateDefns[i32ChildToEnter].m_pfEnteringState();
//			xs = t->m_pxaUserStateDefns[i32ChildToEnter];
//			xs.m_pfEnteringState();
		}
		t->m_u32CurrentState = i32ChildToEnter;
		i32ChildToEnter = t->m_pxaUserStateDefns[i32ChildToEnter].
			m_i32DefaultChildToEnter;
	}


#if defined (TESTING)
	cout << "Initial state set to "
		<< m_u32CurrentState
		<< ".\n";
#endif
}



void createTStatechart(TStatechart* t, xStateType* const xaStates,
	const uStInt u32NS, const uStInt u32InitialState)
{
//	int16_t sz;
	t->m_u32NumStates= u32NS;
	t->m_pxaUserStateDefns = xaStates;

//	t->m_xaStateData = new xInternalState[t->m_u32NumStates];
//	sz = t->m_u32NumStates * sizeof(xInternalState);
//	printf("size of used heap %i\n",sz);
	t->m_xaStateData = malloc (t->m_u32NumStates * sizeof(xInternalState));
//	verifyStateNames(t);  // dont use this line
	generateAncestries(t);
	resetHistoryReturns(t);
	setInitialState(t,u32InitialState);

	t->m_i32ExitingStatesIndex = -1;
	

	/*
#if defined (TESTING)
	cout << endl << flush;
	for (stInt i=0; i<m_u32NumStates; i++)
	{
		for (stInt j=0; j<m_xaStateData[i].vu32ancestry.size(); j++)
		{
			cout << m_xaStateData[i].vu32ancestry[j] << flush;
			cout << "	" << flush;
		}
	}
	cout << endl << flush;
	cout << "current (initial) state is :" << m_u32CurrentState << endl
		<< flush;

#endif */ 
  
}


void destructTStatechart(TStatechart* t)
{
//	delete [] t->m_xaStateData;
	free(t->m_xaStateData);
}




bool stateIsAncestorOf(TStatechart* t,stInt i32Ancestor, uStInt u32Child)
{
	bool	bInAncestry = 0;
	stInt i;

/*
	uStInt currentRec;

	currentRec = u32Child;
	bInAncestry = (currentRec == i32Ancestor);
		
		while ((! bInAncestry) && (currentRec != -1)) {
			currentRec = t->m_pxaUserStateDefns[currentRec].m_i32ParentStateName;
			bInAncestry = (currentRec == i32Ancestor);
		}
	return bInAncestry;
*/	
		bInAncestry = 0;

	for ( i=t->m_xaStateData[u32Child].m_vi32AncestrySize - 1; i>=0; i--)
	{
		if (i32Ancestor == t->m_xaStateData[u32Child].m_vi32Ancestry[i])
		{
			bInAncestry = 1;
		}
	}

	return (bInAncestry);  

  
}

/*

bool inState(TStatechart* t, const stInt u32State)
{
	
/ *	bool	bInState = false;
	uStInt currentRec;

	currentRec = t->m_u32CurrentState;

	bInState = (currentRec == u32State);
		
		while ((! bInState) && (currentRec != -1)) {
			currentRec = t->m_pxaUserStateDefns[currentRec].m_i32ParentStateName;
			bInState = (currentRec == u32State);
		}
	return bInState;
* /	
	
	/ *
	
	
	stInt	i;




	if (u32State == t->m_u32CurrentState)
	{
		return (1);
	}
	for (i=t->m_xaStateData[t->m_u32CurrentState].m_vi32AncestrySize - 1; i>=0; i--)
	{
		if (u32State == t->m_xaStateData[t->m_u32CurrentState].m_vi32Ancestry[i])
		{
			return (1);
		}
	}
	return (0);
}

*/

bool ProcessEvent(TStatechart* t)
{
	uStInt	u32result;			 

	stInt	i32StateBeingTried;
	bool	bMatchFound;
//	xStateType xs;

	t->m_i32ExitingStatesIndex = -1;
	memset(t->m_vu32exitingStates,0,sizeof(t->m_vu32exitingStates));

	bMatchFound = 0;
	i32StateBeingTried = t->m_u32CurrentState;
	while ((-1 != i32StateBeingTried) && (!bMatchFound))
	{

#if defined (TESTING)
		cout << "About to look for a match from inside state "
			<< i32StateBeingTried
			<< ".\n";
#endif

//		t->m_vu32exitingStates.insert(t->m_vu32exitingStates.end(), static_cast<uStInt>(i32StateBeingTried));

		t->m_i32ExitingStatesIndex++;
		t->m_vu32exitingStates[t->m_i32ExitingStatesIndex] = i32StateBeingTried;
		
//		xStateType xs;
		u32result = t->m_pxaUserStateDefns[i32StateBeingTried].m_pfu32EventChecker();

//		xs = t->m_pxaUserStateDefns[i32StateBeingTried].m_pfu32EventChecker();
//		u32result = xs.m_pfu32EventChecker();
		bMatchFound = (u32result ==  uStIntHandlingDone);

#if defined (TESTING)
		if (bMatchFound)
		{
			cout << "Match found from within state "
				<< i32StateBeingTried
				<< ".\n";
		}
#endif
		i32StateBeingTried = t->m_pxaUserStateDefns[i32StateBeingTried].
			m_i32ParentStateName;
	}

	return (bMatchFound);
}




//
// Name:	beginEventAction
//
// Desc.:	Called from user's code when their event handler method
//			recognizes an event match.  This method should be the first
//			thing called from within the body of that action handler.
//			This method causes all state exits to be performed, in
//			preparation for the user to perform their actions defined on
//			the event arrow itself.
//
// In:		t, the object to drive via the engine.
//			u32DestStateAndFlags, the destination state and any
//			modifying flags like "history".
//
//	Out:	u32LastStateExited, tracks the last state we exited;
//			needed for performing correct state re-entries later.
//
  
void beginEventAction(TStatechart* t, uStInt u32DestState,
	uStInt* u32LastStateExited)
{
	int i1;

	uStInt	u32StateBeingLeft = 0xff; //= 0xffffffff;
	for ( i1=0; i1<=t->m_i32ExitingStatesIndex; i1++)
	{
		
		u32StateBeingLeft = t->m_vu32exitingStates[i1];

		if (0 != t->m_pxaUserStateDefns[u32StateBeingLeft].m_pfLeavingState)
		{
			t->m_pxaUserStateDefns[u32StateBeingLeft].m_pfLeavingState();

//			xStateType xs;
//			xs = t->m_pxaUserStateDefns[u32StateBeingLeft];
//			xs.m_pfLeavingState();

		}
		*u32LastStateExited = u32StateBeingLeft;

		if (t->m_pxaUserStateDefns[t->m_pxaUserStateDefns[u32StateBeingLeft].m_i32ParentStateName].m_keepHistory)
		{
			t->m_xaStateData[t->m_pxaUserStateDefns[u32StateBeingLeft].
				m_i32ParentStateName].m_stIntHistoryReturnState =
				u32StateBeingLeft;
		}
	}

	u32StateBeingLeft = t->m_pxaUserStateDefns[u32StateBeingLeft].m_i32ParentStateName;

	while ((-1 != u32StateBeingLeft) &&
		(!stateIsAncestorOf(t,u32StateBeingLeft, u32DestState)))
	{
		if (0 != t->m_pxaUserStateDefns[u32StateBeingLeft].m_pfLeavingState)
		{
			t->m_pxaUserStateDefns[u32StateBeingLeft].m_pfLeavingState();
//			xStateType xs;
//			xs = t->m_pxaUserStateDefns[u32StateBeingLeft];
//			xs.m_pfLeavingState();
			
		}
		*u32LastStateExited = u32StateBeingLeft;

		u32StateBeingLeft =
			t->m_pxaUserStateDefns[u32StateBeingLeft].m_i32ParentStateName;

		if (t->m_pxaUserStateDefns[t->m_pxaUserStateDefns[u32StateBeingLeft].m_i32ParentStateName].m_keepHistory)
		{
			t->m_xaStateData[t->m_pxaUserStateDefns[u32StateBeingLeft].
				m_i32ParentStateName].m_stIntHistoryReturnState =
				u32StateBeingLeft;
		}
	}

	// Now, cover one last case:  the case where the dest state _is_
	// the one we were about to exit->  If so, exit it also.
	if (u32StateBeingLeft == u32DestState)
	{
		if (0 != t->m_pxaUserStateDefns[u32StateBeingLeft].m_pfLeavingState)
		{
			t->m_pxaUserStateDefns[u32StateBeingLeft].m_pfLeavingState();
//			xStateType xs;
//			xs = t->m_pxaUserStateDefns[u32StateBeingLeft];
//			xs.m_pfLeavingState();
		}
		t->u32LastStateExited = u32StateBeingLeft;

		if (t->m_pxaUserStateDefns[t->m_pxaUserStateDefns[u32StateBeingLeft].m_i32ParentStateName].m_keepHistory)
		{
			t->m_xaStateData[t->m_pxaUserStateDefns[u32StateBeingLeft].
				m_i32ParentStateName].m_stIntHistoryReturnState =
				u32StateBeingLeft;
		}
	}
	
	return;
}


//
// Name:	enterDestinationState
//
// Desc.:	Enters states, using history information if requested,
//			and default child information if necessary to reach
//			a simple state.
//
// In:		t, the object to drive via the engine.
//			u32NewState, the state we're destined for (can be a
//			composite state).
//			bWithHistory, true if we're supposed to enter a state
//			with history.
//			u32IndexOfFirstStateToEnter, the index (in the
//			ancestry of the destination state) of the first state
//			we are supposed to enter.
//
// Returns:	the new simple state's name.
//
  
uStInt enterDestinationState(TStatechart* t,
	const uStInt u32NewState, 
	const uStInt u32IndexOfFirstStateToEnter)
{
	uStInt	ue;
//	xStateType xs;
	uStInt		u32StateTransitionallyIn;

	ue = u32IndexOfFirstStateToEnter;
	do
	{
		if (0 != t->m_pxaUserStateDefns[t->m_xaStateData[u32NewState].
			m_vi32Ancestry[ue]].m_pfEnteringState)
		{
			t->m_pxaUserStateDefns[t->m_xaStateData[u32NewState].
				m_vi32Ancestry[ue]].m_pfEnteringState();

//			xs = t->m_pxaUserStateDefns[u32NewState];
//			xs.m_pfEnteringState();	
		}
		ue++;
	}	
	while (ue <= t->m_xaStateData[u32NewState].m_vi32AncestrySize - 1);

	u32StateTransitionallyIn = u32NewState;
	while ((-1 != t->m_xaStateData[u32StateTransitionallyIn].
			m_stIntHistoryReturnState) ||
		(-1 != t->m_pxaUserStateDefns[u32StateTransitionallyIn].m_i32DefaultChildToEnter) )
	{
		if (-1 != t->m_xaStateData[u32StateTransitionallyIn].m_stIntHistoryReturnState)
			u32StateTransitionallyIn =	 t->m_xaStateData[u32StateTransitionallyIn].m_stIntHistoryReturnState;
		else
			u32StateTransitionallyIn=t->m_pxaUserStateDefns[u32StateTransitionallyIn].m_i32DefaultChildToEnter;
		if (0 != t->m_pxaUserStateDefns[u32StateTransitionallyIn].
			m_pfEnteringState)
		{
			t->m_pxaUserStateDefns[u32StateTransitionallyIn].m_pfEnteringState();
//			xs = t->m_pxaUserStateDefns[u32StateTransitionallyIn];
//			xs.m_pfEnteringState();

		}
	}
	return (u32StateTransitionallyIn);
}


  


//
// Name:	endEventAction
//
// Desc.:	To be called as the last thing inside the caller's method
//			that handles this event->  Calls methods to effect re-entry
//			into the appropriate states.
//
// In:		t, the object to drive via the engine.
//			u32DestStateAndFlags, the destination state and any
//			modifying flags like "history".	MUST MATCH the value
//			passed into BeginEventAction for the same event handler.
//			u32LastStateExited, must be the value passed back in the
//			corresponding BeginEventAction call.
//
  
  
  
  
void endEventAction(TStatechart* t, uStInt u32DestState,
	uStInt u32LastStateExited)
{
	
	stInt	i32IndexOfFirstStateToEnter = -1;
	stInt ue;
	stInt i1;
	
	if (stateIsAncestorOf(t,u32LastStateExited, u32DestState))
	{
		for (ue=0; ue<t->m_xaStateData[u32DestState].m_vi32AncestrySize; ue++)
		{
			if (t->m_xaStateData[u32DestState].m_vi32Ancestry[ue] ==
				u32LastStateExited)
			{
				i32IndexOfFirstStateToEnter = ue;
				break;
			}
		}
	}
	else
	{
		stInt	i32MaxIndex;
		if (t->m_xaStateData[u32DestState].m_vi32AncestrySize >
			t->m_xaStateData[t->u32LastStateExited].m_vi32AncestrySize)
		{
			i32MaxIndex = t->m_xaStateData[u32LastStateExited].m_vi32AncestrySize - 1;
		}
		else
		{
			i32MaxIndex = t->m_xaStateData[u32DestState].m_vi32AncestrySize - 1;
		}

		for ( i1=0; i1<=i32MaxIndex; i1++)
		{
			i32IndexOfFirstStateToEnter = i1;

			if (t->m_xaStateData[u32LastStateExited].m_vi32Ancestry[i1] !=
				t->m_xaStateData[u32DestState].m_vi32Ancestry[i1])
			{
//				assert(0 != i1);	
				break;
			}
		}
	}
	
	t->m_u32CurrentState = enterDestinationState(t,
		u32DestState,
		i32IndexOfFirstStateToEnter);
	return;
	
}





