# Lab Work 6: C++20 Coroutines (Variant 13)

## Description

Three coroutines (players A, B and C) play a game with two positive whole numbers.
The players take turns. On each turn, a player does exactly one action with one of
the two numbers (the player chooses which one):

1. add 1;
2. multiply by 2;
3. divide by 3 (only if the number can be divided by 3 without a remainder).

A player cannot do the same action two turns in a row.
The first player to make the sum of the two numbers at least 10 times bigger than
the starting sum wins. The winner prints a message and stops.
The main program starts the game and prints a log of all moves and the winner.

The task does not require a smart strategy, so players choose a random allowed move.

## Technologies

- **C++20**:
  - **coroutines** (`<coroutine>`): `co_await`, `co_return`, `promise_type`,
    `std::coroutine_handle`, `std::suspend_always`;
  - `std::optional`: a value that may be missing (a player's previous action, the winner);
  - `<random>`: the `std::mt19937` generator and `std::uniform_int_distribution`;
  - `std::from_chars` (`<charconv>`): strict conversion of text to a number;
  - exceptions: a custom `InputError` type.
- **CMake**: building the project.
- **MSVC (Visual Studio Build Tools)** in **VS Code** with the CMake Tools extension.
  The code also builds with GCC 11+ and Clang 14+.

## Implementation Method

### Main idea

Each player is a separate coroutine. A coroutine makes one move and then pauses
(`co_await`), giving control back to the main program. The main program (the scheduler)
wakes up the players one by one in a circle: A → B → C → A → …
While a coroutine is paused, its local variables (including its last action) are kept
in the coroutine frame. That is how each player "remembers" what it did last time.

Everything runs in one thread. The players never run at the same time; they take turns
and pass control to each other. Because of this, no synchronization (mutexes) is needed.

### How the game runs

1. `InputReader` reads and checks the input data.
2. `Game` creates three player coroutines. Because `initial_suspend` returns
   `suspend_always`, they start paused and do not make any move yet.
3. `Game::play()` calls `resume()` for each player in turn.
4. The player picks an allowed move (`RandomStrategy`), applies it to the shared game
   state (`GameRules`), prints the move to the log, and then:
   - if it has won, prints a message and finishes (`co_return`);
   - otherwise, pauses (`co_await std::suspend_always{}`).
5. The scheduler sees that the coroutine has finished (`done()`) and announces the winner.
6. The `PlayerTask` objects are destroyed and free the frames of all coroutines (RAII),
   so there are no memory leaks.

As a safety limit against an endless game, the game stops after 100,000 moves.
In practice, a game ends after a few dozen moves.

### Code structure

| File | Responsibility |
|---|---|
| `gameRules.h/.cpp` | game state: two numbers, target sum, checking and applying a move |
| `randomStrategy.h/.cpp` | choosing a random allowed move (not repeating the previous action) |
| `playerTask.h/.cpp` | coroutine type: `promise_type` and an RAII wrapper over `coroutine_handle` |
| `game.h/.cpp` | player coroutine, turn scheduler, printing the game log |
| `inputReader.h/.cpp` | reading and validating input data |
| `main.cpp` | entry point: choosing the input source, starting the game, handling errors |

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```powershell
.\build\Debug\Lab-work-6.exe input\example.txt   # read data from a file
.\build\Debug\Lab-work-6.exe                     # type data from the keyboard (finish with Ctrl+Z, Enter)
```

## Input Format

A text file (or standard input) contains 2 or 3 whole numbers separated by spaces
or line breaks:

```
<number1> <number2> [seed]
```

| Field | Required | Allowed values | Meaning |
|---|---|---|---|
| number1 | yes | integer from 1 to 10^12 | first starting number |
| number2 | yes | integer from 1 to 10^12 | second starting number |
| seed | no | integer from 0 to 4294967295 | seed for the random number generator |

- Empty lines and lines starting with `#` are ignored (comments).
- With the same seed, the game repeats exactly; without a seed, every run gives a new game.
- The 10^12 limit guarantees that values in the game never overflow `int64_t`.

Example (`input/example.txt`):

```
# Starting numbers and generator seed
3 5 42
```

## Error Handling

If the input is invalid, the program prints a message to `stderr` and exits with code 1.
It checks for:

- wrong number of values (fewer than two or more than three);
- non-numeric and fractional values (`abc`, `3.5`, `12x`);
- numbers that are too large (overflow);
- numbers outside the allowed range (0, negative numbers, an invalid seed);
- a file that cannot be opened;
- extra command-line arguments.

Examples of invalid input are in the `input/bad_*.txt` files.
