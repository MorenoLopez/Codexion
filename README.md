*This project has been created as part of the 42 curriculum by horarivo.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic "dining philosophers" problem. A group of coders sit in a circular co-working hub, sharing a set of USB dongles placed on a table between them. Compiling quantum code requires two dongles held simultaneously (one on each side), so coders must compete, wait, and cooperate to access this limited shared resource without ever running out of time (burning out).

Each coder repeatedly cycles through four phases:

- **Acquiring dongles** — waiting for both neighboring dongles to become available.
- **Compiling** — holding both dongles for `time_to_compile` milliseconds.
- **Debugging** — a fixed idle phase of `time_to_debug` milliseconds.
- **Refactoring** — a fixed idle phase of `time_to_refactor` milliseconds, after which the coder immediately tries to compile again.

If a coder fails to start a new compile within `time_to_burnout` milliseconds since the beginning of the simulation or since their last compile started, they burn out and the simulation stops. The simulation also stops successfully once every coder has completed at least `number_of_compiles_required` compiles.

The project's goal is to implement this simulation entirely with POSIX threads (one thread per coder, plus a dedicated monitor thread), while correctly handling the classic concurrency issues that arise from shared, contested resources: deadlocks, starvation, fair arbitration between competing threads, and precise, race-free logging.

## Instructions

### Compilation

```bash
make
```

This produces the `codexion` executable using `cc` with `-Wall -Wextra -Werror -pthread`.

Other available rules:

