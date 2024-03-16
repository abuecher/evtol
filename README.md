# evtol problem


## Tenets
- Use good OOP design
- Must be coded in C++
- Must include unit tests (only a few different types of unit tests needed, does not need full coverage) - I should also include some stuff like static analyzers, and threading tests, etc
- TODO and NOTE are OK for things that might take too long to properly implement.
-



# Concepts

The table provided can help determine what components I need to model:

(Can remove these ticks in emacs, but the CLion WYSIWYG editor does not render well)
```
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Company Name         | Alpha | Bravo | Charlie | Delta | Echo | Notes                                          |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Cruise Speed (MPH)   | 120   | 100   | 160     | 90    | 30   |                                                |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Battery (kWh)        | 320   | 100   | 220     | 120   | 150  |                                                |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Time to Charge (hrs) | 0.6   | 0.2   | 0.8     | 0.62  | 0.3  |                                                |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Energy use at Cruise | 1.6   | 1.5   | 2.2     | 0.8   | 5.8  |                                                |
| (kWh/mile)           |       |       |         |       |      |                                                |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Passenger Count      | 4     | 5     | 3       | 2     | 2    |                                                |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
| Probability of       | 0.25  | 0.10  | 0.05    | 0.22  | 0.61 | Q: What is this?                               |
| fault per hour       |       |       |         |       |      | Is it a random chance of fault?                |
|                      |       |       |         |       |      | Or is it a forced cool down when it exceeds 1? |
|                      |       |       |         |       |      | How long is cool down or is vehicle destroyed? |
|                      |       |       |         |       |      |                                                |
|----------------------|-------|-------|---------|-------|------|------------------------------------------------|
```



# Note
I wrote most of this in emacs (WSL emacs-GUI). It's nice for the mouse-free navigation (Markjdown-major mode much like
orgmode for editing ascii tables even if they dont render properly w/o the tick marks.
