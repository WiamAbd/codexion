*This project has been created as part of the 42 curriculum by wabdella.*

## Description

Codexion simulates a group of coders sitting in a circular co-working hub, competing
for a limited pool of shared USB dongles needed to compile "quantum code". It is a
concurrency exercise built on the classic Dining Philosophers problem: each coder
repeatedly compiles, debugs, and refactors, and must acquire two adjacent dongles
simultaneously in order to compile. If a coder goes too long without starting a new
compile, they burn out and the simulation stops. The goal of the project is to build a
correct, deadlock-free, starvation-free multithreaded scheduler (FIFO or EDF) around
this constraint, using only POSIX threads, mutexes, and condition variables.

## Instructions

### Compilation

```
make
```

Compiles with `-Wall -Wextra -Werror -pthread` and produces the `codexion` binary at
the project root. Other targets: `make clean`, `make fclean`, `make re`.

### Execution

```
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

- `number_of_coders`: number of coders, and also the number of dongles.
- `time_to_burnout` (ms): if a coder does not start a new compile within this many ms
  of their last compile start (or the start of the simulation), they burn out.
- `time_to_compile` / `time_to_debug` / `time_to_refactor` (ms): duration of each phase.
- `number_of_compiles_required`: the simulation stops once every coder has completed
  at least this many compiles (unless a burnout happens first).
- `dongle_cooldown` (ms): once released, a dongle cannot be re-acquired until this
  many ms have passed.
- `scheduler`: `fifo` or `edf`, the policy each dongle uses to arbitrate between
  waiting coders.

Example:

```
./codexion 5 2000 200 200 200 10 0 fifo
```

## Resources

- Course material and man pages for `pthread_mutex_*`, `pthread_cond_*`, and
  `pthread_create`/`pthread_join`.
- The classic Dining Philosophers problem (Dijkstra) as the conceptual model for the
  dongle-acquisition deadlock, and the standard asymmetric-ordering solution (odd/even
  acquisition order) used here to avoid circular wait.
- Earliest Deadline First (EDF) scheduling as a real-time scheduling policy, adapted so
  each dongle grants access to the waiting coder with the nearest burnout deadline.
- **AI usage**: AI was not used to write the original implementation. It was used
  afterwards, in a review/debugging capacity: to audit the code against the project's
  grading scale by actually building and running it against the reference test cases;
  to trace down and fix a real concurrency bug where the two "has taken a dongle" log
  lines and the following "is compiling" line could be interleaved with another
  coder's log line under contention (fixed by printing all three under one continuous
  hold of the output mutex, in `log_start_compile`); and to clean up a few Norm issues
  (a leftover debug `printf`, a multi-statement line, and mixed tab/space indentation)
  in `parsing.c`, `dongle.c`, and `coder.c`. Every AI-suggested change was reviewed and
  re-tested against the benchmark cases before being kept.

## Blocking cases handled

- **Deadlock (circular wait) prevention**: each coder needs two adjacent dongles to
  compile, structurally identical to the Dining Philosophers problem. Coders acquire
  their two dongles in an order based on the parity of their id (`get_dongles` in
  `coder.c`): odd-numbered coders take their own-index dongle first then their
  neighbor's, even-numbered coders take the reverse order. This breaks the
  circular-wait condition — one of Coffman's four necessary conditions for deadlock —
  without needing a global lock ordering across all dongles.
- **Contention fairness across a coder's two dongles**: both of a coder's dongle
  requests are stamped with the same arrival timestamp, taken once before either
  acquisition starts (`acquire_two_dongles` in `coder.c`), instead of timestamping the
  second request only after the first dongle was already obtained. This keeps a
  coder's queue priority for its second dongle tied to when it actually became ready
  to compile, rather than being unfairly pushed back by however long the first dongle
  took — which otherwise lets a delay on one dongle compound into a growing delay on
  the other every cycle.
- **No hold-and-wait across dongles' internal locks**: each dongle's own mutex is held
  only briefly, inside `acquire_dongle`/`release_dongle`, and is released before the
  next dongle is even attempted. A coder can be blocked waiting for its second dongle
  while holding the first, but it never holds two dongle mutexes at once, so
  contention on one dongle can never block progress on another dongle's internal
  state.
- **Starvation prevention / fair arbitration**: each dongle keeps a priority queue (a
  custom binary heap, `heap.c` / `heap_utils.c`) of pending requests, ordered by
  arrival time (`fifo`) or by deadline `last_compile_start + time_to_burnout` (`edf`).
  A coder can only take a dongle once it reaches the front of that dongle's queue, so
  no coder is perpetually skipped under feasible parameters.
- **Deterministic tie-breaking**: because timestamps are only millisecond-precise, two
  requests can legitimately arrive (or share a deadline) within the same millisecond.
  `compare()` in `heap_utils.c` breaks such ties by coder id, so the heap ordering —
  and therefore the grant order — stays fully deterministic even in that edge case.
- **Dongle cooldown**: `release_dongle` stamps the dongle with
  `available_at = now + dongle_cooldown`; `can_take()` refuses to grant the dongle
  until that time has passed, so a dongle can never be re-taken before its cooldown
  elapses.
- **No dongle duplication**: a dongle's `available` flag and wait-queue are protected
  by that dongle's own mutex, and are only ever flipped to "held" inside the same
  critical section that pops the winning request off the queue, so two coders can
  never simultaneously believe they hold the same dongle.
- **Precise burnout detection**: a dedicated monitor thread polls each coder's
  `last_compile_start` (protected by that coder's own mutex) roughly every
  millisecond and stops the simulation as soon as any coder crosses its deadline,
  keeping the reported burnout time within the required tolerance of the actual
  deadline.
- **Log serialization**: all output goes through a single global print mutex. The two
  "has taken a dongle" lines and the following "is compiling" line for a given coder
  are printed inside one continuous hold of that mutex (`log_start_compile`) instead
  of three separate lock/unlock calls, so another coder's log line can never be
  interleaved in the middle of that three-line sequence.
- **Clean shutdown**: when the simulation stops (burnout or completion),
  `stop_simulation` broadcasts every dongle's condition variable so no coder thread is
  left blocked forever inside `pthread_cond_wait` / `pthread_cond_timedwait`; a coder
  that wakes up to a stopped simulation removes its own pending request from the heap
  before exiting, leaving no stale entries behind.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle** (`t_dongle.mutex`): protects that dongle's
  `available` flag, `available_at` cooldown timestamp, and its wait-queue heap. Every
  read or mutation of these fields happens while holding this mutex.
- **`pthread_cond_t` per dongle** (`t_dongle.cond`): coders waiting for a dongle block
  on this condition variable (via `pthread_cond_wait`, or `pthread_cond_timedwait`
  when a cooldown deadline is known) instead of busy-waiting; `release_dongle` and
  shutdown both call `pthread_cond_broadcast` so every waiter re-checks whether it can
  now proceed.
- **`pthread_mutex_t` per coder** (`t_coder.state_mutex`): protects that coder's own
  `last_compile_start` and `compile_count`. These fields are written by the coder's
  own thread and read by both that thread (when building a dongle request) and the
  monitor thread (burnout/completion checks), so every access goes through this mutex
  to prevent a race between the coder updating its state and the monitor reading it.
- **A global `state_mutex`** (`t_sim.state_mutex`): protects the single `stop` flag
  shared by every coder thread and the monitor thread, always accessed through
  `simulation_stopped()` / `stop_simulation()`.
- **A global `print_mutex`** (`t_sim.print_mutex`): serializes every line printed to
  stdout, and — critically — is held across whole groups of related lines (the two
  "has taken a dongle" lines plus "is compiling") rather than released between them,
  so a coder's state transition is never split across another thread's output.
- No global variables are used anywhere: all shared state lives inside `t_sim`,
  `t_coder`, and `t_dongle`, reached only through the pointers passed into each
  thread, which keeps every critical section's scope explicit and auditable.
