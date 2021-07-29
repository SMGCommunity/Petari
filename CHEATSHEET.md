# Super Mario Galaxy 1 Cheatsheet

## Possible differences

* Sometimes the correct output for if / else operations is the opposite of what you wrote, such as:

```cpp
if (something)
{
    // some code
}
else
{
    // some other code
}```

⬇️

```cpp
if (!something)
{
    // some other code
}
else
{
    // some code
}```

* Be sure to pay attention on the `beq` / `ble` etc instructions

* if / else may have been written as ternary operations

```cpp
if (something == 5)
{
    variable = 1;
}
else
{
    variable = 10;
}```

⬇️

```cpp
variable = (something == 5) ? 1 : 10;
```

* Check if the function you are decompiling has an inlined function
* Wrong conditions (ie. using < instead of <= by accident)

## Internal functions

* Sometimes there are internal functions used that generate some code, such as the `frsqrte` instruction. These do not need `#include` to use. In these cases, try to add two underscores at the beginning:
    * __frsqrte = frsqrte
    * __fabs    = fabs
    * There are many more, if there are common ones, I will add them to this list

## Behaviors

If you ever catch this:
```
cntlzw r0, r0
srwi r3, r0, 5
```
This is the result of a !(expr) used as a return value in a BOOLEAN function.

* If the destructor of a class is called with r4 being `-1`, it means that the class destructor that is being called is stored in the class, and is not inherited. However, if r4 is `0`, that means the class being destructed is what the current class is inheriting.