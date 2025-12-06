#pragma once

#include <revolution/types.h>

class ActorStateBaseInterface;
class LiveActor;
class Nerve;
class NerveExecutor;

namespace MR {
    /// @brief Initializes the actor state keeper for a nerve executor.
    /// @param pExecutor The nerve executor to initialize the state keeper for.
    /// @param capacity Maximum number of states that can be stored in the state keeper.
    /// @note This must be called before adding any states to the executor.
    void initActorStateKeeper(NerveExecutor* pExecutor, s32 capacity);

    /// @brief Initializes and registers an actor state with a nerve executor.
    /// @param pExecutor The nerve executor that will manage this state.
    /// @param pActorState The state interface to initialize and register.
    /// @param pNerve The nerve associated with this state for state machine transitions.
    /// @param pStateName State name or identifier string for debugging/lookup purposes.
    /// @note The state interface's init() method will be called during this process.
    void initActorState(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState, const Nerve* pNerve, const char* pStateName);

    /// @brief Updates a specific actor state for a live actor without changing nerves.
    /// @param pActor The live actor associated with the state.
    /// @param pActorState The specific state to update.
    /// @return true if the state was active and processed, false if the state was inactive.
    /// @note Calls the state's start() method on first step, then update() if conditions are met.
    bool updateActorState(LiveActor* pActor, ActorStateBaseInterface* pActorState);

    /// @brief Updates an actor state and transitions to the next nerve if the state completed.
    /// @param pActor The live actor whose state should be updated.
    /// @param pActorState The specific state to update.
    /// @param pNextNerve The nerve to transition to if the state update succeeds.
    /// @return True if the state was updated and nerve transition occurred, false otherwise.
    /// @note Only transitions to the next nerve if the state indicates it should update.
    bool updateActorStateAndNextNerve(LiveActor* pActor, ActorStateBaseInterface* pActorState, const Nerve* pNextNerve);

    /// @brief Updates the current active state of a nerve executor.
    /// @param pExecutor The nerve executor whose current state should be updated.
    /// @return true if the current state was active and processed, false if no active state.
    /// @note Uses the executor's state keeper to update whichever state is currently active.
    bool updateActorState(NerveExecutor* pExecutor);

    /// @brief Updates a specific actor state without changing nerves.
    /// @param pExecutor The nerve executor associated with the state.
    /// @param pActorState The specific state to update.
    /// @return true if the state was active and processed, false if the state was inactive.
    /// @note Calls the state's start() method on first step, then update() if conditions are met.
    bool updateActorState(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState);

    /// @brief Updates the current active state and transitions to the next nerve if completed.
    /// @param pExecutor The nerve executor whose current state should be updated.
    /// @param pNextNerve The nerve to transition to if the current state update succeeds.
    /// @return true if the current state was updated and nerve transition occurred, false otherwise.
    /// @note Uses the executor's state keeper to update whichever state is currently active.
    bool updateActorStateAndNextNerve(NerveExecutor* pExecutor, const Nerve* pNextNerve);

    /// @brief Updates an actor state and transitions to the next nerve if the state completed.
    /// @param pExecutor The nerve executor whose state should be updated.
    /// @param pActorState The specific state to update.
    /// @param pNextNerve The nerve to transition to if the state update succeeds.
    /// @return true if the state was updated and nerve transition occurred, false otherwise.
    /// @note Only transitions to the next nerve if the state indicates it should update.
    bool updateActorStateAndNextNerve(NerveExecutor* pExecutor, ActorStateBaseInterface* pActorState, const Nerve* pNextNerve);
};  // namespace MR
