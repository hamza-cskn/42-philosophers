# 42-Philosophers

[The dining philosophers problem](https://arc.net/l/quote/lzhimdns) is a famous problem in computer science. It is a metaphor to help people to understand multi-threaded(concurrent) systems and problems with shared memory blocks.

## About the problem
### The problem
* There is a round table and a meal in the middle.
* There are N philosophers around the table.
* Philosophers have 3 states(with order): eat, sleep, and think.
* If philosophers cannot eat enough often, they die.
* Every philosopher should eat and not die.
* There are sticks between each philosopher. One for the right and one for the left. Totally N sticks exist.
* Philosophers can only take sticks from his right and left ones.
* Philosophers need 2 sticks to eat. Because these are not spoons or forks. These are sticks in the end lol.
  * So if all of the philosophers take one stick at the same time everybody will starve. (which state we call 'deadlock' in computer science.)

### Explanation
Let's break down what the problem means on the technical side.

* Every philosopher is a thread/process.
* Every stick is a mutex lock.
* The meal is a critical section.

## Compiling

1. Clone this repository.
2. Use the `make` command to compile.
3. You'll get an executable file named `philo`. Run it with arguments.

**Arguments:** `./philo <philosopher count> <time to die> <time to eat> <time to sleep> [must eat count]`

Times must be specified as milliseconds. 