```bash
make clean   # remove object files
make fclean  # remove object files and the binary
make re      # fclean + all
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and number of dongles). |
| `time_to_burnout` | Milliseconds before a coder burns out if they haven't started compiling. |
| `time_to_compile` | Milliseconds a compile takes (two dongles held). |
| `time_to_debug` | Milliseconds spent debugging. |
| `time_to_refactor` | Milliseconds spent refactoring. |
| `number_of_compiles_required` | Minimum compiles per coder for the simulation to stop successfully. |
| `dongle_cooldown` | Milliseconds a dongle stays unavailable after being released. |
| `scheduler` | Arbitration policy: `fifo` or `edf`. |

All arguments are mandatory and validated (positive integers only, `scheduler` must be exactly `fifo` or `edf`). Invalid input prints `Invalid arguments` and exits with status 1.

Example:

```bash
./codexion 5 800 200 200 200 5 100 fifo
```

Example log line format:

```
[ 203 ] 2 is compiling
```

Meaning: at 203 ms since the start of the simulation, coder 2 changed state to "is compiling".

## Blocking cases handled

**Deadlock prevention (Coffman's conditions).** Compiling requires holding two dongles simultaneously (hold-and-wait), which under a naive acquisition order (every coder always taking their left dongle first, then their right one) produces a circular wait: each coder holds one dongle and waits for the one held by their neighbor. This is broken by reversing the acquisition order for exactly one coder (the highest-numbered one), which removes the circular dependency: that coder takes its right dongle first, then its left one, while every other coder does the opposite. This is the classic asymmetric solution to the dining philosophers deadlock.

**Starvation prevention / fair arbitration.** Each dongle keeps its own wait queue implemented as a binary min-heap, keyed either by arrival timestamp (`fifo`) or by deadline (`edf`, computed as `last_compile_start + time_to_burnout`). A coder is only allowed to take a dongle once it is both available and at the root of the heap (i.e. has the highest priority among current waiters for that specific dongle). This prevents a single coder from repeatedly winning the race for a dongle purely by chance.

**Cooldown handling.** Every dongle records `last_release`, the timestamp of its last release. A dongle is only considered available once `dongle_cooldown` milliseconds have passed since that timestamp; this condition is part of the same wait loop as the heap-priority check, re-evaluated every time the waiting thread wakes up.

**Precise burnout detection.** A dedicated monitor thread polls every coder's `last_compile_start` every 5 milliseconds and compares the elapsed time against `time_to_burnout`. This interval was chosen to keep the detection well within the 10 ms precision window required by the subject, without wasting CPU on a tighter busy-loop.

**Clean, race-free shutdown.** When a burnout is detected (or every coder has reached `number_of_compiles_required`), the shared `stop` flag is set under a mutex and every dongle's condition variable is broadcast so that no thread remains asleep waiting on a dongle that will never come. Any coder that was queued but did not get to compile removes itself from that dongle's wait queue before returning, so no stale queue entries are left behind.

**Log serialization.** All state-change logs are written through a single function that locks a dedicated logging mutex around the `printf` call, guaranteeing that two threads can never interleave their output on a single line.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle** protects that dongle's state (`is_taken`, `last_release`) and its wait queue, so no two coders can simultaneously believe they have acquired the same dongle.
- **`pthread_cond_t` per dongle**, used with `pthread_cond_timedwait` rather than a plain `pthread_cond_wait`. A short timeout (5 ms) is used so that a waiting coder periodically re-checks the cooldown condition even if no `broadcast` occurs in the meantime — a plain `cond_wait` would risk a coder sleeping forever if the cooldown expires without any dongle activity to wake it. `release_dongle` calls `pthread_cond_broadcast` so that every coder currently waiting on that dongle re-evaluates whether it is now their turn.
- **A dedicated `state_mutex`** protects fields that are read and written across threads outside of the dongle logic itself: the global `stop` flag, and each coder's `last_compile_start` / `compiles_done`. This is kept separate from the logging mutex, since the two protect unrelated pieces of state and mixing them would make the locking harder to reason about.
- **A dedicated `log_mutex`** protects only the shared standard output stream.
- **A binary min-heap per dongle** acts as the actual priority queue backing both `fifo` and `edf` scheduling: `heap_push` inserts a waiting coder with its priority key, `heap_peek` lets a waiting coder check (without removing anything) whether it is currently first in line, `heap_pop` removes the coder that is about to acquire the dongle, and `heap_remove` allows a coder to withdraw its own entry if the simulation stops while it was still waiting.

Race conditions are avoided by never reading or writing any of the fields above outside of their associated mutex, and by re-checking all wait conditions (dongle availability, cooldown, heap priority, `stop`) inside a `while` loop around `pthread_cond_timedwait` rather than assuming a single wake-up means the condition is met — protecting against both spurious wake-ups and cases where several coders are woken by the same broadcast but only one of them should actually proceed.

Thread-safe communication between the coder threads and the monitor thread happens exclusively through this shared, mutex-protected state (`stop`, `last_compile_start`, `compiles_done`): the monitor thread never signals coders directly, it only sets `stop` and broadcasts every dongle's condition variable, letting each coder thread notice the change on its own next wake-up or condition check.


## Resources

- [POSIX Threads Programming (LLNL Tutorial)](https://hpc-tutorials.llnl.gov/posix/) — general reference on `pthread_create`, `pthread_join`, mutexes and condition variables.
- The [Linux man pages](https://linux.die.net/man/) for `pthread_mutex_init`, `pthread_cond_wait`, `pthread_cond_timedwait`, `clock_gettime`, `gettimeofday`.
- The [classic "Dining Philosophers" problem (Dijkstra, 1965)](https://en.wikipedia.org/wiki/Dining_philosophers_problem) as conceptual background for the deadlock and starvation issues addressed here.
- [Coffman's conditions for deadlock](https://faq.computersciencewiki.org/index.php/home/article/coffman-conditions) (mutual exclusion, hold and wait, no preemption, circular wait), used as a checklist while designing the acquisition order.
- [Binary heap (priority queue) data structure](https://www.hackerearth.com/practice/notes/heaps-and-priority-queues/), used as the basis for implementing the dongle wait queues for both fifo and edf scheduling.

### AI usage

An AI assistant (Claude) was used throughout the project as a learning and debugging aid:

- Explaining core concepts: pthreads basics (`pthread_create`, `pthread_join`), mutexes, condition variables (`pthread_cond_wait`, `pthread_cond_timedwait`), and the mechanics of a binary heap (array-based representation, parent/child index formulas, ...).

- Discussing design trade-offs, such as busy-waiting versus condition variables, and per-dongle wait queues versus a global scheduling structure.

